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


/* Returns: 
 >= 0 - successfull, 0 means conversion doesn't use multibyte sequences 
   -1 - error during iconv_open call 
   -2 - error during iconv_close call 
   ---------------------------------- 
   This function expects that multibyte encoding in 'charset' wouldn't have 
   characters with more than 3 bytes. It is not intended to convert UTF-8 because 
   we'll never receive UTF-8 in our handler (it is handled by Exat itself). 
*/ 
int fill_encoding_info (const char *charset, XML_Encoding * info) 
{ 
  iconv_t cd = (iconv_t)(-1); 
  int flag; 
  printf ("Using encpoding %s \n", charset );

#if G_BYTE_ORDER == G_LITTLE_ENDIAN 
  cd = iconv_open ("UCS-2LE", charset); 
#else 
  cd = iconv_open ("UCS-2BE", charset); 
#endif 
 
  if (cd == (iconv_t) (-1)){ 
      printf("Failed to open iconv\n");
      return -1; 
  } 
 
  { 
    unsigned short out; 
    unsigned char buf[4]; 
    unsigned int i0, i1, i2; 
    int result; 
    flag = 0; 
    for (i0 = 0; i0 < 256; i0++) 
    { 
        buf[0] = i0; 
        info->map[i0] = 0; 
        //result = try (cd, buf, 1, &out); 
        if (result < 0) 
	{ 
	} 
        else if (result > 0) 
	{ 
	    printf(" #### map[%d]=%d\n", i0, out );
            info->map[i0] = out; 
	} 
        else 
	{ 
            for (i1 = 0; i1 < 0x100; i1++) 
	    { 
                buf[1] = i1; 
                ///result = try (cd, buf, 2, &out); 
                if (result < 0) { 
		} 
                else if (result > 0) { 
                    flag++; 
                    info->map[i0] = -2; 
		} 
                else { 
                    for (i2 = 0; i2 < 0x100; i2++) 
		    { 
                        buf[2] = i2; 
                        ////result = try (cd, buf, 3, &out); 
                        if (result < 0) 
			{ 
                          } 
                        else if (result > 0) 
                          { 
			      flag++; 
			      info->map[i0] = -3; 
                          } 
                      } 
		} 
	    } 
          } 
    } 
  } 
 
  if (iconv_close (cd) < 0) 
  { 
      printf("Failed to close iconv\n");
      return -2; 
  } 
  return flag; 
} 

static int 
iconv_convertor (void *data, const char *s) 
{ 
    XML_Encoding *info = (XML_Encoding*) data; 
    int res; 

  if (s == NULL) 
    return -1; 
  return -1; 
} 

static void 
my_release (void *data) 
{ 
  iconv_t cd = (iconv_t) data; 
  if (iconv_close (cd) != 0) 
    { 
/// TODO: uh no.      exit (1); 
    } 
} 

int 
handle_unknown_xml_encoding (void *encodingHandleData, 
			     const XML_Char * name, 
			     XML_Encoding * info) 
{ 
  int result; 
  result = fill_encoding_info (name, info); 
  if (result >= 0) 
  { 
      /*  
	  Special case: client asked for reverse conversion, we'll provide him with 
	  iconv descriptor which handles it. Client should release it by himself. 
      */ 
      if(encodingHandleData != NULL) 
	  *((iconv_t *)encodingHandleData) = iconv_open(name, "UTF-8"); 
      /*  
	  Optimization: we do not need conversion function if encoding is one-to-one,  
	  info->map table will be enough  
      */ 
      if (result == 0) 
      { 
          info->data = NULL; 
          info->convert = NULL; 
          info->release = NULL; 
	  printf("Using empty converter\n");
          return 1; 
      } 
      /*  
	  We do need conversion function because this encoding uses multibyte sequences 
      */ 
      info->data = (void *) iconv_open ("UTF-8", name); 
      if ((int)info->data == -1) 
	  return -1; 
      info->convert = iconv_convertor; 
      info->release = my_release; 
      printf("Using multibyte converter\n");
      return 1; 
  } 
  if(encodingHandleData != NULL)  
      *(iconv_t *)encodingHandleData = NULL; 
  printf("Failed to find a converter\n");
  return 0; 
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

