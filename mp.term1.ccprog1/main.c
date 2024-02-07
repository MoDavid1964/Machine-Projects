/**
 * This is to certify that this project is my own work, based on my personal efforts in studying and applying the concepts learned. I have constructed 
 * the functions and their respective algorithms and corresponding code by myself. The program was run, tested, and debugged by my own efforts. I further certify 
 * that I have not copied in part or whole or otherwise plagiarized the work of other students and/or persons.
 * 
 *    Malks Mogen M. David, DLSU ID# 12306991
 * 
 * 
 *                             ,     :     ,                           
 *                        '.    ;    :    ;    ,`                      
 *                    '-.   '.   ;   :   ;   ,`   ,-`                  
 *                 "-.   '-.  '.  ;  :  ;  ,`  ,-`   ,-"               
 *                    "-.   '-. '. ; : ; ,` ,-`   ,-"                  
 *               '"--.   '"-.  '-.'  '  `.-`  ,-"`   ,--"`             
 *     __             '"--.  '"-.   ...    _"`  ,--"`                  
 *    |  |--.---.-.----.--.--.-----.::::-.|  |_     .-----.--.--.-----.
 *    |     |  _  |   _|  |  |  -__|__ --||   _|    |__ --|  |  |     |
 *    |__|__|___._|__|  \___/|_____|_____||____|    |_____|_____|__|__|
 * 
 * 
 *               |\      _,,,---,,_    
 *        ZZZzz /,`.-'`'    -.  ;-;;,_ 
 *             |,4-  ) )-,_. ,\ (  `'-'
 *            '---''(_/--'  `-'\_)     
 * 
 * 
 * Description: Harvest Sun: a mini-game with farming and money. BTW, I did NOT use ncurses; everything was coded from scratch.
 * Programmed by: Malks Mogen M. David of S19A
 * Last modified: 26/11/2023
 * Version: 1.0.0
 * Acknowledgements:
 * 
 *    I would like to say thank you to the following stackoverflow heroes who have spared me from lapses of ignorance when coding certain aspects of this project:
 *      
 *      [A Better Understanding of malloc()]....https://stackoverflow.com/questions/2987207/why-do-i-get-a-c-malloc-assertion-failure
 *      [A Better Understanding of Strings].....https://stackoverflow.com/questions/4834811/strcat-concat-a-char-onto-a-string
 *      [Avoid Function Pointers in Structs]....https://stackoverflow.com/questions/17052443/c-function-inside-struct
 *      [Backspace in printf()].................https://stackoverflow.com/questions/9174396/c-remove-printf-output
 *      [Console Dimensions in Windows].........https://stackoverflow.com/questions/6812224/getting-terminal-size-in-c-for-windows
 *      [Difference of malloc() and calloc()]...https://stackoverflow.com/questions/1538420/difference-between-malloc-and-calloc
 *      [Disable Input Buffering in Linux]......https://stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar-for-reading-a-single-character-only
 *      [Incompatible Pointer Types in C].......https://stackoverflow.com/questions/16160799/incompatible-pointer-type-in-c
 *      [Modifying Console Size in Windows].....https://stackoverflow.com/questions/12900713/reducing-console-size
 *      [Speeding Up printf() in C].............https://stackoverflow.com/questions/47087039/how-to-speed-up-printf-in-c
 *      [String Assignment May Leak Memory].....https://stackoverflow.com/questions/10063222/freeing-strings-in-c
 *      [String Switching in C is Sad]..........https://stackoverflow.com/questions/4014827/how-can-i-compare-strings-in-c-using-a-switch-statement
 *      [Terminal Dimensions in Linux]..........https://stackoverflow.com/questions/1022957/getting-terminal-width-in-c
 *      [WIN32 vs. _WIN32]......................https://stackoverflow.com/questions/662084/whats-the-difference-between-the-win32-and-win32-defines-in-c
 * 
 *    And to the following ASCII artists who have helped give this game its air of polish:
 *      
 *      [ASCII Text Art Generator]..............https://patorjk.com/software/taag/#p=display
 *      [ASCII Art Collection]..................https://www.asciiart.eu/
 *      [ASCII Art Co UK].......................https://ascii.co.uk/
 *      [ASCII Art De]..........................http://www.ascii-art.de/
 * 
 *    And to the following markdown editor I have found rather handy in drafting something pleasing to the eyes:
 * 
 *      [The Last Markdown Editor, Ever]........https://dillinger.io/
 * 
 *    And lastly, to the following scribes who have published well-meant documentation on the things I needed most:
 *      
 *      [ASCII Code Table]......................https://www.ascii-code.com/CP1252/178
 *      [ANSI Escape Sequences].................https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
 *      [Basic system() Documentation]..........https://pubs.opengroup.org/onlinepubs/9699919799/functions/system.html
 *      [C Standard Library]....................https://www.tutorialspoint.com/c_standard_library
 *      [C setvbuf() Function]..................https://www.tutorialspoint.com/c_standard_library/c_function_setvbuf.htm
 *      [Generic Pointers]......................https://www.oreilly.com/library/view/mastering-algorithms-with/1565924533/ch02s05.html
 *      [No Nested Functions in C]..............https://www.geeksforgeeks.org/nested-functions-c/
 *      [To typedef Or Not To typedef structs]..https://www.reddit.com/r/C_Programming/comments/kr4i80/to_typedef_structs_or_not/
 *      [Windows Console Library Functions].....https://learn.microsoft.com/en-us/windows/console/console-functions
 *      [Why system() is Evil]..................https://cplusplus.com/articles/j3wTURfi/
 * 
 * Anyhow, I present to you this amalgamation of C files which have condensed themselves over drops of uninterrupted keystrokes uttered throughout the wee hours of the morning.
 * There is no greater high than writing a novel's worth of C lines that *seem* to work as intended.
 * (Watch me jinx myself and f*ck up the garbage collection smh)
 * Currently more than 7500 lines... that's more lines than I have under my eyes.
 * Given that this took a total of a little under two weeks to develop, that's more than 500 lines a day, on average.
 * 
 * Also I'm rather proud to say that this project uses no external libraries. 
 * ALl the functionalities present here were coded by yours truly with nothing but the help of the C Standard Library (and some Windows header files I guess).
 * If you want a more detailed explanation of how to use the program (and how some of the functions work), check out the README.
 * 
 * PS: Don't mind the warnings, they're just pointer type mismatches because I used function pointers with void pointer parameters inside some of my callback declarations :DD
 * PS: Lastly, I don't know why I didn't typedef my structs... I probably had to type an extra 1000 words because of that.
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

  // I KNOW system is bad, but I'm not really a hacker trying to run a malicious program on your device, right (or am I? OwO)
  // Although at some point it messed up my program build, and it broke during the dev process ://
  // Don't worry, it works now :DD
  
/**
 * ######################
 * ###  COMPILE STEP  ###
 * ######################
*/

  int bCompiled = 0;
  printf("\nCompiling game...\n");

  // If we're on Windows
  #ifdef _WIN32
    bCompiled = !system("gcc src\\game.c -o build\\game.win.exe -std=c99 -Wall 2> build\\logs\\.log.txt");

  // If we're in a Unix-based environment (like I am)
  // You can try running this program in WSL if you wanna see what it's like, although it's honestly not that different
  #else
    bCompiled = !system("gcc src/game.c -o build/game.unix.o -std=c99 -Wall 2> build/logs/.log.txt");
  #endif

  // ALL THIS DOES IS FORMAT THE WARNINGS, YOU CAN COMMENT THESE OUT DW
  // Fix the logs displayed
  // I'm too lazy to do an if check for the python interpreter
  system("python ./build/logs/fix_logs.py 2> ./build/logs/.e.py.txt");
  system("python3 ./build/logs/fix_logs.py 2> ./build/logs/.e.py.txt");
  // Comment the code above ^^ if you don't want me to run python

  // Some feedback output
  if(bCompiled) {
    printf("Success!\n");
    printf("Running game...\n");
  } else {
    printf("Hmmm, something went wrong...\n");
    printf("Check out the log file in the build folder for more info.\n");
  }

