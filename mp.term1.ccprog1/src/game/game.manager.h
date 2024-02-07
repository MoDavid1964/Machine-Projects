/**
 * Handles all the overarching functions of the game.
*/

#ifndef GAME_MANAGER
#define GAME_MANAGER

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Import all the assets into the file.
#include "game.assets.h"
#include "game.catalogue.h"
#include "game.manager.min.h"

#include "objects/game.obj.player.h"
#include "objects/game.obj.farm.h"
#include "objects/game.obj.shop.h"

#include "classes/game.class.product.h"
#include "classes/game.class.plot.h"
#include "classes/game.class.stock.h"

#include "enums/game.enum.state.h"

#include "../utils/utils.key.h"
#include "../utils/utils.ui.h"
#include "../utils/utils.text.h"
#include "../utils/utils.selector.h"

// Some constants
#define GAME_MIN_WIDTH 128
#define GAME_MIN_HEIGHT 38

#define GAME_FARM_WIDTH 10
#define GAME_FARM_HEIGHT 3

#define GAME_PLAYER_GOLD 50
#define GAME_PLAYER_ENERGY 30

/**
 * A struct for the game singleton.
*/
struct Game {

  // Is first time playing; minified (0) or full (1) mode
  int bFirst;
  int dMode;

  // Dialog box info
  enum DialogState eDialogState;
  char *sDialogMessage;

  // Where in the dialogue we are
  int dDialogueIndex;

  // String inputs
  char *sCurrentIntInput;
  char *sInputWarning;

  // Game states
  enum GameState eGameState;
  enum PlayState ePlayState;

  // String templates
  char *sFooterFrontTemplate;
  char *sFooterBlankFrontTemplate;

  char *sSelectDefaultTemplate;
  char *sSelectSelectedTemplate;
  char *sSelectDisabledTemplate;

  // Scene selectors
  struct UtilsSelector *pMenuSelector;
  struct UtilsSelector *pPlaySelector;
  struct UtilsSelector *pDialogSelector;
  struct UtilsSelector *pCatalogueSelector;

  // UI objects
  struct UtilsText *pScreenText;
  struct UtilsText *pHeaderText;
  struct UtilsText *pFooterText;

  // Game objects
  struct Player *pPlayer;
  struct Farm *pFarm;
  struct Shop *pShop;

  // Function lists
  void (**pUIFuncArray)(char cInput, struct Game *this);
  void (**pIOFuncArray)(char cInput, struct Game *this);
  
  void (**pPlayFuncArray)(struct Game *this);
  void (**pHandlerFuncArray)(struct Game *this);

  // Game assets and catalogue
  struct GameAssets *ASSETS;
  struct GameCatalogue *CATALOGUE;
};

// Forward declarations of the functions we need to register into the struct

// UI functions
void Game_menuUI        (char cInput, struct Game *this);
void Game_playUI        (char cInput, struct Game *this);
void Game_guideUI       (char cInput, struct Game *this);
void Game_controlsUI    (char cInput, struct Game *this);
void Game_authorUI      (char cInput, struct Game *this);

// IO functions
void Game_menuIO        (char cInput, struct Game *this);
void Game_playIO        (char cInput, struct Game *this);
void Game_guideIO       (char cInput, struct Game *this);
void Game_controlsIO    (char cInput, struct Game *this);
void Game_authorIO      (char cInput, struct Game *this);

// Game play functions
void Game_dialogUI      (char cInput, struct Game *this);
void Game_dialogIO      (char cInput, struct Game *this);
void Game_playStartUI   (char *sInput, struct Game *this);
void Game_playStartIO   (char *sInput, struct Game *this);

void Game_playHome      (struct Game *this);
void Game_playFarm      (struct Game *this);
void Game_playShop      (struct Game *this);

// Context and state handlers
void Game_menu          (struct Game *this);
void Game_play          (struct Game *this);
void Game_guide         (struct Game *this);
void Game_controls      (struct Game *this);
void Game_author        (struct Game *this);

/**
 * ###########################
 * ###  GAME CONSTRUCTION  ###
 * ###########################
*/

