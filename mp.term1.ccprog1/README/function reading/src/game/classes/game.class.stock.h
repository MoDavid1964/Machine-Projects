/**
 * I know C isn't an OOP but I'm gonna pretend it is.
 * 
 * A class that stores information of stocks present in the store.
 * Also allows us to know how much stock the player has.
*/

#ifndef GAME_CLASS_STOCK
#define GAME_CLASS_STOCK

#include <stdlib.h>

// We need the catalogue
#include "../game.catalogue.h"

/**
 * Defines a stock class, primarily for fruits.
 * Calloc funkiness part 3.
*/
struct Stock {

  // What type of product is stored by the stock instance
  enum ProductType eProductType;

  int dCostToBuy;
  int dCostToSell;
  
  // How much of the stock exists
  // Helps us know how much the player has
  // Helps us know how much stock is left in the shop (although for the purposes of this game its infinite)
  int dAmount;
};

/**
 * ############################
 * ###  STOCK CONSTRUCTION  ###
 * ############################
*/

/**
 * Allocates memory for an instance of Stock.
 * 
 * @return  {struct Stock *}  Returns a pointer to the memory location where the instance was created.
*/
struct Stock *Stock_new() {
  struct Stock *pStock;
  
  pStock = calloc(1, sizeof(*pStock));
  
  // In case it wasn't able to allocate or not enough memory present
  if(pStock == NULL)
    return NULL;

  return pStock;
}

/**
 * Frees memory for a destroyed instance of Stock.
 * 
 * @param   {struct Stock *}  this  The instance to be destroyed.
*/
void Stock_kill(struct Stock *this) {
  free(this);
}

/**
 * Initializes the properties of the stock.
 * 
 * @param   {struct Stock *}    this          Pointer to an instance of stock.
 * @param   {enum ProductType}  eProductType  What type of product is stored by the stock instance.
 * @param   {int}               dCostToBuy    Price of the stock instance.
 * @param   {int}               dCostToSell   Selling price of the stock instance.
 * @param   {int}               dAmount       How much of the stock is stored by the instance.
*/
void Stock_init(struct Stock *this, enum ProductType eProductType, int dCostToBuy, int dCostToSell, int dAmount) {
  
  // Initialize the values
  this->eProductType = eProductType;
  this->dCostToBuy = dCostToBuy;
  this->dCostToSell = dCostToSell;
  this->dAmount = dAmount;
}

/**
 * A helper function that creates a stock of a certain amount and a certain type.
 * 
 * @param   {enum ProductType}        eProductType  The type of product in the stock.
 * @param   {struct GameCatalogue *}  pCatalogue    A reference for the information of the products. 
 * @param   {int}                     dAmount       How much of the product is in the stock instance.
 * @return  {struct Stock *}                        Returns a pointer to the created instance.
*/
struct Stock *Stock_create(enum ProductType eProductType, struct GameCatalogue *pCatalogue, int dAmount) {

  struct Stock *pStock = Stock_new();
  Stock_init(pStock, eProductType, 
    pCatalogue->dProductCostToBuyArray[eProductType], 
    pCatalogue->dProductCostToSellArray[eProductType], dAmount);
  
  return pStock;
}

/**
 * ###################################
 * ###  STOCK READERS AND WRITERS  ###
 * ###################################
*/

/**
 * Gets the current amount of product stored by the stock.
 * 
 * @param   {struct Stock *}  this      The stock instance.
 * @return  {int}                       How much product is stored by the stock.
*/
int Stock_getAmount(struct Stock *this) {
  return this->dAmount;
}

/**
 * Updates amount of product stored by the stock.
 * Returns a boolean on whether or not the change was successful.
 * 
 * @param   {struct Stock *}  this            The instance to be modified.
 * @param   {int}             dAmountChange   How much to add or subtract for the new amount of product.
 * @return  {int}                             A boolean that indicates whether or not the change was successful.
*/
int Stock_updateAmount(struct Stock *this, int dAmountChange) {
  if(this->dAmount + dAmountChange >= 0) {
    this->dAmount += dAmountChange;
    return 1;
  }

  return 0;
}

/**
 * Gets the price of a certain amount of stock for buying.
 * Returns 0 if there is not enough stock to buy that amount.
 * 
 * @param   {struct Stock *}  this      The instance to be read.
 * @param   {int}             dAmount   The amount of stock to be bought.
 * @return  {int}                       The price of the amount of stock when bought.
*/
int Stock_getBuyPrice(struct Stock *this, int dAmount) {
  if(dAmount <= this->dAmount) 
    return dAmount * this->dCostToBuy;

  return 0;
}

/**
 * Buys a certain amount of the stock.
 * Returns the price of buying that amount of stock.
 * 
 * @param   {struct Stock *}  this      The instance to be modified.
 * @param   {int}             dAmount   How much to buy.
 * @return  {int}                       A boolean that indicates whether or not the change was successful.
*/
int Stock_buyAmount(struct Stock *this, int dAmountChange) {
  if(Stock_updateAmount(this, -dAmountChange)) 
    return Stock_getBuyPrice(this, dAmountChange);

  return 0;
}

/**
 * Gets the price of a certain amount of stock for selling.
 * Returns 0 if there is not enough stock to sell that amount.
 * 
 * @param   {struct Stock *}  this      The instance to be read.
 * @param   {int}             dAmount   The amount of stock to be sold.
 * @return  {int}                       The price of the amount of stock when sold.
*/
int Stock_getSellPrice(struct Stock *this, int dAmount) {
  if(dAmount <= this->dAmount) 
    return dAmount * this->dCostToSell;

  return 0;
}

/**
 * Sells a certain amount of the stock.
 * Returns the cost of selling that amount of stock.
 * 
 * @param   {struct Stock *}  this      The instance to be modified.
 * @param   {int}             dAmount   How much to sell.
 * @return  {int}                       A boolean that returns the price of the selling.
*/
int Stock_sellAmount(struct Stock *this, int dAmountChange) {
  if(Stock_updateAmount(this, -dAmountChange)) 
    return Stock_getSellPrice(this, dAmountChange);

  return 0;
}

#endif
