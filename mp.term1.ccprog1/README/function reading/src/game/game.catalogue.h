/**
 * A catalogue of the different products within the game.
*/

#ifndef GAME_CATALOGUE
#define GAME_CATALOGUE

// Number of the types of crops we have in the game (including the null value)
#define CATALOGUE_SIZE 4

/**
 * An enum that stores all the different types of products in the game.
 * Note that I have decided to omit the other crop for the sake of following the course specs.
*/
enum ProductType { 
  PRODUCT_NULL,
  // PRODUCT_APPLE,
  PRODUCT_BANANA, 
  PRODUCT_CORN, 
  PRODUCT_MANGO, 
};

/**
 * Stores the different information we need about the crops.
 * The name of the crop, the character code, and its other specific details.
*/
struct GameCatalogue {

  // Arrays for storing pertinent product information
  char cProductCodeArray[CATALOGUE_SIZE];
  char *sProductNameArray[CATALOGUE_SIZE];

  int dProductCostToBuyArray[CATALOGUE_SIZE];
  int dProductCostToSellArray[CATALOGUE_SIZE];
  int dProductWaterReqArray[CATALOGUE_SIZE];

  int dSize;
};

/**
 * A function that initializes the information in the catalogue.
 * 
 * @param   {struct GameCatalogue *}  this  The catalogue.
*/
void GameCatalogue_init(struct GameCatalogue *this) {
  
  // this->cProductCodeArray[PRODUCT_APPLE] = 'A';
  // this->sProductNameArray[PRODUCT_APPLE] = "apple";
  // this->dProductCostToBuyArray[PRODUCT_APPLE] = 6;
  // this->dProductCostToSellArray[PRODUCT_APPLE] = 9;
  // this->dProductWaterReqArray[PRODUCT_APPLE] = 7;
  
  this->cProductCodeArray[PRODUCT_BANANA] = 'B';
  this->sProductNameArray[PRODUCT_BANANA] = "banana";
  this->dProductCostToBuyArray[PRODUCT_BANANA] = 3;
  this->dProductCostToSellArray[PRODUCT_BANANA] = 4;
  this->dProductWaterReqArray[PRODUCT_BANANA] = 4;

  this->cProductCodeArray[PRODUCT_CORN] = 'C';
  this->sProductNameArray[PRODUCT_CORN] = "corn";
  this->dProductCostToBuyArray[PRODUCT_CORN] = 5;
  this->dProductCostToSellArray[PRODUCT_CORN] = 7;
  this->dProductWaterReqArray[PRODUCT_CORN] = 6;

  this->cProductCodeArray[PRODUCT_MANGO] = 'M';
  this->sProductNameArray[PRODUCT_MANGO] = "mango";
  this->dProductCostToBuyArray[PRODUCT_MANGO] = 7;
  this->dProductCostToSellArray[PRODUCT_MANGO] = 10;
  this->dProductWaterReqArray[PRODUCT_MANGO] = 8;

  this->dSize = CATALOGUE_SIZE;
}

#endif