/**
 * Initializes the game and everything in it.
 * 
 * @param   {struct Game *}           this        Refers to the game object we're initializing.
 * @param   {struct GameAssets *}     pAssets     A copy of the assets the game needs.
 * @param   {struct GameCatalogue *}  pCatalogue  A copt of the crop data needed by the game.
*/
void Game_init(struct Game *this, struct GameAssets *pAssets, struct GameCatalogue *pCatalogue) {

  // Set up game states and stuff
  this->bFirst = 1;
  this->dMode = 1;
  this->dDialogueIndex = 0;

  this->sCurrentIntInput = calloc(UTILS_KEY_MAX_DIGITS, sizeof(char));
  this->sInputWarning = UtilsUI_createBuffer();

  // Dialog box
  this->eDialogState = DIALOG_NULL;
  this->sDialogMessage = UtilsUI_createBuffer();
  
  // State handles
  this->eGameState = GAME_MENU;
  this->ePlayState = PLAY_SELECTING;

  // Some template strings
  this->sFooterFrontTemplate = "    %10s  | %s";
  this->sFooterBlankFrontTemplate = "                | %s";

  this->sSelectDefaultTemplate = "  [ ]  %s   ";
  this->sSelectSelectedTemplate = "  -=> [x]  %s       ";
  this->sSelectDisabledTemplate = "  [-]  %s   ";

  // Game asset and catalogue set up
  this->ASSETS = pAssets;
  this->CATALOGUE = pCatalogue;

  // Function registration
  void (*pUIAndIOMember) (char cInput, struct Game* this);
  void (*pPlayMember) (struct Game* this);
  void (*pHandlerMember) (struct Game* this);

  this->pUIFuncArray = calloc(6, sizeof(pUIAndIOMember));
  this->pIOFuncArray = calloc(6, sizeof(pUIAndIOMember));
  this->pPlayFuncArray = calloc(3, sizeof(pPlayMember));
  this->pHandlerFuncArray = calloc(5, sizeof(pHandlerMember));

  this->pUIFuncArray[GAME_MENU] = &Game_menuUI;
  this->pUIFuncArray[GAME_PLAY] = &Game_playUI;
  this->pUIFuncArray[GAME_GUIDE] = &Game_guideUI;
  this->pUIFuncArray[GAME_CONTROLS] = &Game_controlsUI;
  this->pUIFuncArray[GAME_AUTHOR] = &Game_authorUI;
  this->pUIFuncArray[GAME_DIALOG] = &Game_dialogUI;

  this->pIOFuncArray[GAME_MENU] = &Game_menuIO;
  this->pIOFuncArray[GAME_PLAY] = &Game_playIO;
  this->pIOFuncArray[GAME_GUIDE] = &Game_guideIO;
  this->pIOFuncArray[GAME_CONTROLS] = &Game_controlsIO;
  this->pIOFuncArray[GAME_AUTHOR] = &Game_authorIO;
  this->pIOFuncArray[GAME_DIALOG] = &Game_dialogIO;
  
  this->pPlayFuncArray[PLAY_HOME] = &Game_playHome;
  this->pPlayFuncArray[PLAY_FARM] = &Game_playFarm;
  this->pPlayFuncArray[PLAY_SHOP] = &Game_playShop;

  this->pHandlerFuncArray[GAME_MENU] = &Game_menu;
  this->pHandlerFuncArray[GAME_PLAY] = &Game_play;
  this->pHandlerFuncArray[GAME_GUIDE] = &Game_guide;
  this->pHandlerFuncArray[GAME_CONTROLS] = &Game_controls;
  this->pHandlerFuncArray[GAME_AUTHOR] = &Game_author;

  // Define the selectors
  struct UtilsSelector *pMenuSelector = UtilsSelector_create(0, 
    this->sSelectDefaultTemplate, 
    this->sSelectSelectedTemplate, 
    this->sSelectDisabledTemplate);
  
  struct UtilsSelector *pPlaySelector = UtilsSelector_create(0, 
    this->sSelectDefaultTemplate, 
    this->sSelectSelectedTemplate, 
    this->sSelectDisabledTemplate);
  
  struct UtilsSelector *pDialogSelector = UtilsSelector_create(0, 
    this->sSelectDefaultTemplate, 
    this->sSelectSelectedTemplate, 
    this->sSelectDisabledTemplate);
  
  struct UtilsSelector *pCatalogueSelector = UtilsSelector_create(0, 
    "          [ ]  %-16s   ",
    "      -=> [x]  %-16s   ",
    "          [-]  %-16s   ");

  // Main menu options
  UtilsSelector_addOption(pMenuSelector, "begin   ", GAME_PLAY);
  UtilsSelector_addOption(pMenuSelector, "how to  ", GAME_GUIDE);
  UtilsSelector_addOption(pMenuSelector, "keys    ", GAME_CONTROLS);
  UtilsSelector_addOption(pMenuSelector, "author  ", GAME_AUTHOR);
  UtilsSelector_addOption(pMenuSelector, "quit    ", GAME_QUIT);

  // Game options
  UtilsSelector_addOption(pPlaySelector, "go to home", PLAY_HOME);
  UtilsSelector_addOption(pPlaySelector, "go to farm", PLAY_FARM);
  UtilsSelector_addOption(pPlaySelector, "visit shop", PLAY_SHOP);

  // Dialog options
  UtilsSelector_addOption(pDialogSelector, "Okay", 1);
  UtilsSelector_addOption(pDialogSelector, " No ", 0);

  // Catalogue options
  for(int i = 0; i < pCatalogue->dSize; i++) 
    UtilsSelector_addOption(pCatalogueSelector, pCatalogue->sProductNameArray[i], i);
  UtilsSelector_addOption(pCatalogueSelector, "go back", pCatalogue->dSize);
  UtilsSelector_increment(pCatalogueSelector);

  // Copy all selectors
  this->pMenuSelector = pMenuSelector;
  this->pPlaySelector = pPlaySelector;
  this->pDialogSelector = pDialogSelector;
  this->pCatalogueSelector = pCatalogueSelector;

  // Define the game objects
  struct Player *pPlayer = 
    Player_create(
      GAME_PLAYER_GOLD, 
      GAME_PLAYER_ENERGY, 
      GAME_PLAYER_ENERGY, 
      this->CATALOGUE);

  struct Farm *pFarm = 
    Farm_create(
      GAME_FARM_WIDTH, 
      GAME_FARM_HEIGHT, 
      this->CATALOGUE);
  
  struct Shop *pShop = 
    Shop_create(
      this->CATALOGUE);

  // Copy all the objects into the struct
  this->pPlayer = pPlayer;
  this->pFarm = pFarm;
  this->pShop = pShop;
}

/**
 * ################################
 * ###  GAME UTILITY FUNCTIONS  ###
 * ################################
*/

/**
 * Returns whether or not the game dialogue is done.
 * 
 * @param   {struct Game *}   this  The game object.
 * @return  {int}                   A boolean indicating whether or not the dialogue has finished.
*/
int Game_isDialogueDone(struct Game *this) {
  return this->dDialogueIndex >= this->ASSETS->DIALOGUE_TEXT_LEN;
}

/**
 * Creates the header for the in-game UI.
 * 
 * @param   {struct Game*}  this  The game object.
*/
void Game_makeHeader(struct Game *this) {

  // Create the header string
  char *sHeaderString = UtilsUI_createBuffer();
  sprintf(sHeaderString, "----[ %s ]--[ Day: %4d ]--[ Days Starved: %4d ]--[ Energy: %4d ]--[ Gold: %4d ]----",
    this->pPlayer->sName,
    this->pPlayer->dTime + 1,
    this->pPlayer->dDaysStarved,
    this->pPlayer->dEnergy,
    this->pPlayer->dGold);

  // Create the header
  this->pHeaderText = UtilsText_create();
  UtilsText_addNewLines(this->pHeaderText, 1);
  UtilsText_addPatternLines(this->pHeaderText, 1, "_");
  UtilsText_addPatternLines(this->pHeaderText, 1, ":=");
  UtilsText_addText(this->pHeaderText, UtilsText_paddedText(sHeaderString, "-", UTILS_TEXT_LEFT_ALIGN));
  free(sHeaderString);
}

