/**
 * A minified version of the game.
 * This is probably the messiest file amongst all of them.
 * I'm not saying the others are messy, but the implementation of this file was rather haphazard as it was a last minute addition.
*/

#ifndef GAME_MINI
#define GAME_MINI

#include "game.catalogue.h"

#include "enums/game.enum.state.h"

#include "objects/game.obj.player.h"
#include "objects/game.obj.farm.h"
#include "objects/game.obj.shop.h"

#include "../utils/utils.selector.h"
#include "../utils/utils.text.h"
#include "../utils/utils.ui.h"
#include "../utils/utils.io.h"
#include "../utils/utils.key.h"

#define GAME_MINI_MIN_WIDTH 80
#define GAME_MINI_MIN_HEIGHT 32

#define GAME_MINI_FARM_WIDTH 10
#define GAME_MINI_FARM_HEIGHT 3

#define GAME_MINI_PLAYER_GOLD 50
#define GAME_MINI_PLAYER_ENERGY 30

/**
 * Holds the state and data for the minified version of the game.
*/
struct GameMini {
  
  // Game objects
  struct Player *pPlayer;
  struct Farm *pFarm;
  struct Shop *pShop;

  // Game state and parameters
  enum PlayState ePlayState;
  enum GameState eGameState;
  int bIsGameOver;

  // IO objects
  int dIntInput;
  char *sCurrentInput;
  char *sFeedbackString;

  // UI objects  
  char *sDivider;
  char *sLiner;

  struct UtilsText *pScreenText;
  struct UtilsText *pHeaderText;
  struct UtilsText *pFooterText;

  // Game catalogue
  struct GameCatalogue *CATALOGUE;
  int bCropSownStatesArray[CATALOGUE_SIZE];
  int dCropLastWateredArray[CATALOGUE_SIZE];
  int dCropWaterStatesArray[CATALOGUE_SIZE];
};

/**
 * Initializes the minified version of the game.
 * 
 * @param   {struct GameMini *}       this        The mini game object.
 * @param   {struct GameCatalogue *}  pCatalogue  A reference to the data of the game products.
*/
void GameMini_init(struct GameMini *this, struct GameCatalogue *pCatalogue) {
  this->CATALOGUE = pCatalogue;

  // I find it funny that i had to add these restrictions because of the MP specs
  for(int i = 0; i < CATALOGUE_SIZE; i++) {
    this->bCropSownStatesArray[i] = 0;
    this->dCropLastWateredArray[i] = -1;
    this->dCropWaterStatesArray[i] = 0;
  }

  // Game objects
  struct Player *pPlayer = 
    Player_create(
      GAME_MINI_PLAYER_GOLD, 
      GAME_MINI_PLAYER_ENERGY, 
      GAME_MINI_PLAYER_ENERGY,
      this->CATALOGUE);

  struct Farm *pFarm = 
    Farm_create(
      GAME_MINI_FARM_WIDTH, 
      GAME_MINI_FARM_HEIGHT, 
      this->CATALOGUE);
  
  struct Shop *pShop = 
    Shop_create(
      this->CATALOGUE);

  this->pPlayer = pPlayer;
  this->pFarm = pFarm;
  this->pShop = pShop;

  // Game state and parameters
  this->ePlayState = PLAY_SELECTING;
  this->eGameState = GAME_MENU;
  this->bIsGameOver = 0;

  // IO objects
  this->dIntInput = 0;
  this->sCurrentInput = calloc(UTILS_KEY_MAX_INPUT, sizeof(char));
  this->sFeedbackString = calloc(UTILS_KEY_MAX_INPUT, sizeof(char));
  
  // UI objects
  this->sLiner =   "###############################################################";
  this->sDivider = "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-";

  struct UtilsText *pScreenText = UtilsText_create();
  struct UtilsText *pFooterText = UtilsText_create();

  this->pScreenText = pScreenText;
  this->pFooterText = pFooterText;
}

/**
 * Creates the footer for the entire game.
 * 
 * @param   {struct GameMini *}   this  The mini game object.
*/
void GameMini_makeFooter(struct GameMini *this) {
  this->pFooterText = UtilsText_create();
  UtilsText_addText(this->pFooterText, UtilsText_paddedText(
      "[ Mo David @2023 ]    ", " ", UTILS_TEXT_RIGHT_ALIGN));
  UtilsText_addNewLines(this->pFooterText, 1);
}

