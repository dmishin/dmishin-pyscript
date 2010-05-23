/*****************************************************************
 * outline.c
 *
 * Copyright 1999, Clark Cooper
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the same terms as Perl.
 *
 * Read an XML document from standard input and print an element
 * outline on standard output.
 */


#include <stdio.h>
#include <expat.h>
#include <iconv.h>
#include <string>
#include <cassert>

#define BUFFSIZE	8192

char Buff[BUFFSIZE];

////////////////////////////////////////////////
// C++ wrapper for the expat (yeah, the self-made one )
//////////////////////////////////////////////
class ExpatParser{
private:
    XML_Parser parser;

    static int  handle_unknown_xml_encoding (void *encodingHandleData, 
					     const XML_Char * name, 
					     XML_Encoding * info) ;
    
    static void start_handler(void *data, const char *el, const char **attr);
    static void end_handler(void *data, const char *el);
    static void character_handler( void* data, const char * chars, int len );
public:
    ExpatParser();
    virtual ~ExpatParser();

    int current_line();
    std::string get_error_message();
    XML_Status parse( const char * buff, int len, bool done);

    virtual void start( const char * el, const char **attr) = 0;
    virtual void end( const char * el) = 0;
    virtual void character( const char * chars, int len ) = 0;
};

///C++ wrapper implementation
ExpatParser::ExpatParser()
{
    parser = XML_ParserCreate( NULL );
    assert( parser );
    XML_SetUserData( parser, this );
    XML_SetElementHandler( parser, ExpatParser::start_handler, ExpatParser::end_handler );
    XML_SetUnknownEncodingHandler( parser, ExpatParser::handle_unknown_xml_encoding, NULL );
    XML_SetCharacterDataHandler( parser, ExpatParser::character_handler );
}
ExpatParser::~ExpatParser()
{
    if (parser){
	XML_ParserFree( parser );
	parser = NULL;
    }
}
int ExpatParser::current_line()
{		    
    assert( parser );
    return (int)XML_GetCurrentLineNumber( parser );
}
std::string ExpatParser::get_error_message()
{
    assert( parser );
    return XML_ErrorString(XML_GetErrorCode( parser ));
}
void ExpatParser::start_handler(void *data, const char *el, const char **attr)
{
    ExpatParser * self = reinterpret_cast<ExpatParser*>(data);
    assert( self );
    self->start( el, attr);
}
void ExpatParser::end_handler(void *data, const char *el)
{
    ExpatParser * self = reinterpret_cast<ExpatParser*>(data);
    assert( self );
    self->end( el );
}
void ExpatParser::character_handler( void* data, const char * chars, int len )
{
    ExpatParser * self = reinterpret_cast<ExpatParser*>(data);
    assert( self );
    self->character( chars, len );
}

XML_Status ExpatParser::parse( const char * data, int len, bool done )
{
    assert( parser ); assert( len >= 0 );
    return XML_Parse( parser, data, len, (int)done );
}

int ExpatParser::handle_unknown_xml_encoding (void *encodingHandleData, 
						     const XML_Char * name, 
						     XML_Encoding * info) 
{
    const size_t BUFFER_SUZE = 8;

    char out_buffer[ BUFFER_SUZE ];
    iconv_t coder = iconv_open ("UCS-2LE", name); 

    //Building the encoding table
    for ( unsigned int c=0; c<256; c++){
	//C is the character index
	//Convert this character, using the iconv conversion library
	char in_buffer = static_cast<char>(c);
	char * p_in_buffer = &in_buffer;
	size_t in_buffer_avail = 1;//1 byte available
	char * p_out_buffer = out_buffer;
	size_t out_buffer_avail =  BUFFER_SUZE;//8 bytes are freee
	int result = iconv( coder,
			    &p_in_buffer, &in_buffer_avail, 
			    &p_out_buffer, &out_buffer_avail );
	if (result < 0){
	    //some error occured
	    //either the encoding is non 1-byte,
	    //or coding is impossible.
	    info->map[ c ] = 0xFFFD; //unicode replacement character
	}else{
	    //determine output length
	    //output is little-endian.
	    int unicode_index = 0;
	    int shift = 0;
	    for( char * p_out = out_buffer; p_out != p_out_buffer; ++p_out ){
		unicode_index += static_cast<int>( static_cast<unsigned char>(*p_out) ) << shift; //read current 8 bits
		shift += 8;
	    }
	    //put the unicode codepoint to the encoding map
	    info->map[ c ] = unicode_index;
	}
    }
    if (iconv_close ( coder ) < 0) 
    { 
	printf("Failed to close iconv\n");
	return XML_STATUS_ERROR; 
    }
    info->data = NULL;
    info->convert = NULL;
    info->release = NULL;
    return XML_STATUS_OK;
}
///////////////////////////////////////////////
// Concreate parser implementation
///////////////////////////////////////////////

class OutlinePrinter: public ExpatParser{
    int Depth;
public:
    OutlinePrinter(): Depth(0){};
    virtual void start( const char * el, const char **attr );
    virtual void end( const char * el );
    virtual void character( const char * data, int len ) {};
};

void OutlinePrinter::start( const char * el, const char ** attr )
{
    int i;

    for (i = 0; i < Depth; i++)
	printf("  ");

    printf("%s", el);

    for (i = 0; attr[i]; i += 2) {
	printf(" %s='%s'", attr[i], attr[i + 1]);
    }

    printf("\n");
    Depth++;
}
void OutlinePrinter::end( const char * el )
{
    Depth--;
}

int main(int argc, char **argv) {
    OutlinePrinter p;
    
    for (;;) {
	int done;
	int len;

	len = fread(Buff, 1, BUFFSIZE, stdin);
	if (ferror(stdin)) {
	    fprintf(stderr, "Read error\n");
	    exit(-1);
	}
	done = feof(stdin);

	if (! p.parse( Buff, len, done) ) {
	    fprintf(stderr, "Parse error at line %d:\n%s\n",
		    p.current_line(),
		    p.get_error_message().c_str() );
	    exit(-1);
	}

	if (done)
	    break;
    }
    return 0;
}  /* End of main */

