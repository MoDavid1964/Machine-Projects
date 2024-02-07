/**
 * File that creates a farm object.
 * The farm consists of a grid represented by a one-dimensional array.
 * It also has a length and width, which for this project is 3 x 10 to fit the specification of 30 plots of land.
 * 
 * The UI and IO components of the farm in full mode are separated here.
 * However, the same is not true for the default mode as such components were minimal therein.
*/

#ifndef GAME_OBJ_FARM
#define GAME_OBJ_FARM

#include "../../utils/utils.key.h"
#include "../../utils/utils.text.h"
#include "../../utils/utils.selector.h"
#include "../../utils/utils.ui.h"

#include "../classes/game.class.plot.h"
#include "../classes/game.class.product.h"

#include "../enums/game.enum.farm.h"
#include "../enums/game.enum.state.h"

#include "../game.catalogue.h"

#define FARM_MAX_WIDTH 16
#define FARM_MAX_HEIGHT 16

/**
 * The farm object stores information about the plots and their states.
 * The farm is represented by a grid of plots.
*/
struct Farm {
  int dWidth;
  int dHeight;
  int dSize;

  int dSelectorX;
  int dSelectorY;
  int dModifiedPlots;
  int bIsSelecting;
  int bSelectionQueue[FARM_MAX_WIDTH * FARM_MAX_HEIGHT];

  char *sWarningText;
  char *sPastActionNameArray[6];
  char *sPresentActionNameArray[6];

  enum FarmAction eCurrentAction;
  enum ProductType eCurrentCrop;
  
  struct UtilsSelector *pFarmSelector;
  struct Plot *pPlotArray[FARM_MAX_WIDTH * FARM_MAX_HEIGHT];
  struct GameCatalogue *pCatalogue;
};

/**
 * ###########################
 * ###  FARM CONSTRUCTION  ###
 * ###########################
*/

/**
 * Creates a new farm object.
 * 
 * @return  {struct Farm *}   A pointer to the object.
*/
struct Farm *Farm_new() {
  struct Farm *pFarm;

  pFarm = calloc(1, sizeof(*pFarm));

  if(pFarm == NULL)
    return NULL;

  return pFarm;
}

/**
 * Initializes a farm object.
 * 
 * @param   {struct Farm *}           this        The farm object to be initialized.
 * @param   {int}                     dWidth      The width of the farm grid.
 * @param   {int}                     dHeight     The height of the farm grid.
 * @param   {struct GameCatalogue *}  pCatalogue  A reference for the crop info.
*/
void Farm_init(struct Farm *this, int dWidth, int dHeight, struct GameCatalogue *pCatalogue) {

  // Just to make sure nothing bad happens
  if(dWidth > FARM_MAX_WIDTH) dWidth = FARM_MAX_WIDTH;
  if(dHeight > FARM_MAX_HEIGHT) dHeight = FARM_MAX_HEIGHT;

  // Store the dimensions
  this->dWidth = dWidth;
  this->dHeight = dHeight;
  this->dSize = dWidth * dHeight;

  // Needed for the UI
  this->dSelectorX = 0;
  this->dSelectorY = 0;
  this->bIsSelecting = 0;
  this->dModifiedPlots = 0;

  this->eCurrentAction = FARM_NULL;
  this->eCurrentCrop = PRODUCT_NULL;

  this->sWarningText = UtilsUI_createBuffer();

  this->sPastActionNameArray[FARM_NULL] = "nothing";
  this->sPastActionNameArray[FARM_INSPECT] = "viewed";
  this->sPastActionNameArray[FARM_TILL] = "tilled";
  this->sPastActionNameArray[FARM_SOW] = "sown";
  this->sPastActionNameArray[FARM_WATER] = "watered";
  this->sPastActionNameArray[FARM_HARVEST] = "harvested";

  this->sPresentActionNameArray[FARM_NULL] = "nothing";
  this->sPresentActionNameArray[FARM_INSPECT] = "viewing";
  this->sPresentActionNameArray[FARM_TILL] = "tilling";
  this->sPresentActionNameArray[FARM_SOW] = "sowing";
  this->sPresentActionNameArray[FARM_WATER] = "watering";
  this->sPresentActionNameArray[FARM_HARVEST] = "harvesting";

  // Populate the grid with instances of Plot
  for(int i = 0; i < this->dSize; i++) {
    this->pPlotArray[i] = Plot_create();
    this->bSelectionQueue[i] = 0;
  }

  // Create the farm selector for choosing actions
  struct UtilsSelector *pFarmSelector = UtilsSelector_create(0, 
    "     [ ]  %s   ", 
    "     -=> [x]  %s       ", 
    "     [-]  %s   ");

  UtilsSelector_addOption(pFarmSelector, "view farm     ", FARM_INSPECT);
  UtilsSelector_addOption(pFarmSelector, "till plots    ", FARM_TILL);
  UtilsSelector_addOption(pFarmSelector, "sow seeds     ", FARM_SOW);
  UtilsSelector_addOption(pFarmSelector, "water crops   ", FARM_WATER);
  UtilsSelector_addOption(pFarmSelector, "harvests crops", FARM_HARVEST);
  UtilsSelector_addOption(pFarmSelector, "do nothing    ", FARM_NULL);

  this->pFarmSelector = pFarmSelector;
}

/**
 * Creates an initialized farm object.
 * 
 * @param   {int}                     dWidth      The width of the farm grid.
 * @param   {int}                     dHeight     The height of the farm grid.
 * @param   {struct GameCatalogue *}  pCatalogue  A reference for the plant info.
 * @return  {struct Farm *}                       A pointer to the created object.
*/
struct Farm *Farm_create(int dWidth, int dHeight, struct GameCatalogue *pCatalogue) {
  struct Farm *pFarm = Farm_new();

  Farm_init(pFarm, dWidth, dHeight, pCatalogue);

  return pFarm;
}

/**
 * Destroys a farm object.
 * 
 * @param   {struct Farm *}   A pointer to the object to be destroyed.
*/
void Farm_kill(struct Farm *this) {
  free(this);
}

/**
 * ##################################
 * ###  FARM READERS AND WRITERS  ###
 * ##################################
*/

/**
 * Returns the x coordinate of the selector.
 * 
 * @param   {struct Farm *}     this  The farm object.
 * @return  {int}                     The x coordinate of the cursor.
*/
int Farm_getSelectorX(struct Farm *this) {
  return this->dSelectorX;
}

