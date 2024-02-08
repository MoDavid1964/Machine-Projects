/**
 * @ Author: Mo David
 * @ Create Time: 2024-02-07 02:12:46
 * @ Modified time: 2024-02-09 02:03:30
 * @ Description:
 *    Contains the overarching logic of the game.
 */

#include "game.system.h"
#include "./utils/utils.io.h"
#include "./utils/utils.buffer.h"
#include "./utils/utils.graphics.h"

#include <stdio.h>

int main() {

  System system;
  Buffer *buffer = Buffer_create(64);

  System_init(&system); 

  IO_clear();
  IO_setSize(64, 16);

  Buffer_newText(buffer, "Hello world1");
  Buffer_addText(buffer, Graphics_getCodeBG(0xffffff));
  Buffer_addText(buffer, Graphics_getCodeFG(0x212121));
  Buffer_addText(buffer, "Hello world2");
  Buffer_addText(buffer, Graphics_getCodeBG(0x123456));
  Buffer_addText(buffer, Graphics_getCodeFG(0x654321));
  Buffer_addText(buffer, "Hello world3");

  printf("%d\n", buffer->dWidth);
  printf("%s", buffer->sText);

  // for(int i = 0; i < 8; i++){
  //   for(int j = 0; j < 8; j++)
  //     printf("%d", World_getBit(system.WIN_CONFIGS[1], i, j));
  //   printf("\n");
  // }

  IO_readChar();

  return 0;
}