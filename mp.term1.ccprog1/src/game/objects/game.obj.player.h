/**
 * The file creates the player object.
*/

#ifndef GAME_OBJ_PLAYER
#define GAME_OBJ_PLAYER

#include <stdlib.h>

#include "../game.catalogue.h"
#include "../classes/game.class.stock.h"

// 0: The player has the chance to continue if they get to eat breakfast at the start of the fourth day.
// 1: The player dies at the end of the third day of starving
#define PLAYER_STRICT_DEATH_MODE 1

#define PLAYER_NAME_MAX_LEN 32
#define PLAYER_BREAKFAST_COST 10
#define PLAYER_MAX_DAYS_STARVED 3

/**
 * A struct for the player object.
*/
struct Player {
  char *sName;

  int dTime;
  int dDaysStarved;
  int bIsStarving;

  int dGold;
  int dEnergy;
  int dDefaultEnergy;

  // The stuff of the player
  struct Stock *pSeedStockArray[CATALOGUE_SIZE];
  struct Stock *pCropStockArray[CATALOGUE_SIZE];
};

/**
 * #############################
 * ###  PLAYER CONSTRUCTION  ###
 * #############################
*/

/**
 * A constructor for the player object.
 * 
 * @return  {struct Player *}   A pointer to the created object.
*/
struct Player *Player_new() {
  struct Player *pPlayer;

  pPlayer = calloc(1, sizeof(*pPlayer));

  if(pPlayer == NULL)
    return NULL;

  return pPlayer;
}

/**
 * This function initializes the properties of the object.
 * 
 * @param   {struct Player *}         this            Pointer to the player object.
 * @param   {int}                     dGold           The starting amount of gold for the player.
 * @param   {int}                     dEnergy         The starting amount of energy for the player.
 * @param   {int}                     dDefaultEnergy  How much energy the player has after sleep.
 * @param   {struct GameCatalogue *}  pCatalogue      A reference for initializing the stock array.
*/
void Player_init(struct Player *this, int dGold, int dEnergy, int dDefaultEnergy, struct GameCatalogue *pCatalogue) {
  
  // Player name is set using a writing function
  this->sName = calloc(PLAYER_NAME_MAX_LEN, sizeof(char));

  this->dTime = 0;
  this->dDaysStarved = 0;
  this->bIsStarving = 0;

  this->dGold = dGold;
  this->dEnergy = dEnergy;
  this->dDefaultEnergy = dDefaultEnergy;

  // Initialize the stock array with all empty stock values
  for(int i = 0; i < pCatalogue->dSize; i++) {
    this->pSeedStockArray[i] = Stock_create(i, pCatalogue, 0);
    this->pCropStockArray[i] = Stock_create(i, pCatalogue, 0);
  }
}

/**
 * Creates an initialized player object.
 *  
 * @param   {int}                     dGold           The starting amount of gold for the player.
 * @param   {int}                     dEnergy         The starting amount of energy for the player.
 * @param   {int}                     dDefaultEnergy  How much energy the player has after sleep.
 * @param   {struct GameCatalogue *}  pCatalogue      A reference for initializing the stock array.
 * @return  {struct Player *}                         A pointer to the created object.
*/
struct Player *Player_create(int dGold, int dEnergy, int dDefaultEnergy, struct GameCatalogue *pCatalogue) {
  struct Player *pPlayer = Player_new();
  Player_init(pPlayer, dGold, dEnergy, dDefaultEnergy, pCatalogue);

  return pPlayer;
}

/**
 * Destroys a player object.
 *  
 * @param  {struct Player *}  this  A pointer to the object.
*/
void Player_kill(struct Player *this) {
  free(this);
}

/**
 * ####################################
 * ###  PLAYER READERS AND WRITERS  ###
 * ####################################
*/

/**
 * Sets the name of the player object.
 * If the provided name is too long, then nothing happens.
 * 
 * @param   {struct Player *}   this    The object to be modified.
 * @param   {char *}            sName   The name to be given.
*/
void Player_setName(struct Player *this, char *sName) {
  if(strlen(sName) <= PLAYER_NAME_MAX_LEN) strcpy(this->sName, sName);
  else strcpy(this->sName, "your name is too long :p");
}

/**
 * Gets the name of the player object.
 * 
 * @param   {struct Player *}   this    The object to be read.
 * @return  {char *}                    The name of the player object.
*/
char *Player_getName(struct Player *this) {
  return this->sName;
}

