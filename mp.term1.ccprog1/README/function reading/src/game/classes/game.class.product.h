/**
 * I know C isn't an OOP but I'm gonna pretend it is.
 * In retrospect, I should have just named this class "Plant" instead of "Product", but I was too scared to refactor my code.
 * By the time I realized how much of a misnomer it was, I was well over 3000+ lines into coding this project.
 * I didn't want to have to sift my code for funky comment changes and weird bugs :/
 * 
 * A product has properties that depend on what plant type it is.
 * A product instance can have property values independent of other product instances.
 *    (As an example, two different PRODUCT_BANANA-type products can have different harvest times).
*/

#ifndef GAME_CLASS_PRODUCT
#define GAME_CLASS_PRODUCT

#include <stdlib.h>

// We need the catalogue
#include "../game.catalogue.h"

/**
 * Defines a product class, primarily for crops.
 * Calloc shenanigans again.
*/
struct Product {
  enum ProductType eType;
  char *sProductCode;
  char *sProductName;
  
  int dCostToBuy;
  int dCostToSell;

  int dWaterReq;
  int dWaterAmt;
  int dTimePlanted;
  int dLastWatered;
};

/**
 * ##############################
 * ###  PRODUCT CONSTRUCTION  ###
 * ##############################
*/

/**
 * Allocates memory for an instance of Product.
 * 
 * @return  {struct Product *}  Returns a pointer to the memory location where the instance was created.
*/
struct Product *Product_new() {
  struct Product *pProduct;
  
  pProduct = calloc(1, sizeof(*pProduct));
  
  // In case it wasn't able to allocate or not enough memory present
  if(pProduct == NULL)
    return NULL;

  return pProduct;
}

/**
 * Frees memory for a destroyed instance of Product.
 * 
 * @param   {struct Product *}  this  The instance to be destroyed.
*/
void Product_kill(struct Product *this) {
  free(this);
}

/**
 * Initializes the properties of the product.
 * 
 * @param   {struct Product *}  this          Pointer to an instance of Product.
 * @param   {enum ProductType}  eType         The type of product we got.
 * @param   {char}              sProductCode  The character that refers to the product type.  
 * @param   {char *}            sProductName  The name string used to refer to the product.
 * @param   {int}               dCostToBuy    Price of the product instance.
 * @param   {int}               dCostToSell   Selling price of the product instance.
 * @param   {int}               dWaterReq     How much water is needed to grow the product.
 * @param   {int}               dWaterAmt     How much water the plant has gotten.
 * @param   {int}               dTimePlanted  When the product was planted.
*/
void Product_init(struct Product *this, enum ProductType eType, char cProductCode, char *sProductName, int dCostToBuy, int dCostToSell, int dWaterReq, int dWaterAmt, int dTimePlanted) {
  this->eType = eType;
  this->sProductCode = calloc(1, sizeof(char));
  this->sProductCode[0] = cProductCode;
  this->sProductName = sProductName;
  
  this->dCostToBuy = dCostToBuy;
  this->dCostToSell = dCostToSell;
  
  this->dWaterReq = dWaterReq;
  this->dWaterAmt = dWaterAmt;
  
  this->dTimePlanted = dTimePlanted;
  this->dLastWatered = -1;
}

/**
 * A helper function that creates a product of a certain type with default values
 * 
 * @param   {enum ProductType}        eType         The type of product created.
 * @param   {struct GameCatalogue *}  pCatalogue    A reference for the information of the products.
 * @param   {int}                     dTimePlanted  When the product instance was planted.
 * @return  {struct Product *}                      Returns a pointer to the created instance.
*/
struct Product *Product_create(enum ProductType eType, struct GameCatalogue* pCatalogue, int dTimePlanted) {

  struct Product *pProduct = Product_new();
  Product_init(pProduct, eType,
    pCatalogue->cProductCodeArray[eType],
    pCatalogue->sProductNameArray[eType],
    pCatalogue->dProductCostToBuyArray[eType],
    pCatalogue->dProductCostToSellArray[eType],
    pCatalogue->dProductWaterReqArray[eType], 
    0, dTimePlanted);

  return pProduct;
}

/**
 * #####################################
 * ###  PRODUCT READERS AND WRITERS  ###
 * #####################################
*/

/**
 * A function that returns the growth state of the crop.
 * 0 means the crop is less than halfway being fully watered.
 * 1 means the crop is at least halfway being fully watered.
 * 2 means the crop has been fully watered.
 * 
 * @param   {struct Product *}  this  The product instance to be read.
 * @return  {int}                     The growth state of the product.
*/
int Product_getState(struct Product *this) {
  return (2 * this->dWaterAmt / this->dWaterReq);
}

/**
 * A function that type of the crop.
 * 
 * @param   {struct Product *}  this          The product instance to be read.
 * @return  {enum ProductType}                The type of the crop.
*/
enum ProductType Product_getType(struct Product *this) {
  return this->eType;
}

/**
 * A function that returns how much water the crop has received.
 * 
 * @param   {struct Product *}  this  The product instance to be read.
 * @return  {int}                     The amount of water already given to the product.
*/
int Product_getWaterAmt(struct Product *this) {
  return this->dWaterAmt;
}

/**
 * A function that returns how much water the crop needs.
 * 
 * @param   {struct Product *}  this  The product instance to be read.
 * @return  {int}                     The amount of water required by the product.
*/
int Product_getWaterReq(struct Product *this) {
  return this->dWaterReq;
}

/**
 * A function that returns the product code of the crop as a string.
 * 
 * @param   {struct Product *}  this  The product instance to be read.
 * @return  {char *}                  The product code.
*/
char *Product_getCode(struct Product *this) {
  return this->sProductCode;
}

/**
 * A function that returns the product name of the crop as a string.
 * 
 * @param   {struct Product *}  this  The product instance to be read.
 * @return  {char *}                  The product name.
*/
char *Product_getName(struct Product *this) {
  return this->sProductName;
}

/**
 * A function that returns when the product was last given water.
 * 
 * @param   {struct Product *}  this  The product instance to be read.
 * @return  {int}                     The time when the product was last given water.
*/
int Product_getLastWatered(struct Product *this) {
  return this->dLastWatered;
}

/**
 * #########################
 * ###  PRODUCT METHODS  ###
 * #########################
*/

/**
 * A function that waters the crop.
 * 
 * @param   {struct Product *}  this    The instance to be watered.
 * @param   {int}               dTime   The current day.
 * @return  {int}                       Whether or not the watering was successful.
*/
int Product_water(struct Product *this, int dTime) {
  
  // Can't be watered more than once in a day
  if(this->dLastWatered < dTime) {

    // Can't over water plants
    if(this->dWaterAmt < this->dWaterReq) {
      this->dLastWatered = dTime;
      this->dWaterAmt++;

      return 1;
    }
  }

  return 0;
}

#endif
