#include <assert.h>
extern "C"{
#include <zip.h>
#include <iconv.h>
};

#include <iostream>
#include <vector>
#include <sstream>
#include <boost/regex.hpp>
#include "expat_wrapper.hpp"

using namespace std;

class Reader{
public:
    virtual int read()=0;
};

class ZipReader: public Reader{
private:
    zip* archive;
    zip_file* zfile;
    unsigned char* buffer;
    int read_data;
    int cur_pos;
    int buffer_size;
public:
    ZipReader( zip* archive, int index, int buffer_size = 1024 );
//    ZipReader( zip* archive, const char* fname );
    virtual ~ZipReader();
    //read one char. returns false, 
    virtual int read();
};

/**************** Limited reader *****************************
 */
class LimitedReader: public Reader{
private:
    Reader & parent;
    size_t limit;
    size_t readBytes;
public:
    LimitedReader( Reader & parent_, size_t limit_ );
    virtual int read();
};
LimitedReader::LimitedReader( Reader & parent_, size_t limit_ )
    :parent( parent_ ),
     limit( limit_ ),
     readBytes( 0 )
{};
int LimitedReader::read()
{
    if (readBytes < limit ){
	int rval = parent.read();
	if (rval != -1 ){
	    readBytes ++;
	}
	return rval;
    }else{
	return -1;
    }
};
    

/**************** ZipReader implementation *******************
 */
ZipReader::ZipReader( zip* archive_, int index, int buffer_size_ )
{
    assert( archive_ );
    archive = archive_;

    int num_files = zip_get_num_files( archive );
    assert( index >=0 );
    assert( index < num_files );
    
    //trying to open the file...
    zfile = zip_fopen_index( archive, index, 0 );
    assert( zfile );

    //allocate buffer
    buffer_size = buffer_size_;
    buffer = new unsigned char[ buffer_size ];
    read_data = 0;//currently, buffer is empty.
    cur_pos = 0;//current position in the buffer
}

ZipReader::~ZipReader()
{
    int err = zip_fclose( zfile );
    assert( err== 0);
    delete[] buffer;
}

int ZipReader::read()
{
    if ( cur_pos < read_data ){
	unsigned char rval = buffer[ cur_pos ];
	cur_pos ++;
	return static_cast<int>(rval);
    }else{
	//need to read some data
	int bytes_read = zip_fread( zfile, static_cast<void*>(buffer), buffer_size );
	if (bytes_read == -1 ){
	    //failed to read more data
	    return -1;
	}else if (bytes_read == 0){
	    //read 0 bytes
	    return -1;
	}else if (bytes_read > 0){
	    //successsfully read more data
	    cur_pos = 0;
	    read_data = bytes_read;
	    return read();
	}else{
	    assert( false );//something unexpected: negative values, not equal -1
	}
    }
}

/************************** Expat-based parser ********************/

struct cstring_end_iterator{
    char & operator *()const{ throw std::logic_error("Access to the string end iterator"); };
};
template <class Iterator>
bool operator==( const Iterator &i, const cstring_end_iterator &e)
{
    return (*i)=='\0';
}
template <class Iterator>
bool operator!=( const Iterator &i, const cstring_end_iterator &e)
{
    return (*i)!='\0';
}

template <class char_iterator1, class char_iterator2>
void put_escaped( std::ostream & os, char_iterator1 begin, char_iterator2 end, bool escape_quotes=false )
{
    for (char_iterator1 ic = begin; ic!= end; ++ic){
	char c = *ic;
	if ( c == '<'){
	    os<<"&lt;";
	}else if ( c == '>' ){
	    os<<"&gt;";
	}else if ( c == '&' ){
	    os<<"&amp;";
	}else if ( c == '"' && escape_quotes ){
	    os<<"&quot;";
	}else{
	    os<<c;
	}
    }
}

class DescriptionExtractor: public ExpatParser
{
private:
    enum State{
	STATE_WAIT_DESC,
	STATE_READING_DESC,
	STATE_DONE
    };
    State state;
    int level;
    std::stringstream desc;
public:
    DescriptionExtractor();
    virtual void start( const char * el, const char **attr);
    virtual void end( const char * el);
    virtual void character( const char * chars, int len );
    bool extract( Reader & rdr );
    std::string get_desc()const;
private:
    void write_tag( const char * el, const char **attr);
};

