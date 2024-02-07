/**
 * All the ASCII art I'll ever need.
 * ALso includes the game dialogue which is available in the default game mode.
*/

#ifndef GAME_ASSETS
#define GAME_ASSETS

#define TITLE_SPRITE_LENGTH 10
#define WELCOME_SPRITE_LENGTH 4
#define PAUSE_SPRITE_LENGTH 5
#define GOVER_SPRITE_LENGTH 4
#define WARNING_SPRITE_LENGTH 7

#define SELECTING_SPRITE_LENGTH 13
#define HOME_SPRITE_LENGTH 8
#define FARM_SPRITE_LENGTH 10
#define SHOP_SPRITE_LENGTH 12
#define INVENTORY_SPRITE_LENGTH 8
#define CONTROLS_SPRITE_LENGTH 9
#define AUTHOR_SPRITE_LENGTH 4

#define DIALOGUE_TEXT_LENGTH 18
#define SCENE_SELECT_TEXT_LENGTH 6
#define SCENE_HOME_MESSAGE_TEXT_LENGTH 3
#define SCENE_FARM_SELECT_TEXT_LENGTH 12
#define SCENE_SHOP_SELECT_TEXT_LENGTH 6
#define GUIDE_TEXT_LENGTH 12
#define CONTROLS_TEXT_LENGTH 5
#define AUTHOR_TEXT_LENGTH 4
#define DIVIDER_TEXT_LENGTH 1

/**
 * A struct that stores all the ASCII art as arrays of strings alongside their respective sizes.
*/
struct GameAssets {

  char *TITLE_SPRITE[TITLE_SPRITE_LENGTH];
  char *WELCOME_SPRITE[WELCOME_SPRITE_LENGTH];
  char *PAUSE_SPRITE[PAUSE_SPRITE_LENGTH];
  char *GOVER_SPRITE[GOVER_SPRITE_LENGTH];
  char *WARNING_SPRITE[WARNING_SPRITE_LENGTH];

  char *SELECTING_SPRITE[SELECTING_SPRITE_LENGTH];
  char *HOME_SPRITE[HOME_SPRITE_LENGTH];
  char *FARM_SPRITE[FARM_SPRITE_LENGTH];
  char *SHOP_SPRITE[SHOP_SPRITE_LENGTH];
  char *INVENTORY_SPRITE[INVENTORY_SPRITE_LENGTH];
  char *CONTROLS_SPRITE[CONTROLS_SPRITE_LENGTH];
  char *AUTHOR_SPRITE[AUTHOR_SPRITE_LENGTH];

  char *DIALOGUE_TEXT[DIALOGUE_TEXT_LENGTH];
  char *SCENE_SELECT_TEXT[SCENE_SELECT_TEXT_LENGTH];
  char *SCENE_HOME_MESSAGE_TEXT[SCENE_HOME_MESSAGE_TEXT_LENGTH];
  char *SCENE_FARM_SELECT_TEXT[SCENE_FARM_SELECT_TEXT_LENGTH];
  char *SCENE_SHOP_SELECT_TEXT[SCENE_SHOP_SELECT_TEXT_LENGTH];
  char *GUIDE_TEXT[GUIDE_TEXT_LENGTH];
  char *CONTROLS_TEXT[CONTROLS_TEXT_LENGTH];
  char *AUTHOR_TEXT[AUTHOR_TEXT_LENGTH];
  char *DIVIDER_TEXT[DIVIDER_TEXT_LENGTH];

  int TITLE_SPRITE_LEN;
  int WELCOME_SPRITE_LEN;
  int PAUSE_SPRITE_LEN;
  int GOVER_SPRITE_LEN;
  int WARNING_SPRITE_LEN;

  int SELECTING_SPRITE_LEN;
  int HOME_SPRITE_LEN;
  int FARM_SPRITE_LEN;
  int SHOP_SPRITE_LEN;
  int INVENTORY_SPRITE_LEN;
  int CONTROLS_SPRITE_LEN;
  int AUTHOR_SPRITE_LEN;