/**
 * Returns the y coordinate of the selector.
 * 
 * @param   {struct Farm *}     this  The farm object.
 * @return  {int}                     The y coordinate of the cursor.
*/
int Farm_getSelectorY(struct Farm *this) {
  return this->dSelectorY;
}

/**
 * Returns the information of the currently selected plot.
 * 
 * @param   {struct Farm *}     this  The farm object.
 * @return  {struct Plot *}           The plot selected by the cursor.
*/
struct Plot *Farm_getCurrentPlot(struct Farm *this) {
  return this->pPlotArray[this->dSelectorY * this->dWidth + this->dSelectorX];
}

/**
 * Returns whether or not the currently selected plot has been queued for modification.
 * 
 * @param   {struct Farm *}     this  The farm object.
 * @return  {int}                     A boolean indicating whether or not the plot has been queued.
*/
int Farm_getCurrentQueueStatus(struct Farm *this) {
  return this->bSelectionQueue[this->dSelectorY * this->dWidth + this->dSelectorX];
}

/**
 * Returns the current action to be done on the farm.
 * 
 * @param   {struct Farm *}     this  The farm object.
 * @return  {enum FarmAction}         The current action to be performed on the farm.
*/
enum FarmAction Farm_getCurrentAction(struct Farm *this) {
  return this->eCurrentAction;
}

/**
 * Sets the current action on the farm.
 * 
 * @param   {struct Farm *}     this          The farm object.
 * @param   {enum FarmAction}   eFarmAction   The new farm action. 
*/
void Farm_setCurrentAction(struct Farm *this, enum FarmAction eFarmAction) {
  this->eCurrentAction = eFarmAction;
}

/**
 * Returns the current crop to be used on the farm.
 * 
 * @param   {struct Farm *}     this  The farm object.
 * @return  {enum ProductType}        The type of crop to be used on the farm.
*/
enum ProductType Farm_getCurrentCrop(struct Farm *this) {
  return this->eCurrentCrop;
}

/**
 * Sets the current crop on the farm.
 * 
 * @param   {struct Farm *}     this          The farm object.
 * @param   {enum ProductType}  eProductType  The new farm crop for use. 
*/
void Farm_setCurrentCrop(struct Farm *this, enum ProductType eProductType) {
  this->eCurrentCrop = eProductType;
}

/**
 * Returns whether or not there exist plots that can be tilled.
 * 
 * @param   {struct Farm *}     this          The farm object.
 * @return  {int}                             The number of plots that may be tilled.
*/
int Farm_canTill(struct Farm *this) {
  int dTillable = 0;
  
  for(int i = 0; i < this->dSize; i++)
    if(Plot_getState(this->pPlotArray[i]) == PLOT_UNTILLED)
      dTillable++;

  return dTillable;
}

/**
 * Returns whether or not there exist plots that can be sown.
 * 
 * @param   {struct Farm *}     this          The farm object.
 * @return  {int}                             The number of plots that may be sown.
*/
int Farm_canSow(struct Farm *this) {
  int dCanBeSown = 0;
  
  for(int i = 0; i < this->dSize; i++)
    if(Plot_getState(this->pPlotArray[i]) == PLOT_TILLED)
      dCanBeSown++;

  return dCanBeSown;
}

/**
 * Returns whether or not there exist crops that can be watered.
 * 
 * @param   {struct Farm *}     this    The farm object.
 * @param   {int}               dTime   The time when the action is being executed.
 * @return  {int}                       The number of plots that may be watered.
*/
int Farm_canWater(struct Farm *this, int dTime) {
  int dWaterable = 0;
  
  for(int i = 0; i < this->dSize; i++)
    if(Plot_getState(this->pPlotArray[i]) == PLOT_SOWN)
      if(Plot_getProductState(this->pPlotArray[i]) < 2)
        if(Plot_getProductLastWatered(this->pPlotArray[i]) < dTime)
          dWaterable++;

  return dWaterable;
}

/**
 * Returns whether or not there exist crops of the current type that can be watered.
 * 
 * @param   {struct Farm *}     this          The farm object.
 * @param   {int}               dTime         The time when the action is being executed.
 * @return  {int}                             The number of plots that may be watered.
*/
int Farm_canWaterCrop(struct Farm *this, int dTime) {
  int dWaterable = 0;
  
  for(int i = 0; i < this->dSize; i++)
    if(Plot_getState(this->pPlotArray[i]) == PLOT_SOWN && Plot_getProductType(this->pPlotArray[i]) == this->eCurrentCrop)
      if(Plot_getProductState(this->pPlotArray[i]) < 2)
        if(Plot_getProductLastWatered(this->pPlotArray[i]) < dTime)
          dWaterable++;

  return dWaterable;
}

/**
 * Returns whether or not there exist plots that can be harvested.
 * 
 * @param   {struct Farm *}     this          The farm object.
 * @return  {int}                             The number of plots that may be harvested.
*/
int Farm_canHarvest(struct Farm *this) {
  int dHarvestable = 0;
  
  for(int i = 0; i < this->dSize; i++)
    if(Plot_getState(this->pPlotArray[i]) == PLOT_SOWN)
      if(Plot_getProductState(this->pPlotArray[i]) == 2)
        dHarvestable++;

  return dHarvestable;
}

/**
 * Returns whether or not there exist plots with crops of the current type that can be harvested.
 * 
 * @param   {struct Farm *}     this  The farm object.
 * @return  {int}                     The number of plots that may be harvested.
*/
int Farm_canHarvestCrop(struct Farm *this) {
  int dHarvestable = 0;
  
  for(int i = 0; i < this->dSize; i++)
    if(Plot_getState(this->pPlotArray[i]) == PLOT_SOWN && Plot_getProductType(this->pPlotArray[i]) == this->eCurrentCrop)
      if(Plot_getProductState(this->pPlotArray[i]) == 2)
        dHarvestable++;

  return dHarvestable;
}

/**
 * Tills the specified number of plots on the farm.
 * The function does nothing if the specified number of plots cannot be tilled.
 * 
 * @param   {struct Farm *}     this    The farm object.
 * @param   {int}               dPlots  The number of plots to till.
*/
void Farm_till(struct Farm *this, int dPlots) {
  if(dPlots <= Farm_canTill(this)) {
    for(int i = 0; i < this->dSize && dPlots > 0; i++) {
      if(Plot_getState(this->pPlotArray[i]) == PLOT_UNTILLED) {
        Plot_till(this->pPlotArray[i]);
        dPlots--;
      }
    }
  } 
}