/**
 * Gets the current time stored in the player.
 * 
 * @param   {struct Player *}   this    The object to be read.
 * @return  {int}                       The current time stored by the player.
*/
int Player_getTime(struct Player *this) {
  return this->dTime;
}

/**
 * Returns the amount of gold the player has.
 * 
 * @param   {struct Player *}   this    The object to be read.
 * @return  {int}                       The current wealth of the player.
*/
int Player_getGold(struct Player *this) {
  return this->dGold;
}

/**
 * Returns the amount of energy the player has.
 * 
 * @param   {struct Player *}   this    The object to be read.
 * @return  {int}                       The current energy of the player.
*/
int Player_getEnergy(struct Player *this) {
  return this->dEnergy;
}

/**
 * Gets a piece of the inventory of the player.
 * 
 * @param   {struct Player *}   this          The object to be read.
 * @param   {ProductType}       eProductType  Which seed type to select.
 * @return  {struct Stock *}                  The selected seed from the seed bags.
*/
struct Stock *Player_getSeedStock(struct Player *this, enum ProductType eProductType) {
  return this->pSeedStockArray[eProductType];
}

/**
 * Sets how much seed bags were used or purchased by the player.
 * 
 * @param   {struct Player *}   this            The object to be read.
 * @param   {ProductType}       eProductType    Which seed type to select.
 * @param   {int}               dChangeAmount   How much seed bags were acquired or used.
*/
void Player_updateSeedStock(struct Player *this, enum ProductType eProductType, int dChangeAmount) {
  if(this->pSeedStockArray[eProductType]->dAmount + dChangeAmount >= 0) 
    this->pSeedStockArray[eProductType]->dAmount += dChangeAmount;
}

/**
 * Gets a piece of the inventory of the player.
 * 
 * @param   {struct Player *}   this          The object to be read.
 * @param   {ProductType}       eProductType  Which crop type to select.
 * @return  {struct Stock *}                  The selected stock from the harvested crops.
*/
struct Stock *Player_getCropStock(struct Player *this, enum ProductType eProductType) {
  return this->pCropStockArray[eProductType];
}

/**
 * Sets how much crops were sold or harvested by the player.
 * 
 * @param   {struct Player *}   this            The object to be read.
 * @param   {ProductType}       eProductType    Which crop type to select.
 * @param   {int}               dChangeAmount   How much crops were acquired or sold.
*/
void Player_updateCropStock(struct Player *this, enum ProductType eProductType, int dChangeAmount) {
  if(this->pCropStockArray[eProductType]->dAmount + dChangeAmount >= 0) 
    this->pCropStockArray[eProductType]->dAmount += dChangeAmount;
}

/**
 * Updates the player's amount of gold.
 * Returns 0 if the change was not successful.
 * 
 * @param   {struct Player *}   this          The object to be read.
 * @param   {int}               dGoldChange   How much change to add / subtract to the player's wealth.
 * @return  {int}                             A boolean indicating whether or not the change was successful.
*/
int Player_updateGold(struct Player *this, int dGoldChange) {
  if(this->dGold + dGoldChange >= 0) {
    this->dGold += dGoldChange;
    
    // Is not broke
    return 1;
  } else {

    // Is broke
    return 0;
  }
}

/**
 * Updates the player's amount of energy.
 * Returns 0 if the change was not successful.
 * 
 * @param   {struct Player *}   this            The object to be read.
 * @param   {int}               dEnergyChange   How much energy to add / subtract to the player's energy.
 * @return  {int}                               A boolean indicating whether or not the change was successful.
*/
int Player_updateEnergy(struct Player *this, int dEnergyChange) {
  if(this->dEnergy + dEnergyChange >= 0) {
    this->dEnergy += dEnergyChange;
    
    // Is not tired
    return 1;
  } else {

    // Is tired
    return 0;
  }
}

/**
 * Checks if player has died of starvation.
 * 
 * @param   {struct Player *}   this  The player object.
 * @return  {int}                     Has the player died of starvation?
*/
int Player_isDead(struct Player *this) {
  return this->dDaysStarved >= PLAYER_MAX_DAYS_STARVED + 1;
}

/**
 * Checks if player is starving for the current day.
 * 
 * @param   {struct Player *}   this  The player object.
 * @return  {int}                     Is the player starving?
*/
int Player_isStarving(struct Player *this) {
  return this->bIsStarving;
}

/**
 * ########################
 * ###  PLAYER METHODS  ###
 * ########################
*/

