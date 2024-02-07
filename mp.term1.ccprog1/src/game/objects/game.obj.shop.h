/**
 * The game shop.
 * The UI and IO components of the shop in full mode are separated here.
 * However, the same is not true for the default mode as such components were minimal therein.
*/

#ifndef GAME_OBJ_SHOP
#define GAME_OBJ_SHOP

#include "../../utils/utils.selector.h"
#include "../../utils/utils.key.h"
#include "../../utils/utils.ui.h"

#include "../game.catalogue.h"
#include "../game.assets.h"

#include "../classes/game.class.stock.h"
#include "../enums/game.enum.shop.h"
#include "../enums/game.enum.state.h"

/**
 * A struct for the shop singleton.
*/
struct Shop {
  int dRepositorySize;
  char *sActionResponse;

  enum ShopAction eCurrentAction;
  enum ProductType eCurrentCrop;

  char *sPastActionNameArray[3];
  char *sPresentActionNameArray[3];

  // The stuff in the shop
  struct Stock *pStockArray[CATALOGUE_SIZE];

  // Helps do the selection 
  struct UtilsSelector *pShopSelector;
};

/**
 * ###########################
 * ###  SHOP CONSTRUCTION  ###
 * ###########################
*/

/**
 * Creates a new shop object.
 * 
 * @return  {struct Shop *}   A pointer to the object.
*/
struct Shop *Shop_new() {
  struct Shop *pShop;

  pShop = calloc(1, sizeof(*pShop));

  if(pShop == NULL)
    return NULL;

  return pShop;
}

/**
 * Initializes the shop object.
 * 
 * @param   {struct Shop *}   this    The instance of the shop class to be initialized.
 * @param   {struct GameCatalogue *}  pCatalogue  The catalogue to be used as a reference for the shop.
*/
void Shop_init(struct Shop *this, struct GameCatalogue *pCatalogue) {

  // Set up the shop data
  this->dRepositorySize = pCatalogue->dSize;
  this->sActionResponse = UtilsUI_createBuffer();

  this->eCurrentAction = SHOP_NULL;
  this->eCurrentCrop = PRODUCT_NULL;

  this->sPastActionNameArray[SHOP_NULL] = "nothing";
  this->sPastActionNameArray[SHOP_BUY] = "bought";
  this->sPastActionNameArray[SHOP_SELL] = "sold";

  this->sPresentActionNameArray[SHOP_NULL] = "nothing";
  this->sPresentActionNameArray[SHOP_BUY] = "buying";
  this->sPresentActionNameArray[SHOP_SELL] = "selling";
  
  // Populate the stock array
  for(int i = 0; i < pCatalogue->dSize; i++) {

    // Infinite stock for each item
    struct Stock *pStock = Stock_create(i, pCatalogue, (1 << 24));

    // Copy into the array
    this->pStockArray[i] = pStock;
  }

  // Create the shop selector
  struct UtilsSelector *pShopSelector = UtilsSelector_create(0, 
    "     [ ]  %s   ", 
    "     -=> [x]  %s       ", 
    "     [-]  %s   ");

  // Shop options
  UtilsSelector_addOption(pShopSelector, "buy seeds     ", SHOP_BUY);
  UtilsSelector_addOption(pShopSelector, "sell crops    ", SHOP_SELL);
  UtilsSelector_addOption(pShopSelector, "do nothing    ", SHOP_NULL);

  this->pShopSelector = pShopSelector;
}

/**
 * Creates an initialized shop object.
 * 
 * @param   {struct GameCatalogue *}  pCatalogue  A reference for the plant info.
 * @return  {struct Shop *}                       A pointer to the created object.
*/
struct Shop *Shop_create(struct GameCatalogue *pCatalogue) {
  struct Shop *pShop = Shop_new();

  Shop_init(pShop, pCatalogue);

  return pShop;
}

/**
 * Destroys a shop object.
 * 
 * @param   {struct Shop *}   A pointer to the object to be destroyed.
*/
void Shop_kill(struct Shop *this) {
  free(this);
}

/**
 * ##################################
 * ###  SHOP READERS AND WRITERS  ###
 * ##################################
*/

/**
 * Returns the current action selected for the shop.
 * 
 * @param   {struct Shop *}     this  The shop object.
 * @return  {enum ShopAction}         The current action selected by the user.
*/
enum ShopAction Shop_getCurrentAction(struct Shop *this) {
  return this->eCurrentAction;
}

