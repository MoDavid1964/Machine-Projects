/**
 * @ Author: Mo David
 * @ Create Time: 2024-02-07 12:20:45
 * @ Modified time: 2024-02-09 01:27:43
 * @ Description:
 *    Defines the buffer class, which we use to store text strings.
 *    The class uses a linked list to store contiguous strings, as this allows faster insertion and deletion.
 */

#ifndef UTILS_BUFFER_
#define UTILS_BUFFER_

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

/**
 * Constructors and destructors
*/
Buffer *Buffer_new();

Buffer *Buffer_init(Buffer *this, int dRenderWidth);

Buffer *Buffer_create(int dRenderWidth);

void Buffer_kill(Buffer *this);

/**
 * Getters and setters
*/
char *Buffer_getText(Buffer *this);

/**
 * Operations
*/
int Buffer_updateRenderWidth(Buffer *this);

void Buffer_clearText(Buffer *this);

void Buffer_newText(Buffer *this, char *sNewText);

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
 * //
 * ////
 * //////    Buffer operations
 * ////////
 * ////////// 
*/

/**
 * Computes the render length of the string, exluding ANSI escape sequences.
 * 
 * @param   { Buffer * }  this  The buffer instance to be modified.
 * @return  { int }             The newly computed render length of the stored string.
*/
int Buffer_updateRenderWidth(Buffer *this) {

  // Reference for starting and ending ANSI escape sequences
  // Note that the termination character ("m") is the one used for color escape strings.
  // Color strings will be the only escape sequences we will use within the buffers.
  char sFront[2] = "\x1b";
  char sBack[2] = "m";

  // Some more vars
  int i = 0, j = 0;
  int dLen = strlen(this->sText);
  int bEscaped = 0;

  // Reset the width first
  this->dWidth = 0;

  do {

    // Check if it's the end of an escape string
    for(j = 0; j < strlen(sBack) && i + j < dLen; j++) {
      if(this->sText[i + j] != sBack[j])
        j = strlen(sBack) + 1;
    }

    // End of escape sequence
    if(j == strlen(sBack) && bEscaped) {
      bEscaped = 0;

      // Makes sure the terminating sequence isnt included in length
      this->dWidth--;
    }

    // Check if it's the start of an escape string
    for(j = 0; j < strlen(sFront) && i + j < dLen; j++) {
      if(this->sText[i + j] != sFront[j])
        j = strlen(sFront) + 1;
    }

    // If it's escaped
    if(j == strlen(sFront))
      bEscaped = 1;

    // Update width if it's not an escape string
    if(!bEscaped)
      this->dWidth++;

    // If the render length is already maxxed out
    // This also means that we cannot end the string with an ANSI escape sequence
    // If ever we do, the sequence will essentially be erased.
    if(this->dWidth > this->dRenderWidth)
      this->sText[i] = 0;

  } while(this->sText[++i] != 0);

  // Return the new value
  return this->dWidth;
}

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
 * Replaces the contents of the string in the buffer.
 * 
 * @param   { Buffer * }  this      The buffer instance to be modified.
 * @param   { char * }    sNewText  The new string to be copied.
*/
void Buffer_newText(Buffer *this, char *sNewText) {
  Buffer_clearText(this);
  snprintf(this->sText, BUFFER_MAX_LENGTH, "%s", sNewText);
  
  // Recompute render width
  Buffer_updateRenderWidth(this);
}

/**
 * Appends text to the end of the string stored by the buffer.
 * 
 * @param   { Buffer * }  this      The buffer instance to be modified.
 * @param   { char * }    sAddText  The string to be appended.
*/
void Buffer_addText(Buffer *this, char *sAddText) {
  snprintf(this->sText, BUFFER_MAX_LENGTH, "%s%s", this->sText, sAddText);

  // Recompute render width
  Buffer_updateRenderWidth(this);
}

#endif