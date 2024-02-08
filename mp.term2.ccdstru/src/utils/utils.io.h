/**
 * @ Author: Mo David
 * @ Create Time: 2024-02-07 02:12:46
 * @ Modified time: 2024-02-09 01:48:09
 * @ Description:
 *    A file containing some useful helper functions for low-level input and output operations.
 */

#ifndef UTILS_IO_
#define UTILS_IO_

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
 * //
 * ////
 * //////    Windows
 * ////////
 * ////////// 
*/

#ifdef _WIN32
#include <conio.h>
#include <windows.h>

/**
 * A struct to hold some variables so we don't pollute the global namespace.
 * This has to be here because the Unix implementation of this struct actually has stuff in it.
*/
typedef struct IO IO;

struct IO {
  
};

/**
 * Init and exit
*/
IO *IO_init(IO *this);

void IO_exit(IO *this);

/**
 * Operators
*/
int IO_getWidth();

int IO_getHeight();

int IO_setSize(int dWidth, int dHeight);

void IO_clear();

char IO_readChar();

/**
 * This only exists here because I need to set some stuff up for Unix-based OS's.
 * 
 * @param   { IO * }  this  The IO object to initialize.
 * @return  { IO * }        The initialized IO object.
*/
IO *IO_init(IO *this) {
  return this;
}


/**
 * Helper function that returns the width of the console.
 * Note that this function is responsive to resizing.
 * 
 * @return  { int }   The number of characters along the width of the console.
*/
int IO_getWidth() {

  // I must say this is a painfully long name for a data type
  CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
  
  // Some library functions from windows.h that return the dimensions of the console
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleScreenBufferInfo);

  // Note the plus one is needed to get the inclusive value of the difference
  return consoleScreenBufferInfo.srWindow.Right - consoleScreenBufferInfo.srWindow.Left + 1;
}

/**
 * Helper function that returns the height of the console.
 * Note that this function is responsive to resizing.
 * 
 * @return  { int }   The number of lines in the console.
*/
int IO_getHeight() {
  
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
 * @param   { int }   dWidth    The new width of the console.
 * @param   { int }   dHeight   The new height of the console.
 * @return  { int }             Boolean indicating whether or not change was successful.
*/
int IO_setSize(int dWidth, int dHeight) {

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

    // The screen size was changed
    // Note that the unix version of the function returns 0 since it does nothing
    return 1;
} 

/**
 * Helper function that clears the console.
*/
void IO_clear() {

  // A neat regex expression that clears the current console and moves the cursor
  // \e[H   Puts the cursor at the home position 
  // \e[2J  Erases entire screen
  // \e[3J  Erases saved lines
  printf("\e[H\e[2J\e[3J");

  // I know calling system is bad BUT...
  // Also I think the new Windows terminal broke some of my sht,,
  system("cls");
}

/**
 * Helper function that gets a single character without return key.
 * 
 * @return  { char }  Returns the character read from the conaole.
*/
char IO_readChar() {
  return getch();
}

/**
 * This only exists mainly because I need to do some housekeeping for Unix-based OS's.
 * 
 * @param   { IO * }  this  The IO object to clean up.
*/
void IO_exit(IO *this) {

  // Reset colors and clear the screen
  printf("\x1b[38;5;255m");
  printf("\x1b[48;5;232m");
  
  for(int i = IO_getHeight(); --i;) 
    for(int j = IO_getWidth(); --j;) 
      printf(" ");
  
  IO_clear();
}

/**
 * //
 * ////
 * //////    Unix-based OS
 * ////////
 * ////////// 
*/

#else
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

/**
 * A struct to hold some variables so we don't pollute the global namespace.
 * Stores the original settings of the terminal so we can revert them back after the program exuts.
*/
typedef struct IO IO;

struct IO {
  struct termios defaultSettings;
  struct termios overrideSettings;
};

/**
 * Init and exit
*/
IO *IO_init(IO *this);

void IO_exit(IO *this);

/**
 * Operators
*/
int IO_getWidth();

int IO_getHeight();

int IO_setSize(int dWidth, int dHeight);

void IO_clear();

char IO_readChar();

/**
 * Sets up some stuff for IO handling.
 * Overrides default terminal settings so I can replicate getch behaviour on Unix-based OS's.
 * 
 * @param   { IO * }  this  The IO object to initialize.
 * @return  { IO * }        The initialized instance.
*/
IO *IO_init(IO *this) {

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

  return this;
}

/**
 * Helper function that returns the width of the console.
 * Note that this function is responsive to resizing.
 * 
 * @return  { int }   The number of characters along the width of the console.
*/
int IO_getWidth() {
  
  // A library function from ioctl.h that gets the current terminal size
  struct winsize windowSize;
  ioctl(0, TIOCGWINSZ, &windowSize);

  return windowSize.ws_col;
}

/**
 * Helper function that returns the height of the console.
 * Note that this function is responsive to resizing.
 * 
 * @return  { int }   The number of lines in the console.
*/
int IO_getHeight() {
  
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
 * @param   { int }   dWidth    The new width of the console.
 * @param   { int }   dHeight   The new height of the console.
 * @return  { int }             Boolean indicating whether change was successful.
*/
int IO_setSize(int dWidth, int dHeight) {
  return 0;
} 

/**
 * Helper function that clears the console.
*/
void IO_clear() {

  // A neat regex expression that clears the current console and moves the cursor
  // \e[H   Puts the cursor at the home position 
  // \e[2J  Erases entire screen
  // \e[3J  Erases saved lines
  printf("\e[H\e[2J\e[3J");
}

/**
 * Helper function that gets a single character without return key.
 * 
 * @return  { char }  Returns the character read from the conaole.
*/
char IO_readChar() {
  return getchar();
}

/**
 * Clean up the stuff I used.
 * 
 * @param   { IO * }  this  The IO object to clean up.
*/
void IO_exit(IO *this) {

  // Reset the colors 
  printf("\x1b[38;5;255m");
  printf("\x1b[48;5;232m");
  
  for(int i = IO_getHeight(); --i;) 
    for(int j = IO_getWidth(); --j;) 
      printf(" ");

  IO_clear();

  // Return the terminal to its default state
  // Again, the function is from termios.h
  tcsetattr(STDIN_FILENO, TCSANOW, &this->defaultSettings);
}

#endif

#endif