/**
 * Sets the current action selected for the shop.
 * 
 * @param   {struct Shop *}     this          The shop object.
 * @param   {enum ShopAction}   eShopAction   The current action selected by the user.
*/
void Shop_setCurrentAction(struct Shop *this, enum ShopAction eShopAction) {
  this->eCurrentAction = eShopAction;
}

/**
 * Returns the current crop selected for the shop.
 * 
 * @param   {struct Shop *}   this  The shop object.
 * @return  {int}                   The current crop selected by the user.
*/
enum ProductType Shop_getCurrentCrop(struct Shop *this) {
  return this->eCurrentCrop;
}

/**
 * Sets the current crop selected for the shop.
 * 
 * @param   {struct Shop *}     this          The shop object.
 * @param   {enum ProductType}  eProductType  The current product selected by the user.
*/
void Shop_setCurrentCrop(struct Shop *this, enum ProductType eProductType) {
  this->eCurrentCrop = eProductType;
}

/**
 * ######################
 * ###  SHOP METHODS  ###
 * ######################
*/

/**
 * A function that returns how much a certain number of items costs.
 * 
 * @param   {struct Shop *}     this          The shop object.
 * @param   {int}               dAmount       The amount of product we want to buy.
 * @return  {int}                             The total cost of the products.
*/
int Shop_getCurrentBuyCost(struct Shop *this, int dAmount) {
  return this->pStockArray[this->eCurrentCrop]->dCostToBuy * dAmount;
}

/**
 * A function that allows you to buy a certain amount of product from the shop.
 * Returns 0 if the purchase was not successful.
 * 
 * @param   {struct Shop *}     this          The shop object.
 * @param   {int}               dAmount       The amount of product we want to buy.
 * @return  {int}                             Indicates whether or not the purchase was successful.
*/
int Shop_buyCurrentProduct(struct Shop *this, int dAmount) {
  if(dAmount < this->pStockArray[this->eCurrentCrop]->dAmount) {
    this->pStockArray[this->eCurrentCrop]->dAmount -= dAmount;
    return 1;
  }
  return 0;
}

/**
 * A function that returns how much a certain number of items will give.
 * 
 * @param   {struct Shop *}     this          The shop object.
 * @param   {int}               dAmount       The amount of product we want to sell.
 * @return  {int}                             The total gold generated by the products.
*/
int Shop_getCurrentSellCost(struct Shop *this, int dAmount) {
  return this->pStockArray[this->eCurrentCrop]->dCostToSell * dAmount;
}

/**
 * A function that allows you to sell a certain amount of product to the shop.
 * 
 * @param   {struct Shop *}     this          The shop object.
 * @param   {int}               dAmount       The amount of product we want to sell.
*/
void Shop_sellCurrentProduct(struct Shop *this, int dAmount) {
  this->pStockArray[this->eCurrentCrop]->dAmount += dAmount;
}

/**
 * #######################
 * ###  SHOP UI AND IO ###
 * #######################
*/