/**
 * Lists down the crops for selecting.
 * 
 * @param   {struct GameMini *}   this  The mini game object.
*/
void GameMini_cropSelection(struct GameMini *this) {
  int dLines = 5;

  for(int i = 1; i < this->CATALOGUE->dSize; i++) {
    char *sCatalogueOption = UtilsUI_createBuffer();

    if(this->ePlayState == PLAY_SHOP) 
      sprintf(sCatalogueOption, "[%c] %-12s : %2d gold", 
        this->CATALOGUE->cProductCodeArray[i], this->CATALOGUE->sProductNameArray[i],
        Shop_getCurrentAction(this->pShop) == SHOP_BUY ? 
          this->CATALOGUE->dProductCostToBuyArray[i] : this->CATALOGUE->dProductCostToSellArray[i]);
    else
      sprintf(sCatalogueOption, "[%c] %-12s", this->CATALOGUE->cProductCodeArray[i], this->CATALOGUE->sProductNameArray[i]);
    UtilsText_addText(this->pScreenText, sCatalogueOption);

    dLines--;
  }

  if(this->ePlayState != PLAY_SHOP) UtilsText_addText(this->pScreenText, "[G] Go back     ");
  else UtilsText_addText(this->pScreenText, "[G] Go back               ");
  UtilsText_addNewLines(this->pScreenText, 1);
  if(strlen(this->sFeedbackString)) UtilsText_addText(this->pScreenText, this->sFeedbackString);
  else UtilsText_addNewLines(this->pScreenText, 1);

  while(dLines--)
    UtilsText_addNewLines(this->pScreenText, 1);
}

