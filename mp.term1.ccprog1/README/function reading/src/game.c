/**
 * The actual main function where the game executes.
 * The main function here receives the parameters passed to the entry point of the program.
*/

#include "utils/utils.io.h"

#include "game/game.manager.h"
#include "game/game.manager.min.h"

#include "game/game.assets.h"
#include "game/game.catalogue.h"

int main(int argc, char *argv[]) {

  // This is necessary for cross-platform compatibility (Windows + Unix)
  // Coding this was rather tedious: console behavior is just handled quite differently by both
  // I use Ubuntu so it was kinda necessary
  struct UtilsIO utilsIO;
  UtilsIO_init(&utilsIO);
  
  // Game assets
  struct GameAssets assets;
  GameAssets_init(&assets);

  // Game catalogue
  struct GameCatalogue catalogue;
  GameCatalogue_init(&catalogue);

  // Create game
  struct Game game;
  Game_init(&game, &assets, &catalogue);
  Game_conf(&game, argv[1], argv[2]);
  Game_exec(&game);

  // Also needed for the program to work across platforms (Windows + Unix)
  UtilsIO_exit(&utilsIO);

  return 0;
}