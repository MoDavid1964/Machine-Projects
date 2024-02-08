/**
 * @ Author: Mo David
 * @ Create Time: 2024-02-06 15:01:38
 * @ Modified time: 2024-02-09 01:41:06
 * @ Description:
 *    The main file builds the actual program itself
 *    It also configures the execution environment for the program.
 */

#include <stdlib.h>
#include <stdio.h>

// The reason I do this is so as to be able to build differently for Windows and Unix devices
// Plus, specifying different terminal arguments becomes easier to handle and less of a clutter
// Also, there are just so many other possible things I have to handle with regard to differences in execution environment
int main(int argc, char *argv[]) {

  // Windows environments
  #ifdef _WIN32
    system("clear");
    printf("\e[H\e[2J\e[3J");

    // A note for Windows users:
    //    The reason I have to specify conhost.exe to run the app is because Windows 11 (and other Windows 10 users)
    //    have begun using the Windows Terminal in lieu of the good ol' CMD. However, Windows Terminal currently does
    //    not support window resizing and so I was forced to find a workaround to this. Hence, we have here a manual
    //    execution of conhost.exe (which, mind you, is actually what cmd.exe uses as a terminal anyway).
    printf("(1) Compiling the program...\n");
    system("gcc -std=c99 -Wall src\\game.c -o build\\game.win.exe 2> build\\log.win.txt");
    printf(" -  Compile success!\n(2) Running the program...\n");
    system("%windir%\\SysNative\\conhost.exe build\\game.win.exe");
    printf(" -  Program terminated."); 
    
  // Unix environments
  #else
    printf("(1) Compiling the program...\n");
    system("gcc -std=c99 -Wall ./src/game.c -o ./build/game.unix.o 2> ./build/log.unix.txt");
    printf(" -  Compile success!\n(2) Running the program...\n");
    system("./build/game.unix.o");
    printf(" -  Program terminated."); 
  #endif

  return 0;
}