/**
 * Sows the specified number of plots on the farm.
 * The function does nothing if there are too many plots to sow.
 * 
 * @param   {struct Farm *}           this        The farm object.
 * @param   {int}                     dPlots      The number of plots to sow.
 * @param   {struct GameCatalogue *}  pCatalogue  The type of crop to sow.
 * @param   {int}                     dTime       When the action was executed.
*/
void Farm_sow(struct Farm *this, int dPlots, struct GameCatalogue *pCatalogue, int dTime) {
  if(dPlots <= Farm_canSow(this)) {
    for(int i = 0; i < this->dSize && dPlots > 0; i++) {
      if(Plot_getState(this->pPlotArray[i]) == PLOT_TILLED) {
        struct Product *pProduct = Product_create(this->eCurrentCrop, pCatalogue, dTime);

        Plot_sow(this->pPlotArray[i], pProduct);
        dPlots--;
      }
    }
  } 
}

/**
 * Waters the currently selected crop on the farm.
 * 
 * @param   {struct Farm *}   this    The farm object.
 * @param   {int}             dTime   When the action was executed.
*/
void Farm_water(struct Farm *this, int dTime) {
  for(int i = 0; i < this->dSize; i++) {
    if(Plot_getState(this->pPlotArray[i]) == PLOT_SOWN && 
      Plot_getProductType(this->pPlotArray[i]) == this->eCurrentCrop) {
      
      if(Plot_getProductLastWatered(this->pPlotArray[i]) < dTime)
        Plot_water(this->pPlotArray[i], dTime);
    }
  } 
}

/**
 * Harvests the currently selected crop on the farm.
 * 
 * @param   {struct Farm *}   this  The farm object.
*/
void Farm_harvest(struct Farm *this) {
  for(int i = 0; i < this->dSize; i++) {
    if(Plot_getState(this->pPlotArray[i]) == PLOT_SOWN && 
      Plot_getProductType(this->pPlotArray[i]) == this->eCurrentCrop) {

      if(Plot_getProductState(this->pPlotArray[i]) == 2)
        Plot_harvest(this->pPlotArray[i]);
    }
  }
}

/**
 * ###############################
 * ###  FARM HELPER FUNCTIONS  ###
 * ###############################
*/

/**
 * Enables selecting plots.
 * 
 * @param   {struct Farm *this}   The farm object we're modifying.
*/
void Farm_startSelecting(struct Farm *this) {
  this->dModifiedPlots = 0;
  this->bIsSelecting = 1;
}

/**
 * Disables selecting plots.
 * 
 * @param   {struct Farm *this}   The farm object we're modifying.
*/
void Farm_stopSelecting(struct Farm *this) {
  
  // Store how many plots were modified
  for(int i = 0; i < this->dSize; i++)
    if(this->bSelectionQueue[i])
      this->dModifiedPlots++;

  this->bIsSelecting = 0;
}

/**
 * Moves the selector to the right.
 * 
 * @param   {struct Farm *this}   The farm object we're modifying.
*/
void Farm_incrementX(struct Farm *this) {
  if(this->bIsSelecting) 
    this->dSelectorX = (this->dSelectorX + 1) % this->dWidth;
}

/**
 * Moves the selector to the left.
 * 
 * @param   {struct Farm *this}   The farm object we're modifying.
*/
void Farm_decrementX(struct Farm *this) {
  if(this->bIsSelecting)
    this->dSelectorX = (this->dSelectorX - 1 + this->dWidth) % this->dWidth;
}

/**
 * Moves the selector a line down.
 * 
 * @param   {struct Farm *this}   The farm object we're modifying.
*/
void Farm_incrementY(struct Farm *this) {
  if(this->bIsSelecting)
    this->dSelectorY = (this->dSelectorY + 1) % this->dHeight;
}

/**
 * Moves the selector a line up.
 * 
 * @param   {struct Farm *this}   The farm object we're modifying.
*/
void Farm_decrementY(struct Farm *this) {
  if(this->bIsSelecting)
    this->dSelectorY = (this->dSelectorY - 1 + this->dHeight) % this->dHeight;
}

/**
 * Returns the length of the queue.
 * 
 * @param   {struct Farm *}   this  The farm object.
 * @return  {int}                   The length of the farm object queue.
*/
int Farm_getQueueLength(struct Farm* this) {
  int dLength = 0;

  for(int i = 0; i < this->dSize; i++)
    dLength += this->bSelectionQueue[i] ? 1 : 0;

  return dLength;
}

/**
 * Adds the currently selected plot to the queue.
 * 
 * @param   {struct Farm *}   this    The farm object we're modifying.
 * @param   {int}             dTime   When we're doing the action.
*/
void Farm_queueSelected(struct Farm *this, int dTime) {
  int dIndex = this->dSelectorY * this->dWidth + this->dSelectorX;
  struct Plot *pPlot = this->pPlotArray[dIndex];

  if(this->bIsSelecting) {
    
    // Only allow untilled plots to be tilled, 
    // tilled plots to be sown, and 
    // fully grown plants to be harvested
    switch(this->eCurrentAction) {
      case FARM_TILL:
        if(Plot_getState(pPlot) == PLOT_UNTILLED)
          this->bSelectionQueue[dIndex] = 1;
        break;

      case FARM_SOW:
        if(Plot_getState(pPlot) == PLOT_TILLED)
          this->bSelectionQueue[dIndex] = 1;
        break;

      case FARM_WATER:
        if(Plot_getState(pPlot) == PLOT_SOWN)
          if(Plot_getProductState(pPlot) < 2)
            if(Plot_getProductLastWatered(pPlot) < dTime)
              this->bSelectionQueue[dIndex] = 1;
        break;

      case FARM_HARVEST:
        if(Plot_getState(pPlot) == PLOT_SOWN)
          if(Plot_getProductState(pPlot) == 2)
            this->bSelectionQueue[dIndex] = 1;
        break;

      default: break;
    }
  }
}