/**
 * Displays the shop UI.
 * 
 * @param   {struct Shop *}           this                  The shop object whose information we're using.
 * @param   {struct Player *}         pPlayer               The player object whose information we're using.
 * @param   {struct UtilsText *}      pScreenText           The text object we're the display will be placed.
 * @param   {struct UtilsSelector *}  pCatalogueSelector    A selector for all the game products.
 * @param   {struct GameCatalogue *}  pCatalogue            A list of all the products available in the game.
 * @param   {char **}                 sShopSprite           The shop ASCII art.
 * @param   {int}                     dShopSpriteSize       The size of the ASCII art.
 * @param   {char *}                  sCurrentIntInput      Current user input needed for some stuff in the game.
 * @param   {char *}                  sInputWarning         A warning for the user input in case its not formatted right.
*/
void Shop_UI(
  struct Shop *this, 
  struct Player *pPlayer,
  struct UtilsText *pScreenText,  
  struct UtilsSelector *pCatalogueSelector,

  struct GameCatalogue *pCatalogue,
  char **sShopSprite, 
  int dShopSriteSize, 
  
  char *sCurrentIntInput, 
  char *sInputWarning) {

  // The player is currently selecting an action to do on the shop
  if(this->eCurrentAction == SHOP_NULL) {

    // Show shop shack
    UtilsText_addNewLines(pScreenText, 2);
    UtilsText_addText(pScreenText, "You are now at the shop.");
    UtilsText_addNewLines(pScreenText, 1);
    UtilsText_addBlock(pScreenText, sShopSprite, dShopSriteSize);
    UtilsText_addNewLines(pScreenText, 3);
    UtilsText_addText(pScreenText, "What do you want to do?");
    UtilsText_addNewLines(pScreenText, 1);

    // Display shop action selector
    for(int i = 0; i < UtilsSelector_getLength(this->pShopSelector); i++)
      UtilsText_addText(pScreenText, UtilsSelector_getOptionFormatted(this->pShopSelector, i));
    UtilsText_addNewLines(pScreenText, 1);

  // There is already an action to be done
  } else {

    // Configure and display the selector
    UtilsSelector_setAllAvailability(pCatalogueSelector, 0);
    switch(this->eCurrentAction) {
      
      // User is gonna buy some seeds
      case SHOP_BUY: 
        for(int i = 1; i < pCatalogue->dSize; i++)
          if(Player_getGold(pPlayer) >= pCatalogue->dProductCostToBuyArray[i])
            UtilsSelector_setOptionAvailability(pCatalogueSelector, i, 1);
        UtilsSelector_setOptionAvailability(pCatalogueSelector, pCatalogue->dSize, 1);
        UtilsSelector_setFirstAvailable(pCatalogueSelector);

        // User can purchase seed bags
        if(UtilsSelector_getAvailableCount(pCatalogueSelector) - 1 || strlen(this->sActionResponse)) {
          UtilsText_addText(pScreenText, "You are now choosing a seed type to (BUY).");
          UtilsText_addNewLines(pScreenText, 1);

          // Print the options
          UtilsText_addText(pScreenText, "                                   | buying price    ");
          UtilsText_addText(pScreenText, "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|=-=-=-=-=-=-=-=-=");
          
          for(int i = 1; i <= pCatalogue->dSize; i++) { 
            char *sCatalogueLine = UtilsUI_createBuffer();
            if(i != pCatalogue->dSize) sprintf(sCatalogueLine, "%24s | %-2d gold %7s",
                UtilsSelector_getOptionFormatted(pCatalogueSelector, i), pCatalogue->dProductCostToBuyArray[i], " ");
            else sprintf(sCatalogueLine, "%24s |                ", 
                UtilsSelector_getOptionFormatted(pCatalogueSelector, i));
            UtilsText_addText(pScreenText, sCatalogueLine);
          }

          // The user has chosen a product to buy
          if(this->eCurrentCrop != PRODUCT_NULL) {
            char *sProductName = UtilsUI_createBuffer();
            char *sPromptLine = UtilsUI_createBuffer();
            char *sInfoLine = UtilsUI_createBuffer();
            
            // Define the different strings to print
            sProductName = UtilsUI_toUpper(pCatalogue->sProductNameArray[this->eCurrentCrop]);

            // Where the user input is gonna appear
            sprintf(sPromptLine, "No. of (%s) seeds to (BUY); (0) to cancel: %-16s", sProductName,
              strlen(sCurrentIntInput) ? sCurrentIntInput : "________________");
            
            // Show how much gold the amount of seeds costs
            if(UtilsKey_stringToInt(sCurrentIntInput) >= 0)
              sprintf(sInfoLine, "%*s                          (GOLD) needed: %-16d", 
                (int) strlen(sProductName), " ",
                Shop_getCurrentBuyCost(this, UtilsKey_stringToInt(sCurrentIntInput)));

            // Display the dormatted strings
            UtilsText_addNewLines(pScreenText, 2);
            UtilsText_addText(pScreenText, sPromptLine);
            UtilsText_addText(pScreenText, sInfoLine);

            if(strlen(sInputWarning)) {
              UtilsText_addNewLines(pScreenText, 2);
              UtilsText_addText(pScreenText, sInputWarning);
              UtilsText_addNewLines(pScreenText, 1);
            } else {
              UtilsText_addNewLines(pScreenText, 4);
            }
          } else {
            UtilsText_addNewLines(pScreenText, 2);
            if(strlen(this->sActionResponse)) UtilsText_addText(pScreenText, this->sActionResponse);
            else UtilsText_addNewLines(pScreenText, 1);
            UtilsText_addNewLines(pScreenText, 5);
          }

        // User does not have enough gold to make a purchase
        } else {
          UtilsText_addNewLines(pScreenText, 1); 
          UtilsText_addText(pScreenText, "You currently do not have enough gold to buy any seeds.");
          UtilsText_addNewLines(pScreenText, 1);
          UtilsText_addText(pScreenText, "Try again later once you have something to buy with.");
          UtilsText_addNewLines(pScreenText, 1);
          UtilsText_addText(pScreenText, "[Enter] to continue doing something else.");  
          UtilsText_addNewLines(pScreenText, 1);
        }
        break;

      case SHOP_SELL:

        // Configure selection
        for(int i = 1; i < pCatalogue->dSize; i++)
          if(Stock_getAmount(Player_getCropStock(pPlayer, i)))
            UtilsSelector_setOptionAvailability(pCatalogueSelector, i, 1);
        UtilsSelector_setOptionAvailability(pCatalogueSelector, pCatalogue->dSize, 1);
        UtilsSelector_setFirstAvailable(pCatalogueSelector);

        if(UtilsSelector_getAvailableCount(pCatalogueSelector) - 1 || strlen(this->sActionResponse)) {
          UtilsText_addText(pScreenText, "You are now choosing a crop to (SELL)."); 
          UtilsText_addNewLines(pScreenText, 1);

          // Print the options
          UtilsText_addText(pScreenText, "                                   | selling price   ");
          UtilsText_addText(pScreenText, "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=|=-=-=-=-=-=-=-=-=");

          for(int i = 1; i <= pCatalogue->dSize; i++) { 
            char *sCatalogueLine = UtilsUI_createBuffer();
            if(i != pCatalogue->dSize) sprintf(sCatalogueLine, "%24s | %-2d gold %7s",
                UtilsSelector_getOptionFormatted(pCatalogueSelector, i),
                pCatalogue->dProductCostToSellArray[i], " ");
            else sprintf(sCatalogueLine, "%24s |                ", 
                UtilsSelector_getOptionFormatted(pCatalogueSelector, i));
            UtilsText_addText(pScreenText, sCatalogueLine);
          }

          // The user has chosen a product to sell
          if(this->eCurrentCrop != PRODUCT_NULL) {
            char *sProductName = UtilsUI_createBuffer();
            char *sPromptLine = UtilsUI_createBuffer();
            char *sInfoLine = UtilsUI_createBuffer();
            
            // Define the different strings to print
            sProductName = UtilsUI_toUpper(pCatalogue->sProductNameArray[this->eCurrentCrop]);

            // Where the user input is gonna appear
            sprintf(sPromptLine, "No. of (%s) crops to (SELL); (0) to cancel: %-16s", sProductName,
              strlen(sCurrentIntInput) ? sCurrentIntInput : "________________");
            
            // Show how much gold the amount of crops will give
            if(UtilsKey_stringToInt(sCurrentIntInput) >= 0)
              sprintf(sInfoLine, "%*s                           (GOLD) profit: %-16d", 
                (int) strlen(sProductName), " ",
                Shop_getCurrentSellCost(this, UtilsKey_stringToInt(sCurrentIntInput)));

            // Display the dormatted strings
            UtilsText_addNewLines(pScreenText, 2);
            UtilsText_addText(pScreenText, sPromptLine);
            UtilsText_addText(pScreenText, sInfoLine);

            if(strlen(sInputWarning)) {
              UtilsText_addNewLines(pScreenText, 2);
              UtilsText_addText(pScreenText, sInputWarning);
              UtilsText_addNewLines(pScreenText, 1);
            } else {
              UtilsText_addNewLines(pScreenText, 4);
            }
          } else {
            UtilsText_addNewLines(pScreenText, 2);
            if(strlen(this->sActionResponse)) UtilsText_addText(pScreenText, this->sActionResponse);
            else UtilsText_addNewLines(pScreenText, 1);
            UtilsText_addNewLines(pScreenText, 5);
          }

        // User does not have any crops they can sell
        } else {
          UtilsText_addNewLines(pScreenText, 1); 
          UtilsText_addText(pScreenText, "You currently do not have any harvested crops to sell.");
          UtilsText_addNewLines(pScreenText, 1);
          UtilsText_addText(pScreenText, "Try again later once you have something to sell.");
          UtilsText_addNewLines(pScreenText, 1);
          UtilsText_addText(pScreenText, "[Enter] to continue doing something else."); 
          UtilsText_addNewLines(pScreenText, 1); 
        }
        break;

      default: break;
    }
  }
}

