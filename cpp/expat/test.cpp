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
#include <string>
#include "expat_wrapper.hpp"
#define BUFFSIZE 8192
char Buff[BUFFSIZE];

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
	bool done;
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