/**
 * Frees the header memory block.
 * 
 * @param   {struct Game *}   this  The game object.
*/
void Game_killHeader(struct Game *this) {
  UtilsText_kill(this->pHeaderText);
}

/**
 * Creates the footer for the in-game UI.
 * 
 * @param   {struct Game*}  this  The game object.
*/
void Game_makeFooter(struct Game *this) {

  // Create the footer
  this->pFooterText = UtilsText_create();
  char *sFooterString = UtilsUI_createBuffer();

  // Copyright mark
  if(this->eGameState == GAME_MENU) {
    UtilsText_addText(this->pFooterText, UtilsText_paddedText(
      "[ Mo David @2023 ]    ", " ", UTILS_TEXT_RIGHT_ALIGN));
    UtilsText_addNewLines(this->pFooterText, 1);
  
  // If not in menu
  } else {
    UtilsText_addPatternLines(this->pFooterText, 1, "_");
    UtilsText_addPatternLines(this->pFooterText, 1, "'-");
    
    // If there is still dialogue to display
    if(!Game_isDialogueDone(this)) {
      
      // Dialogue display
      sprintf(sFooterString, "         %2d/%2d  | %s",
        this->dDialogueIndex + 1, 
        this->ASSETS->DIALOGUE_TEXT_LEN, 
        this->ASSETS->DIALOGUE_TEXT[this->dDialogueIndex]);

      // Create dialogue in the footer
      UtilsText_addText(this->pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
      UtilsText_addNewLines(this->pFooterText, 2);
      UtilsText_addText(this->pFooterText, UtilsText_paddedText(
        "-=-{ [SPACE BAR] to continue; [Z] to skip tutorial; [Q] to exit to main menu. }-=-=", "-=", UTILS_TEXT_RIGHT_ALIGN));
    
    // The footer displays additional information on selections 
    // It can also provide tips and what not
    } else {
      switch(this->ePlayState) {

        // Selection description display
        case PLAY_SELECTING:

          sprintf(sFooterString, this->sFooterFrontTemplate,
            UtilsSelector_getCurrentOption(this->pPlaySelector), 
            this->ASSETS->SCENE_SELECT_TEXT[UtilsSelector_getCurrentIndex(this->pPlaySelector) * 2]);
          UtilsText_addText(this->pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
          
          sprintf(sFooterString, this->sFooterBlankFrontTemplate,
            this->ASSETS->SCENE_SELECT_TEXT[UtilsSelector_getCurrentIndex(this->pPlaySelector) * 2 + 1]);
          UtilsText_addText(this->pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));          
          UtilsText_addNewLines(this->pFooterText, 1);
          break;
        
        // Home message display
        case PLAY_HOME:
          
          sprintf(sFooterString, this->sFooterFrontTemplate,
            "home", this->ASSETS->SCENE_HOME_MESSAGE_TEXT[0]);
          UtilsText_addText(this->pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
          
          if(!Player_isStarving(this->pPlayer))
            sprintf(sFooterString, this->sFooterBlankFrontTemplate, this->ASSETS->SCENE_HOME_MESSAGE_TEXT[1]);
          else 
            sprintf(sFooterString, this->sFooterBlankFrontTemplate, this->ASSETS->SCENE_HOME_MESSAGE_TEXT[2]);
              
          UtilsText_addText(this->pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
          UtilsText_addNewLines(this->pFooterText, 1);
          break;

        // Displays farm info
        case PLAY_FARM:
          Farm_footer(
            this->pFarm, this->pPlayer, this->pFooterText, this->pCatalogueSelector, 
            this->CATALOGUE, this->ASSETS->SCENE_FARM_SELECT_TEXT,
            this->sFooterFrontTemplate, this->sFooterBlankFrontTemplate);
          break;

        case PLAY_SHOP:
          Shop_footer(
            this->pShop, this->pPlayer, this->pFooterText, this->pCatalogueSelector, 
            this->CATALOGUE, this->ASSETS->SCENE_SHOP_SELECT_TEXT,
            this->sFooterFrontTemplate, this->sFooterBlankFrontTemplate);
          break;

        // Empty footer
        default:
          UtilsText_addNewLines(this->pFooterText, 3);
          break;
      }

      UtilsText_addText(this->pFooterText, UtilsText_paddedText(
        "-=-{ [I] to view inventory; [H] to view controls; [Q] to exit to main menu. }-=-=", "-=", UTILS_TEXT_RIGHT_ALIGN));
    }
  }

  free(sFooterString);
}

/**
 * Frees the header memory block.
 * 
 * @param   {struct Game *}   this  The game object.
*/
void Game_killFooter(struct Game *this) {
  UtilsText_kill(this->pFooterText);
}

/**
 * ############################
 * ###  GAME UI COMPONENTS  ###
 * ############################
*/ 

/**
 * Displays the visual component of the menu.
 * This is used as a callback function.
 * 
 * @param   {char}            cInput  The current user key input.
 * @param   {struct Game *}   this    The game object.
*/
void Game_menuUI(char cInput, struct Game *this) {
  this->pScreenText = UtilsText_create();
  Game_makeFooter(this);

  // Title
  UtilsText_addBlock(this->pScreenText, this->ASSETS->TITLE_SPRITE, this->ASSETS->TITLE_SPRITE_LEN);
  UtilsText_addNewLines(this->pScreenText, 3);

  // Selector
  for(int i = 0; i < UtilsSelector_getLength(this->pMenuSelector); i++)
    UtilsText_addText(this->pScreenText, UtilsSelector_getOptionFormatted(this->pMenuSelector, i));
  UtilsText_addNewLines(this->pScreenText, 3);

  UtilsText_addText(this->pScreenText, this->ASSETS->DIVIDER_TEXT[0]);
  UtilsText_addNewLines(this->pScreenText, 2);

  UtilsText_addText(this->pScreenText, "[Enter] to choose an option");
  UtilsText_addText(this->pScreenText, "[X] and [C] to change selection");
  
  // Formatting
  this->pScreenText = UtilsUI_centerXY(this->pScreenText);
  UtilsUI_footer(this->pScreenText, this->pFooterText);

  // Print final text
  UtilsIO_clear();
  UtilsUI_print(this->pScreenText);

  // Garbage collection!
  UtilsText_kill(this->pScreenText);
  Game_killFooter(this);
}

/**
 * In the default game mode, this function requests for the user's name.
 * 
 * @param   {char *}          sInput  User input provided by the calling function.
 * @param   {struct Game *}   this    The game object.
*/
void Game_playStartUI(char *sInput, struct Game *this) {
  this->pScreenText = UtilsText_create();

  if(strlen(this->sInputWarning))
    UtilsText_addNewLines(this->pScreenText, 2);

  // Create UI prompt
  UtilsText_addText(this->pScreenText, "[ Enter A Name ]");
  UtilsText_addText(this->pScreenText, strlen(sInput) ? sInput : "________________");

  // In case name was too long
  if(strlen(this->sInputWarning)) {
    UtilsText_addNewLines(this->pScreenText, 1);
    UtilsText_addText(this->pScreenText, this->sInputWarning);
  }
  
  // Formatting
  this->pScreenText = UtilsUI_centerXY(this->pScreenText);

  // Print final text
  UtilsIO_clear();
  UtilsUI_print(this->pScreenText);

  // Garbage collection!
  UtilsText_kill(this->pScreenText);
}

/**
 * The actual UI of the gameplay.
 * 
 * @param   {char}            cInput  The current user input.
 * @param   {struct Game *}   this    The game object.
*/
void Game_playUI(char cInput, struct Game *this) {
  this->pScreenText = UtilsText_create();

  // Create the game header and footer
  Game_makeHeader(this);
  Game_makeFooter(this);

  // Create game UI
  switch(this->ePlayState) {

    case PLAY_SELECTING:

      // Show selector
      if(Game_isDialogueDone(this)) {
        UtilsText_addNewLines(this->pScreenText, 1);
        UtilsText_addText(this->pScreenText, "It's a brand new day.");
        UtilsText_addNewLines(this->pScreenText, 1);
        UtilsText_addBlock(this->pScreenText, this->ASSETS->SELECTING_SPRITE, this->ASSETS->SELECTING_SPRITE_LEN);
        UtilsText_addNewLines(this->pScreenText, 2);
        UtilsText_addText(this->pScreenText, "Where do you want to go?");
        UtilsText_addNewLines(this->pScreenText, 1);

        // Display main action selector
        for(int i = 0; i < UtilsSelector_getLength(this->pPlaySelector); i++)
          UtilsText_addText(this->pScreenText, UtilsSelector_getOptionFormatted(this->pPlaySelector, i));
        
      // Show game title
      } else {
        UtilsText_addBlock(this->pScreenText, this->ASSETS->WELCOME_SPRITE, this->ASSETS->WELCOME_SPRITE_LEN);
        UtilsText_addNewLines(this->pScreenText, 1);
      }
      break;

    case PLAY_HOME:

      // Show home message and ASCII art
      UtilsText_addText(this->pScreenText, "You just went home.");
      UtilsText_addNewLines(this->pScreenText, 1);
      UtilsText_addBlock(this->pScreenText, this->ASSETS->HOME_SPRITE, this->ASSETS->HOME_SPRITE_LEN);
      UtilsText_addNewLines(this->pScreenText, 2);
      UtilsText_addText(this->pScreenText, "[Enter] to continue");
      break;

    case PLAY_FARM:
      Farm_UI(
        this->pFarm, this->pPlayer, this->pScreenText, this->pCatalogueSelector,
        this->CATALOGUE, this->ASSETS->FARM_SPRITE, this->ASSETS->FARM_SPRITE_LEN,
        this->sCurrentIntInput, this->sInputWarning);
      break;

    case PLAY_SHOP:
      Shop_UI(
        this->pShop, this->pPlayer, this->pScreenText, this->pCatalogueSelector, 
        this->CATALOGUE, this->ASSETS->SHOP_SPRITE, this->ASSETS->SHOP_SPRITE_LEN,
        this->sCurrentIntInput, this->sInputWarning);
      break;
  }

  UtilsText_addNewLines(this->pScreenText, 3);

  // Formatting
  this->pScreenText = UtilsUI_centerXY(this->pScreenText);
  UtilsUI_header(this->pScreenText, this->pHeaderText);
  UtilsUI_footer(this->pScreenText, this->pFooterText);

  // Print final text
  UtilsIO_clear();
  UtilsUI_print(this->pScreenText);

  // Garbage collection!
  UtilsText_kill(this->pScreenText);
  Game_killHeader(this);
  Game_killFooter(this);

}

/**
 * UI for the guide page.
 * 
 * @param   {char}            cInput  The current user input.
 * @param   {struct Game *}   this    The game object.
*/
void Game_guideUI(char cInput, struct Game *this) {
  this->pScreenText = UtilsText_create();
  
  // Content
  UtilsText_addNewLines(this->pScreenText, 2);
  UtilsText_addBlock(this->pScreenText, this->ASSETS->GUIDE_TEXT, this->ASSETS->GUIDE_TEXT_LEN);
  UtilsText_addNewLines(this->pScreenText, 3);
  UtilsText_addText(this->pScreenText, this->ASSETS->DIVIDER_TEXT[0]);
  UtilsText_addNewLines(this->pScreenText, 2);
  UtilsText_addText(this->pScreenText, "You can check an outline of key controls in the \"keys\" section");
  UtilsText_addText(this->pScreenText, "[Q] to return to the menu");

  // Formatting
  this->pScreenText = UtilsUI_centerXY(this->pScreenText);
  
  // Print final text
  UtilsIO_clear();
  UtilsUI_print(this->pScreenText);

  // Garbage collection!
  UtilsText_kill(this->pScreenText);
}

/**
 * UI for the about the controls page.
 * 
 * @param   {char}            cInput  The current user input.
 * @param   {struct Game *}   this    The game object.
*/
void Game_controlsUI(char cInput, struct Game *this) {
  this->pScreenText = UtilsText_create();
  
  // Content
  UtilsText_addBlock(this->pScreenText, this->ASSETS->CONTROLS_SPRITE, this->ASSETS->CONTROLS_SPRITE_LEN);
  UtilsText_addNewLines(this->pScreenText, 1);
  UtilsText_addBlock(this->pScreenText, this->ASSETS->CONTROLS_TEXT, this->ASSETS->CONTROLS_TEXT_LEN);
  UtilsText_addNewLines(this->pScreenText, 2);
  UtilsText_addText(this->pScreenText, this->ASSETS->DIVIDER_TEXT[0]);
  UtilsText_addNewLines(this->pScreenText, 2);
  UtilsText_addText(this->pScreenText, "The controls will be explained in more detail as you play.");
  UtilsText_addText(this->pScreenText, "[Q] to return to the menu");

  // Formatting
  this->pScreenText = UtilsUI_centerXY(this->pScreenText);
  
  // Print final text
  UtilsIO_clear();
  UtilsUI_print(this->pScreenText);

  // Garbage collection!
  UtilsText_kill(this->pScreenText);
}

/**
 * UI for the about the author page.
 * 
 * @param   {char}            cInput  The current user input.
 * @param   {struct Game *}   this    The game object.
*/
void Game_authorUI(char cInput, struct Game *this) {
  this->pScreenText = UtilsText_create();
  
  // Content
  UtilsText_addNewLines(this->pScreenText, 4);
  UtilsText_addBlock(this->pScreenText, this->ASSETS->AUTHOR_SPRITE, this->ASSETS->AUTHOR_SPRITE_LEN);
  UtilsText_addNewLines(this->pScreenText, 2);
  UtilsText_addBlock(this->pScreenText, this->ASSETS->AUTHOR_TEXT, this->ASSETS->AUTHOR_TEXT_LEN);
  UtilsText_addNewLines(this->pScreenText, 3);
  UtilsText_addText(this->pScreenText, this->ASSETS->DIVIDER_TEXT[0]);
  UtilsText_addNewLines(this->pScreenText, 2);
  UtilsText_addText(this->pScreenText, "Check the code for additional credits and links");
  UtilsText_addText(this->pScreenText, "[Q] to return to the menu");

  // Formatting
  this->pScreenText = UtilsUI_centerXY(this->pScreenText);
  
  // Print final text
  UtilsIO_clear();
  UtilsUI_print(this->pScreenText);

  // Garbage collection!
  UtilsText_kill(this->pScreenText);
}

/**
 * Displays the dialog box.
 * This is used as a callback function.
 * 
 * @param   {char}            cInput  The current user key input.
 * @param   {struct Game *}   this    The game object.
*/
void Game_dialogUI(char cInput, struct Game *this) {

  // Create the dialog box
  this->pScreenText = UtilsText_create();

  // Reminder on how the keys work
  if(this->eDialogState == DIALOG_HELP) {

    UtilsText_addBlock(this->pScreenText, this->ASSETS->CONTROLS_SPRITE, this->ASSETS->CONTROLS_SPRITE_LEN);
    UtilsText_addNewLines(this->pScreenText, 1);
    UtilsText_addBlock(this->pScreenText, this->ASSETS->CONTROLS_TEXT, this->ASSETS->CONTROLS_TEXT_LEN);
    UtilsText_addNewLines(this->pScreenText, 2);
    UtilsText_addText(this->pScreenText, this->ASSETS->DIVIDER_TEXT[0]);
    UtilsText_addNewLines(this->pScreenText, 2);
    UtilsText_addText(this->pScreenText, "[Q] to return to the game");
    UtilsText_addNewLines(this->pScreenText, 1);
  
  // Player inventory
  } else if(this->eDialogState == DIALOG_INVENTORY) {

    // Display all the plants and how much the player has
    UtilsText_addNewLines(this->pScreenText, 2);
    UtilsText_addBlock(this->pScreenText, this->ASSETS->INVENTORY_SPRITE, this->ASSETS->INVENTORY_SPRITE_LEN);
    UtilsText_addNewLines(this->pScreenText, 3);

    for(int i = 0; i < this->CATALOGUE->dSize; i++) {
      char *sStockDisplay = UtilsUI_createBuffer();
      
      // Skips the null catalogue type
      if(!i) {

        // Table header
        sprintf(sStockDisplay, "  %-24s | %15s | %15s ", " ", "seed bags", "harvested crops");
        UtilsText_addText(this->pScreenText, sStockDisplay);
        UtilsText_addText(this->pScreenText, "=-=-=-=-=-=-=-=-=-=-=-=-=-=|=-=-=-=-=-=-=-=-=|=-=-=-=-=-=-=-=-=");
      
      // Displays each plant and how much of it the player has
      } else {
        
        // Table row
        sprintf(sStockDisplay, "  %-24s | %9d seeds | %7d to sell ", 
          this->CATALOGUE->sProductNameArray[i], 
          Stock_getAmount(Player_getSeedStock(this->pPlayer, i)),
          Stock_getAmount(Player_getCropStock(this->pPlayer, i)));

        UtilsText_addText(this->pScreenText, sStockDisplay);
      }
    }

    // Add a variable number of new lines in case we add more products
    UtilsText_addNewLines(this->pScreenText, (10 - this->CATALOGUE->dSize) > 0 ? (8 - this->CATALOGUE->dSize) : 0);
    UtilsText_addText(this->pScreenText, this->ASSETS->DIVIDER_TEXT[0]);
    UtilsText_addNewLines(this->pScreenText, 2);
    UtilsText_addText(this->pScreenText, "[Q] to return to the game");
    UtilsText_addNewLines(this->pScreenText, 6);

  // Display other popup types
  } else {
    UtilsText_addNewLines(this->pScreenText, 3);

    // Show the associated text art
    switch(this->eDialogState) {
      case DIALOG_PAUSED: UtilsText_addBlock(this->pScreenText, this->ASSETS->PAUSE_SPRITE, this->ASSETS->PAUSE_SPRITE_LEN); break;
      case DIALOG_GAMEOVER: UtilsText_addBlock(this->pScreenText, this->ASSETS->GOVER_SPRITE, this->ASSETS->GOVER_SPRITE_LEN); break;
      case DIALOG_EASTER: UtilsText_addBlock(this->pScreenText, this->ASSETS->AUTHOR_SPRITE, this->ASSETS->AUTHOR_SPRITE_LEN); break;
      default: break;
    }
    
    UtilsText_addNewLines(this->pScreenText, 2);
    UtilsText_addText(this->pScreenText, UtilsText_centeredText(this->sDialogMessage));
    UtilsText_addNewLines(this->pScreenText, 1);
    
    // Provide a yes or no option
    if(this->eDialogState != DIALOG_GAMEOVER) {
      UtilsText_addText(this->pScreenText, UtilsText_centeredText(UtilsSelector_getOptionFormatted(this->pDialogSelector, 0)));
      UtilsText_addText(this->pScreenText, UtilsText_centeredText(UtilsSelector_getOptionFormatted(this->pDialogSelector, 1)));
      UtilsText_addNewLines(this->pScreenText, 4);
    
    // No options available when game over
    } else {
      UtilsText_addText(this->pScreenText, UtilsText_centeredText("[Enter] to return to the menu"));
      UtilsText_addNewLines(this->pScreenText, 5);
    }
  }

  // Format it
  UtilsUI_centerXY(this->pScreenText);
  UtilsIO_clear();

  // Print it
  UtilsUI_print(this->pScreenText);
  
  // Garbage collection
  UtilsText_kill(this->pScreenText);
}

/**
 * ############################
 * ###  GAME IO COMPONENTS  ###
 * ############################
*/

/**
 * Handles IO interaction with the menu.
 * It is also used as a callback function.
 * 
 * @param   {char}            cInput  The current user key input.
 * @param   {struct Game *}   this    The game object.
*/
void Game_menuIO(char cInput, struct Game *this) {
  if(cInput == 'X') UtilsSelector_decrement(this->pMenuSelector);
  if(cInput == 'C') UtilsSelector_increment(this->pMenuSelector);
}

/**
 * In the default game mode, this function sets the user's name.
 * 
 * @param   {char *}          sInput  User input provided by the calling function.
 * @param   {struct Game *}   this    The game object.
*/
void Game_playStartIO(char *sInput, struct Game *this) {
  Player_setName(this->pPlayer, sInput);

  if(strlen(sInput) > PLAYER_NAME_MAX_LEN) {
    strcpy(sInput, "");
    sprintf(this->sInputWarning, "Name should be at most (%d) characters.", PLAYER_NAME_MAX_LEN);
  } else {
    strcpy(this->sInputWarning, "");
    this->bFirst = 0;
  }
}


/**
 * Handles the IO of the gameplay section.
 * 
 * @param   {char}            cInput  User input provided by the calling function.
 * @param   {struct Game *}   this    The game object.
*/
void Game_playIO(char cInput, struct Game *this) {
  switch(this->ePlayState) {
    
    // Player is choosing what to do
    case PLAY_SELECTING:
      if(cInput == 'X') UtilsSelector_decrement(this->pPlaySelector);
      if(cInput == 'C') UtilsSelector_increment(this->pPlaySelector);

      if(cInput == ' ') this->dDialogueIndex++;
      if(cInput == 'Z') this->dDialogueIndex = this->ASSETS->DIALOGUE_TEXT_LEN;

      // Change where we are and what we're doing in the gameplay
      if(Game_isDialogueDone(this))
        if(UtilsKey_isReturn(cInput, ""))
          this->pPlayFuncArray[UtilsSelector_getCurrentValue(this->pPlaySelector)](this);
      break;

    // Player just went home
    case PLAY_HOME:
      this->ePlayState = PLAY_SELECTING;
      break;

    // Player went to the farm
    case PLAY_FARM:
      Farm_IO(
        this->pFarm, this->pPlayer, this->pCatalogueSelector, this->CATALOGUE,
        cInput, this->sCurrentIntInput, this->sInputWarning, 
        &this->ePlayState, &this->eGameState);

      break;

    // Player went to the shop
    case PLAY_SHOP:
      Shop_IO(
        this->pShop, this->pPlayer, this->pCatalogueSelector, this->CATALOGUE,
        cInput, this->sCurrentIntInput, this->sInputWarning, 
        &this->ePlayState, &this->eGameState);

      break;
  }

  if(cInput == 'Q') {
    this->eDialogState = DIALOG_PAUSED;
    strcpy(this->sDialogMessage, "Are you sure you want to exit to the main menu?");
  }

  if(cInput == 'H')
    this->eDialogState = DIALOG_HELP;

  if(cInput == 'I')
    this->eDialogState = DIALOG_INVENTORY;

}

/**
 * Handles the IO of the guide section.
 * I know this is empty but keeping it here makes everything cleaner.
 * 
 * @param   {char}            cInput  User input provided by the calling function.
 * @param   {struct Game *}   this    The game object.
*/
void Game_guideIO(char cInput, struct Game *this) {

}


/**
 * Handles the IO of the controls section.
 * I know this is empty but keeping it here makes everything cleaner.
 * 
 * @param   {char}            cInput  User input provided by the calling function.
 * @param   {struct Game *}   this    The game object.
*/
void Game_controlsIO(char cInput, struct Game *this) {

}

/**
 * Handles the IO of the author section.
 * I know this is empty but keeping it here makes everything cleaner.
 * 
 * @param   {char}            cInput  User input provided by the calling function.
 * @param   {struct Game *}   this    The game object.
*/
void Game_authorIO(char cInput, struct Game *this) {

}

/**
 * Handles dialog box interaction.
 * 
 * @param   {char}            cInput  User input provided by the calling function.
 * @param   {struct Game *}   this    The game object.
*/
void Game_dialogIO(char cInput, struct Game *this) {
  if(cInput == 'X') UtilsSelector_decrement(this->pDialogSelector);
  if(cInput == 'C') UtilsSelector_increment(this->pDialogSelector);
}

/**
 * #############################
 * ###  GAME PLAY FUNCTIONS  ###
 * #############################
*/

/**
 * Sets up all the processes associated with going home.
 * 
 * @param   {struct Game *}   this    The game object.
*/
void Game_playHome(struct Game *this) {
  
  // Go home, sleep and eat breakfast
  this->ePlayState = PLAY_HOME;
  Player_goHome(this->pPlayer);

  // If game over
  if(Player_isDead(this->pPlayer)) {
    this->eDialogState = DIALOG_GAMEOVER;
    strcpy(this->sDialogMessage, "You starved for three consecutive days!");

    // Copy name so we don't have to ask for it again
    char* sName = Player_getName(this->pPlayer);

    // Reset the game objects
    Player_kill(this->pPlayer);
    Farm_kill(this->pFarm);
    Shop_kill(this->pShop);

    struct Player *pPlayer = 
      Player_create(
        GAME_PLAYER_GOLD, 
        GAME_PLAYER_ENERGY, 
        GAME_PLAYER_ENERGY,
        this->CATALOGUE);

    struct Farm *pFarm = 
      Farm_create(
        GAME_FARM_WIDTH, 
        GAME_FARM_HEIGHT, 
        this->CATALOGUE);

    struct Shop *pShop = 
      Shop_create(
        this->CATALOGUE);

    Player_setName(pPlayer, sName);

    // Copy all the objects into the struct
    this->pPlayer = pPlayer;
    this->pFarm = pFarm;
    this->pShop = pShop;

    // Go to menu after the dialog box
    this->ePlayState = PLAY_SELECTING;
    this->eGameState = GAME_MENU;
  }
}

/**
 * Sets up all the processes associated with going to the farm.
 * 
 * @param   {struct Game *}   this    The game object.
*/
void Game_playFarm(struct Game *this) {
  this->ePlayState = PLAY_FARM;
}

/**
 * Sets up all the processes associated with visiting the shop.
 * 
 * @param   {struct Game *}   this    The game object.
*/
void Game_playShop(struct Game *this) {
  this->ePlayState = PLAY_SHOP;
}

/**
 * #############################
 * ###  GAME STATE HANDLERS  ###
 * #############################
*/

/**
 * Handles and puts together all the menu components.
 * 
 * @param   {struct Game *}   this  The game object.
*/
void Game_menu(struct Game *this) {

  // Implementing this part using callbacks was a pain in the *ss
  // Eventually I thought I needed to create a helper class to move data across the functions
  // I created something called UtilsBit specifically for that purpose
  // Then I realized I could do something like void *ptr for a generic pointer type
  // :/

  // Keep receiving input until user presses enter
  UtilsKey_inputPoll(&UtilsKey_isNotReturn, Game_menuUI, Game_menuIO, this, "");

  // Scene switching once user presses enter
  this->eGameState = UtilsSelector_getCurrentValue(this->pMenuSelector);
}

/**
 * Unifies all the gameplay components.
 * 
 * @param   {struct Game *}   this  The game object.
*/
void Game_play(struct Game *this) {

  // Keep receiving input until user presses enter
  if(this->bFirst)
    UtilsKey_inputString(&Game_playStartUI, &Game_playStartIO, this);
  else 
    UtilsKey_inputPoll(&UtilsKey_isNot, 
      this->pUIFuncArray[GAME_PLAY], 
      this->pIOFuncArray[GAME_PLAY], this, "HIQ\n\r");

  // If there are dialog boxes to display, don't show the game
  if(this->eDialogState != DIALOG_NULL) {
    switch(this->eDialogState) {
      
      case DIALOG_PAUSED:
        UtilsKey_inputPoll(&UtilsKey_isNotReturn, 
          this->pUIFuncArray[GAME_DIALOG], 
          this->pIOFuncArray[GAME_DIALOG], this, " ");

        // Exit to menu if user presses enter and selects "Okay"
        if(UtilsSelector_getCurrentValue(this->pDialogSelector)) 
          this->eGameState = GAME_MENU;
        break;

      case DIALOG_HELP:
      case DIALOG_INVENTORY:
        
        // Return to game when user presses Q
        UtilsKey_inputPoll(&UtilsKey_isNot,
          this->pUIFuncArray[GAME_DIALOG], 
          this->pIOFuncArray[GAME_DIALOG], this, "Q");
        break;

      case DIALOG_GAMEOVER:
        
        // User can only hit enter to leave the game over dialog box
        UtilsKey_inputPoll(&UtilsKey_isNotReturn,
          this->pUIFuncArray[GAME_DIALOG], 
          this->pIOFuncArray[GAME_DIALOG], this, " ");
        break;

      default: break;
    }

    this->eDialogState = DIALOG_NULL;
  }
}

/**
 * Handles all the game guide components.
 * 
 * @param   {struct Game *}   this  The game object.
*/
void Game_guide(struct Game *this) {

  // Keep receiving input until user presses Q
  UtilsKey_inputPoll(&UtilsKey_isNot, 
    this->pUIFuncArray[GAME_GUIDE], 
    this->pIOFuncArray[GAME_GUIDE], this, "Q");

  // Return to menu
  this->eGameState = GAME_MENU;
}

/**
 * Handles all the game guide components.
 * 
 * @param   {struct Game *}   this  The game object.
*/
void Game_controls(struct Game *this) {

  // Keep receiving input until user presses Q
  UtilsKey_inputPoll(&UtilsKey_isNot,
    this->pUIFuncArray[GAME_CONTROLS], 
    this->pIOFuncArray[GAME_CONTROLS], this, "Q");

  // Return to menu
  this->eGameState = GAME_MENU;
}

/**
 * A little something about the author and the game.
 * 
 * @param   {struct Game *}   this  The game object.
*/
void Game_author(struct Game *this) {

  // Keep receiving input until user presses Q
  UtilsKey_inputPoll(&UtilsKey_isNot, 
    this->pUIFuncArray[GAME_AUTHOR], 
    this->pIOFuncArray[GAME_AUTHOR], this, "Q");

  // Return to menu
  this->eGameState = GAME_MENU;
}

/**
 * Displays a warning that the console size is too small (if it is).
 * 
 * @param   {struct Game *}   this  The game object.
*/
void Game_consoleWarning(struct Game *this) {

  // Generate warning text
  char *sCurConSize = calloc(32, sizeof(char));
  char *sMinConSize = calloc(32, sizeof(char));

  this->pScreenText = UtilsText_create();
  UtilsText_addBlock(this->pScreenText, this->ASSETS->WARNING_SPRITE, this->ASSETS->WARNING_SPRITE_LEN);
  UtilsText_addNewLines(this->pScreenText, 1);

  sprintf(sCurConSize, "current: [%3d x %3d]", UtilsIO_getWidth(), UtilsIO_getHeight());
  UtilsText_addText(this->pScreenText, sCurConSize);
  sprintf(sMinConSize, "minimum: [%3d x %3d]", GAME_MIN_WIDTH, GAME_MIN_HEIGHT);
  UtilsText_addText(this->pScreenText, sMinConSize);

  // Format
  UtilsUI_centerXY(this->pScreenText);
  
  // Print
  UtilsIO_clear();
  UtilsUI_print(this->pScreenText);

  // Garbage collection
  free(sCurConSize);
  free(sMinConSize);
  UtilsText_kill(this->pScreenText);
  
  // Press enter to continue
  UtilsIO_readChar();
}

/**
 * Configures the running mode of the game.
 * 
 * @param   {struct Game *}   this    The game object.
 * @param   {char *}          sMode   Let's the game run in the default, debug, or minimal mode.
 * @param   {char *}          sScene  Specifies the scene to start on for debug mode.
*/
void Game_conf(struct Game *this, char *sMode, char *sScene) {

  int dMode = 0;

  // Convert to ints
  if(!strcmp(sMode, "default")) dMode = 0;
  if(!strcmp(sMode, "debug")) dMode = 1;
  if(!strcmp(sMode, "full")) dMode = 2;

  // Change game scene
  if(!strcmp(sScene, "play")) this->ePlayState = PLAY_SELECTING;
  if(!strcmp(sScene, "home")) this->ePlayState = PLAY_HOME;
  if(!strcmp(sScene, "farm")) this->ePlayState = PLAY_FARM;
  if(!strcmp(sScene, "shop")) this->ePlayState = PLAY_SHOP;
  
  // Different running modes
  switch(dMode) {

    // Default mode (minified mode)
    case 0: 
      this->dMode = 0;
      break;
    
    // Debug mode
    case 1: 

      printf("\x1b[38;5;232m");
      printf("\x1b[48;5;220m");

      // Change the variables
      this->bFirst = 0;
      this->dDialogueIndex = this->ASSETS->DIALOGUE_TEXT_LEN;
      this->eGameState = GAME_PLAY;

      // Just a nifty little indicator
      Player_setName(this->pPlayer, "DEBUG MODE"); 
      
      // God mode for debugging
      for(int i = 0; i < this->CATALOGUE->dSize; i++) {
        this->pPlayer->pSeedStockArray[i]->dAmount = 100;
        this->pPlayer->pCropStockArray[i]->dAmount = 100;
      }

      // Populate the farm just so its easier to debug
      for(int i = 0; i < this->pFarm->dSize; i++) {
        struct Product *p = Product_create((i % (this->CATALOGUE->dSize - 1) + 1), this->CATALOGUE, 0);
        p->dWaterAmt = (i % 2) ? (i % p->dWaterReq) : p->dWaterReq;
        p->dTimePlanted = 0;
        p->dLastWatered = 0;

        this->pFarm->pPlotArray[i]->eState = i % 3;
        this->pFarm->pPlotArray[i]->pProduct = p;
      }
      break;

    // Full mode
    case 2: 
      printf("\x1b[38;5;232m");
      printf("\x1b[48;5;254m");

      this->eGameState = GAME_MENU; 
      break;
  }
}

/**
 * Executes the full version of the game.
 * Switches between scenes and what not.
 * 
 * @param   {struct Game *}   this  The game object.
*/
void Game_execFull(struct Game *this) {
  do {

    // Also, if you're on Windows, this should work
    // It resize the console to the minimum size needed by the game
    // Doesn't happen on Linux based systems where I didn't implement automatic terminal resizing 
    if(UtilsIO_getWidth() < GAME_MIN_WIDTH || UtilsIO_getHeight() < GAME_MIN_HEIGHT) {
      if(!UtilsIO_setSize(GAME_MIN_WIDTH, GAME_MIN_HEIGHT))
        Game_consoleWarning(this);

    // Screen size can work for the game
    } else {

      // Run the function associated with the scene
      this->pHandlerFuncArray[this->eGameState](this);

    } 
  } while(this->eGameState != GAME_QUIT);

  printf("\x1b[38;5;255m");
  printf("\x1b[48;5;232m");

  // Clean the screen
  UtilsIO_clear();
}

/**
 * Executes the minified version of the game.
 * 
 * @param   {struct Game *}   this  The game object.
*/
void Game_execMini(struct Game *this) {
  struct GameMini gameMini;

  GameMini_init(&gameMini, this->CATALOGUE);
  GameMini_exec(&gameMini);
}

/**
 * Executes the full version of the game.
 * 
 * @param   {struct Game *}   this  The game object.
*/
void Game_exec(struct Game *this) {
  if(this->dMode)
    Game_execFull(this);
  else
    Game_execMini(this);
}

#endif