  int DIALOGUE_TEXT_LEN;
  int SCENE_SELECT_TEXT_LEN;
  int SCENE_HOME_MESSAGE_TEXT_LEN;
  int SCENE_FARM_SELECT_TEXT_LEN;
  int GUIDE_TEXT_LEN;
  int CONTROLS_TEXT_LEN;
  int AUTHOR_TEXT_LEN;
  int DIVIDER_TEXT_LEN;
};

/**
 * Defines all the sprites and images in the game.
 * 
 * @param   {struct GameAssets *}   this  The GameAssets object.
*/
void GameAssets_init(struct GameAssets *this) {
  
/**
 * ######################
 * ###  GAME SPRITES  ###
 * ######################
*/

  //
  // Game title
  // https://patorjk.com/software/taag/#p=display 

  this->TITLE_SPRITE[0] = "                         ,     :     ,                           ";
  this->TITLE_SPRITE[1] = "                    '.    ;    :    ;    ,`                      ";
  this->TITLE_SPRITE[2] = "                '-.   '.   ;   :   ;   ,`   ,-`                  ";
  this->TITLE_SPRITE[3] = "             \"-.   '-.  '.  ;  :  ;  ,`  ,-`   ,-\"               ";
  this->TITLE_SPRITE[4] = "                \"-.   '-. '. ; : ; ,` ,-`   ,-\"                  ";
  this->TITLE_SPRITE[5] = "           '\"--.   '\"-.  '-.'  '  `.-`  ,-\"`   ,--\"`             ";
  this->TITLE_SPRITE[6] = " __             '\"--.  '\"-.   ...    _\"`  ,--\"`                  ";
  this->TITLE_SPRITE[7] = "|  |--.---.-.----.--.--.-----.::::-.|  |_     .-----.--.--.-----.";
  this->TITLE_SPRITE[8] = "|     |  _  |   _|  |  |  -__|__ --||   _|    |__ --|  |  |     |";
  this->TITLE_SPRITE[9] = "|__|__|___._|__|  \\___/|_____|_____||____|    |_____|_____|__|__|";
  

  //
  // Welcome message
  // https://patorjk.com/software/taag/#p=display

  this->WELCOME_SPRITE[0] = "                 __                            ";
  this->WELCOME_SPRITE[1] = ".--.--.--.-----.|  |.----.-----.--------.-----.";
  this->WELCOME_SPRITE[2] = "|  |  |  |  -__||  ||  __|  _  |        |  -__|";
  this->WELCOME_SPRITE[3] = "|________|_____||__||____|_____|__|__|__|_____|";

  //
  // Paused
  // https://patorjk.com/software/taag/#p=display                              

  this->PAUSE_SPRITE[0] = "                                                                  __ ";
  this->PAUSE_SPRITE[1] = ".-----.---.-.--------.-----.    .-----.---.-.--.--.-----.-----.--|  |";
  this->PAUSE_SPRITE[2] = "|  _  |  _  |        |  -__|    |  _  |  _  |  |  |__ --|  -__|  _  |";
  this->PAUSE_SPRITE[3] = "|___  |___._|__|__|__|_____|    |   __|___._|_____|_____|_____|_____|";
  this->PAUSE_SPRITE[4] = "|_____|                         |__|                                 ";

  //
  // Game over
  // https://patorjk.com/software/taag/#p=display

  this->GOVER_SPRITE[0] = ".-----.---.-.--------.-----.    .-----.--.--.-----.----.";
  this->GOVER_SPRITE[1] = "|  _  |  _  |        |  -__|    |  _  |  |  |  -__|   _|";
  this->GOVER_SPRITE[2] = "|___  |___._|__|__|__|_____|    |_____|\\___/|_____|__|  ";
  this->GOVER_SPRITE[3] = "|_____|                                                 ";

  //
  // Warning for wrong console size
  // https://patorjk.com/software/taag/#p=display

  this->WARNING_SPRITE[0] = " _      _____   ___  _  _______  _______     ._______. ";
  this->WARNING_SPRITE[1] = "| | /| / / _ | / _ \\/ |/ /  _/ |/ / ___/     | >_ <  | ";
  this->WARNING_SPRITE[2] = "| |/ |/ / __ |/ , _/    // //    / (_ /      |       | ";
  this->WARNING_SPRITE[3] = "|__/|__/_/ |_/_/|_/_/|_/___/_/|_/\\___/       |_______| ";
  this->WARNING_SPRITE[4] = "                                                       ";
  this->WARNING_SPRITE[5] = "The dimensions of your console are too small to display";
  this->WARNING_SPRITE[6] = "the game. Please resize the console then hit [Enter] :D";

  //
  // Bench with lamppost
  // https://www.asciiart.eu/buildings-and-places/furniture/benches

  this->SELECTING_SPRITE[0] = "    .=gp.          ";
  this->SELECTING_SPRITE[1] = " .'/$$$$           ";
  this->SELECTING_SPRITE[2] = " || \"TP\"           ";
  this->SELECTING_SPRITE[3] = " ||          .:    ";
  this->SELECTING_SPRITE[4] = " ||       .-' |    ";
  this->SELECTING_SPRITE[5] = " ||    .-'    |    ";
  this->SELECTING_SPRITE[6] = " ||    |      !____";
  this->SELECTING_SPRITE[7] = " ||    |   .-'  .-'";
  this->SELECTING_SPRITE[8] = " ||    '.____.-'(  ";
  this->SELECTING_SPRITE[9] = " ||     \\  /  /__\\ ";
  this->SELECTING_SPRITE[10] = " ||      )(        ";
  this->SELECTING_SPRITE[11] = "|::|    /__\\       ";
  this->SELECTING_SPRITE[12] = "|::|               ";

  //
  // Home furniture
  // https://www.asciiart.eu/buildings-and-places/furniture/chairs

  this->HOME_SPRITE[0] = "   __________.          ";
  this->HOME_SPRITE[1] = "  /_/-----/_/|     __   ";
  this->HOME_SPRITE[2] = "  ( ( ' ' ( (|   /'--'\\ ";
  this->HOME_SPRITE[3] = "  (_( ' ' (_(|  /.    .\\";
  this->HOME_SPRITE[4] = "  / /=====/ /|    '||'  ";
  this->HOME_SPRITE[5] = " /_//____/_/ |     ||   ";
  this->HOME_SPRITE[6] = "(o|:.....|o) |     ||   ";
  this->HOME_SPRITE[7] = "|_|:_____|_|/'    _||_  ";

  //
  // Shop
  // https://www.asciiart.eu/buildings-and-places/houses

  this->SHOP_SPRITE[0] = "       ___________________       ";
  this->SHOP_SPRITE[1] = "      /\\        ______    \\      ";
  this->SHOP_SPRITE[2] = "     //_\\       \\    /\\    \\     ";
  this->SHOP_SPRITE[3] = "    //___\\       \\__/  \\    \\    ";
  this->SHOP_SPRITE[4] = "   //_____\\       \\ |[]|     \\   ";
  this->SHOP_SPRITE[5] = "  //_______\\       \\|__|      \\  ";
  this->SHOP_SPRITE[6] = " /XXXXXXXXXX\\                  \\ ";
  this->SHOP_SPRITE[7] = "/_I_II  I__I_\\__________________\\";
  this->SHOP_SPRITE[8] = "  I_I|  I__I_____[]_|_[]_____I   ";
  this->SHOP_SPRITE[9] = "  I_II  I__I_____[]_|_[]_____I   ";
  this->SHOP_SPRITE[10] = "  I II__I  I     XXXXXXX     I   ";
  this->SHOP_SPRITE[11] = "~~~~~\"   \"~~~~~~~~~~~~~~~~~~~~~~~~";

  //
  // Inventory
  // http://www.ascii-art.de/ascii/c/chest.txt         

  this->INVENTORY_SPRITE[0] = "  __________   ";
  this->INVENTORY_SPRITE[1] = " /\\____;;___\\  ";
  this->INVENTORY_SPRITE[2] = "| /         /  ";
  this->INVENTORY_SPRITE[3] = " `. ())oo() .  ";
  this->INVENTORY_SPRITE[4] = "  |\\(%()*^^()^\\";
  this->INVENTORY_SPRITE[5] = "  | |---------|";
  this->INVENTORY_SPRITE[6] = "  \\ |    ))   |";
  this->INVENTORY_SPRITE[7] = "   \\|_________|";

  //
  // Farm
  // https://ascii.co.uk/art/farm

  this->FARM_SPRITE[0] = "                    |           |_             ";
  this->FARM_SPRITE[1] = "                    |__________//=\\________    ";
  this->FARM_SPRITE[2] = "                   .'\\-'-'-'-'-'-'-'-'-'-'-\\   ";
  this->FARM_SPRITE[3] = "|--|--|--|--|--|-.'   \\-'-'-'-'-'-'-'-'-'-'-\\  ";
  this->FARM_SPRITE[4] = "|\\|             '-.    \\-'-'-'-'-'-'-'-'-'-'-\\ ";
  this->FARM_SPRITE[5] = " \\|\\|           |-.'-._ \\-'-'-'-'-'-'-'-'-'-'-\\";
  this->FARM_SPRITE[6] = "   \\|\\|--|--|-- | /'._ '|    ___       ___    |";
  this->FARM_SPRITE[7] = "     \\|--|--|-- |/ | /| |  [|_|_|]   [|_|_|]  |";
  this->FARM_SPRITE[8] = "                '-.|/ | |  [|_|_|]   [|_|_|]  |";
  this->FARM_SPRITE[9] = "                   '-.| |_____________________|";
  
  // 
  // Keyboard layout
  // My custom ASCII art
  
  this->CONTROLS_SPRITE[0] = ".___.       .___.              ";
  this->CONTROLS_SPRITE[1] = "| Q | .___. | E |              ";
  this->CONTROLS_SPRITE[2] = "|___| | W | |___|              ";
  this->CONTROLS_SPRITE[3] = "  .___|___|___.       .___.___.";
  this->CONTROLS_SPRITE[4] = "  | A | S | D |       | Enter |";
  this->CONTROLS_SPRITE[5] = "  |___|___|___|       |___ ___|";
  this->CONTROLS_SPRITE[6] = "        .___ ___.              ";
  this->CONTROLS_SPRITE[7] = "        | X | C |              ";
  this->CONTROLS_SPRITE[8] = "        |___|___|              ";

  //
  // Author cat
  // https://www.asciiart.eu/animals/cats

  this->AUTHOR_SPRITE[0] = "       |\\      _,,,---,,_    ";
  this->AUTHOR_SPRITE[1] = "ZZZzz /,`.-'`'    -.  ;-;;,_ ";
  this->AUTHOR_SPRITE[2] = "     |,4-  ) )-,_. ,\\ (  `'-'";
  this->AUTHOR_SPRITE[3] = "    '---''(_/--'  `-'\\_)     ";

/**
 * ####################
 * ###  GAME TEXTS  ###
 * ####################
*/

  //
  // Game Dialogue
  //
  this->DIALOGUE_TEXT[0] = "Welcome to Harvest Sun! So this is your first time playing, eh?";
  this->DIALOGUE_TEXT[1] = "Don't worry, I'll walk you through it.";
  this->DIALOGUE_TEXT[2] = "You see that header at the top of the screen?";
  this->DIALOGUE_TEXT[3] = "That tells you everything you need to know about your character.";
  this->DIALOGUE_TEXT[4] = "Energy is what enables you to do farming activities so you can earn money from crops.";
  this->DIALOGUE_TEXT[5] = "Without any energy your character won't be able to do anything, and must go home to sleep.";
  this->DIALOGUE_TEXT[6] = "Sleeping automatically resets the amount of energy you have. HOWEVER...";
  this->DIALOGUE_TEXT[7] = "Going home also means you will eat breakfast the following day, and breakfast costs gold...";
  this->DIALOGUE_TEXT[8] = "If we're being precise, it costs ten gold.";
  this->DIALOGUE_TEXT[9] = "Right now, you're all good and well because you have fifty pieces of gold. BUT...";
  this->DIALOGUE_TEXT[10] = "If you use up all of it and can't buy breakfast, you'll end up starving the following day.";
  this->DIALOGUE_TEXT[11] = "Three consecutive days of starvation spells death for the player. In other words, game over.";
  this->DIALOGUE_TEXT[12] = "So how do we get more gold? Well, it's simple!";
  this->DIALOGUE_TEXT[13] = "All you have to do is go to the farm and start planting.";
  this->DIALOGUE_TEXT[14] = "Eventually, after watering your crops a certain number of times, they'll be ready for harvest.";
  this->DIALOGUE_TEXT[15] = "And guess what, harvested crops can be sold for gold! Isn't that neat?";
  this->DIALOGUE_TEXT[16] = "Anyway, enough chit chat.";
  this->DIALOGUE_TEXT[17] = "What are you waiting for? Start farming!";

  //
  // Scene selection explainers and scene messages
  //
  
  this->SCENE_SELECT_TEXT[0] = "Sleep and restore your energy to its default amount. Eat breakfast for (10) gold.";
  this->SCENE_SELECT_TEXT[1] = "If you do not have enough gold to buy breakfast, you will starve the next day.";
  
  this->SCENE_SELECT_TEXT[2] = "Check out the current state of your crops.";
  this->SCENE_SELECT_TEXT[3] = "You can water crops (if you have any) or plant more (if you have seed bags).";
  
  this->SCENE_SELECT_TEXT[4] = "You can buy seed bags with gold.";
  this->SCENE_SELECT_TEXT[5] = "You can also sell harvested crops to earn more money.";

  // Going home
  this->SCENE_HOME_MESSAGE_TEXT[0] = "You just slept and regained all your energy!";
  this->SCENE_HOME_MESSAGE_TEXT[1] = "You also spent (10) pieces of gold on breakfast.";
  
  this->SCENE_HOME_MESSAGE_TEXT[2] = "Unfortunately, you did not have enough gold to buy breakfast. You will starve today.";

  // Going to the farm
  this->SCENE_FARM_SELECT_TEXT[0] = "View the current state of your farm and inspect individual plots.";
  this->SCENE_FARM_SELECT_TEXT[1] = "Plot information will show up on the footer.";

  this->SCENE_FARM_SELECT_TEXT[2] = "Till selected plots in your farm. Tilling a single plot costs (1) energy.";
  this->SCENE_FARM_SELECT_TEXT[3] = "All plots are initially untilled, and seeds can only be sown on tilled plots.";

  this->SCENE_FARM_SELECT_TEXT[4] = "Sow seeds on tilled plots. Sowing a single seed costs (1) energy.";
  this->SCENE_FARM_SELECT_TEXT[5] = "Seeds cannot be planted on untilled plots.";

  this->SCENE_FARM_SELECT_TEXT[6] = "Water crops that have been planted. Watering a single crop costs (1) energy.";
  this->SCENE_FARM_SELECT_TEXT[7] = "You can only water crops once a day.";

  this->SCENE_FARM_SELECT_TEXT[8] = "Harvest crops that have fully grown. Harvesting a single crop costs (1) energy.";
  this->SCENE_FARM_SELECT_TEXT[9] = "Harvested crops can be sold in the shop.";

  this->SCENE_FARM_SELECT_TEXT[10] = "Do nothing on the farm.";
  this->SCENE_FARM_SELECT_TEXT[11] = "You get to select another action to do.";

  // Visiting the shop
  this->SCENE_SHOP_SELECT_TEXT[0] = "Buy some seeds from the shop.";
  this->SCENE_SHOP_SELECT_TEXT[1] = "Bought seeds can be planted on the farm.";

  this->SCENE_SHOP_SELECT_TEXT[2] = "Sell harvested crops here.";
  this->SCENE_SHOP_SELECT_TEXT[3] = "Get some of that gold!";

  this->SCENE_SHOP_SELECT_TEXT[4] = "Do nothing on the shop.";
  this->SCENE_SHOP_SELECT_TEXT[5] = "You get to select another action.";

  //
  // How the game works
  //

  this->GUIDE_TEXT[0] = "HarvestSun a is farming game where the player earns money through the planting of     ";
  this->GUIDE_TEXT[1] = "crops. Although there are no criteria to grant the player any form of victory, the    ";
  this->GUIDE_TEXT[2] = "the goal of the game may well be to maximize the profits generated by the player.     ";
  this->GUIDE_TEXT[3] = "                                                                                      ";
  this->GUIDE_TEXT[4] = "The player starts off with 50 pieces of gold and 30 units of energy. In order to plant";
  this->GUIDE_TEXT[5] = "crops, the player has to expend energy, which can only be replenished by sleeping;    ";
  this->GUIDE_TEXT[6] = "however, the player must also eat breakfast at the start of each day, which costs them";
  this->GUIDE_TEXT[7] = "some gold. If the player fails to do so, they may eventually starve to death. The game";
  this->GUIDE_TEXT[8] = "requires you to find a balance of the actions of the player to allow them to survive. ";
  this->GUIDE_TEXT[9] = "                                                                                      ";
  this->GUIDE_TEXT[10] = "Although I cannot say the game is particularly challenging, finding the right sequence";
  this->GUIDE_TEXT[11] = "of actions to survive isn't necessarily a no-brainer either.                          ";

  //
  // The game controls
  //

  this->CONTROLS_TEXT[0] = "[Q]                   -=>     Quit the current scene.                    ";
  this->CONTROLS_TEXT[1] = "[E]                   -=>     Select an object to be modified.           ";
  this->CONTROLS_TEXT[2] = "[W], [A], [S], [D]    -=>     Used for selecting items in a grid layout. ";
  this->CONTROLS_TEXT[3] = "[X], [C]              -=>     Toggle options present in a selection.     ";
  this->CONTROLS_TEXT[4] = "[Enter]               -=>     Finalize an action / select an option.     ";

  //
  // About the author
  //

  this->AUTHOR_TEXT[0] = "Mo David is a code enthusiast who loves cats and dabbles in occasional music-making. ";
  this->AUTHOR_TEXT[1] = "He also writes poetry on the side amidst a chronic addiction to math. Nevertheless,  ";
  this->AUTHOR_TEXT[2] = "despite the prevailing stereotypes on tryhard programmers, Mo apparently does not    ";
  this->AUTHOR_TEXT[3] = "(usually) engage in the consumption of caffeinated beverages.                        ";

  //
  // Dividers
  //

  this->DIVIDER_TEXT[0] = "                    .=-=.=-=.=-=.=-=.=-=.=-=.=-=.=-=.=-=.=-=.=-=.=-=.=-=.=-=.=-=.                    ";

/**
 * #####################
 * ###  ASSET SIZES  ###
 * #####################
*/

  // Asset lengths
  this->TITLE_SPRITE_LEN = TITLE_SPRITE_LENGTH;
  this->WELCOME_SPRITE_LEN = WELCOME_SPRITE_LENGTH;
  this->PAUSE_SPRITE_LEN = PAUSE_SPRITE_LENGTH;
  this->GOVER_SPRITE_LEN = GOVER_SPRITE_LENGTH;
  this->WARNING_SPRITE_LEN = WARNING_SPRITE_LENGTH;

  this->SELECTING_SPRITE_LEN = SELECTING_SPRITE_LENGTH;
  this->HOME_SPRITE_LEN = HOME_SPRITE_LENGTH;
  this->FARM_SPRITE_LEN = FARM_SPRITE_LENGTH;
  this->SHOP_SPRITE_LEN = SHOP_SPRITE_LENGTH;
  this->INVENTORY_SPRITE_LEN = INVENTORY_SPRITE_LENGTH;
  this->CONTROLS_SPRITE_LEN = CONTROLS_SPRITE_LENGTH;
  this->AUTHOR_SPRITE_LEN = AUTHOR_SPRITE_LENGTH;

  this->DIALOGUE_TEXT_LEN = DIALOGUE_TEXT_LENGTH;
  this->SCENE_SELECT_TEXT_LEN = SCENE_SELECT_TEXT_LENGTH;
  this->SCENE_HOME_MESSAGE_TEXT_LEN = SCENE_HOME_MESSAGE_TEXT_LENGTH;
  this->GUIDE_TEXT_LEN = GUIDE_TEXT_LENGTH;
  this->CONTROLS_TEXT_LEN = CONTROLS_TEXT_LENGTH;
  this->AUTHOR_TEXT_LEN = AUTHOR_TEXT_LENGTH;
  this->DIVIDER_TEXT_LEN = DIVIDER_TEXT_LENGTH;
}

#endif