/**
 * Removes the currently selected plot from the queue.
 * 
 * @param   {struct Farm *this}   The farm object we're modifying.
*/
void Farm_unqueueSelected(struct Farm *this) {
  int dIndex = this->dSelectorY * this->dWidth + this->dSelectorX;

  if(this->bIsSelecting)
    this->bSelectionQueue[dIndex] = 0;
}

/**
 * Toggles the truth value of the currently selected plot in the selection array.
 * 
 * @param   {struct Farm *this}   The farm object we're modifying.
*/
void Farm_toggleSelected(struct Farm *this, int dTime) {
  int dIndex = this->dSelectorY * this->dWidth + this->dSelectorX;

  // Instead of just using !=, this saves us from repeating the conditional check in Farm_queueSelected()
  if(this->bIsSelecting) {
    if(!this->bSelectionQueue[dIndex])
      Farm_queueSelected(this, dTime);
    else 
      Farm_unqueueSelected(this);
  }
}

/**
 * Empties the selection queue of the farm object.
 * 
 * @param   {struct Farm *this}   The farm object we're modifying.
*/
void Farm_clearQueue(struct Farm *this) {
  for(int i = 0; i < this->dSize; i++)
    this->bSelectionQueue[i] = 0;
}

/**
 * Performs the current action on the queued plots.
 * 
 * @param   {struct Farm *}           this        The farm object we're modifying.
 * @param   {struct Player *}         pPlayer     The player doing the action.
 * @param   {int}                     dTime       When the action is being done.
 * @param   {struct GameCatalogue *}  pCatalogue  A reference for the different product info.
 * @return  {int}                                 Returns whether or not the processing of the queue was a success.
*/
int Farm_processQueue(struct Farm *this, struct Player *pPlayer, struct GameCatalogue *pCatalogue, int dTime) {
  int bSuccess = 1;
  strcpy(this->sWarningText, "");

  switch(this->eCurrentAction) {

    // We till the plots
    case FARM_TILL:
      if(Player_tillPlots(pPlayer, Farm_getQueueLength(this))) {
        for(int i = 0; i < this->dSize; i++)
          if(this->bSelectionQueue[i]) 
            Plot_till(this->pPlotArray[i]);   

      // The player did not have enough energy to till the plots
      } else {
        bSuccess = 0;
        strcpy(this->sWarningText, "You do not have enough energy to till those plots.");
      }
      
      break;
    
    // Plant something on the plots
    case FARM_SOW:
      if(Player_sowSeeds(pPlayer, this->eCurrentCrop, Farm_getQueueLength(this))) {
        for(int i = 0; i < this->dSize; i++)
          if(this->bSelectionQueue[i]) 
            Plot_sow(this->pPlotArray[i], Product_create(this->eCurrentCrop, pCatalogue, dTime));
      
      // The player did not have enough seeds or energy to sow those plots.
      } else {
        bSuccess = 0;
        strcpy(this->sWarningText, "You do not have enough seeds or energy to sow those plots.");
      }
      break;

    // Give the crops water
    case FARM_WATER:
      if(Player_waterCrops(pPlayer, Farm_getQueueLength(this))) {
        for(int i = 0; i < this->dSize; i++)
          if(this->bSelectionQueue[i]) 
            Plot_water(this->pPlotArray[i], dTime);
      
      // The player did not have enough energy to water those plots.
      } else {
        bSuccess = 0;
        strcpy(this->sWarningText, "You do not have enough energy to water these plots.");
      }

      // Plot_water(this->pPlotArray[i], dTime);
      break;

    // Harvest the crops from the farm
    case FARM_HARVEST:
      if(Player_harvestCrops(pPlayer, Farm_getQueueLength(this))) {
        for(int i = 0; i < this->dSize; i++)
          if(this->bSelectionQueue[i]) {
            Player_harvestACrop(pPlayer, this->pPlotArray[i]->pProduct->eType);
            Plot_harvest(this->pPlotArray[i]);
          }
      } else {
        bSuccess = 0;
        strcpy(this->sWarningText, "You do not have enough energy to harvest these plots.");
      }
      break;

    default: break;
  }

  // Stop selection process
  Farm_stopSelecting(this);
  Farm_clearQueue(this);

  this->dModifiedPlots *= bSuccess;
  return bSuccess;
}

/**
 * #######################
 * ###  FARM UI AND IO ###
 * #######################
*/

/**
 * A helper function that creates a text array that represents the farm grid.
 * 
 * @param   {struct Farm *}       this    The farm object.
 * @param   {int}                 dTime   When the farm is being displayed.
 * @return  {struct UtilsText *}          An output representing the grid UI of the farm. 
*/
struct UtilsText *Farm_displayGrid(struct Farm *this, int dTime) {
  struct UtilsText *pOutput = UtilsText_create();
  int dWidth = this->dWidth;
  int dHeight = this->dHeight;

