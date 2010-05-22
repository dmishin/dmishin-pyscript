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

#define BUFFSIZE	8192

char Buff[BUFFSIZE];

int Depth;

void
start(void *data, const char *el, const char **attr) {
    int i;

    for (i = 0; i < Depth; i++)
	printf("  ");

    printf("%s", el);

    for (i = 0; attr[i]; i += 2) {
	printf(" %s='%s'", attr[i], attr[i + 1]);
    }

    printf("\n");
    Depth++;
}  /* End of start handler */

void
end(void *data, const char *el) {
    Depth--;
}  /* End of end handler */

int 
handle_unknown_xml_encoding (void *encodingHandleData, 
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




int
main(int argc, char **argv) {
    XML_Parser p = XML_ParserCreate(NULL);
    if (! p) {
	fprintf(stderr, "Couldn't allocate memory for parser\n");
	exit(-1);
    }
    XML_SetElementHandler(p, start, end);
    XML_SetUnknownEncodingHandler( p, handle_unknown_xml_encoding, NULL);
    
    for (;;) {
	int done;
	int len;

	len = fread(Buff, 1, BUFFSIZE, stdin);
	if (ferror(stdin)) {
	    fprintf(stderr, "Read error\n");
	    exit(-1);
	}
	done = feof(stdin);

	if (! XML_Parse(p, Buff, len, done)) {
	    fprintf(stderr, "Parse error at line %d:\n%s\n",
		    (int)XML_GetCurrentLineNumber(p),
		    XML_ErrorString(XML_GetErrorCode(p)));
	    exit(-1);
	}

	if (done)
	    break;
    }
    return 0;
}  /* End of main */