/**
 * Handles the interaction with the shop object.
 * 
 * @param   {struct Shop *}           this                  The shop object.
 * @param   {struct Player *}         pPlayer               The player object whose information we're using.
 * @param   {struct UtilsSelector *}  pCatalogueSelector    A selector for all the game products.
 * @param   {struct GameCatalogue *}  pCatalogue            A reference to the info on the different game products.
 * @param   {char}                    cInput                The current key input of the user.
 * @param   {char *}                  sCurrentIntInput      The current integer input of the user.
 * @param   {char *}                  sInputWarning         A warning for the user input in case its not formatted right.
 * @param   {enum PlayState *}        pPlayState            A reference to the current PlayState.
 * @param   {enum GameState *}        pGameState            A reference to the current GameState.
*/
void Shop_IO(
  struct Shop *this, 
  struct Player *pPlayer,
  struct UtilsSelector *pCatalogueSelector,
  struct GameCatalogue *pCatalogue,
  
  char cInput, 
  char *sCurrentIntInput, 
  char *sInputWarning, 
  
  enum PlayState *pPlayState,
  enum GameState *pGameState) {
  
  // Choose actions first
  if(this->eCurrentAction == SHOP_NULL) {
    if(cInput == 'X') UtilsSelector_decrement(this->pShopSelector);
    if(cInput == 'C') UtilsSelector_increment(this->pShopSelector);

    // If player hits enter
    if(UtilsKey_isReturn(cInput, "")) {
      this->eCurrentAction = UtilsSelector_getCurrentValue(this->pShopSelector);
      
      // Return to main selection area
      if(UtilsSelector_getCurrentValue(this->pShopSelector) == SHOP_NULL)
        *pPlayState = PLAY_SELECTING;
    }

  // Select product to sell or buy
  } else {

    // Clear the console warning and input response each time
    strcpy(sInputWarning, "");

    // User is choosing a product
    if(this->eCurrentCrop == PRODUCT_NULL) {
      if(cInput == 'X') UtilsSelector_decrement(pCatalogueSelector);
      if(cInput == 'C') UtilsSelector_increment(pCatalogueSelector);
      
      // Enter is hit
      if(UtilsKey_isReturn(cInput, "") && !strlen(this->sActionResponse)) {

        // Proceed to choose which product to buy or sell
        if(UtilsSelector_getAvailableCount(pCatalogueSelector) && 
          UtilsSelector_getCurrentValue(pCatalogueSelector) != pCatalogue->dSize) {
          
          strcpy(sCurrentIntInput, "");
          this->eCurrentCrop = UtilsSelector_getCurrentValue(pCatalogueSelector);
        
        // If can't do anything, go back
        } else {
          this->eCurrentAction = SHOP_NULL;
        }
      }

      // Reset sActionResponse
      strcpy(this->sActionResponse, "");

    // Product has been chosen; user is inputting how much they want
    } else {   
      char *sInput = calloc(1, sizeof(char));
      sprintf(sInput, "%c", cInput);

      // If user is not done inputting
      if(!UtilsKey_isReturn(cInput, "")) {
        
        // While it hasn't exceeded the max value or something
        if(strlen(sCurrentIntInput) < UTILS_KEY_MAX_DIGITS) {
          if(!UtilsKey_isBackspace(cInput, "")) {
            if(UtilsKey_isNum(cInput)) 
              strcat(sCurrentIntInput, sInput);
          }
        }

        // Backspace handling condition
        if(strlen(sCurrentIntInput)) {
          if(UtilsKey_isBackspace(cInput, ""))
            sCurrentIntInput[strlen(sCurrentIntInput) - 1] = 0;
        }
      
      // The user is done inputting the number
      } else {
        if(UtilsKey_isReturn(cInput, "")) {
          int dAmount = UtilsKey_stringToInt(sCurrentIntInput);

          // If input was a valid number
          if(dAmount >= 0) {
            switch(UtilsSelector_getCurrentValue(this->pShopSelector)) {
              
              // User is gonna buy those stuff
              case SHOP_BUY:
                if(Player_buyCrop(pPlayer, this->eCurrentCrop, dAmount, Shop_getCurrentBuyCost(this, dAmount))) {
                  Shop_buyCurrentProduct(this, dAmount);
                  sprintf(this->sActionResponse, "You just (BOUGHT) an amount of (%d) (%s) seeds.", 
                    dAmount, UtilsUI_toUpper(pCatalogue->sProductNameArray[this->eCurrentCrop]));

                  this->eCurrentCrop = PRODUCT_NULL;
                } else strcpy(sInputWarning, "You do not have enough gold to make that purchase.");
                break;
                
              // User is gonna sell those stuff
              case SHOP_SELL:
                if(Player_sellCrop(pPlayer, this->eCurrentCrop, dAmount, Shop_getCurrentSellCost(this, dAmount))) {
                  Shop_sellCurrentProduct(this, dAmount);
                  sprintf(this->sActionResponse, "You just (SOLD) an amount of (%d) (%s) crops.", 
                    dAmount, UtilsUI_toUpper(pCatalogue->sProductNameArray[this->eCurrentCrop]));

                  this->eCurrentCrop = PRODUCT_NULL;
                } else strcpy(sInputWarning, "You do not have enough crops to sell that amount.");
                break;
            }
          
          // Input was not valid; try again
          } else {
            strcpy(sInputWarning, "Number must be >= 0 and < 10^9. Enter a (0) to cancel.");
          }
        }
      }
    }
  }
}

