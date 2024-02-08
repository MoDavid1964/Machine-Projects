/**
 * A library for the text buffer class (which we can then render to the screen).
*/

#ifndef UTILS_BUFFER
#define UTILS_BUFFER

#include <stdio.h>
#include <stdlib.h>

// Maximum length of a buffer
// Includes characters not rendered to the screen, such as:
//    (1) ANSI Escape Sequences
//    (2) The null byte
#define BUFFER_MAX_LENGTH 1024

/**
 * A class that stores a single buffer of text and a pointer to another buffer of text.
 * This class will be used to implement linked lists.
 * @class
*/
typedef struct Buffer Buffer;

struct Buffer {
  char *sText;        // The content of the buffer instance
  int dWidth;         // The render length of the stored string (excluding ANSI escape sequences)
  int dRenderWidth;   // The maximum number of *rendered* characters in the string
  int dHeight;        // The height of the chain of buffers; only has a value if the buffer node represents the head

  Buffer *pNext;  // A reference to an adjacent node
};

// Constructors and destructors
Buffer *Buffer_new();
Buffer *Buffer_init(Buffer *this, int dRenderWidth);
Buffer *Buffer_create(int dRenderWidth);
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
Buffer *Buffer_init(Buffer *this, int dRenderWidth) {
  this->sText = calloc(BUFFER_MAX_LENGTH, sizeof(char));

  this->dWidth = 0;
  this->dRenderWidth = dRenderWidth;
  this->dHeight = 0;

  return this;
}

/**
 * Creates an initialized instance of the buffer class.
 * 
 * @return  { Buffer * }  A pointer to the created instance.
*/
Buffer *Buffer_create(int dRenderWidth) {
  Buffer *pBuffer = Buffer_new();

  Buffer_init(pBuffer, dRenderWidth);

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

/**
 * Returns a reference to the text stored by the buffer.
 * 
 * @param   { Buffer * }  this  The buffer instance to be read.
 * @return  { char * }          The pointer to the string stored by the buffer.
*/
char *Buffer_getText(Buffer *this) {
  return this->sText;
}

/**
 * Replaces the contents of the string in the buffer.
 * 
 * @param   { Buffer * }  this      The buffer instance to be modified.
 * @param   { char * }    sNewText  The new string to be copied.
*/
void Buffer_setText(Buffer *this, char *sNewText) {
  Buffer_clearText(this);
  snprintf(this->sText, BUFFER_MAX_LENGTH, "%s", sNewText);
}

/**
 * //
 * ////
 * //////    Buffer operations
 * ////////
 * ////////// 
*/

/**
 * Clears the current contents of a buffer instance.
 * Resets the current renderable string width to 0.
 * 
 * @param   { Buffer * }  this  The buffer instance to be modified.
*/
void Buffer_clearText(Buffer *this) {
  free(this->sText);

  this->dWidth = 0;
  this->sText = calloc(BUFFER_MAX_LENGTH, sizeof(char));
}

/**
 * Appends text to the end of the string stored by the buffer.
 * 
 * @param   { Buffer * }  this      The buffer instance to be modified.
 * @param   { char * }    sAddText  The string to be appended.
*/
void Buffer_addText(Buffer *this, char *sAddText) {
  snprintf(this->sText, BUFFER_MAX_LENGTH, "%s%s", this->sText, sAddText);
}

#endif