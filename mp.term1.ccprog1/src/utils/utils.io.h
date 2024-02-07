/**
 * A file containing some useful helper functions for low-level input and output operations
 * Although utils.ui.h and utils.key.h also deal with input and output, those files primarily deal with IO constructs created for convenience, and not for system differences.
*/

#ifndef UTILS_IO
#define UTILS_IO

// Some other important header files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Some useful constants
#define UTILS_IO_BS 8       // Backspace
#define UTILS_IO_LF 10      // Line feed
#define UTILS_IO_CR 13      // Carriage return
#define UTILS_IO_SPACE 32   // Space
#define UTILS_IO_DEL 127    // Delete

#define UTILS_IO_MAX_INPUT 1024

// It's funny how these things had to be machine-specific
// These are just some workarounds I decided to implement for a better UI
// If you're asking why I had to do this, it's because I use Ubuntu and you probably use Windows D;

/**
 * #################
 * ###  WINDOWS  ###
 * #################
*/
#ifdef _WIN32
#include <conio.h>
#include <windows.h>

/**
 * A struct to hold some variables so we don't pollute the global namespace.
 * This has to be here because the Unix implementation of this struct actually has stuff in it.
*/
struct UtilsIO {

};

/**
 * This only exists here because I need to set some stuff up for Unix-based OS's.
 * 
 * @param   {struct UtilsIO *}  this  The UtilsIO object to initialize.
*/
void UtilsIO_init(struct UtilsIO *this) {

}


/**
 * Helper function that returns the width of the console.
 * Note that this function is responsive to resizing.
 * 
 * @return  {int}   The number of characters along the width of the console.
*/
int UtilsIO_getWidth() {

  // I must say this is a painfully long name for a data type
  CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
  
  // Some library functions from windows.h that return the dimensions of the console
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo);

  // While we're at it, let's change the buffering behaviour in case the console was resized
  // _IOFBF means data is written to the output stream once the buffer is full
  // _IOLBF (the default) writes data once a newline is encountered
  // char cBufferArray[dWidth];
  setvbuf(stdout, NULL, _IOFBF, consoleScreenBufferInfo.srWindow.Right - consoleScreenBufferInfo.srWindow.Left + 1);

  // Note the plus one is needed to get the inclusive value of the difference
  return consoleScreenBufferInfo.srWindow.Right - consoleScreenBufferInfo.srWindow.Left + 1;
}

/**
 * Helper function that returns the height of the console.
 * Note that this function is responsive to resizing.
 * 
 * @return  {int}   The number of lines in the console.
*/
int UtilsIO_getHeight() {
  
  // I must say this is a painfully long name for a struct
  CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
  
  // Some library functions from windows.h that return the dimensions of the console
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo);

  // Note the plus one is needed to get the inclusive value of the difference
  return consoleScreenBufferInfo.srWindow.Bottom - consoleScreenBufferInfo.srWindow.Top + 1;
}

/**
 * A helper function that resizes the console window.
 * This function only works on Windows (I could not find an implementation for Unix users).
 * 
 * Note that the function is implemented this way BECAUSE: 
 *    (1) the buffer size apparently cannot be smaller than the console size AND
 *    (2) the console size cannot be bigger than the screen buffer.
 * 
 * By shrinking the window size to the absolute minimum, we spare ourselves from "crushing" the buffer size into the window size when shrinking.
 * It also prevents the window size from "crashing" into the buffer when making it bigger.
 * When I tried using a rudimentary implementation that did without the minWIndowSize step, some nasty scrollbars appeared on the side.
 * 
 * @param   {int}   dWidth    The new width of the console.
 * @param   {int}   dHeight   The new height of the console.
 * @return  {int}             Boolean indicating whether or not change was successful.
*/
int UtilsIO_setSize(int dWidth, int dHeight) {

    // Create some objects to define console properties
    COORD const size = { dWidth, dHeight };

    // Set the console window to the smallest possible size first
    SMALL_RECT const minWindowSize = { 0, 0, 1, 1 };
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &minWindowSize);
    
    // Modify the buffer size 
    // This stores the text on the console and does not reflect the actual console size
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), size);
    
    // Update the actual console size into the new window size
    SMALL_RECT const newWindowSize = { 0, 0, dWidth - 1, dHeight - 1 };
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &newWindowSize);

    // This is another thing I found elsewhere which speeds printf up
    // Console output by default is buffered per line, which means everytime we counter a \n things slow down
    // In other words, in only prints in chunks of lines
    // In order to circumvent that hindrance, we set the buffer size ourselves
    
    // _IOFBF means data is written to the output stream once the buffer is full
    // _IOLBF (the default) writes data once a newline is encountered
    // We don't need to do this for Unix anymore cuz it's already quite fast for some reason.
    setvbuf(stdout, NULL, _IOFBF, dWidth);

    // The screen size was changed
    // Note that the unix version of the function returns 0 since it does nothing
    return 1;
} 

