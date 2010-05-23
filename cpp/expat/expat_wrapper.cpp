#include <iconv.h>
#include <cassert>
#include "expat_wrapper.hpp"

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
	return XML_STATUS_ERROR; 
    }
    info->data = NULL;
    info->convert = NULL;
    info->release = NULL;
    return XML_STATUS_OK;
}