  // Generate the entire grid
  for(int i = 0; i < dHeight; i++) {
    char *sRow;

    // Top lines of text
    if(!i) {

      // Selector utility
      char *sTopRow1 = calloc(6 * dWidth + 11, sizeof(char));
      for(int j = 0; j < dWidth; j++) 
        if(this->dSelectorX == j && this->bIsSelecting) 
          strcat(sTopRow1, "   v  ");
        else 
          strcat(sTopRow1, "      ");

      strcat(sTopRow1, " ");  
      UtilsText_addText(pOutput, sTopRow1);

      // Top row
      char *sTopRow2 = calloc(6 * dWidth + 1, sizeof(char));
      for(int j = 0; j < dWidth; j++) 
        strcat(sTopRow2, "._____");
      
      strcat(sTopRow2, "."); 
      UtilsText_addText(pOutput, sTopRow2);
    }

    // Generate the row
    for(int j = 0; j < 3; j++) {
      sRow = calloc(6 * dWidth + 11, sizeof(char));

      for(int k = 0; k < dWidth; k++) {
        int dIndex = i * dWidth + k;

        if(!k) {
          if(this->dSelectorY == i && !(j - 1) && this->bIsSelecting) 
            strcat(sRow, ">  |");
          else 
            strcat(sRow, "   |");
        
        } else {
          strcat(sRow, "|");
        }

        // Yes this is necessary!! for an awesome UI
        // Looks like a "switch case" hell tho,,
        // There's too much nesting...
        // PS I know this could've been coded in a much more concise manner, BUT
        // I wanted to be able to visualize the plot icons in the code itself.
        
        // If the plot is selected while selection is enabled
        if(k == this->dSelectorX && i == this->dSelectorY && this->bIsSelecting) {
          
          // The selector is alread on a selected plot
          if(this->bSelectionQueue[dIndex]) {
            switch(j) {
              case 0: strcat(sRow, "`. .`"); break;
              case 1: strcat(sRow, " .0. "); break;
              case 2: strcat(sRow, "`___`"); break;
            }

          // The selector is on an unselected plot
          } else {

            // No plant on it
            if(this->pPlotArray[dIndex]->eState != PLOT_SOWN) {
              switch(j) {
                case 0: strcat(sRow, "`. .`"); break;
                case 1: strcat(sRow, " .'. "); break;
                case 2: strcat(sRow, "`___`"); break;
              }
            
            // There's a plant on it
            } else {
              char *sProductCode = Plot_getProductCode(this->pPlotArray[dIndex]);

              switch(j) {
                case 0: strcat(sRow, "`. .`"); break;
                case 1: strcat(sRow, " ."); strcat(sRow, sProductCode); strcat(sRow, ". "); break;
                case 2: strcat(sRow, "`___`"); break;
              }
            }
          }

        // If the plot is queued for an action.
        } else if(this->bSelectionQueue[dIndex]) {
          
          // If there's no plant on the plot
          if(this->pPlotArray[dIndex]->eState != PLOT_SOWN) {
            switch(j) {
              case 0: strcat(sRow, "     "); break;
              case 1: strcat(sRow, " (0) "); break;
              case 2: strcat(sRow, "_____"); break;
            }

          // There is a plant on the plot
          } else {
            char *sProductCode = Plot_getProductCode(this->pPlotArray[dIndex]);
            int dProductState = Plot_getProductState(this->pPlotArray[dIndex]);  
            
            // Queueing for harvest
            if(dProductState == 2) {
              switch(j) {
                case 0: strcat(sRow, sProductCode); 
                        strcat(sRow,  "   $"); break;
                case 1: strcat(sRow, " (0) "); break;
                case 2: strcat(sRow, "____"); strcat(sRow, sProductCode); break;
              }

            // Queueing for watering
            } else {
              switch(j) {
                case 0: strcat(sRow, sProductCode); 
                        strcat(sRow,  "    "); break;
                case 1: strcat(sRow, " (0) "); break;
                case 2: strcat(sRow, "____"); strcat(sRow, sProductCode); break;
              }
            }
          }

        // If there's a plant on the plot
        } else if(this->pPlotArray[dIndex]->eState == PLOT_SOWN) {

          // Some variables to make life easier
          char *sProductCode = Plot_getProductCode(this->pPlotArray[dIndex]);
          int dProductState = Plot_getProductState(this->pPlotArray[dIndex]);
          int dProductLastWatered = Plot_getProductLastWatered(this->pPlotArray[dIndex]);

          // Print different stuff for different growth stages
          switch(dProductState) {
            
            // The crop has yet to grow much
            case 0:
              switch(j) {
                case 0: strcat(sRow, sProductCode); 
                        strcat(sRow,  "   ");  strcat(sRow, dProductLastWatered < dTime ? " " : "!"); break;
                case 1: strcat(sRow, " _._ "); break;
                case 2: strcat(sRow, "____"); strcat(sRow, sProductCode); break;
              }
              break;

            // The plant is halfway from being ready to harvest
            case 1:
              switch(j) {
                case 0: strcat(sRow, sProductCode); 
                        strcat(sRow,  " , ");  strcat(sRow, dProductLastWatered < dTime ? " " : "!"); break;
                case 1: strcat(sRow, " _|_ "); break;
                case 2: strcat(sRow, "____"); strcat(sRow, sProductCode); break;
              }
              break;

            // The crop is halfway from being ready to harvest
            case 2:
              switch(j) {
                case 0: strcat(sRow, sProductCode); 
                        strcat(sRow,  " # $");  break;
                case 1: strcat(sRow, " _|_ "); break;
                case 2: strcat(sRow, "____"); strcat(sRow, sProductCode); break;
              }
              break;
          }

        // The plot is empty
        } else {

          // The plot is not tilled
          if(this->pPlotArray[dIndex]->eState == PLOT_UNTILLED) {
            switch(j) {
              case 0: strcat(sRow, "     "); break;
              case 1: strcat(sRow, "     "); break;
              case 2: strcat(sRow, "____'"); break;
            }

          // The plot is tilled
          } else if(this->pPlotArray[dIndex]->eState == PLOT_TILLED) {
            switch(j) {
              case 0: strcat(sRow, "'    "); break;
              case 1: strcat(sRow, " ^^^ "); break;
              case 2: strcat(sRow, "____'"); break;
            }
          }
        }
      }

      strcat(sRow, "|   ");
      UtilsText_addText(pOutput, sRow);
    }
  }

  return pOutput;
}

