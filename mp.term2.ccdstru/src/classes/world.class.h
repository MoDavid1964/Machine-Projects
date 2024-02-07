/**
 * This file defines an instantiable class that stores grid information (we call this the world).
*/

#ifndef GAME_WORLD_CLASS
#define GAME_WORLD_CLASS

#include <stdint.h>
#include <stdlib.h>

#define WORLD_MAX_SIZE 8

/**
 * //
 * ////
 * //////    World class infrastructure
 * ////////
 * ////////// 
*/

/**
 * The world class describes a grid of bits which can helps us store information.
 * Note that the bits are stored in reverse; in other words:
 *    (1) The cell (1, 1) corresponds to the UPPER RIGHT bit
 *    (2) The cell (6, 6) corresponds to the LOWER LEFT bit
 * @class
*/
typedef struct World World;

struct World {
  uint8_t bits[WORLD_MAX_SIZE];
};

/**
 * Creates a new instance of the world class.
 * 
 * @return  { World * }   A pointer to the created instance of the world class.
*/
World *World_new() {
  World *pWorld = calloc(1, sizeof(*pWorld));

  if(pWorld == NULL)
    return NULL;
  
  return pWorld;
}

/**
 * Initializes an instance of the world class.
 * Sets all the bits to 0.
 * 
 * @param   { World * }   this  A pointer to the instance of the world class.
 * @return  { World * }         A pointer to the initialized instance of the world class.
*/
World *World_init(World *this) {
  for(int i = 0; i < WORLD_MAX_SIZE; i++)
    this->bits[i] = 0;

  return this;
}

/**
 * Creates an initialized instance of the world class.
 * Uses the init function above.
 * 
 * @return  { World * }   A pointer to the newly created initialized instance of the world class.
*/
World *World_create() {
  World *pWorld = World_new();

  World_init(pWorld);

  return pWorld;
}

/**
 * Deallocates the memory associated with an instance of the world class.
 * 
 * @param   { World * }   this  The instance of the world class to be deallocated.
*/
void World_kill(World *this) {
  free(this);
}

/**
 * //
 * ////
 * //////    World class getters and setters
 * ////////
 * ////////// 
*/

/**
 * Retrieves the value of the bit at the specified location.
 * 
 * @param   { World * }   this  The world instance to be read.
 * @param   { short }     x     The x-coordinate of the bit.
 * @param   { short }     y     The y-coordinate of the bit.
 * @return  { int }             The current value stored by the bit.
*/
int World_getBit(World *this, short x, short y) {
  return this->bits[y] & (1 << x) ? 1 : 0;
}

/**
 * Updates the value of the bit at the specified location.
 * 
 * @param   { World * }   this  The world instance to be updated.
 * @param   { short }     x     The x-coordinate of the bit.
 * @param   { short }     y     The y-coordinate of the bit.
 * @param   { int }       n     The value to put into the bit (either true or false only).
*/
void World_setBit(World *this, short x, short y, int n) {
  if(n) this->bits[y] |= (1 << x);
  else this->bits[y] &= ~(1 << x);
}

/**
 * //
 * ////
 * //////    World class operations
 * ////////
 * ////////// 
*/

/**
 * Returns whether or not a world instance has a bit configuration that is a subset of another world instance.
 * 
 * @param   { World * }   ref   The reference instance.
 * @param   { World * }   sub   The instance to be checked within ref.
 * @return  { int }             Whether or not sub is a subset of ref.
*/
int World_contains(World *pWorldRef, World *pWorldSub) {
  for(int i = 0; i < WORLD_MAX_SIZE; i++)
    if(pWorldSub->bits[i] != (pWorldSub->bits[i] & pWorldRef->bits[i])) 
      return 0;
      
  return 1;
}

#endif