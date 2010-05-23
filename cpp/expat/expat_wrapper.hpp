#pragma once
#ifndef _EXPAT_WRAPPER_H_
#define _EXPAT_WRAPPER_H_
////////////////////////////////////////////////
// C++ wrapper for the expat (yeah, the self-made one )
//////////////////////////////////////////////
#include <expat.h>
#include <string>

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


#endif /* _EXPAT_WRAPPER_H_ */
