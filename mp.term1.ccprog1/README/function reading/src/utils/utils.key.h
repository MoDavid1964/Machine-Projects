/**
 * A file containing some useful helper functions for keyboard related interactions.
 * The main way IO is incorporated in the game is through input polling done with a while loop.
 * Callback functions are passed into the keyboard input handlers below.
 * Note that all of the current warnings in the .log.txt file are because of the functions in this file.
*/

#ifndef UTILS_KEY
#define UTILS_KEY

#include <string.h>

#include "utils.io.h"
#include "utils.selector.h"

#define UTILS_KEY_MAX_DIGITS 8
#define UTILS_KEY_MAX_INPUT 256

/**
 * A struct to store some important info so we don't pollute the global namespace.
 * It's empty tho but I'll just keep it here in case I need to add something in the future.
*/
struct UtilsKey {
  
};

/**
 * ########################
 * ###  INPUT HANDLING  ###
 * ########################
*/

/**
 * Enables some basic interaction with the game by returning the uppercase version of a character.
 * I don't know why I didn't realize until after the entire ordeal of coding this project that toupper() exists.
 * 
 * @return  {char}  Returns the uppercase alphabet character of the input.
*/
char UtilsKey_uppercaseChar() {

  // Read input
  char cInput = UtilsIO_readChar();

  // If it's already uppercase
  if(64 < cInput && cInput < 91)
    return cInput;
  
  // If it's lowercase
  if(96 < cInput && cInput < 123)
    return cInput - 32;
  
  // A bit redundant but I prefer the readability smhh
  return cInput;
}

/**
 * Enables input to be polled from the user.
 * Input will constantly be asked from the user until enter is pressed.
 * Uses uppercase version of alpha characters.
 * 
 * @param   {int (*)(char cInput)}                fCondition    The truth value of this callback determines whether the loop will still keep executing.
 * @param   {void (*)(char cInput, void *pData)}  fPreProcess   A callback function to be executed before each read instance of the loop.
 * @param   {void (*)(char cInput, void *pData)}  fPostProcess  A callback function to be executed after each read instance of the loop.
 * @param   {void *}                              pData         Some additional data we might want to give to post and pre.
 * @param   {char *}                              sReference    A string that contains characters that terminate the loop.
*/
void UtilsKey_inputPoll(
  int (*fCondition)(char cInput, char *sReference),
  void (*fPreProcess)(char cInput, void *pData),
  void (*fPostProcess)(char cInput, void *pData), 
  void *pData, char *sReference) {
  
  char cInput = 0;

  do {

    // Initial callback, then read character, the final callback
    (*fPreProcess)(cInput, pData); cInput = UtilsKey_uppercaseChar(); (*fPostProcess)(cInput, pData);

  // Condition has to come after because input is initialized to 0
  } while((*fCondition)(cInput, sReference));
}

/**
 * Enables string input from the user.
 * Terminates when enter is pressed.
 * 
 * @param   {void (*)(char* sInput, void *pData)}   fProcess      A callback function to be executed before each read instance of the loop.
 * @param   {void (*)(char* sInput, void *pData)}   fExitProcess  Final callback for when enter has been pressed.
 * @param   {void *}                                pData         Some additional data we might want to give to post and pre.
*/
void UtilsKey_inputString(
  void (*fProcess)(char* sInput, void *pData),
  void (*fExitProcess)(char* sInput, void *pData),
  void *pData) {
  
  char *sInput = UtilsIO_newInputStr();

  do {

    // Callback on the input
    (*fProcess)(sInput, pData);

  // Condition has to come after because input is initialized to 0
  } while(UtilsIO_inputStrOut(sInput));

  // Final callback when enter has been pressed
  (*fExitProcess)(sInput, pData);
}

/**
 * ##########################
 * ###  INPUT PROCESSING  ###
 * ##########################
*/

/**
 * Returns whether or not a character represents a digit character.
 * 
 * @param   {char}  cInput  The character to be evaluated.
 * @return  {int}   A boolean indicating whether or not the character is a digit.
*/
int UtilsKey_isNum(char cInput) {
  return 47 < cInput && cInput < 58;
}

/**
 * Returns whether or not a character represents an alphabet character.
 * 
 * @param   {char}  cInput  The character to be evaluated.
 * @return  {int}           A boolean indicating whether or not the character is in the standard alphabet.
*/
int UtilsKey_isAlpha(char cInput) {
  char cInputCaps = toupper(cInput);
  
  return 64 < cInputCaps && cInputCaps < 91;
}

/**
 * Returns whether or not the character is a newline or EOF.
 * I know it's kinda redundant with UtilsIO_isReturn, 
 *    BUT I don't wanna have to access two different header files when coding the other game components.
 * 
 * @param   {char}    cInput  The character to be evaluated.
 * @param   {char*}   sDummy  Dummy input to conform with callback requirements.
 * @return  {int}             A boolean indicating whether the character is a newline or EOF.
*/
int UtilsKey_isReturn(char cInput, char *sDummy) {
  return UtilsIO_isReturn(cInput);
}

/**
 * Returns whether or not the character is a backspace or delete character.
 * 
 * @param   {char}    cInput  The character to be evaluated.
 * @param   {char*}   sDummy  Dummy input to conform with callback requirements.
 * @return  {int}             A boolean indicating whether the character is a backspace.
*/
int UtilsKey_isBackspace(char cInput, char *sDummy) {
  return UtilsIO_isBackspace(cInput);
}

/**
 * Just another helper function.
 * 
 * @param   {char}    cInput  The character to be evaluated.
 * @param   {char*}   sDummy  Dummy input to conform with callback requirements.
 * @return  {int}             A boolean indicating whether the character is NOT a newline or EOF.
*/
int UtilsKey_isNotReturn(char cInput, char *sDummy) {
  return !UtilsIO_isReturn(cInput);
}

/**
 * If character is not in char array.
 * 
 * @param   {char}    cInput      The character to be evaluated.
 * @param   {char *}  sReference  The character set to be checked with.
 * @return  {int}                 Boolean that returns true if character is not in the char array.
*/
int UtilsKey_isNot(char cInput, char *sReference) {
  for(int i = 0; i < strlen(sReference); i++) {
    if(!(cInput - sReference[i]))
      return 0;
  }
  
  return 1;
}

/**
 * A dummy function we can use to instantly terminate the inputPoll method.
 * 
 * @param   {char}    cInput      The character to be evaluated.
 * @param   {char *}  sReference  The character set to be checked with.
 * @return  {int}                 A default falsy value.
*/
int UtilsKey_dummy(char cInput, char *sReference) {
  return 0;
}

/**
 * ##########################
 * ###  HELPER FUNCTIONS  ###
 * ##########################
*/

/**
 * Converts positive integer strings into their respective int values.
 * 
 * @param   {char *}  sInt  The string version of the integer to be converted.
 * @return  {int}           The value of the said string.
*/
int UtilsKey_stringToInt(char *sInt) {
  
  // The max value we can convert is 999999999 (nine nines)
  if(strlen(sInt) > 9) 
    return -1;

  // There must be a string to parse
  if(!strlen(sInt))
    return -1;

  return atoi(sInt);
}

#endif