/**
 * Makes the player go home.
 * They sleep then eat breakfast when they wake up, ASSUMING they can buy it.
 * 
 * @param   {struct Player *}   this  The object who will perform the action.
*/
void Player_goHome(struct Player *this) {

  // Update day counter and days starved and replenish energy
  this->dTime++;
  this->dDaysStarved++;
  this->bIsStarving = 1;
  Player_updateEnergy(this, this->dDefaultEnergy - this->dEnergy);

  // If they get to eat, they don't starve; depends on the death mode strictness
  if(!PLAYER_STRICT_DEATH_MODE) {
    if(Player_updateGold(this, -PLAYER_BREAKFAST_COST)) {
      this->dDaysStarved = 0;
      this->bIsStarving = 0;
    }
  } else {
    if(!Player_isDead(this)) {
      if(Player_updateGold(this, -PLAYER_BREAKFAST_COST)) {
        this->dDaysStarved = 0;
        this->bIsStarving = 0;
      }
    }
  }
}

/**
 * Updates the player state after tilling the given number of plots.
 * 
 * @param   {struct Player *}   this    The player object.
 * @param   {int}               dPlots  How many plots are gonna be tilled.
 * @return  {int}                       A boolean indicating whether or not the action was successful.
*/
int Player_tillPlots(struct Player *this, int dPlots) {
  return Player_updateEnergy(this, -dPlots);
}

/**
 * Updates the players state after sowing the provided amount of seeds.
 * 
 * @param   {struct Player *}   this          The player object.
 * @param   {enum ProductType}  eProductType  The kind of seed to be sown.
 * @param   {int}               dSeeds        The number of seeds that are to be sown.
 * @return  {int}                             A boolean indicating whether or not the action was successful.
*/
int Player_sowSeeds(struct Player *this, enum ProductType eProductType, int dSeeds) {
  if(Stock_getAmount(this->pSeedStockArray[eProductType]) >= dSeeds) {
    if(Player_updateEnergy(this, -dSeeds)) {
      return Stock_updateAmount(this->pSeedStockArray[eProductType], -dSeeds);
    }
  }

  return 0;
}

/**
 * Updates the player's state after watering crops.
 * 
 * @param   {struct Player *}   this    The player object.
 * @param   {int}               dCrops  The number of crops to be watered.
 * @return  {int}                       A boolean indicating whether or not the watering was succesful.
*/
int Player_waterCrops(struct Player *this, int dCrops) {
  return Player_updateEnergy(this, -dCrops);
}

/**
 * Updates the state of the player after harvesting a number of various crops.
 * 
 * @param   {struct Player *}   this    The player object.
 * @param   {int}               dCrops  The number of crops to be harvested.
 * @return  {int}                       Whether or not the action was successful.
*/
int Player_harvestCrops(struct Player *this, int dCrops) {
  return Player_updateEnergy(this, -dCrops);
}

/**
 * Harvests a single crop of the specified type.
 * 
 * @param   {struct Player *}   this          The player object.
 * @param   {enum ProductType}  eProductType  The type of the product to be harvested.
*/
void Player_harvestACrop(struct Player *this, enum ProductType eProductType) {
  Stock_updateAmount(this->pCropStockArray[eProductType], 1);
}

/**
 * Updates the state of the player after buying a certain amount of seeds.
 * 
 * @param   {struct Player *}   this          The player object.
 * @param   {enum ProductType}  eProductType  The type of product to be sold.
 * @param   {int}               dAmount       How much of the product is to be sold.
 * @param   {int}               dCost         How much you need to pay for the crops.
 * @return  {int}                             A boolean indicating whether or not the action was successful.
*/
int Player_buyCrop(struct Player *this, enum ProductType eProductType, int dAmount, int dCost) {
  if(Player_updateGold(this, -dCost)) {
    Player_updateSeedStock(this, eProductType, dAmount);

    return 1;
  }

  return 0;
}

/**
 * Updates the state of the player after selling a certain amount of crops.
 * 
 * @param   {struct Player *}   this          The player object.
 * @param   {enum ProductType}  eProductType  The type of product to be sold.
 * @param   {int}               dAmount       How much of the product is to be sold.
 * @param   {int}               dCost         How much you earn from the crops.
 * @return  {int}                             A boolean indicating whether or not the action was successful.
*/
int Player_sellCrop(struct Player *this, enum ProductType eProductType, int dAmount, int dCost) {
  if(Stock_getAmount(Player_getCropStock(this, eProductType)) >= dAmount) {
    Stock_updateAmount(this->pCropStockArray[eProductType], -dAmount);

    return Player_updateGold(this, dCost);
  }

  return 0;
}

#endif