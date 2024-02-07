/**
 * A library for the text buffer class (which we can then render to the screen).
*/

#ifndef UTILS_BUFFER
#define UTILS_BUFFER

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_MAX 1024

/**
 * A class that stores a single buffer of text and a pointer to another buffer of text.
 * This class will be used to implement linked lists.
 * @class
*/
typedef struct Buffer Buffer;

struct Buffer {
  char *sText;  // The content of the buffer instance
  int dWidth;   // The render length of the stored string (excluding ANSI escape sequences)
  int dHeight;  // The height of the chain of buffers; only has a value if the buffer node represents the head

  Buffer *pNext;  // A reference to an adjacent node
};

// Constructors and destructors
Buffer *Buffer_new();
Buffer *Buffer_init(Buffer *this);
Buffer *Buffer_create();
void Buffer_kill(Buffer *this);

// Getters and setters
char *Buffer_getText(Buffer *this);
void Buffer_setText(Buffer *this, char *sNewText);

// Operations
void Buffer_clearText(Buffer *this);
void Buffer_addText(Buffer *this, char *sAddText);

/**
 * //
 * ////
 * //////    Buffer constructors and destructors
 * ////////
 * ////////// 
*/

/**
 * Creates a new instance of the buffer class.
 * 
 * @return  { Buffer * }  A pointer to the created instance.
*/
Buffer *Buffer_new() {
  Buffer *pBuffer = calloc(1, sizeof(*pBuffer));

  if(pBuffer == NULL)
    return NULL;

  return pBuffer;
}

/**
 * Initializes an instance of the buffer class.
*/
Buffer *Buffer_init(Buffer *this) {
  this->dWidth = 0;
  this->dHeight = 0;

  return this;
}

/**
 * Creates an initialized instance of the buffer class.
 * 
 * @return  { Buffer * }  A pointer to the created instance.
*/
Buffer *Buffer_create() {
  Buffer *pBuffer = Buffer_new();

  Buffer_init(pBuffer);

  return pBuffer;
}

/**
 * Deletes an instance of the buffer class.
 * 
 * @param   { Buffer * }  A pointer to the instance.
*/
void Buffer_kill(Buffer *this) {
  free(this);
}

/**
 * //
 * ////
 * //////    Buffer getters and setters
 * ////////
 * ////////// 
*/

char *Buffer_getText(Buffer *this) {
  return this->sText;
}

void Buffer_setText(Buffer *this, char *sNewText) {
  int dBytes = snprintf(NULL, 0, "%s", sNewText) + 1;

  Buffer_clearText(this);
  this->sText = malloc(dBytes);

  snprintf(this->sText, dBytes, "%s", sNewText);
}

/**
 * //
 * ////
 * //////    Buffer operations
 * ////////
 * ////////// 
*/

void Buffer_clearText(Buffer *this) {
  free(this->sText);
}

void Buffer_addText(Buffer *this, char *sAddText) {
  int dBytes = snprintf(NULL, 0, "%s%s", this->sText, sAddText) + 1;

  char *sNewText = malloc(dBytes);

  Buffer_clearText(this);
  this->sText = sNewText;
}

#endif