/**
 * ############################
 * ###  CONFIGURE RUN ARGS  ###
 * ############################
*/

  // For debugging purposes
  char *args = calloc(32, sizeof(char));
  if(argc > 1) {

    // Debugging mode
    if(!strcmp(argv[1], "debug")) {

      // Debug certain parts of the full gameplay
      if(argc > 2) {
        if(!strcmp(argv[2], "play")) args = "debug play";
        else if(!strcmp(argv[2], "home")) args = "debug home";
        else if(!strcmp(argv[2], "farm")) args = "debug farm";
        else if(!strcmp(argv[2], "shop")) args = "debug shop";
      } else {
        args = "debug play";
      }

    // Minimal version of the game
    } else if(!strcmp(argv[1], "full")) {
      args = "full na";
      
    // Default mode of the game
    } else {
      args = "default na";
    }
  
  // No args
  } else {
    args = "default na";
  }

/**
 * ##################
 * ###  RUN STEP  ###
 * ##################
*/
  
  if(bCompiled) {

    // Run the game itself
    char execCommand[128];
    #ifdef _WIN32
      sprintf(execCommand, "build\\game.win.exe %s", args);
      system(execCommand);
    #else
      sprintf(execCommand, "./build/game.unix.o %s", args);
      system(execCommand);
    #endif
  }

  return 0;
}