/**
 * Handles the UI of the minified version of the game.
 * 
 * @param   {char}                cInput  The user keystroke.
 * @param   {struct GameMini *}   this    The mini game object.
*/
void GameMini_UI(char cInput, struct GameMini *this) {
  this->pScreenText = UtilsText_create();
  char* sHeaderLine = UtilsUI_createBuffer();

  sprintf(sHeaderLine, !this->bIsGameOver ? 
    "###  Day: %4d (%1d Starved)  #  Energy: %4d  #  Gold: %4d  ###" : 
    "###  Day: %4d (You died)  ##  Energy: %4d  #  Gold: %4d  ###",
    this->pPlayer->dTime + 1,
    this->pPlayer->dDaysStarved,
    this->pPlayer->dEnergy,
    this->pPlayer->dGold);

  UtilsText_addText(this->pScreenText, this->sLiner);
  UtilsText_addText(this->pScreenText, sHeaderLine);
  UtilsText_addText(this->pScreenText, this->sLiner);
  UtilsText_addNewLines(this->pScreenText, 3);

  switch(this->eGameState) {
    case GAME_MENU:
      if(strlen(this->sFeedbackString)) {
        UtilsText_addNewLines(this->pScreenText, 2);
        UtilsText_addText(this->pScreenText, this->sFeedbackString);    UtilsText_addNewLines(this->pScreenText, 2);
        UtilsText_addText(this->pScreenText, this->sDivider);           UtilsText_addNewLines(this->pScreenText, 1);
        UtilsText_addText(this->pScreenText, "[Enter] to continue.");   UtilsText_addNewLines(this->pScreenText, 7);

      } else {
        UtilsText_addText(this->pScreenText, "This is Harvest Sun.");       UtilsText_addNewLines(this->pScreenText, 1);
        UtilsText_addText(this->pScreenText, "Select a place to go to.");   UtilsText_addNewLines(this->pScreenText, 2);
        UtilsText_addText(this->pScreenText, this->sDivider);               UtilsText_addNewLines(this->pScreenText, 1);
        UtilsText_addText(this->pScreenText, "[H] Home");
        UtilsText_addText(this->pScreenText, "[F] Farm");
        UtilsText_addText(this->pScreenText, "[S] Shop");
        UtilsText_addText(this->pScreenText, "[Q] Quit");                   UtilsText_addNewLines(this->pScreenText, 4);
      }
      break;  

    case GAME_PLAY:
      switch(this->ePlayState) {
        
        // The player went home
        case PLAY_HOME:
          if(this->bIsGameOver) {
            UtilsText_addText(this->pScreenText, "GAME OVER!");                                               UtilsText_addNewLines(this->pScreenText, 1);
            UtilsText_addText(this->pScreenText, "Unfortunately, you did not eat for 3 consecutive days!");   UtilsText_addNewLines(this->pScreenText, 2);
            UtilsText_addText(this->pScreenText, this->sDivider);                                             UtilsText_addNewLines(this->pScreenText, 1);
            UtilsText_addText(this->pScreenText, "[Enter] to continue.");                                     UtilsText_addNewLines(this->pScreenText, 7);

            // I did not want to put this here but oh well
            // I really did try to separate UI and IO functionalities
            this->eGameState = GAME_QUIT;

          } else {
            UtilsText_addText(this->pScreenText, "You just went HOME.");              UtilsText_addNewLines(this->pScreenText, 1);
            UtilsText_addText(this->pScreenText, Player_isStarving(this->pPlayer) ? 
              "You did not have enough gold to eat breakfast." :
              "You regained your energy and ate breakfast for (10) gold!");           UtilsText_addNewLines(this->pScreenText, 2);
            UtilsText_addText(this->pScreenText, this->sDivider);                     UtilsText_addNewLines(this->pScreenText, 1);
            UtilsText_addText(this->pScreenText, "[Enter] to continue.");             UtilsText_addNewLines(this->pScreenText, 7);
          }
          break;

        // The player went to the farm
        case PLAY_FARM:
          if(Farm_getCurrentAction(this->pFarm) == FARM_NULL) {
            if(strlen(this->sFeedbackString)) {
              UtilsText_addNewLines(this->pScreenText, 2);
              UtilsText_addText(this->pScreenText, this->sFeedbackString);    UtilsText_addNewLines(this->pScreenText, 2);
              UtilsText_addText(this->pScreenText, this->sDivider);           UtilsText_addNewLines(this->pScreenText, 1);
              UtilsText_addText(this->pScreenText, "[Enter] to continue.");   UtilsText_addNewLines(this->pScreenText, 7);
            
            } else {
              UtilsText_addText(this->pScreenText, "You went to the FARM.");    UtilsText_addNewLines(this->pScreenText, 1);
              UtilsText_addText(this->pScreenText, "Choose something to do.");  UtilsText_addNewLines(this->pScreenText, 2);
              UtilsText_addText(this->pScreenText, this->sDivider);             UtilsText_addNewLines(this->pScreenText, 1);
              UtilsText_addText(this->pScreenText, "[T] Till plots    ");
              UtilsText_addText(this->pScreenText, "[S] Sow seeds     ");
              UtilsText_addText(this->pScreenText, "[W] Water plants  ");
              UtilsText_addText(this->pScreenText, "[H] Harvest plants");
              UtilsText_addText(this->pScreenText, "[G] Go back       ");       UtilsText_addNewLines(this->pScreenText, 3);
            }
          
          // The player has chosen something to do
          } else {
            char *sActionName = UtilsUI_createBuffer();
            char *sActionInfo = UtilsUI_createBuffer();
            char *sActionInput = UtilsUI_createBuffer();
            char *sProductName = UtilsUI_createBuffer();

            if(Farm_getCurrentAction(this->pFarm) == FARM_TILL || Farm_getCurrentCrop(this->pFarm) != PRODUCT_NULL) {
              switch(Farm_getCurrentAction(this->pFarm)) {
                
                // Player is gonna till some plots.
                case FARM_TILL:
                  sprintf(sActionInfo, "There are currently (%d) (UNTILLED) plots on the farm.", Farm_canTill(this->pFarm));
                  UtilsText_addText(this->pScreenText, "You are about to (TILL) some plots.");  UtilsText_addNewLines(this->pScreenText, 1);
                  UtilsText_addText(this->pScreenText, sActionInfo);                            UtilsText_addNewLines(this->pScreenText, 2);
                  UtilsText_addText(this->pScreenText, this->sDivider);                         UtilsText_addNewLines(this->pScreenText, 1);

                  sprintf(sActionInput, "Number of plots to (TILL): %-*.*s", UTILS_KEY_MAX_DIGITS, UTILS_KEY_MAX_DIGITS, 
                    strlen(this->sCurrentInput) ? this->sCurrentInput : "________");
                  UtilsText_addText(this->pScreenText, sActionInput);
                  UtilsText_addNewLines(this->pScreenText, 1);

                  if(strlen(this->sFeedbackString)) UtilsText_addText(this->pScreenText, this->sFeedbackString);
                  else UtilsText_addNewLines(this->pScreenText, 1);
                  UtilsText_addNewLines(this->pScreenText, 5);
                  break;

                // Player is gonna sow some seeds.
                case FARM_SOW:
                  strcpy(sProductName, UtilsUI_toUpper(this->CATALOGUE->sProductNameArray[Farm_getCurrentCrop(this->pFarm)]));
                  sprintf(sActionName, "You are about to (SOW) some plots with (%s) seeds.", sProductName);
                  sprintf(sActionInfo, "There are (%d) (TILLED) plots; you have (%d) (%s) seeds.", 
                    Farm_canSow(this->pFarm), Stock_getAmount(this->pPlayer->pSeedStockArray[Farm_getCurrentCrop(this->pFarm)]), sProductName);

                  UtilsText_addText(this->pScreenText, sActionName);      UtilsText_addNewLines(this->pScreenText, 1);
                  UtilsText_addText(this->pScreenText, sActionInfo);      UtilsText_addNewLines(this->pScreenText, 2);
                  UtilsText_addText(this->pScreenText, this->sDivider);   UtilsText_addNewLines(this->pScreenText, 1);

                  sprintf(sActionInput, "Number of plots to (SOW): %-*.*s", UTILS_KEY_MAX_DIGITS, UTILS_KEY_MAX_DIGITS, 
                    strlen(this->sCurrentInput) ? this->sCurrentInput : "________");
                  UtilsText_addText(this->pScreenText, sActionInput);
                  UtilsText_addNewLines(this->pScreenText, 1);

                  if(strlen(this->sFeedbackString)) UtilsText_addText(this->pScreenText, this->sFeedbackString);
                  else UtilsText_addNewLines(this->pScreenText, 1);
                  UtilsText_addNewLines(this->pScreenText, 5);
                  break;

                default: break;
              }

            // Let the player choose a crop first
            } else {
              sprintf(sActionInfo, "Select a crop for (%s).", UtilsUI_toUpper(
                this->pFarm->sPresentActionNameArray[Farm_getCurrentAction(this->pFarm)]));

              UtilsText_addText(this->pScreenText, "Before you can proceed, choose a crop first.");   UtilsText_addNewLines(this->pScreenText, 1);
              UtilsText_addText(this->pScreenText, sActionInfo);                                      UtilsText_addNewLines(this->pScreenText, 2);
              UtilsText_addText(this->pScreenText, this->sDivider);                                   UtilsText_addNewLines(this->pScreenText, 1);

              GameMini_cropSelection(this);
            }
          }
          break;

        case PLAY_SHOP:
          if(Shop_getCurrentAction(this->pShop) == SHOP_NULL) {
            if(strlen(this->sFeedbackString)) {
              UtilsText_addNewLines(this->pScreenText, 2);
              UtilsText_addText(this->pScreenText, this->sFeedbackString);    UtilsText_addNewLines(this->pScreenText, 2);
              UtilsText_addText(this->pScreenText, this->sDivider);           UtilsText_addNewLines(this->pScreenText, 1);
              UtilsText_addText(this->pScreenText, "[Enter] to continue.");   UtilsText_addNewLines(this->pScreenText, 7);
            
            } else {
              UtilsText_addText(this->pScreenText, "You went to the SHOP.");    UtilsText_addNewLines(this->pScreenText, 1);
              UtilsText_addText(this->pScreenText, "Choose something to do.");  UtilsText_addNewLines(this->pScreenText, 2);
              UtilsText_addText(this->pScreenText, this->sDivider);             UtilsText_addNewLines(this->pScreenText, 1);
              UtilsText_addText(this->pScreenText, "[B] Buy seeds ");
              UtilsText_addText(this->pScreenText, "[S] Sell crops");
              UtilsText_addText(this->pScreenText, "[G] Go back   ");           UtilsText_addNewLines(this->pScreenText, 5);
            }
          
          // The player has chosen something to do
          } else {
            char *sActionInfo = UtilsUI_createBuffer();
            char *sActionInput = UtilsUI_createBuffer();
            char *sActionInputLog = UtilsUI_createBuffer();
            int dCostAmount = 0;

            sprintf(sActionInfo, "Select a crop for (%s).", UtilsUI_toUpper(
              this->pShop->sPresentActionNameArray[Shop_getCurrentAction(this->pShop)]));

            UtilsText_addText(this->pScreenText, "Before you can proceed, choose a crop first.");   UtilsText_addNewLines(this->pScreenText, 1);
            UtilsText_addText(this->pScreenText, sActionInfo);                                      UtilsText_addNewLines(this->pScreenText, 2);
            UtilsText_addText(this->pScreenText, this->sDivider);                                   UtilsText_addNewLines(this->pScreenText, 1);

            if(Shop_getCurrentCrop(this->pShop) == PRODUCT_NULL) {
              GameMini_cropSelection(this);
            } else {
              if(Shop_getCurrentAction(this->pShop) == SHOP_BUY) 
                dCostAmount = UtilsKey_stringToInt(this->sCurrentInput) * this->CATALOGUE->dProductCostToBuyArray[Shop_getCurrentCrop(this->pShop)];
              else if(Shop_getCurrentAction(this->pShop) == SHOP_SELL) 
                dCostAmount = UtilsKey_stringToInt(this->sCurrentInput) * this->CATALOGUE->dProductCostToSellArray[Shop_getCurrentCrop(this->pShop)];

              sprintf(sActionInput,   "No. of (%s) crops for (%s): %-*.*s", 
                UtilsUI_toUpper(this->CATALOGUE->sProductNameArray[Shop_getCurrentCrop(this->pShop)]),
                UtilsUI_toUpper(this->pShop->sPresentActionNameArray[Shop_getCurrentAction(this->pShop)]),
                UTILS_KEY_MAX_DIGITS, UTILS_KEY_MAX_DIGITS, strlen(this->sCurrentInput) ? this->sCurrentInput : "________");
              
              sprintf(sActionInputLog, "%*s %*s %*s            (TOTAL): %-*d",
                (int) strlen(this->CATALOGUE->sProductNameArray[Shop_getCurrentCrop(this->pShop)]), " ",
                (int) strlen(this->pShop->sPresentActionNameArray[Shop_getCurrentAction(this->pShop)]), " ",
                
                UTILS_KEY_MAX_DIGITS + (
                  (int) strlen(this->CATALOGUE->sProductNameArray[Shop_getCurrentCrop(this->pShop)]) + 
                  (int) strlen(this->pShop->sPresentActionNameArray[Shop_getCurrentAction(this->pShop)])) % 2, 
                " ", UTILS_KEY_MAX_DIGITS * 2, dCostAmount);

              UtilsText_addText(this->pScreenText, sActionInput);    
              if(strlen(this->sCurrentInput)) UtilsText_addText(this->pScreenText, sActionInputLog);
              else UtilsText_addNewLines(this->pScreenText, 1);
              
              if(strlen(this->sFeedbackString)) UtilsText_addText(this->pScreenText, this->sFeedbackString);
              else UtilsText_addNewLines(this->pScreenText, 1);
              UtilsText_addNewLines(this->pScreenText, 5);
            }
          }
          break;

        default: break;
      }
      break;

    default: break;
  }

  UtilsText_addNewLines(this->pScreenText, 1);
  UtilsText_addText(this->pScreenText, this->sLiner);
  UtilsText_addText(this->pScreenText, this->sLiner);
  
  // Format it
  UtilsUI_centerXY(this->pScreenText);
  GameMini_makeFooter(this);
  UtilsUI_footer(this->pScreenText, this->pFooterText);

  // Print it
  UtilsIO_clear();
  UtilsUI_print(this->pScreenText);
  
  // Garbage collection
  UtilsText_kill(this->pScreenText);
  UtilsText_kill(this->pFooterText);
}