std::string DescriptionExtractor::get_desc()const
{
    return desc.str();
}
bool DescriptionExtractor::extract( Reader & rdr )
{
    const int BUFFER_SIZE = 128;
    char buffer[ BUFFER_SIZE ];
    int buffer_used = 0;

    while (true){
	bool finished = false;
	buffer_used = 0;
	while (buffer_used < BUFFER_SIZE ){
	    int c = rdr.read();
	    if ( c == -1 ){
		finished = true;
		break;
	    }
	    buffer[ buffer_used ] = static_cast<char>(c);
	    buffer_used ++;
	}
	//parse char
	XML_Status result = parse( buffer, buffer_used, finished );
	if ( state == STATE_DONE ){
	    //finished. Return
	    break;
	}
	if ( result != XML_STATUS_OK ){
	    cerr<<"Error parsing"<<endl;
	    cerr<<get_desc()<<endl;
	    return false;
	}
    }
    return true;
}

DescriptionExtractor::DescriptionExtractor( )
    :state(STATE_WAIT_DESC),
     level(0)
{
}

void DescriptionExtractor::write_tag( const char * el, const char **attr)
{
    for( int i =0;i<level; ++i)
	desc<<" ";
    desc<<"<"<<el;
    //write attributes
    while( *attr ){
	std::string aname = attr[0];
	size_t ns_sep_pos = aname.rfind( ':' );
	if (ns_sep_pos != aname.npos ){
	    //found
	    aname = aname.substr( ns_sep_pos + 1 ); //remove anmespace part
	}
	desc<<" "<< aname <<"=\"";

	put_escaped( desc, attr[1], cstring_end_iterator(), true );
	desc<<"\"";
	attr += 2;
    }
    desc<<">";
    level ++;
}

void DescriptionExtractor::start( const char * el, const char **attr)
{
    switch(state){
	case STATE_WAIT_DESC:
	    if ( strcmp( el, "description" ) == 0){
		state = STATE_READING_DESC;
		level = 0;
	    }
	    break;
	case STATE_DONE:
	    break;//ignore
	case STATE_READING_DESC:
	    write_tag( el, attr );
	    break;
	default:
	    assert( false );
    }
    return;
}
void DescriptionExtractor::end( const char * el)
{
    //pass
    if ( state == STATE_READING_DESC){
	if ( strcmp(el, "description" )==0 ){
	    state = STATE_DONE;
	    if ( level != 0){
		cerr<<"Warning! description ended not at level 0:"<<level<<endl;
	    }
	}else{
	    level -- ;
	    desc<<"</"<<el<<">\n";
	}
    }
}
void DescriptionExtractor::character( const char * chars, int len )
{
    switch(state){
	case STATE_READING_DESC:
	    put_escaped( desc, chars, chars+len );
	    break;
	case STATE_WAIT_DESC:
	case STATE_DONE:
	    break;//ignore
	default:
	    assert( false );
    }
}

/************************** Main code *********************/
void process_file( zip* archive, const string & archive_name, int index )
{
    ZipReader reader ( archive, index );
    
    const char * file_name = zip_get_name( archive, index, 0);
    if (!file_name ){
	cerr<<"Zero archive name!"<<endl;
	return;
    }

//    LimitedReader limReader( reader, 20*1024 );
    DescriptionExtractor ext;

    cout<<"<book>"<<endl;
    cout<<"    <archive>";
    put_escaped( cout, archive_name.begin(), archive_name.end());
    cout<<"</archive>"<<endl;

    cout<<"    <file>";
    put_escaped(cout, file_name, cstring_end_iterator() );
    cout<<"</file>"<<endl;

    if (ext.extract( reader ) ){
	cout << "<description>"<<endl<<ext.get_desc()<<"</description>"<<endl;
	
    }else{
	cout<<"<error/>"<<endl;

	cerr<<"Error at line:"<<ext.current_line()<<endl;//<<": "<<ext.get_error_message();
	cerr<<"Broken xml: "<<archive_name<<":"<<file_name<<endl;
	
    }
    cout<<"</book>"<<endl;
}

int process_archive( const char * fpath )
{
    string archive_name( fpath );
    int errp = 0;
    zip* arch = zip_open( fpath, 0/*flags*/, &errp );
    if (! arch ){
	cerr<<"Error #"<<errp<<" when opending ZIP file"<<endl;
	return errp;
    };
    //reading file contents
    int num_files = zip_get_num_files( arch );
    for ( int i = 0; i<num_files; ++i){
	process_file( arch, archive_name, i );
    }
    errp = zip_close( arch );
    if ( errp != 0 ){
	cerr<<"Error closing zip file: #"<<errp<<endl;
	return errp;
    };
    return 0;
}
void show_help()
{
    cout<<" usage: program fil1.zip file2.zip ... >output.xml"<<endl;
}
int main(int argc, char *argv[])
{
//    const char* fpath = "/home/dim/Download/Books/lib.rus.ec/94671-95816.zip";
    if (argc==1){
	show_help();
	return 0;
    };

    cout<<"<library>"<<endl;
    for (int i =1; i<argc; ++i){
	process_archive( argv[i] );
    };
    cout<<"</library>"<<endl;
    return 0;
}
