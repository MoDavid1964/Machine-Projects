/**
 * A library for the text buffer class (which we can then render to the screen).
*/

#ifndef UTILS_BUFFER
#define UTILS_BUFFER

#include <stdio.h>

/**
 * //
 * ////
 * //////    Buffer class infrastructure
 * ////////
 * ////////// 
*/

/**
 * A class that stores text buffers so we only have to call printf once.
 * @class
*/
typedef struct Buffer Buffer;

struct Buffer {
  char *text[]; 
};

#endif