/**
 * Some enums that can help define plot states and possible farm actions.
*/

#ifndef GAME_ENUM_FARM
#define GAME_ENUM_FARM

/**
 * Helps manage the states of the plots on the farm grid.
*/
enum PlotState { 
  PLOT_UNTILLED, 
  PLOT_TILLED, 
  PLOT_SOWN,
};

/**
 * Specifies the actions possible on the farm.
*/
enum FarmAction {
  FARM_NULL,
  FARM_TILL,
  FARM_SOW,
  FARM_WATER,
  FARM_HARVEST,
  FARM_INSPECT,
};

#endif