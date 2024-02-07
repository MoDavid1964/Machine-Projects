/**
 * A helper struct to store an object of text.
 * Acts like an output buffer which we can output all at once after it has been formatted.
*/

#ifndef UTILS_TEXT
#define UTILS_TEXT

#include <string.h>
#include <stdlib.h>

#include "../utils/utils.io.h"

#define UTILS_TEXT_MAX_LINES 1024

/**
 * An instantiable that acts like a buffer of text and enables formatting displays.
 * Stores an array of text.
*/
struct UtilsText {
  char *sTextArray[UTILS_TEXT_MAX_LINES];
  int dLength;
};

/**
 * Enum for text alignment.
*/
enum UtilsText_Alignment { 
  UTILS_TEXT_CENTER_ALIGN, 
  UTILS_TEXT_LEFT_ALIGN, 
  UTILS_TEXT_RIGHT_ALIGN
};

/**
 * ###########################
 * ###  TEXT CONSTRUCTION  ###
 * ###########################
*/

/**
 * Returns a new instance of the UtilsText class.
 * 
 * @return  {struct UtilsText *}  A pointer to the created instance.
*/
struct UtilsText *UtilsText_new() {
  struct UtilsText *pUtilsText;

  pUtilsText = calloc(1, sizeof(*pUtilsText));

  if(pUtilsText == NULL)
    return NULL;
  
  return pUtilsText;
}

/**
 * Initializes the object.
 * 
 * @param   {struct UtilsText *}  this  The instance to be initialized.
*/
void UtilsText_init(struct UtilsText *this) {
  this->dLength = 0;
}

/**
 * Creates an initialized instance of the class.
 * 
 * @return  {struct UtilsText *}  The created instance.
*/
struct UtilsText *UtilsText_create() {  
  struct UtilsText *pUtilsText = UtilsText_new();
  UtilsText_init(pUtilsText);

  return pUtilsText;
}

/**
 * Destroys a specified instance.
 * 
 * @param   {struct UtilsText *}  this  The instance to be destroyed.
*/
void UtilsText_kill(struct UtilsText *this) {
  free(this);
}

/**
 * ##################################
 * ###  TEXT READERS AND WRITERS  ###
 * ##################################
*/

/**
 * Adds a line of a repeated string pattern.
 * 
 * @param   {struct UtilsText *}  this      The instance to be modified.  
 * @param   {int}                 dLines    The number of such lines to be added.
 * @param   {char *}              sPattern   The string to be repeated.
*/
void UtilsText_addPatternLines(struct UtilsText *this, int dLines, char *sPattern) {
  int dWidth = UtilsIO_getWidth();

  while(dLines-- && this->dLength < UTILS_TEXT_MAX_LINES) {
    int dLength = 0;
    char *sLine = calloc(dWidth + 1, sizeof(char));

    while(dLength < dWidth - strlen(sPattern)) {
      strcat(sLine, sPattern);
      dLength += strlen(sPattern);
    }

    this->sTextArray[this->dLength] = calloc(dWidth + 1, sizeof(char));
    this->sTextArray[this->dLength] = sLine;
    this->dLength++;
  }
}

/**
 * Adds the specified number of newlines to the array.
 * 
 * @param   {struct UtilsText *}  this    The instance to be modified.  
 * @param   {int}                 dLines  The number of newlines to be done.
*/
void UtilsText_addNewLines(struct UtilsText *this, int dLines) {
  UtilsText_addPatternLines(this, dLines, " ");
}

/**
 * Adds a new string to the instance and updates the length variable.
 * If the array has already reached the maximum number of lines, nothing happens.
 * 
 * @param   {struct UtilsText *}  this    The instance to be modified.  
 * @param   {char *}              sText   The string to be added.
*/
void UtilsText_addText(struct UtilsText *this, char *sText) {
  if(this->dLength < UTILS_TEXT_MAX_LINES) {
    int dWidth = UtilsIO_getWidth();

    this->sTextArray[this->dLength] = calloc(dWidth + 1, sizeof(char));
    if(strlen(sText) > dWidth) sText[dWidth] = 0;
    strcpy(this->sTextArray[this->dLength], sText);
    
    this->dLength++;
  }
}

/**
 * Adds a new string to the instance and updates the length variable.
 * If the array has already reached the maximum number of lines, nothing happens.
 * 
 * @param   {struct UtilsText *}  this        The instance to be modified.  
 * @param   {char **}             sTextArray  The array of strings to be added.
 * @param   {int}                 dLines      The number of lines in the array.
*/
void UtilsText_addBlock(struct UtilsText *this, char **sTextArray, int dLines) {
  for(int i = 0; i < dLines; i++)
    UtilsText_addText(this, sTextArray[i]);
}