/**
 * Helper function that clears the console.
*/
void UtilsIO_clear() {

  // I know calling system is bad BUT...
  system("cls");
}

/**
 * Helper function that gets a single character without return key.
 * 
 * @return  {char}  Returns the character read from the conaole.
*/
char UtilsIO_readChar() {
  return getch();
}

/**
 * This only exists mainly because I need to do some housekeeping for Unix-based OS's.
 * 
 * @param   {struct UtilsIO *}  this  The UtilsIO object to clean up.
*/
void UtilsIO_exit(struct UtilsIO *this) {

  // Reset colors and clear the screen
  printf("\x1b[38;5;255m");
  printf("\x1b[48;5;232m");
  
  for(int i = UtilsIO_getHeight(); --i;) 
    for(int j = UtilsIO_getWidth(); --j;) 
      printf(" ");
  
  UtilsIO_clear();
}

/**
 * #######################
 * ###  UNIX-BASED OS  ###
 * #######################
*/
#else
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

/**
 * A struct to hold some variables so we don't pollute the global namespace.
 * Stores the original settings of the terminal so we can revert them back after the program exuts.
*/
struct UtilsIO {
  struct termios defaultSettings;
  struct termios overrideSettings;
};

/**
 * Sets up some stuff for IO handling.
 * Overrides default terminal settings so I can replicate getch behaviour on Unix-based OS's.
 * 
 * @param   {struct UtilsIO *}  this  The UtilsIO object to initialize.
*/
void UtilsIO_init(struct UtilsIO *this) {

  // Save the default settings of the terminal before overriding them
  // This function is from termios.h
  tcgetattr(STDIN_FILENO, &this->defaultSettings);

  // Create the override
  this->overrideSettings = this->defaultSettings;

  // ICANON usually restricts the terminal to read a single line at a time
  // ICANON also terminates input when \n is encountered
  // ECHO spits the user's keystrokes back at the terminal (like when typing)
  // By disabling both we can mimic getch behaviour in a Unix environment!
  this->overrideSettings.c_lflag &= ~(ICANON | ECHO);

  tcsetattr(0, TCSANOW, &this->overrideSettings);
}

/**
 * Helper function that returns the width of the console.
 * Note that this function is responsive to resizing.
 * 
 * @return  {int}   The number of characters along the width of the console.
*/
int UtilsIO_getWidth() {
  
  // A library function from ioctl.h that gets the current terminal size
  struct winsize windowSize;
  ioctl(0, TIOCGWINSZ, &windowSize);

  return windowSize.ws_col;
}

/**
 * Helper function that returns the height of the console.
 * Note that this function is responsive to resizing.
 * 
 * @return  {int}   The number of lines in the console.
*/
int UtilsIO_getHeight() {
  
  // A library function from ioctl.h that gets the current terminal size
  struct winsize windowSize;
  ioctl(0, TIOCGWINSZ, &windowSize);

  return windowSize.ws_row;
}

/**
 * A helper function that resizes the console window.
 * Unfortunately, I could not find a POSIX-compliant implementation.
 * This is just here as a dummy function.
 * 
 * @param   {int}   dWidth    The new width of the console.
 * @param   {int}   dHeight   The new height of the console.
 * @return  {int}             Boolean indicating whether change was successful.
*/
int UtilsIO_setSize(int dWidth, int dHeight) {
  return 0;
} 

/**
 * Helper function that clears the console.
*/
void UtilsIO_clear() {

  // A neat regex expression that clears the current console and moves the cursor
  // \e[H   Puts the cursor at the home position 
  // \e[2J  Erases entire screen
  // \e[3J  Erases saved lines
  printf("\e[H\e[2J\e[3J");
}

/**
 * Helper function that gets a single character without return key.
 * 
 * @return  {char}  Returns the character read from the conaole.
*/
char UtilsIO_readChar() {
  return getchar();
}

