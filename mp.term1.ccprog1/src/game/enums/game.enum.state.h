/**
 * Some enums that can help manage game state.
*/

#ifndef GAME_ENUM_STATE
#define GAME_ENUM_STATE

/**
 * The different game scenes.
*/
enum GameState { 
  GAME_MENU, 
  GAME_PLAY, 
  GAME_GUIDE,
  GAME_CONTROLS, 
  GAME_AUTHOR,
  GAME_DIALOG,
  GAME_QUIT,
};

/**
 * The different gameplay scenes.
*/
enum PlayState { 
  PLAY_SELECTING,
  PLAY_HOME, 
  PLAY_FARM, 
  PLAY_SHOP, 
};

/**
 * The different dialogs and popups.
*/
enum DialogState {
  DIALOG_NULL,
  DIALOG_PAUSED,
  DIALOG_HELP,
  DIALOG_INVENTORY,
  DIALOG_GAMEOVER,
  DIALOG_EASTER,
};

#endif