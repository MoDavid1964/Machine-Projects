/**
 * @ Author: Mo David
 * @ Create Time: 2024-02-07 02:12:46
 * @ Modified time: 2024-02-09 01:31:31
 * @ Description:
 *    Stores the system variables and the system logic.
 *    Implements the specifications of the machine project.
 */

#ifndef GAME_SYSTEM
#define GAME_SYSTEM

#include "./classes/player.class.h"
#include "./classes/world.class.h"

#include <stdint.h>

#define GAME_PLAYERS 2
#define GAME_WORLD_SIZE 6

#define GAME_WIN_QUADS 2
#define GAME_WIN_CONFIGS 4

/**
 * //
 * ////
 * //////    System object infrastructure
 * ////////
 * ////////// 
*/

/**
 * This singleton stores all the system variables related to the game.
 * Note that I distinguish between objects in classes. 
 * Check the main file for more information on this.
*/
typedef struct System System;

struct System {

  // The winning configurations
  World *WIN_QUADS[GAME_WIN_QUADS];
  World *WIN_CONFIGS[GAME_WIN_CONFIGS];
  
  // The different players
  Player *PLAYERS[GAME_PLAYERS];

  // Game constants
  int WORLD_SIZE;
  int TURN_COUNT;

  // Some other game states
  int turnState;
  int overState;
};

/**
 * Initializes the system variables
 * 
 * @param   { System * }  this  The system to be initialized.
 * @return  { System * }        The initialized system.
*/
System *System_init(System *this) {

  // Defining some game constants
  this->WORLD_SIZE = GAME_WORLD_SIZE;
  this->TURN_COUNT = GAME_PLAYERS;

  // Game variables
  this->turnState = 0;  // The first player
  this->overState = 0;  // Initially false

  // The players
  for(int i = 0; i < GAME_PLAYERS; i++)
    this->PLAYERS[i] = Player_create();

  // The quadrant configurations
  this->WIN_QUADS[0] = World_create();
  World_setBit(this->WIN_QUADS[0], 0, 0, 1);  // (1, 1)
  World_setBit(this->WIN_QUADS[0], 1, 1, 1);  // (2, 2)

  this->WIN_QUADS[1] = World_create();
  World_setBit(this->WIN_QUADS[1], 0, 1, 1);  // (1, 2)
  World_setBit(this->WIN_QUADS[1], 1, 0, 1);  // (2, 1)
  
  // The winning configurations  
  this->WIN_CONFIGS[0] = World_create();
  World_setBit(this->WIN_CONFIGS[0], 0, 0, 1);  // (1, 1)
  World_setBit(this->WIN_CONFIGS[0], 0, 2, 1);  // (1, 3)
  World_setBit(this->WIN_CONFIGS[0], 1, 1, 1);  // (2, 2)
  World_setBit(this->WIN_CONFIGS[0], 2, 0, 1);  // (3, 1)
  World_setBit(this->WIN_CONFIGS[0], 2, 2, 1);  // (3, 3)

  this->WIN_CONFIGS[1] = World_create();
  World_setBit(this->WIN_CONFIGS[1], 3, 3, 1);  // (4, 4)
  World_setBit(this->WIN_CONFIGS[1], 3, 5, 1);  // (4, 6)
  World_setBit(this->WIN_CONFIGS[1], 4, 4, 1);  // (5, 5)
  World_setBit(this->WIN_CONFIGS[1], 5, 3, 1);  // (6, 4)
  World_setBit(this->WIN_CONFIGS[1], 5, 5, 1);  // (6, 6)

  this->WIN_CONFIGS[2] = World_create();
  World_setBit(this->WIN_CONFIGS[2], 0, 4, 1);  // (1, 5)
  World_setBit(this->WIN_CONFIGS[2], 1, 3, 1);  // (2, 4)
  World_setBit(this->WIN_CONFIGS[2], 1, 4, 1);  // (2, 5)
  World_setBit(this->WIN_CONFIGS[2], 1, 5, 1);  // (2, 6)
  World_setBit(this->WIN_CONFIGS[2], 2, 4, 1);  // (3, 5)

  this->WIN_CONFIGS[3] = World_create();
  World_setBit(this->WIN_CONFIGS[3], 3, 0, 1);  // (4, 1)
  World_setBit(this->WIN_CONFIGS[3], 3, 2, 1);  // (4, 3)
  World_setBit(this->WIN_CONFIGS[3], 4, 0, 1);  // (5, 1)
  World_setBit(this->WIN_CONFIGS[3], 4, 2, 1);  // (5, 3)
  World_setBit(this->WIN_CONFIGS[3], 5, 0, 1);  // (6, 1)
  World_setBit(this->WIN_CONFIGS[3], 5, 2, 1);  // (6, 3)

  return this;
}

/**
 * //
 * ////
 * //////    Getters and setters
 * ////////
 * ////////// 
*/

/**
 * Gets the current state of the system.
 * 
 * @param   { System * }  this  The system to read.
 * @return  { int }             An integer whose bits describe the state of the system.
*/
int System_getState(System *this) { 
  return 
    this->turnState << 1 & 
    this->overState;
}

/**
 * //
 * ////
 * //////    System operations
 * ////////
 * ////////// 
*/

/**
 * Updates the current state of the system.
 * Coordinates to a point within the world space are needed per update of the system.
 * 
 * @param   { System * }  this  The system to update.
 * @param   { int }       x     A x-coordinate in the world space.
 * @param   { int }       y     A y-coordinate in the world space.
*/
void System_update(System *this, int x, int y) {

  // ! Check for the validity of the coordinates first


  // * Player_updateWorld(this->PLAYERS[this->turnState], x, y);
}

/**
 * Updates the current turn value stored by the system.
 * 
 * @param   { System * }  this  The system to be updated.
*/
void System_turn(System *this) {
  this->turnState += 1;
  this->turnState %= this->TURN_COUNT;
}

#endif