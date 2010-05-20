#include <assert.h>
extern "C"{
#include <zip.h>
#include <iconv.h>
};

#include <iostream>
#include <vector>
#include <boost/regex.hpp>

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
/******************* Pushback reader********************
 */
class PushBackReader :public Reader{
private:
    std::vector<char> buffer;
    Reader & parent;
public:
    PushBackReader( Reader & rdr );
    virtual int read();
    void push( char c );
};

PushBackReader::PushBackReader( Reader & rdr )
    :parent( rdr )
{};

int PushBackReader::read()
{
    if (buffer.size() == 0){
	return parent.read();
    }else{
	char c = buffer.back();
	buffer.pop_back();
	return static_cast<int>(static_cast<unsigned char>(c));
    }
};

void PushBackReader::push( char c)
{
    buffer.push_back( c );
};

std::string readPreamble( PushBackReader & rdr, size_t max_preamble )
{
    std::string buffer;
    bool isFailed = false;
    while ( true ){
	int c = rdr.read();
	if ( c == -1 ){
	    isFailed = true;//unexpected eof;
	    break;
	}
	buffer.push_back( static_cast<char>(c) );
	if (buffer.size()>5 &&
	    buffer[buffer.size()-1] == '>' &&
	    buffer[buffer.size()-2] == '?'){
	    //nd found
	    isFailed = false;
	    break;
	}
	if ( buffer.size() >= max_preamble){
	    isFailed = true;//too long
	    break;
	}
    }
    //now buffer contains preamble.
    //sh back all data
    string::const_reverse_iterator rb, re=buffer.rend();
    for (rb = buffer.rbegin() ; rb != re; ++ rb ){
	rdr.push( *rb );
    };
    if ( ! isFailed )
	return buffer;
    else
	return string();
};


bool parsePreamble( const std::string & preamble, std::string & encoding )
{
    static boost::regex preamble_regex( "<\\?xml.*encoding=\"([^\"]+)\".*\\?>" );

    //using boost regex to parse
    boost::match_results<std::string::const_iterator> match;
    bool isMatched = boost::regex_match( 
	preamble.begin(), preamble.end(),
	match,
	preamble_regex);
    if ( isMatched ){
	encoding = match.str( 1 );
    }else{
	encoding = "";
    }
    return isMatched;
};

std::string getEncoding( PushBackReader & rdr )
{
    std::string enc;
    std::string preamble = readPreamble( rdr, 512 );//max preamble size
    parsePreamble( preamble, enc );
    return enc;
}

template< class IT1, class IT2 >
bool begins_with( IT1 begin1, IT1 end1, IT2 begin2, IT2 end2 )
{
    IT1 i1 = begin1;
    IT2 i2 = begin2;
    while ( i1 != end1 && i2 != end2 ){
	if ( *i1 != *i2 ){
	    return false;
	}
	i1++;
	i2++;
    }
    return i2 == end2;
};

bool isNextText( PushBackReader & rdr, const std::string & text, int pos )
{
    if ((size_t)pos >= text.size())
	return true;//nothing to match
    char c = text[pos];
    int rc = rdr.read();
    if (rc == -1){
	return false;//premature end
    };
    bool rval;
    if (c != static_cast<char>(rc)){
	rval = false;
    }else{
	rval = isNextText( rdr, text, pos + 1);
    }
    rdr.push( static_cast<char>(rc) );
    return rval;
}

/**Skip pushback stream, until given string is found*/
bool skipUntil( PushBackReader & rdr, const std::string & text)
{
    if ( text.size()==0 ) //nothing left to skip
	return true;
    char charToSkip = text[0];
    while( true ){
	int c = rdr.read();
	if (c==-1){
	    return false;//failed to skip, end of file
	}
	if ( static_cast<char>(c) == charToSkip ){
	    //probably, we can skip. Trying to skip the rest of the stirng
	    if (isNextText(rdr, text, 1)){
		//yeah, it skipped successfully!
		rdr.push( charToSkip );//return the char
		return true;//skipp success
	    }
	}
    }
}
/**Extract piece of text between two tags*/
std::string extractPiece( PushBackReader & rdr, const std::string & begin, const std::string & end)
{
    std::string rval;
    if ( !skipUntil( rdr, begin ) ){
	return std::string();
    }
    while (true){
	int c = rdr.read();
	if (c == -1){
	    //premature end
	    cerr<<"Premature end"<<endl;
	    cerr<<rval<<endl;
	    return string();
	};
	rval.push_back(static_cast<char>(c));
	if (begins_with( rval.rbegin(), rval.rend(), end.rbegin(), end.rend())){
	    //reading done
	    return rval;
	}
    }
}
/************************** Change encoding  *********************/
bool convertToUTF8( const std::string & s, const std::string & encoding, std::string & output)
{
    if ( s.size() == 0){
	output = string();
	return true;//no need to encode
    }
    int rval;
    iconv_t ic = iconv_open( "UTF-8", encoding.c_str() );
    if (! ic ){
	cerr<<"Failed to init iconv for encoding "<<encoding<<endl;
	return false;
    }

    const char * src = &(s[0]);

    output.clear();

    size_t bufferSize = s.size()*4;
    char* buffer = new char[ bufferSize ];//should be enough for anything
    

    size_t input_left = s.size(),
	output_left = bufferSize;

    char * pbuffer = buffer;
    size_t nconv = iconv( ic,
			  (char**)(&src), &input_left,
			  &pbuffer, &output_left);
    if (nconv == (size_t)(-1)){
	//failure. Input sequence terminated
	cerr<<"Some error"<<endl;
    }else{
	//converter successfully
	insert_iterator<string> ii (output, output.begin());
	std::copy( buffer, pbuffer, ii);
    }
    delete[] buffer;

    rval = iconv_close( ic );
    if ( rval != 0){
	cerr<<"Failed to close iconv for encoding "<<encoding<<" with error code"<<rval<<endl;
	return false;
    }
    return true;
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

    LimitedReader limReader( reader, 20*1024 );
    PushBackReader pbReader( limReader );
    string enc = getEncoding( pbReader );
    string desc = extractPiece( pbReader, "<description>", "</description>" );

    if (! enc.empty()){
	string descUTF8;
	if (convertToUTF8( desc, enc, descUTF8 ) ){
	    desc.swap(descUTF8);
	}
    }

    cout<<"<book>"<<endl;
    cout<<"    <archive>"<<archive_name<<"</archive>"<<endl;
    cout<<"    <file>"<<file_name<<"</file>"<<endl;
    cout<<desc<<endl;
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

    for (int i =1; i<argc; ++i){
	cout<<"<library>"<<endl;
	process_archive( argv[i] );
	cout<<"</library>"<<endl;
    };
    return 0;
}
