/**
 * I know C isn't an OOP but I'm gonna pretend it is.
 * 
 * A plot helps us store information about the plants we have on the grid.
 * It also gives us helper functions to perform actions on specific plants on the grid.
*/

#ifndef GAME_CLASS_PLOT
#define GAME_CLASS_PLOT

#include <stdlib.h>

#include "../game.catalogue.h"
#include "../enums/game.enum.farm.h"
#include "game.class.product.h"

/**
 * A plot class allows us to modify plots individually.
 * Since this is supposedly an instantiable class we're gonna have to get funky with calloc().
*/
struct Plot {
  enum PlotState eState;
  struct Product *pProduct;
};

/**
 * ###########################
 * ###  PLOT CONSTRUCTION  ###
 * ###########################
*/

/**
 * Allocates memory for an instance of plot.
 * 
 * @return  {struct Plot *}   Returns a pointer to the created instance.
*/
struct Plot *Plot_new() {
  struct Plot *pPlot;

  pPlot = calloc(1, sizeof(*pPlot));

  if(pPlot == NULL)
    return NULL;

  return pPlot;
}

/**
 * Frees memory from a destroyed instance.
 * 
 * @param   {struct Plot *}   this  The instance to be destroyed.
*/
void Plot_kill(struct Plot *this) {
  free(this);
}

/**
 * Initializes a plot with a certain state.
 * All plots are initially untilled.
 * 
 * @param   {struct Plot *}   this    The instance of the plot to be initialized.
*/
void Plot_init(struct Plot *this) {
  this->eState = PLOT_UNTILLED;
}

/**
 * Creates a new plot and initializes it.
 * 
 * @return  {struct Plot *}                       A pointer to the created plot.
*/
struct Plot *Plot_create() {
  struct Plot *pPlot = Plot_new();
  Plot_init(pPlot);

  return pPlot;
}

/**
 * ##################################
 * ###  PLOT READERS AND WRITERS  ###
 * ##################################
*/

/**
 * Returns the current state of the plot.
 * 
 * @param   {struct Plot *this}   The plot instance.
 * @return  {enum PlotState}      The state of the plot.
*/
enum PlotState Plot_getState(struct Plot *this) {
  return this->eState;
}

/**
 * Returns the name of the product stored by the plot.
 * Returns an empty string if there is currently no product stored by the plot.
 * 
 * @param   {struct Plot *this}   The plot instance.
 * @return  {char *}              The product name.
*/
char *Plot_getProductName(struct Plot *this) {
  if(this->eState == PLOT_SOWN)
    return Product_getName(this->pProduct);
  return "";
}

/**
 * Returns the code of the product stored by the plot.
 * Returns an empty string if there is no product.
 * 
 * @param   {struct Plot *}   this  The plot instance.
 * @return  {char *}                The product code.
*/
char *Plot_getProductCode(struct Plot *this) {
  if(this->eState == PLOT_SOWN)
    return Product_getCode(this->pProduct);
  return "";
}

/**
 * Returns the type of the product.
 * Returns PRODUCT_NULL if there is no product.
 * 
 * @param   {struct Plot *}     this  The plot instance.
 * @return  {enum ProductType}        The product type.
*/
enum ProductType Plot_getProductType(struct Plot *this) {
  if(this->eState == PLOT_SOWN)
    return Product_getType(this->pProduct);
  return PRODUCT_NULL;
}

/**
 * Returns the watering state of the product on the plot.
 * Returns -1 if there is no product on the plot.
 * 
 * @param   {struct Plot *}   this  The instance to be read.
 * @return  {int}                   The state of the product on the plot.
*/
int Plot_getProductState(struct Plot *this) {
  if(this->eState == PLOT_SOWN)
    return Product_getState(this->pProduct);
  return -1;
}

/**
 * Returns the how much water the plant has gotten.
 * Returns -1 if there is no product on the plot.
 * 
 * @param   {struct Plot *}   this  The instance to be read.
 * @return  {int}                   How much water the plot has gotten.
*/
int Plot_getProductWaterAmt(struct Plot *this) {
  if(this->eState == PLOT_SOWN)
    return Product_getWaterAmt(this->pProduct);
  return -1;
}

/**
 * Returns the how much water the plant needs.
 * Returns -1 if there is no product on the plot.
 * 
 * @param   {struct Plot *}   this  The instance to be read.
 * @return  {int}                   How much water the plot needs.
*/
int Plot_getProductWaterReq(struct Plot *this) {
  if(this->eState == PLOT_SOWN)
    return Product_getWaterReq(this->pProduct);
  return -1;
}

/**
 * Returns the last time the crop was watered.
 * Return -2 if the plot has no product.
 * 
 * @param   {struct Plot *}   this  The instance to be read.
 * @return  {int}                   The time when the product was last watered.
*/
int Plot_getProductLastWatered(struct Plot *this) {
  if(this->eState == PLOT_SOWN)
    return Product_getLastWatered(this->pProduct);
  return -2;
}

/**
 * ######################
 * ###  PLOT ACTIONS  ###
 * ######################
*/

/**
 * Tills a plot IF it is in the PLOT_UNTILLED state.
 * 
 * @param   {struct Plot *}   this  The instance of the plot to be tilled.
 * @return  {int}                   Return a 1 if the tilling was successful and 0 if otherwise.
*/
int Plot_till(struct Plot *this) {
  if(this->eState == PLOT_UNTILLED) {
    this->eState = PLOT_TILLED;
  
    return 1;
  } else return 0;
}

/**
 * Plants a product on a plot IF it is in the PLOT_TILLED state.
 * Precondition: pProduct is initialized.
 * 
 * @param   {struct Plot *}     this      The instance of the plot to be planted on.
 * @param   {struct Product *}  pProduct  The instance of the product to be planted.
 * @return  {int}                         Return a 1 if the planting was successful, 0 if otherwise.
*/
int Plot_sow(struct Plot *this, struct Product *pProduct) {
  if(this->eState == PLOT_TILLED) {
    this->eState = PLOT_SOWN;
    this->pProduct = pProduct;
    
    return 1; 
  } else return 0;
}

/**
 * Waters the crop on the plot.
 * Precondition: dTime is nonnegative
 * 
 * @param   {struct Plot *}     this      The instance of the plot to be planted on.
 * @param   {int}               dTime     The current day.
 * @return  {int}                         Return a 1 if the watering was successful, 0 if otherwise.
*/
int Plot_water(struct Plot *this, int dTime) {
  return Product_water(this->pProduct, dTime);
}

/**
 * Harvests a plot IF it has a crop on it.
 * Precondition: the product has already fully grown.
 * 
 * @param   {struct Plot *}   this  The instance of the plot to be harvested.
 * @return  {int}                   A boolean indicating whether or no the action was successful.
*/
int Plot_harvest(struct Plot *this) {
  if(this->eState == PLOT_SOWN) {

    // Some more garbage collection
    Product_kill(this->pProduct);
    
    this->eState = PLOT_UNTILLED;
    this->pProduct = NULL;

    return 1;
  } else return 0;
}

#endif