/**
 * Handles the IO of the minified version of the game.
 * 
 * @param   {char}                cInput  The user keystroke.
 * @param   {struct GameMini *}   this    The mini game object.
*/
void GameMini_IO(char cInput, struct GameMini *this) {
  strcpy(this->sFeedbackString, "");

  switch(this->eGameState) {

    // User is selecting where to go
    case GAME_MENU:
      if(cInput == 'Q') {
        this->eGameState = GAME_QUIT;

      // Play an action or smth
      } else if (cInput == 'H' || cInput == 'F' || cInput == 'S') {
        this->eGameState = GAME_PLAY;

        if(cInput == 'H') {
          Player_goHome(this->pPlayer);
          if(Player_isDead(this->pPlayer)) this->bIsGameOver = 1;
          this->ePlayState = PLAY_HOME;
        }

        if(cInput == 'S') this->ePlayState = PLAY_SHOP;
        if(cInput == 'F') {
          if(Player_getEnergy(this->pPlayer)) {
            this->ePlayState = PLAY_FARM;
          } else {
            this->eGameState = GAME_MENU;
            strcpy(this->sFeedbackString, "You do not have enough energy to do anything on the farm.");
          }
        }
      }
      break;
    
    // User is elsewhere and is doing something
    case GAME_PLAY:
      switch(this->ePlayState) {
        
        // Regenerate player per day and terminate program if game over
        case PLAY_HOME:
          this->eGameState = GAME_MENU;
          break;

        // Let user choose something to do then execute the action
        case PLAY_FARM:
          if(Player_getEnergy(this->pPlayer)) {
            if(Farm_getCurrentAction(this->pFarm) == FARM_NULL) {
              if(cInput == 'T') Farm_setCurrentAction(this->pFarm, FARM_TILL);
              if(cInput == 'S') Farm_setCurrentAction(this->pFarm, FARM_SOW);
              if(cInput == 'W') Farm_setCurrentAction(this->pFarm, FARM_WATER);
              if(cInput == 'H') Farm_setCurrentAction(this->pFarm, FARM_HARVEST);
              if(cInput == 'G') this->eGameState = GAME_MENU;

              int dAvailablePlots = 0;
              switch(Farm_getCurrentAction(this->pFarm)) {
                case FARM_TILL: dAvailablePlots = Farm_canTill(this->pFarm); break; 
                case FARM_SOW: dAvailablePlots = Farm_canSow(this->pFarm); break;
                case FARM_WATER: dAvailablePlots = Farm_canWater(this->pFarm, Player_getTime(this->pPlayer)); break;
                case FARM_HARVEST: dAvailablePlots = Farm_canHarvest(this->pFarm); break;
                default: break;
              }
              
              strcpy(this->sFeedbackString, "");

              if(Farm_getCurrentAction(this->pFarm) != FARM_NULL) {
                if(dAvailablePlots) {
                  Farm_setCurrentCrop(this->pFarm, PRODUCT_NULL);
                } else {
                  Farm_setCurrentAction(this->pFarm, FARM_NULL);
                  strcpy(this->sFeedbackString, "There are currently no plots available for that action.");
                }
              }

              // Reset the user input string to pick up a number later on
              strcpy(this->sCurrentInput, "");
              cInput = '\0';
            
            // The user has chosen to do something on the farm
            } else {
              if(Farm_getCurrentAction(this->pFarm) != FARM_TILL && Farm_getCurrentCrop(this->pFarm) == PRODUCT_NULL) {
                if(cInput == 'G') Farm_setCurrentAction(this->pFarm, FARM_NULL);
                
                int dSelected = -1;
                for(int i = 1; i < this->CATALOGUE->dSize; i++) {

                  char *sProductName = UtilsUI_toUpper(this->CATALOGUE->sProductNameArray[i]);
                  if(cInput == this->CATALOGUE->cProductCodeArray[i]) {
                    
                    // When selecting seeds for sowing
                    if(Farm_getCurrentAction(this->pFarm) == FARM_SOW) {
                      if(Stock_getAmount(this->pPlayer->pSeedStockArray[i])) {  
                        if(!this->bCropSownStatesArray[i]) {
                          dSelected = i;
                        } else sprintf(this->sFeedbackString, "The farm already has (%s) seeds planted on it.", sProductName);
                      } else sprintf(this->sFeedbackString, "You do not have (%s) seeds.", sProductName);

                    // When selecting crops for watering
                    } else if(Farm_getCurrentAction(this->pFarm) == FARM_WATER) {
                      if(this->bCropSownStatesArray[i]) {
                        if(this->dCropLastWateredArray[i] < Player_getTime(this->pPlayer)) {
                          if(this->dCropWaterStatesArray[i] < this->CATALOGUE->dProductWaterReqArray[i]) {
                            if(Player_getEnergy(this->pPlayer) >= this->bCropSownStatesArray[i]) {
                              dSelected = i;
                            } else sprintf(this->sFeedbackString, "You don't have enough energy to water your (%s) crops.", sProductName);
                          } else sprintf(this->sFeedbackString, "Your (%s) crops have been fully watered.", sProductName);  
                        } else sprintf(this->sFeedbackString, "Your (%s) crops were just watered today.", sProductName);  
                      } else sprintf(this->sFeedbackString, "The farm does not have crops of type (%s) on it.", sProductName);
                    
                    // When selecting crops for harvesting
                    } else if(Farm_getCurrentAction(this->pFarm) == FARM_HARVEST) {
                      if(this->bCropSownStatesArray[i]) {
                        if(this->dCropWaterStatesArray[i] >= this->CATALOGUE->dProductWaterReqArray[i]) {
                          if(Player_getEnergy(this->pPlayer) >= this->bCropSownStatesArray[i]) {
                            dSelected = i;
                          } else sprintf(this->sFeedbackString, "You don't have enoug energy to harvest your (%s) crops.", sProductName);
                        } else sprintf(this->sFeedbackString, "Your (%s) crops have not been fully watered.", sProductName);  
                      } else sprintf(this->sFeedbackString, "The farm does not have crops of type (%s) on it.", sProductName);
                    }
                  }
                }

                if(dSelected > -1) {
                  Farm_setCurrentCrop(this->pFarm, dSelected);

                  strcpy(this->sCurrentInput, "");
                  cInput = '\0';

                  // For watering and harvesting
                  if(Farm_getCurrentAction(this->pFarm) == FARM_WATER || Farm_getCurrentAction(this->pFarm) == FARM_HARVEST) {
                    int dActionablePlots = 0;
                    switch(Farm_getCurrentAction(this->pFarm)) {
                      case FARM_WATER: dActionablePlots = Farm_canWaterCrop(this->pFarm, Player_getTime(this->pPlayer)); break; 
                      case FARM_HARVEST: dActionablePlots = Farm_canHarvestCrop(this->pFarm); break;
                      default: break;
                    }

                    if(dActionablePlots) {
                      if(Player_updateEnergy(this->pPlayer, -dActionablePlots)) {

                        // If the player can do the action
                        switch(Farm_getCurrentAction(this->pFarm)) {
                          case FARM_WATER: 
                            Farm_water(this->pFarm, Player_getTime(this->pPlayer)); 
                            Farm_setCurrentAction(this->pFarm, FARM_NULL);

                            this->dCropLastWateredArray[Farm_getCurrentCrop(this->pFarm)] = Player_getTime(this->pPlayer);
                            this->dCropWaterStatesArray[Farm_getCurrentCrop(this->pFarm)] += 1;
                            
                            if(this->dCropWaterStatesArray[Farm_getCurrentCrop(this->pFarm)] < 
                              this->CATALOGUE->dProductWaterReqArray[Farm_getCurrentCrop(this->pFarm)])
                              sprintf(this->sFeedbackString, "Your (%s) crops are now (%d/%d) from becoming harvestable.",
                                UtilsUI_toUpper(this->CATALOGUE->sProductNameArray[Farm_getCurrentCrop(this->pFarm)]),
                                this->dCropWaterStatesArray[Farm_getCurrentCrop(this->pFarm)],
                                this->CATALOGUE->dProductWaterReqArray[Farm_getCurrentCrop(this->pFarm)]);
                            else 
                              sprintf(this->sFeedbackString, "Your (%s) crops are now fully watered! You can harvest them now.",
                                UtilsUI_toUpper(this->CATALOGUE->sProductNameArray[Farm_getCurrentCrop(this->pFarm)]));
                            break;
                          
                          case FARM_HARVEST: 
                            Farm_harvest(this->pFarm); 
                            Farm_setCurrentAction(this->pFarm, FARM_NULL);

                            Player_updateCropStock(this->pPlayer, Farm_getCurrentCrop(this->pFarm), dActionablePlots);
                            this->bCropSownStatesArray[Farm_getCurrentCrop(this->pFarm)] = 0;
                            this->dCropLastWateredArray[Farm_getCurrentCrop(this->pFarm)] = -1;
                            this->dCropWaterStatesArray[Farm_getCurrentCrop(this->pFarm)] = 0;

                            sprintf(this->sFeedbackString, "You now have (%d) (%s) crops in your inventory.",
                              Stock_getAmount(Player_getCropStock(this->pPlayer, Farm_getCurrentCrop(this->pFarm))),
                              UtilsUI_toUpper(this->CATALOGUE->sProductNameArray[Farm_getCurrentCrop(this->pFarm)]));
                            break;

                          default: break;
                        }
                      }
                    }
                  }
                }

              // Tilling or sowing
              } else if(Farm_getCurrentAction(this->pFarm) == FARM_TILL || Farm_getCurrentAction(this->pFarm) == FARM_SOW) {
                int dAvailablePlots = 0;

                switch(Farm_getCurrentAction(this->pFarm)) {
                  case FARM_TILL: dAvailablePlots = Farm_canTill(this->pFarm); break; 
                  case FARM_SOW: dAvailablePlots = Farm_canSow(this->pFarm); break;
                  default: break;
                }

                // If nothing is available for doing the action, just go back
                if(!dAvailablePlots) {
                  Farm_setCurrentAction(this->pFarm, FARM_NULL);
                  
                // Otherwise, wait until user presses enter to parse the integer input
                } else if(UtilsKey_isReturn(cInput, "")) {
                  this->dIntInput = UtilsKey_stringToInt(this->sCurrentInput);

                  int bCanDoIt = 1;
                  int dActionablePlots = this->dIntInput;

                  // Don't let the player sow if they don't have enough seeds
                  if(Farm_getCurrentAction(this->pFarm) == FARM_SOW)
                    if(Stock_getAmount(Player_getSeedStock(this->pPlayer, Farm_getCurrentCrop(this->pFarm))) < this->dIntInput)
                      bCanDoIt = 0;

                  // Till the plots if its possible
                  if(dActionablePlots > -1) {
                    if(dActionablePlots <= dAvailablePlots && bCanDoIt) {
                      if(Player_updateEnergy(this->pPlayer, -dActionablePlots)) {

                        // Perform the action
                        switch(Farm_getCurrentAction(this->pFarm)) {
                          case FARM_TILL: 
                            Farm_till(this->pFarm, dActionablePlots); 
                            sprintf(this->sFeedbackString, "You've just (TILLED) a total of (%d) plots.", dActionablePlots);
                            break; 
                          
                          case FARM_SOW: 
                            Farm_sow(this->pFarm, dActionablePlots, this->CATALOGUE, Player_getTime(this->pPlayer));
                            Player_updateSeedStock(this->pPlayer, Farm_getCurrentCrop(this->pFarm), -dActionablePlots);
                            this->bCropSownStatesArray[Farm_getCurrentCrop(this->pFarm)] = dActionablePlots; 
                            sprintf(this->sFeedbackString, "You've just (SOWN) a total of (%d) plots with (%s) seeds.", dActionablePlots,
                              UtilsUI_toUpper(this->CATALOGUE->sProductNameArray[Farm_getCurrentCrop(this->pFarm)]));
                            break;

                          default: break;
                        }

                        Farm_setCurrentAction(this->pFarm, FARM_NULL);
                      
                      // Number too big
                      } else strcpy(this->sFeedbackString, "You do not have enough energy to execute that action.");

                    // Number also too big
                    } else {
                      if(Farm_getCurrentAction(this->pFarm) != FARM_SOW) {
                        sprintf(this->sFeedbackString, "The number exceeds the number of plots available for (%s).", 
                          UtilsUI_toUpper(this->pFarm->sPresentActionNameArray[Farm_getCurrentAction(this->pFarm)]));
                      } else {
                        sprintf(this->sFeedbackString, "The number exceeds the number of plots or seeds available for (%s).", 
                          UtilsUI_toUpper(this->pFarm->sPresentActionNameArray[Farm_getCurrentAction(this->pFarm)]));
                      }
                    }

                  // Nothing was inputted
                  } else strcpy(this->sFeedbackString, "Please enter a valid whole number.");
                  
                  strcpy(this->sCurrentInput, "");
                }
              
                if((strlen(this->sCurrentInput) >= UTILS_KEY_MAX_DIGITS || !UtilsKey_isNum(cInput)) &&
                  !UtilsKey_isBackspace(cInput, ""))
                  cInput = '\0';
              
              }
            }

          // The player has no energy
          } else {
            this->eGameState = GAME_MENU;
            strcpy(this->sFeedbackString, "You do not have enough energy to do anything on the farm.");
          }
          break;
        
        case PLAY_SHOP:
          if(Shop_getCurrentAction(this->pShop) == SHOP_NULL) {
            if(cInput == 'B') Shop_setCurrentAction(this->pShop, SHOP_BUY);
            if(cInput == 'S') Shop_setCurrentAction(this->pShop, SHOP_SELL);
            if(cInput == 'G') this->eGameState = GAME_MENU;

            // Reset shop parameter
            Shop_setCurrentCrop(this->pShop, PRODUCT_NULL);

            // Reset the user input string to pick up a number later on
            strcpy(this->sCurrentInput, "");
            cInput = '\0';
          
          // Let the user buy or sell something
          } else {
            
            // User is choosing a crop
            if(Shop_getCurrentCrop(this->pShop) == PRODUCT_NULL) {
              if(cInput == 'G') Shop_setCurrentAction(this->pShop, FARM_NULL);
              
              for(int i = 1; i < this->CATALOGUE->dSize; i++) {
                if(cInput == this->CATALOGUE->cProductCodeArray[i]) {
                  char *sProductName = UtilsUI_toUpper(this->CATALOGUE->sProductNameArray[i]);

                  if(Shop_getCurrentAction(this->pShop) == SHOP_BUY) {
                    if(Player_getGold(this->pPlayer) >= this->CATALOGUE->dProductCostToBuyArray[i]) Shop_setCurrentCrop(this->pShop, i);
                    else sprintf(this->sFeedbackString, "You do not have enough gold to buy a (%s) seed.", sProductName);
                  }

                  if(Shop_getCurrentAction(this->pShop) == SHOP_SELL) {
                    if(Stock_getAmount(Player_getCropStock(this->pPlayer, i))) Shop_setCurrentCrop(this->pShop, i);
                    else sprintf(this->sFeedbackString, "You do not have (%s) crops of to sell.", sProductName);
                  }
                }
              }

              if(Shop_getCurrentCrop(this->pShop) != PRODUCT_NULL) {
                strcpy(this->sCurrentInput, "");
                cInput = '\0';
              }
            } else if(UtilsKey_isReturn(cInput, "")) {
              this->dIntInput = UtilsKey_stringToInt(this->sCurrentInput);

              if(this->dIntInput > -1) {

                // Buying something
                if(Shop_getCurrentAction(this->pShop) == SHOP_BUY) {
                  if(Player_updateGold(this->pPlayer, -Shop_getCurrentBuyCost(this->pShop, this->dIntInput))) {
                    Player_updateSeedStock(this->pPlayer, Shop_getCurrentCrop(this->pShop), this->dIntInput);

                    sprintf(this->sFeedbackString, "You just (BOUGHT) a total of (%d) (%s) seeds.", this->dIntInput,
                      UtilsUI_toUpper(this->CATALOGUE->sProductNameArray[Shop_getCurrentCrop(this->pShop)]));
                    
                    Shop_setCurrentAction(this->pShop, SHOP_NULL);
                  } else strcpy(this->sFeedbackString, "You do not have enough gold to make that purchase.");
                
                // Selling something
                } else if(Shop_getCurrentAction(this->pShop) == SHOP_SELL) {
                  if(this->dIntInput <= Stock_getAmount(Player_getCropStock(this->pPlayer, Shop_getCurrentCrop(this->pShop)))) {
                    Player_updateGold(this->pPlayer, Shop_getCurrentSellCost(this->pShop, this->dIntInput));
                    Player_updateCropStock(this->pPlayer, Shop_getCurrentCrop(this->pShop), -this->dIntInput);

                    sprintf(this->sFeedbackString, "You just (SOLD) (%d) (%s) crops for (%d).", this->dIntInput,
                      UtilsUI_toUpper(this->CATALOGUE->sProductNameArray[Shop_getCurrentCrop(this->pShop)]),
                      Shop_getCurrentSellCost(this->pShop, this->dIntInput));
                    
                    Shop_setCurrentAction(this->pShop, SHOP_NULL);
                  } else strcpy(this->sFeedbackString, "You do not have enough crops to sell that amount.");
                }

              // Nothing was inputted
              } else strcpy(this->sFeedbackString, "Please enter a valid whole number.");

              strcpy(this->sCurrentInput, "");
            }

            if((strlen(this->sCurrentInput) >= UTILS_KEY_MAX_DIGITS || !UtilsKey_isNum(cInput)) &&
              !UtilsKey_isBackspace(cInput, ""))
              cInput = '\0';
          }
          break;

        default: break;
      }
      break;

    default: break;
  }

  // Append the character to the input string for parsing later on.
  if(UtilsKey_isAlpha(cInput) || UtilsKey_isNum(cInput) || UtilsKey_isBackspace(cInput, "")) {
    char *sInput = calloc(1, sizeof(char));
    sprintf(sInput, "%c", cInput);

    if(!UtilsKey_isReturn(cInput, "")) {
      
      // While it hasn't exceeded the max value or something
      if(strlen(this->sCurrentInput) < UTILS_KEY_MAX_INPUT) {
        if(!UtilsKey_isBackspace(cInput, "")) {
          strcat(this->sCurrentInput, sInput);
        }
      }

      // Backspace handling condition
      if(strlen(this->sCurrentInput)) {
        if(UtilsKey_isBackspace(cInput, ""))
          this->sCurrentInput[strlen(this->sCurrentInput) - 1] = 0;
      }
    }
  }
}

/**
 * Just makes sure the console doesn't shrink too much when the user resizes.
*/
void GameMini_maintainConsoleSize() {
  if(UtilsIO_getWidth() < GAME_MINI_MIN_WIDTH)
    UtilsIO_setSize(GAME_MINI_MIN_WIDTH, UtilsIO_getHeight());
  if(UtilsIO_getHeight() < GAME_MINI_MIN_HEIGHT)
    UtilsIO_setSize(UtilsIO_getWidth(), GAME_MINI_MIN_HEIGHT);
}

/**
 * Executes the actual flow of the mini game.
 * 
 * @param   {struct GameMini *}   this  The mini game object.
*/
void GameMini_exec(struct GameMini *this) {
  do {
    GameMini_maintainConsoleSize();
    
    UtilsKey_inputPoll(&UtilsKey_dummy, &GameMini_UI, &GameMini_IO, this, "");
  } while(this->eGameState != GAME_QUIT);

  UtilsIO_clear();
}

#endif