/**
 * Returns the array of strigns stored by the struct.
 * 
 * @param   {struct UtilsText *}  this    The instance to be read. 
 * @return  {char **}                     The array of strings to be returned.
*/
char **UtilsText_getText(struct UtilsText *this) {
  return this->sTextArray;
}

/**
 * Returns one of the stored lines of text.
 * 
 * @param   {struct UtilsText *}  this    The instance to be read. 
 * @param   {int}                 dIndex  The index of the string to be read. 
 * @return  {char *}                      The line of text with the given index.
*/
char *UtilsText_getTextLine(struct UtilsText *this, int dIndex) {

  // Just in case index is out of bounds
  if(dIndex < this->dLength)
    return this->sTextArray[dIndex];
  return "";
}

/**
 * Returns the number of lines stored by the instance.
 * 
 * @param   {struct UtilsText *}  this  The instance to be read.
 * @return  {int}                       The length of the stored string array.
*/
int UtilsText_getLines(struct UtilsText *this) {
  return this->dLength;
}

/**
 * Combines a source UtilsText object with a destination UtilsText object.
 * 
 * @param   {struct UtilsText *}  this        The instance to be modified.  
 * @param   {struct UtilsText *}  pSource     The source for the text to be merged.
*/
void UtilsText_mergeText(struct UtilsText *this, struct UtilsText *pSource) {
  for(int i = 0; i < UtilsText_getLines(pSource); i++)
    UtilsText_addText(this, UtilsText_getTextLine(pSource, i));
}

/**
 * ################################
 * ###  TEXT UTILITY FUNCTIONS  ###
 * ################################
*/

/**
 * A helper function to created a line of text padded with a certain character set.
 * 
 * @param   {char *}                    sText       The text to be padded.
 * @param   {char *}                    sPadText    The text to be used to pad the content.
 * @param   {enum UtilsText_Alignment}  eAlignment  Whether or not the padding will be applied to both sides.
 * @return  {char *}                                The generated padded text.
*/
char *UtilsText_paddedText(char *sText, char *sPadText, enum UtilsText_Alignment eAlignment) {

  // Width of console; length of text; length of pattern
  int dWidth = UtilsIO_getWidth() - 1;
  int dLength = strlen(sText);
  int dPadLength = strlen(sPadText);

  // Iterator; amount of extra
  int i = 0, dExtras = 0;
  int dPadding = 0;

  char *sOutput = calloc(dWidth + 1, sizeof(char));

  // Conditional just to check if text overflows the console
  if(dLength > dWidth) {
    strcat(sOutput, sText);
  } else {
    switch(eAlignment) {

      // Center alignment
      case UTILS_TEXT_CENTER_ALIGN:
        dPadding = (dWidth - dLength) / 2;

        // Pad the text along both sides
        dExtras = 0;
        for(i = 0; i < dPadding - dPadLength + 1; i += dPadLength) strcat(sOutput, sPadText);
        for(     ; i < dPadding - 1; i++) {
          strcat(sOutput, " ");
          dExtras++; 
        }
        
        strcat(sOutput, sText);

        for(i = 0; i < dExtras; i++) strcat(sOutput, " ");
        for(i = 0; i < dPadding - 1; i += dPadLength) strcat(sOutput, sPadText);
        break;

      // Left alignment
      case UTILS_TEXT_LEFT_ALIGN:
        dPadding = dWidth - dLength;
        strcat(sOutput, sText);

        // Pad the text to the right
        dExtras = 0;
        for(i = 0; i < dPadding - dPadLength + 1; i += dPadLength) strcat(sOutput, sPadText);
        break;

      // Right alignment
      case UTILS_TEXT_RIGHT_ALIGN:
        dPadding = dWidth - dLength;

        // Pad the text to the left
        for(i = 0; i < dPadding - dPadLength + 1; i += dPadLength) strcat(sOutput, sPadText);
        for(     ; i < dPadding; i++) strcat(sOutput, " ");

        strcat(sOutput, sText);
        break;

      default:
        break;
    }
  }

  return sOutput;
}

/**
 * A helper function to create a single line of text centered along the width of the console.
 * 
 * @param   {char *}  sText   The text to be centered.
 * @return  {char *}          The generated centered text.
*/
char *UtilsText_centeredText(char *sText) {
  return UtilsText_paddedText(sText, " ", UTILS_TEXT_CENTER_ALIGN);
}

#endif