/**
 * Displays the UI of the farm.
 * 
 * @param   {struc Farm *}            this                  The farm object whose information we're using.
 * @param   {struct Player *}         pPlayer               The player object whose information we're using.
 * @param   {struct UtilsText *}      pScreenText           The text object we're the display will be placed.
 * @param   {struct UtilsSelector *}  pCatalogueSelector    A selector for all the game products.
 * @param   {struct GameCatalogue *}  pCatalogue            A list of all the products available in the game.
 * @param   {char **}                 sFarmSprite           The farm ASCII art.
 * @param   {int}                     dFarmSpriteSize       The size of the ASCII art.
 * @param   {char *}                  sCurrentIntInput      Current user input needed for some stuff in the game.
 * @param   {char *}                  sInputWarning         A warning for the user input in case its not formatted right.
*/
void Farm_UI(
  struct Farm *this,
  struct Player *pPlayer,
  struct UtilsText *pScreenText,  
  struct UtilsSelector *pCatalogueSelector,

  struct GameCatalogue *pCatalogue,
  char **sFarmSprite, 
  int dFarmSriteSize, 
  
  char *sCurrentIntInput, 
  char *sInputWarning) {
  
  // The player is currently selecting an action to do on the farm
  if(this->eCurrentAction == FARM_NULL) {
    if(Player_getEnergy(pPlayer)) {
      
      // Display the farm barn
      UtilsText_addNewLines(pScreenText, 1);
      UtilsText_addText(pScreenText, "You are now at the farm.");
      UtilsText_addNewLines(pScreenText, 1);
      UtilsText_addBlock(pScreenText, sFarmSprite, dFarmSriteSize);
      UtilsText_addNewLines(pScreenText, 2);
      UtilsText_addText(pScreenText, "What do you want to do?");
      UtilsText_addNewLines(pScreenText, 1);

      // Display farm action selector
      for(int i = 0; i < UtilsSelector_getLength(this->pFarmSelector); i++)
        UtilsText_addText(pScreenText, UtilsSelector_getOptionFormatted(this->pFarmSelector, i));

    // The player has no more energy
    } else {
      UtilsText_addText(pScreenText, "You currently have no energy to do anything on the farm.");
      UtilsText_addNewLines(pScreenText, 1);
      UtilsText_addText(pScreenText, "Maybe you'd better get some rest at home?");
      UtilsText_addNewLines(pScreenText, 1);
      UtilsText_addText(pScreenText, "[Enter] to continue doing something else.");
    } 

  // There is already an action to be done
  } else {
    
    // Just inspect the farm
    if(this->eCurrentAction == FARM_INSPECT) {

      UtilsText_addText(pScreenText, "You are currently inspecting the farm.");
      UtilsText_addNewLines(pScreenText, 1);
      UtilsText_mergeText(pScreenText, Farm_displayGrid(this, Player_getTime(pPlayer)));
      UtilsText_addNewLines(pScreenText, 4);
      UtilsText_addText(pScreenText, "[W], [A], [S], [D] to move.");
      UtilsText_addText(pScreenText, "[Enter] to go back and do another farm thing.");

    // Farm grid display with selection
    } else if(this->eCurrentAction != FARM_SOW || this->eCurrentCrop != PRODUCT_NULL) {
      if(!this->bIsSelecting)
        UtilsText_addNewLines(pScreenText, 1);

      // Before executing the action
      if(this->bIsSelecting) {
        int bActionAvailable = 0;
        char *sActionInfo = UtilsUI_createBuffer();
        char *sSowingInfo = UtilsUI_createBuffer();

        // Determine whether the action can be done
        switch(this->eCurrentAction) {
          case FARM_TILL: bActionAvailable = Farm_canTill(this); break;
          case FARM_SOW: bActionAvailable = Farm_canSow(this); break;
          case FARM_WATER: bActionAvailable = Farm_canWater(this, Player_getTime(pPlayer)); break;
          case FARM_HARVEST: bActionAvailable = Farm_canHarvest(this); break;
          default: break;
        }

        // If there are plots that can be modified, display the grid
        if(bActionAvailable) {
          sprintf(sActionInfo, "(%d) plots are selected for (%s).", Farm_getQueueLength(this),
            UtilsUI_toUpper(this->sPresentActionNameArray[UtilsSelector_getCurrentValue(this->pFarmSelector)])); 
          UtilsText_addNewLines(pScreenText, 1);
          UtilsText_addText(pScreenText, 
            strlen(this->sWarningText) && !Farm_getQueueLength(this) ? this->sWarningText : sActionInfo);

          // Display available seeds when sowing
          if(this->eCurrentAction == FARM_SOW) {
            sprintf(sSowingInfo, "You have (%d) available (%s) seeds.", 
              Stock_getAmount(Player_getSeedStock(pPlayer, this->eCurrentCrop)),
              UtilsUI_toUpper(pCatalogue->sProductNameArray[this->eCurrentCrop]));
            UtilsText_addText(pScreenText, sSowingInfo);
          } else {
            UtilsText_addNewLines(pScreenText, 1);
          }

          UtilsText_mergeText(pScreenText, Farm_displayGrid(this, Player_getTime(pPlayer)));
          UtilsText_addNewLines(pScreenText, 2);

          if(!strlen(this->sWarningText)) {
            UtilsText_addText(pScreenText, "Selected plots are marked with a (0).");
            
            switch(this->eCurrentAction) {
              case FARM_TILL: UtilsText_addText(pScreenText, "Only empty plots may be tilled."); break;
              case FARM_SOW: UtilsText_addText(pScreenText, "Only tilled plots may be sown."); break;
              case FARM_WATER: UtilsText_addText(pScreenText, "Crops marked with a (!) cannot be watered until the next day."); break;
              case FARM_HARVEST: UtilsText_addText(pScreenText, "Only crops marked with a ($) may be harvested."); break;
              default: break;
            }
            
            UtilsText_addNewLines(pScreenText, 1);
            UtilsText_addText(pScreenText, "[W], [A], [S], [D] to move; [E] to select/deselect plot/s.");
            UtilsText_addText(pScreenText, "[Enter] performs the action on all selected plots.");
            UtilsText_addNewLines(pScreenText, 1);
          } else {
            UtilsText_addText(pScreenText, "Try selecting plots again");
            UtilsText_addNewLines(pScreenText, 1);
            UtilsText_addText(pScreenText, "Hit [Enter] without selecting any plots to exit.");
            UtilsText_addNewLines(pScreenText, 3);
          }
        
        // There are no plots that can be modified
        } else {
          UtilsText_addNewLines(pScreenText, 1);
          sprintf(sActionInfo, "No plots are available for %s.", 
            this->sPresentActionNameArray[UtilsSelector_getCurrentValue(this->pFarmSelector)]);
          UtilsText_addText(pScreenText, sActionInfo);
          UtilsText_addNewLines(pScreenText, 1);
          UtilsText_addText(pScreenText, "Do something else for now.");
          UtilsText_addNewLines(pScreenText, 1);
          UtilsText_addText(pScreenText, "[Enter] to continue doing something else.");
          Farm_stopSelecting(this);
        }

      // The action has been executed
      } else {

        UtilsText_addNewLines(pScreenText, 4);
        UtilsText_mergeText(pScreenText, Farm_displayGrid(this, Player_getTime(pPlayer)));
        UtilsText_addNewLines(pScreenText, 2);
        
        // Feedback on the action that was performed
        char *sActionFeedback = UtilsUI_createBuffer();
        if(this->dModifiedPlots) {

          sprintf(sActionFeedback, "You've (%s) a total of (%d) plot/s.", 
            UtilsUI_toUpper(this->sPastActionNameArray[UtilsSelector_getCurrentValue(this->pFarmSelector)]),
            this->dModifiedPlots);
          UtilsText_addText(pScreenText, sActionFeedback);

          switch(this->eCurrentAction) {
            case FARM_TILL: 
              UtilsText_addNewLines(pScreenText, 1);
              UtilsText_addText(pScreenText, "(TILLED) plots are marked with a (^^^)."); 
              break;
            
            case FARM_SOW: 
              UtilsText_addNewLines(pScreenText, 1);
              UtilsText_addText(pScreenText, "(SOWN) plots are marked with a (_._) and their crop code."); 
              break;
            
            case FARM_WATER: 
              UtilsText_addText(pScreenText, "                                             ,          #   ");
              UtilsText_addText(pScreenText, "Crop growth stages are indicated by (_._), (_|_), and (_|_)."); 
              break;
            
            case FARM_HARVEST: 
              UtilsText_addNewLines(pScreenText, 1);
              UtilsText_addText(pScreenText, "(HARVESTED) plots will once again become (UNTILLED)."); 
              break;

            default: break;
          }

        // No plots were modified
        } else {
          sprintf(sActionFeedback, "NO (0) plots were %s.", 
            this->sPastActionNameArray[UtilsSelector_getCurrentValue(this->pFarmSelector)]);
          UtilsText_addText(pScreenText, sActionFeedback);
          UtilsText_addNewLines(pScreenText, 2);
        }

        UtilsText_addNewLines(pScreenText, 1);  
        UtilsText_addText(pScreenText, "[Enter] to continue and do something else.");
        UtilsText_addNewLines(pScreenText, 3);  
      }

      UtilsText_addNewLines(pScreenText, 1);
    
    // Select a product to use on the farm when sowing
    } else {

      // Reset the selector
      UtilsSelector_setAllAvailability(pCatalogueSelector, 0);

      // Prompt and options
      for(int i = 1; i < pCatalogue->dSize; i++) 
        if(Stock_getAmount(Player_getSeedStock(pPlayer, i)))
          UtilsSelector_setOptionAvailability(pCatalogueSelector, i, 1);
      UtilsSelector_setOptionAvailability(pCatalogueSelector, pCatalogue->dSize, 1);
      UtilsSelector_setFirstAvailable(pCatalogueSelector);

      // Player can sow seeds
      if(UtilsSelector_getAvailableCount(pCatalogueSelector) - 1 && Farm_canSow(this)) {
        
        // For some reason you need an odd number of lines or else the UI glitches out.
        UtilsText_addText(pScreenText, "Choose a seed type to plant."); 
        UtilsText_addNewLines(pScreenText, 1);

        // Print the options
        for(int i = 1; i <= pCatalogue->dSize; i++) 
          UtilsText_addText(pScreenText, UtilsSelector_getOptionFormatted(pCatalogueSelector, i));

      // Player cannot sow seeds
      } else {
        if(UtilsSelector_getAvailableCount(pCatalogueSelector) <= 1) {
          UtilsText_addText(pScreenText, "You currently do not have any seeds.");
          UtilsText_addNewLines(pScreenText, 1);
          UtilsText_addText(pScreenText, "Go to the shop first to buy some!");
        } else if(!Farm_canSow(this)) {
          UtilsText_addText(pScreenText, "There are currently no plots where you can sow seeds.");
          UtilsText_addNewLines(pScreenText, 1);
          UtilsText_addText(pScreenText, "Maybe try tilling some or wait for a plot to become vacant.");
        }

        UtilsText_addNewLines(pScreenText, 1);
        UtilsText_addText(pScreenText, "[Enter] to continue doing something else.");
        Farm_stopSelecting(this);
      }
    }
  }
}

