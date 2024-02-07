/**
 * A file containing some useful helper functions for the console UI.
 * Most of these functions work together with the UtilsText class.
 * Note that almost all the functions here return pointers to instances of the UtilsText class.
 * This allows me to chain operations on the said class.
*/

#ifndef UTILS_UI
#define UTILS_UI

#include <string.h>

#include "utils.io.h"
#include "utils.text.h"

#define UTILS_UI_MAX_LINE_LEN 1 << 10

/**
 * A struct to store some important info so we don't pollute the global namespace.
 * It's empty tho but I'll just keep it here in case I need to add something in the future.
*/
struct UtilsUI {

};

/**
 * #######################
 * ###  UI GENERATORS  ###
 * #######################
*/

/**
 * A helper function to create a lines of text centered along the width of the console.
 * 
 * @param   {struct UtilsText *}  pUtilsText  The text object whose contents will be centered.
 * @return  {struct UtilsText *}              The generated centered text.
*/
struct UtilsText *UtilsUI_centerX(struct UtilsText *pUtilsText) {

  // Width of console; length of text
  int dWidth = UtilsIO_getWidth();
  int dLines = UtilsText_getLines(pUtilsText);

  char **sTextArray = UtilsText_getText(pUtilsText);

  for(int i = 0; i < dLines; i++) {
    int dLength = strlen(sTextArray[i]);

    // Conditional just to check if text overflows the console
    if(dLength > dWidth) 
      pUtilsText->sTextArray[i] = sTextArray[i];
    else 
      pUtilsText->sTextArray[i] = UtilsText_centeredText(pUtilsText->sTextArray[i]);
  }

  return pUtilsText;
}

/**
 * A helper function to create text centered on the console.
 * This is honestly implemented in such a disgusting way; it accesses the struct members directly instead of using setters and getters.
 * I don't know why using the setters and getters don't seem to work here hmmm.
 * 
 * @param   {struct UtilsText *}  pUtilsText  An object containing an array of strings.
 * @return  {struct UtilsText *}              The object with its strings centered and formatted.
*/
struct UtilsText *UtilsUI_centerXY(struct UtilsText *pUtilsText) {

  // Height of console; number of lines
  int dHeight = UtilsIO_getHeight();
  int dLines = UtilsText_getLines(pUtilsText);

  // In case the text overflows
  if(dLines >= dHeight) {
    while(dLines--)
      pUtilsText->sTextArray[dLines] = UtilsText_centeredText(pUtilsText->sTextArray[dLines]);

    pUtilsText->dLength = dHeight;
  
  // Center along both width and height of the console
  } else {

    // Compute the padding along the height
    int dPaddingY = (dHeight - dLines) / 2;
    int dPaddingYCounter = dPaddingY;
    int dLine = dLines;

    // Shift the lines upward by the padding amount
    while(dLine--)
      pUtilsText->sTextArray[dLine + dPaddingY] = UtilsText_centeredText(pUtilsText->sTextArray[dLine]);

    // Add whitespace to pad
    while(dPaddingYCounter--) {
      pUtilsText->sTextArray[dPaddingYCounter] = " ";
      pUtilsText->sTextArray[dPaddingY + dLines + dPaddingYCounter] = " ";
    }

    // Update length
    pUtilsText->dLength = dLines + dPaddingY * 2;
  }

  return pUtilsText;
}

/**
 * Modifies the first few lines in the string array of the object to create a header.
 * 
 * @param   {struct UtilsText *}  pUtilsText  The original instance to be modified.
 * @param   {struct UtilsText *}  pHeader     The header whose content will be added to the original instance.
 * @return  {struct UtilsText *}              The modified instance with the header added.
*/
struct UtilsText *UtilsUI_header(struct UtilsText *pUtilsText, struct UtilsText *pHeader) {
  int dHeight = pUtilsText->dLength;
  int dHeaderLength = 0;

  // In case header doesn't fit the buffer
  if(pHeader->dLength > dHeight)
    dHeaderLength = dHeight;
  else
    dHeaderLength = pHeader->dLength;
  
  while(dHeaderLength--)
    pUtilsText->sTextArray[dHeaderLength] = pHeader->sTextArray[dHeaderLength];

  return pUtilsText;
}

/**
 * Modifies the last few lines in the string array of the object to create a footer.
 * 
 * @param   {struct UtilsText *}  pUtilsText  The original instance to be modified.
 * @param   {struct UtilsText *}  pFooter     The footer whose content will be added to the original instance.
 * @return  {struct UtilsText *}              The modified instance with the footer added.
*/
struct UtilsText *UtilsUI_footer(struct UtilsText *pUtilsText, struct UtilsText *pFooter) {
  int dHeight = pUtilsText->dLength;
  int dFooterLength = 0;

  // In case footer doesn't fit the buffer
  if(pFooter->dLength > dHeight)
    dFooterLength = dHeight;
  else
    dFooterLength = pFooter->dLength;

  while(dFooterLength--)
    pUtilsText->sTextArray[dHeight - 1 - dFooterLength] = pFooter->sTextArray[pFooter->dLength - 1 - dFooterLength];

  return pUtilsText;
}

/**
 * ######################
 * ###  UI UTILITIES  ###
 * ######################
*/

/**
 * A moderately useful helper function.
 * 
 * @param   {UtilsText *}   pUtilsText  The strings to be displayed.
*/
void UtilsUI_print(struct UtilsText *pUtilsText) {
  int dWidth = UtilsIO_getWidth();
  int dHeight = UtilsIO_getHeight();

  char *sOutput = calloc((dWidth + 1) * (dHeight + 2), sizeof(char));
  char **sTextArray = UtilsText_getText(pUtilsText);
  int dLength = UtilsText_getLines(pUtilsText);

  // Combine into one string so we only make one call to printf
  for(int i = 0; i < dLength; i++) {
    strcat(sOutput, sTextArray[i]);
    int dGap = dWidth - strlen(sTextArray[i]);
    
    // We have to fill each line buffer
    while(--dGap > 0)
      strcat(sOutput, " ");
    strcat(sOutput, "\n");
  }

  // Add extra lines to fill the buffer
  int dExtra = dHeight - dLength;
  while(dExtra-- > 0) {

    // Create a new line of space characters
    char sLine[dWidth];
    memset(sLine, ' ', sizeof(sLine));

    // We don't want a new line on the last line
    if(dExtra > 0)
      sLine[dWidth - 1] = '\n';

    strcat(sOutput, sLine);
  }
  strcat(sOutput, " ");
  
  // Calling it just once prevents a glitchy console output
  printf("%s", sOutput);
}

/**
 * Converts a string to uppercase.
 * 
 * @param   {char *}  sString   The string to be converted.
 * @return  {char *}            The capitalized version of the string.
*/
char *UtilsUI_toUpper(char *sString) {
  char *sCapitalizedString = calloc(strlen(sString), sizeof(char));
  char *sCharacterAsString = calloc(1, sizeof(char));
  strcpy(sCapitalizedString, "");

  // So messy goddamit C
  for(int i = 0; i < strlen(sString); i++) {
    sprintf(sCharacterAsString, "%c", toupper(sString[i]));
    strcat(sCapitalizedString, sCharacterAsString);
  }

  return sCapitalizedString;
}

/**
 * Creates a default sized string buffer.
 * 
 * @return  A pointer to the buffer in memory.
*/
char *UtilsUI_createBuffer() {
  return calloc(UTILS_UI_MAX_LINE_LEN, sizeof(char));
}

#endif