/**
 * Clean up the stuff I used.
 * 
 * @param   {struct UtilsIO *}  this  The UtilsIO object to clean up.
*/
void UtilsIO_exit(struct UtilsIO *this) {

  // Reset the colors 
  printf("\x1b[38;5;255m");
  printf("\x1b[48;5;232m");
  
  for(int i = UtilsIO_getHeight(); --i;) 
    for(int j = UtilsIO_getWidth(); --j;) 
      printf(" ");

  UtilsIO_clear();

  // Return the terminal to its default state
  // Again, the function is from termios.h
  tcsetattr(STDIN_FILENO, TCSANOW, &this->defaultSettings);
}

#endif

/**
 * ###############################
 * ###  OS-AGNOSTIC FUNCTIONS  ###
 * ###############################
*/

/**
 * Returns whether or not the character is a LF (line feed) or CR (carriage return) character.
 * 
 * @param   {char}  cChar   The character to be evaluated.
 * @return  {int}           A boolean indicating whether or not the character is a return character.
*/
int UtilsIO_isReturn(char cChar) {
  return 
    cChar == UTILS_IO_LF || 
    cChar == UTILS_IO_CR;
}

/**
 * Returns whether or not the character is a backspace character.
 * 
 * @param   {char}  cChar   The character to be evaluated.
 * @return  {int}           A boolean indicating whether or not the character is a backspace or delete.
*/
int UtilsIO_isBackspace(char cChar) {
  return 
    cChar == UTILS_IO_BS || 
    cChar == UTILS_IO_DEL;
}

/**
 * A helper function that creates a string that can hold input values.
 * 
 * @return  {char *}  A pointer to memory that can hold a string of up to UTILS_IO_MAX_INPUT characters.
*/
char *UtilsIO_newInputStr() {
  char *sOutput = calloc(UTILS_IO_MAX_INPUT, sizeof(char));
  return sOutput;
}

/**
 * A helper function that kills a string.
 * 
 * @param   {char *}  sOutput   The string to free from memory.
*/
void UtilsIO_killInputStr(char *sOutput) {
  free(sOutput);
}

/**
 * Helper function that handles character inputs.
 * Prints the input as it is inputted.
 * 
 * @return  {char}  Returns the character from the console.
*/
char UtilsIO_inputChar() {
  char cInput;

  cInput = UtilsIO_readChar();
  printf("%c", cInput);

  return cInput;
}

/**
 * Helper function that handles string inputs.
 * 
 * @return  {char *}  Returns the character string from the console.
*/
char *UtilsIO_inputStr() {
  char *sOutput = calloc(UTILS_IO_MAX_INPUT, sizeof(char));
  char *sInput = calloc(1, sizeof(char));
  char cInput;
  int dLength = 0;

  do {
    cInput = UtilsIO_readChar();
    sprintf(sInput, "%c", cInput);

    // If user is not done inputting
    if(!UtilsIO_isReturn(cInput)) {

      // Backspace handling condition
      if(!UtilsIO_isBackspace(cInput)) {
        strcat(sOutput, sInput);
        printf("%c", cInput);
      } else {
        sOutput[dLength--] = 0;
        printf("\b \b");
      }
    }
  } while(!UtilsIO_isReturn(cInput) && ++dLength < UTILS_IO_MAX_INPUT);

  free(sInput);
  return sOutput;
}

/**
 * Helper function that handles string inputs.
 * This version of the function modifies a string instead of returning one.
 * Note that sOutput should have enough space to receive a maximum of UTILS_IO_MAX_INPUT characters.
 * 
 * @param   {char *}  sOutput   The string to be modified.
 * @return  {int}               A boolean that returns false when a return key has been pressed.
*/
int UtilsIO_inputStrOut(char *sOutput) {
  char *sInput = calloc(1, sizeof(char));
  char cInput;

  if(strlen(sOutput) < UTILS_IO_MAX_INPUT) {
    cInput = UtilsIO_readChar();
    sprintf(sInput, "%c", cInput);  
    
    // If user is not done inputting
    if(!UtilsIO_isReturn(cInput)) {

      // Backspace handling condition
      if(!UtilsIO_isBackspace(cInput)) 
        strcat(sOutput, sInput);
    } else {
      return 0;
    }
  } 
  
  // In case it's a backspace
  if(strlen(sOutput)) {
    if(UtilsIO_isBackspace(cInput))
      sOutput[strlen(sOutput) - 1] = 0;
  }

  free(sInput);
  return 1;
}

#endif