/**
 * This file defines an instantiable class that player information.
*/

#ifndef GAME_PLAYER_CLASS
#define GAME_PLAYER_CLASS

#include "world.class.h"
#include <stdlib.h>

/**
 * 
 * @class
*/
typedef struct Player Player;

struct Player {
  World *pWorld;
};

// Constructors and destructors
Player *Player_new();
Player *Player_init(Player *this);
Player *Player_create();
void Player_kill(Player *this);

// Getters and setters
World *Player_getWorld(Player *this);

// Operations
void Player_addPoint(Player *this, int x, int y);

/**
 * //
 * ////
 * //////    Player constructors and destructors
 * ////////
 * ////////// 
*/

/**
 * Creates a new instance of the player class.
 * 
 * @return  { Player * }  A pointer to the created instance of the player class.
*/
Player *Player_new() {
  Player *pPlayer = calloc(1, sizeof(*pPlayer));

  if(pPlayer == NULL)
    return NULL;
  
  return pPlayer;
}

/**
 * Initializes an instance of the player class.
 * 
 * 
*/
Player *Player_init(Player *this) {
  this->pWorld = World_create();

  return this;
}

/**
 * Creates an initialized instance of the player class.
 * Uses the init function above.
 * 
 * @return  { Player * }  A pointer to the newly created initialized instance of the player class.
*/
Player *Player_create() {
  Player *pPlayer = Player_new();

  Player_init(pPlayer);

  return pPlayer;
}

/**
 * Deallocates the memory associated with an instance of the player class.
 * 
 * @param   { Player * }  this  The instance of the player class to be deallocated.
*/
void Player_kill(Player *this) {
  free(this);
}

/**
 * //
 * ////
 * //////    Player getters and setters
 * ////////
 * ////////// 
*/

/**
 * Returns a pointer to the world instance stored by a player.
 * 
 * @param   { Player * }  this  The instance to be read.
 * @return  { World * }         The instance of world stored by the player.
*/
World *Player_getWorld(Player *this) {
  return this->pWorld;
}

/**
 * //
 * ////
 * //////    Player operations
 * ////////
 * ////////// 
*/

/**
 * Adds an ordered pair unto the world of the player.
 * 
 * @param   { Player * }  this  The player instance to be modified.
 * @param   { int }       x     The x-coordinate of the ordered pair.
 * @param   { int }       y     The y-coordinate of the ordered pair.
*/
void Player_addPoint(Player *this, int x, int y) {
  World_setBit(this->pWorld, x, y, 1);
}

#endif