/**
 * Handles the interaction with the farm object.
 * 
 * @param   {struct Farm *}           this                  The farm object.
 * @param   {struct Player *}         pPlayer               The player object whose information we're using.
 * @param   {struct UtilsSelector *}  pCatalogueSelector    A selector for all the game products.
 * @param   {struct GameCatalogue *}  pCatalogue            A reference to the info on the different game products.
 * @param   {char}                    cInput                The current key input of the user.
 * @param   {char *}                  sCurrentIntInput      The current integer input of the user.
 * @param   {char *}                  sInputWarning         A warning for the user input in case its not formatted right.
 * @param   {enum PlayState *}        pPlayState            A reference to the current PlayState.
 * @param   {enum GameState *}        pGameState            A reference to the current GameState.
*/
void Farm_IO(
  struct Farm *this, 
  struct Player *pPlayer,
  struct UtilsSelector *pCatalogueSelector,
  struct GameCatalogue *pCatalogue,
  
  char cInput, 
  char *sCurrentIntInput, 
  char *sInputWarning, 
  
  enum PlayState *pPlayState,
  enum GameState *pGameState) {
  
  // Choose actions first
  if(this->eCurrentAction == FARM_NULL) {
    if(cInput == 'X') UtilsSelector_decrement(this->pFarmSelector);
    if(cInput == 'C') UtilsSelector_increment(this->pFarmSelector);

    // If player hits enter
    if(UtilsKey_isReturn(cInput, "")) {
      if(Player_getEnergy(pPlayer)) {
        this->eCurrentAction = UtilsSelector_getCurrentValue(this->pFarmSelector);
        
        // Return to main selection area
        if(UtilsSelector_getCurrentValue(this->pFarmSelector) == FARM_NULL)
          *pPlayState = PLAY_SELECTING;
        else 
          Farm_startSelecting(this);
        
      // Player does not have energy
      } else {
        *pPlayState = PLAY_SELECTING;
      }
    }

  // If there's already a selected action, we choose plots to modify
  } else {
    
    // Grid movement
    if(this->eCurrentAction != FARM_SOW || 
      this->eCurrentCrop != PRODUCT_NULL) {

      // Farm grid movement
      if(cInput == 'W') Farm_decrementY(this);
      if(cInput == 'A') Farm_decrementX(this);
      if(cInput == 'S') Farm_incrementY(this);
      if(cInput == 'D') Farm_incrementX(this);

      // Grid selection
      if(cInput == 'E') Farm_toggleSelected(this, Player_getTime(pPlayer));

      // Perform the selected action on the plots
      if(UtilsKey_isReturn(cInput, "")) {
        
        // Modify the states of the queued plots
        if(this->bIsSelecting && this->eCurrentAction != FARM_INSPECT) {
          if(!strlen(this->sWarningText)) {
            if(!Farm_processQueue(this, pPlayer, pCatalogue, Player_getTime(pPlayer)))
              Farm_startSelecting(this);
          } else {
            strcpy(this->sWarningText, "");
          }
        
        // Plots have been modified; go back to do another action
        } else {
          this->eCurrentAction = FARM_NULL;
          this->eCurrentCrop = PRODUCT_NULL;
        }
      }
    
    // Select a product to use for sowing
    } else {
      if(cInput == 'X') UtilsSelector_decrement(pCatalogueSelector);
      if(cInput == 'C') UtilsSelector_increment(pCatalogueSelector);

      // Select the crop to be used on the plot
      if(Farm_canSow(this)) {
        if(UtilsKey_isReturn(cInput, "")) {
          if(UtilsSelector_getCurrentValue(pCatalogueSelector) < pCatalogue->dSize)
            this->eCurrentCrop = UtilsSelector_getCurrentValue(pCatalogueSelector);
          else this->eCurrentAction = FARM_NULL;
        } 
      } else {
        this->eCurrentAction = FARM_NULL;
      }
    }
  }
}