/**
 * Creates the footer while in the shop.
 * 
 * @param   {struct Shop *}           this                        The shop object.
 * @param   {struct Player *}         pPlayer                     The player object whose information we're using.
 * @param   {struct UtilsText *}      pFooterText                 The footer text object to be modified.
 * @param   {struct UtilsSelector *}  pCatalogueSelector          A selector for all the game products.
 * @param   {struct GameCatalogue *}  pCatalogue                  A reference to the info on the different game products.
 * @param   {char **}                 sShopSelectText             An array of texts that provide info on the shop options.
 * @param   {char *}                  sFooterFrontTemplate        A template string that helps format the footer.
 * @param   {char *}                  sFooterBlankFrontTemplate   Another template string that helps format the footer.
*/
void Shop_footer(
  struct Shop *this,
  struct Player *pPlayer,
  struct UtilsText *pFooterText,
  struct UtilsSelector *pCatalogueSelector,
  struct GameCatalogue *pCatalogue,

  char **sShopSelectText,
  char *sFooterFrontTemplate,
  char *sFooterBlankFrontTemplate) {
  
  // A placeholder variable
  char *sFooterString = UtilsUI_createBuffer();

  // The player is currently choosing an action to do on the shop
  if(Shop_getCurrentAction(this) == SHOP_NULL) {

    // Provide action name 
    char *sActionName = this->sPresentActionNameArray[UtilsSelector_getCurrentValue(this->pShopSelector)];

    sprintf(sFooterString, sFooterFrontTemplate, sActionName, sShopSelectText[UtilsSelector_getCurrentIndex(this->pShopSelector) * 2]);
    UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
    sprintf(sFooterString, sFooterBlankFrontTemplate, sShopSelectText[UtilsSelector_getCurrentIndex(this->pShopSelector) * 2 + 1]);
    UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
    UtilsText_addNewLines(pFooterText, 1);
  
  // The user is selecting something to sell or buy
  } else {
    enum ProductType eProductType = UtilsSelector_getCurrentValue(pCatalogueSelector);
    char *sProductName = calloc(16, sizeof(char)); 
    char *sProductDesc = UtilsUI_createBuffer();

    if(eProductType < pCatalogue->dSize) {
      sProductName = pCatalogue->sProductNameArray[eProductType];

      switch(Shop_getCurrentAction(this)) {
        case SHOP_BUY:
          sprintf(sProductDesc, "This crop needs to be watered (%d) times before being harvested.", 
            pCatalogue->dProductWaterReqArray[eProductType]);
          sprintf(sFooterString, sFooterFrontTemplate, sProductName, sProductDesc);
          UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
          
          sprintf(sProductDesc, "It sells for (%d) gold; that's a profit of (%d) gold.", 
            pCatalogue->dProductCostToSellArray[eProductType],
            pCatalogue->dProductCostToSellArray[eProductType] - pCatalogue->dProductCostToBuyArray[eProductType]);
          sprintf(sFooterString, sFooterBlankFrontTemplate, sProductDesc);
          UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
          break;

        case SHOP_SELL:
          sprintf(sProductDesc, "You currently have (%d) of this crop in your inventory.", 
            Stock_getAmount(Player_getCropStock(pPlayer, eProductType)));
          sprintf(sFooterString, sFooterFrontTemplate, sProductName, sProductDesc);
          UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
          sprintf(sFooterString, sFooterBlankFrontTemplate, "");
          UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
          break;

        default: break;
      }
    } else {
      sProductName = "go back";

      sprintf(sFooterString, sFooterFrontTemplate, sProductName, "Return to the counter of the shop.");
      UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
      sprintf(sFooterString, sFooterBlankFrontTemplate, "You get to choose another action to do in the shop.");
      UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
    } 

    UtilsText_addNewLines(pFooterText, 1);
  }
}

#endif