/**
 * Creates the footer while in the farm.
 * 
 * @param   {struct Farm *}           this                        The farm object.
 * @param   {struct Player *}         pPlayer                     The player object whose information we're using.
 * @param   {struct UtilsText *}      pFooterText                 The footer text object to be modified.
 * @param   {struct UtilsSelector *}  pCatalogueSelector          A selector for all the game products.
 * @param   {struct GameCatalogue *}  pCatalogue                  A reference to the info on the different game products.
 * @param   {char **}                 sFarmSelectText             An array of texts that provide info on the farm options.
 * @param   {char *}                  sFooterFrontTemplate        A template string that helps format the footer.
 * @param   {char *}                  sFooterBlankFrontTemplate   Another template string that helps format the footer.
*/
void Farm_footer(
  struct Farm *this,
  struct Player *pPlayer,
  struct UtilsText *pFooterText,
  struct UtilsSelector *pCatalogueSelector,
  struct GameCatalogue *pCatalogue,

  char **sFarmSelectText,
  char *sFooterFrontTemplate,
  char *sFooterBlankFrontTemplate) {
  
  // A placeholder variable
  char *sFooterString = UtilsUI_createBuffer();

  // The player is currently choosing an action to do on the farm
  if(Farm_getCurrentAction(this) == FARM_NULL) {
    
    // Provide action name
    char *sActionName = this->sPresentActionNameArray[UtilsSelector_getCurrentValue(this->pFarmSelector)];

    sprintf(sFooterString, sFooterFrontTemplate, sActionName, sFarmSelectText[UtilsSelector_getCurrentIndex(this->pFarmSelector) * 2]);
    UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
    sprintf(sFooterString, sFooterBlankFrontTemplate, sFarmSelectText[UtilsSelector_getCurrentIndex(this->pFarmSelector) * 2 + 1]);
    UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
    UtilsText_addNewLines(pFooterText, 1);

  // Select a seed to sow
  } else if (Farm_getCurrentAction(this) == FARM_SOW && Farm_getCurrentCrop(this) == PRODUCT_NULL) {
    enum ProductType eProductType = UtilsSelector_getCurrentValue(pCatalogueSelector);
    char *sProductName = calloc(16, sizeof(char)); 
    char *sProductDesc = UtilsUI_createBuffer();

    if(eProductType < pCatalogue->dSize) {
      sProductName = pCatalogue->sProductNameArray[eProductType];
      sprintf(sProductDesc, "You currently have (%d) of this seed in your inventory.", 
        Stock_getAmount(Player_getSeedStock(pPlayer, eProductType)));
    } else {
      strcpy(sProductName, "go back");
      sprintf(sProductDesc, "Return to the farm and do something else.");
    }

    sprintf(sFooterString, sFooterFrontTemplate, sProductName, sProductDesc);
    UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
    sprintf(sFooterString, sFooterBlankFrontTemplate, "");
    UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
    UtilsText_addNewLines(pFooterText, 1);

  // Display info on currently selected plot
  } else {

    // Get the currently selected plot
    struct Plot *pSelectedPlot = Farm_getCurrentPlot(this);
    char *sPlotName = calloc(16, sizeof(char));
    char *sPlotState = UtilsUI_createBuffer();
    char *sProductName = calloc(24, sizeof(char));
    char *sProductState = UtilsUI_createBuffer();

    // Coordinates and state of the current plot
    sprintf(sPlotName, "@(%d, %d)", Farm_getSelectorX(this) + 1, Farm_getSelectorY(this) + 1);
    sprintf(sProductName, "%s", Plot_getProductName(pSelectedPlot));
    strcpy(sProductState, "");

    switch(Plot_getState(pSelectedPlot)) {
      case PLOT_UNTILLED: sprintf(sPlotState, "This plot is currently (UNTILLED)."); break;
      case PLOT_TILLED: sprintf(sPlotState, "This plot is currently (TILLED)."); break;
      case PLOT_SOWN: 
        sprintf(sPlotState, Plot_getProductLastWatered(pSelectedPlot) < Player_getTime(pPlayer) ? 
          "This plot is currently (SOWN) with a (%s) seed." :
          "This plot is currently (SOWN) with a (%s) seed and was already watered today.", UtilsUI_toUpper(sProductName)); 
        
        sprintf(sProductState, Plot_getProductState(pSelectedPlot) < 2 ? 
          "The crop has been watered (%d) out of (%d) times and is (%.0f%%) from being harvestable." :
          "The crop has been fully watered and is now harvestable.", 
          Plot_getProductWaterAmt(pSelectedPlot),
          Plot_getProductWaterReq(pSelectedPlot),
          Plot_getProductWaterAmt(pSelectedPlot) * 100.0 / Plot_getProductWaterReq(pSelectedPlot));
        break;
    }

    // Display information about the currently selected plot
    sprintf(sFooterString, sFooterFrontTemplate, Farm_getCurrentQueueStatus(this) ? "selected" : "unselected", sPlotState);
    UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
    sprintf(sFooterString, sFooterFrontTemplate, sPlotName, sProductState);
    UtilsText_addText(pFooterText, UtilsText_paddedText(sFooterString, " ", UTILS_TEXT_LEFT_ALIGN));
    UtilsText_addNewLines(pFooterText, 1);
  }
}

#endif