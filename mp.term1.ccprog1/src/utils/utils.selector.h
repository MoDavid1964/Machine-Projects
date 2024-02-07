/**
 * A helper class for selection functionality.
 * This is the primary way through which the user interacts with the game in full mode.
*/

#ifndef UTILS_SELECTOR
#define UTILS_SELECTOR

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SELECTION_SIZE 64
#define MAX_WRAPPER_LENGTH 64

/**
 * A instantiable that enables us to easily duplicate the functionality of a selector.
*/
struct UtilsSelector {
  int dSelectionIndex;
  int dSelectionLimit;
  int dSelectionAvailable;
  int bSelectionLooped;

  // Template strings for formatting
  char *sDefaultWrapper;
  char *sSelectedWrapper;
  char *sDisabledWrapper;

  char *sSelectionArray[MAX_SELECTION_SIZE];
  int dSelectionValueArray[MAX_SELECTION_SIZE];
  int bSelectionAvailabilityArray[MAX_SELECTION_SIZE];
};

/**
 * ###############################
 * ###  SELECTOR CONSTRUCTION  ###
 * ###############################
*/

/**
 * Creates a new instance of the UtilsSelector class.
 * 
 * @return  {struct UtilsSelector *}  The created instance.
*/
struct UtilsSelector *UtilsSelector_new() {
  struct UtilsSelector *pUtilsSelector;

  pUtilsSelector = calloc(1, sizeof(*pUtilsSelector));

  if(pUtilsSelector == NULL)
    return NULL;

  return pUtilsSelector;
}

/**
 * Initializes an instance of the UtilsSelector class.
 * Note that if the wrapper template strings are too long, they are simply replaced with "%s".
 * 
 * @param   {struct UtilsSelector *}  this              The instance to be initialized.
 * @param   {int}                     bIsLooped         Whether or not the selection loops or stops at its limits.
 * @param   {char *}                  sDefaultWrapper   A template string specifying the default string formatting of an option.
 * @param   {char *}                  sSelectedWrapper  A template string specifying the format of a selected option.
 * @param   {char *}                  sDisabledWrapper  A template string specifying the format of an unavailable option.
*/
void UtilsSelector_init(struct UtilsSelector *this, int bIsLooped, char *sDefaultWrapper, char *sSelectedWrapper, char *sDisabledWrapper) {
  
  this->dSelectionIndex = 0;
  this->dSelectionLimit = 0;
  this->dSelectionAvailable = 0;
  this->bSelectionLooped = bIsLooped;

  this->sDefaultWrapper = calloc(MAX_WRAPPER_LENGTH, sizeof(char));
  if(strlen(sDefaultWrapper) < MAX_WRAPPER_LENGTH) this->sDefaultWrapper = sDefaultWrapper;
  else this->sDefaultWrapper = "%s";

  this->sSelectedWrapper = calloc(MAX_WRAPPER_LENGTH, sizeof(char));
  if(strlen(sSelectedWrapper) < MAX_WRAPPER_LENGTH) this->sSelectedWrapper = sSelectedWrapper;
  else this->sSelectedWrapper = "%s";

  this->sDisabledWrapper = calloc(MAX_WRAPPER_LENGTH, sizeof(char));
  if(strlen(sDisabledWrapper) < MAX_WRAPPER_LENGTH) this->sDisabledWrapper = sDisabledWrapper;
  else this->sDisabledWrapper = "%s";
}

/**
 * Creates an initialized instance of the class.
 * 
 * @param   {int}                     bIsLooped         Whether or not the selection loops.
 * @param   {char *}                  sDefaultWrapper   A template string specifying the default string formatting of an option.
 * @param   {char *}                  sSelectedWrapper  A template string specifying the format of a selected option.
 * @param   {char *}                  sDisabledWrapper  A template string specifying the format of an unavailable option.
 * @return  {struct UtilsSelector *}                    The initialized instance.
*/
struct UtilsSelector *UtilsSelector_create(int bIsLooped, char *sDefaultWrapper, char *sSelectedWrapper, char *sDisabledWrapper) {
  struct UtilsSelector *pUtilsSelector = UtilsSelector_new();
  UtilsSelector_init(pUtilsSelector, bIsLooped, sDefaultWrapper, sSelectedWrapper, sDisabledWrapper);

  return pUtilsSelector;
}

/**
 * Destroys an instance of the class.
 * 
 * @param   {struct UtilsSelector *}  this  The instance to be destroyed.
*/
void UtilsSelector_kill(struct UtilsSelector *this) {
  free(this);
}

/**
 * ######################################
 * ###  SELECTOR READERS AND WRITERS  ###
 * ######################################
*/

/**
 * Adds a new option to the selection.
 * 
 * @param   {struct UtilsSelector *}    this                  The instance to be modified.
 * @param   {char *}                    sOption               The new option. 
 * @param   {int}                       dOptionValue          The value associated with the option.
*/
void UtilsSelector_addOption(struct UtilsSelector *this, char *sOption, int dOptionValue) {
  if(this->dSelectionLimit < MAX_SELECTION_SIZE) {
    this->sSelectionArray[this->dSelectionLimit] = sOption;
    this->dSelectionValueArray[this->dSelectionLimit] = dOptionValue;
    this->bSelectionAvailabilityArray[this->dSelectionLimit] = 1;

    this->dSelectionAvailable++;
    this->dSelectionLimit++;
  }
}

/**
 * Returns a certain option.
 * 
 * @param   {struct UtilsSelector *}  this      The instance to be modified.
 * @param   {int}                     dIndex    The index of the option. 
 * @return  {char *}                            The string value of the option.
*/
char *UtilsSelector_getOption(struct UtilsSelector *this, int dIndex) {

  // In case user does a dumb dumb
  dIndex %= this->dSelectionLimit;

  return this->sSelectionArray[dIndex];
}

/**
 * Returns a certain option.
 * 
 * @param   {struct UtilsSelector *}  this            The instance to be modified.
 * @param   {int}                     dIndex          The index of the option. 
 * @param   {int}                     bAvailability   Whether or not the option can be selected.
*/
void UtilsSelector_setOptionAvailability(struct UtilsSelector *this, int dIndex, int bAvailability) {
  this->bSelectionAvailabilityArray[dIndex] = bAvailability;
  
  // This implementation suffices cuz we only have a max of 128 options
  this->dSelectionAvailable = 0;
  for(int i = 0; i < this->dSelectionLimit; i++)
    if(this->bSelectionAvailabilityArray[i])
      this->dSelectionAvailable++;

}

/**
 * Sets the availability of all options.
 * 
 * @param   {struct UtilsSelector *}  this            The selector instance to be modified.
 * @param   {int}                     bAvailability   The availability boolean to assign to the options.
*/
void UtilsSelector_setAllAvailability(struct UtilsSelector *this, int bAvailability) {
  this->dSelectionAvailable = bAvailability ? this->dSelectionLimit : 0;
  for(int i = 0; i < this->dSelectionLimit; i++)
    this->bSelectionAvailabilityArray[i] = bAvailability;
}

/**
 * If the current value of the selector is unavailable, we gett the first available value in the array.
 * 
 * @param   {struct UtilsSelector *}  this  The selector instance to be modified.
*/
void UtilsSelector_setFirstAvailable(struct UtilsSelector *this) {
  if(this->dSelectionAvailable && !this->bSelectionAvailabilityArray[this->dSelectionIndex]) {
    this->dSelectionIndex = 0;

    while(!this->bSelectionAvailabilityArray[this->dSelectionIndex] && this->dSelectionIndex + 1 < this->dSelectionLimit)
      this->dSelectionIndex++;
  }
}

/**
 * Returns the value of a certain option.
 * 
 * @param   {struct UtilsSelector *}  this      The instance to be modified.
 * @param   {int}                     dIndex    The index of the option.
 * @return  {int}                               The value of the chosen option.
*/
int UtilsSelector_getOptionValue(struct UtilsSelector *this, int dIndex) {

  // In case user does a dumb dumb
  dIndex %= this->dSelectionLimit;

  return this->dSelectionValueArray[dIndex];
}

/**
 * Gets how many options are available in the selector.
 * 
 * @param   {struct UtilsSelector *}  this    The instance to be read.
 * @return  {int}                             How many available options there are.
*/
int UtilsSelector_getAvailableCount(struct UtilsSelector *this) {
  return this->dSelectionAvailable;
}

/**
 * Gets the current selected index.
 * 
 * @param   {struct UtilsSelector *}  this    The instance to be read.
 * @return  {int}                             The index of the current option.
*/
int UtilsSelector_getCurrentIndex(struct UtilsSelector *this) {
  return this->dSelectionIndex;
}

/**
 * Gets the option of the current selected index.
 * 
 * @param   {struct UtilsSelector *}  this    The instance to be read.
 * @return  {char *}                          The current option.
*/
char *UtilsSelector_getCurrentOption(struct UtilsSelector *this) {
  return this->sSelectionArray[this->dSelectionIndex];
}

/**
 * Gets the value of the current selected option.
 * 
 * @param   {struct UtilsSelector *}  this    The instance to be read.
 * @return  {int}                             The index of the current option.
*/
int UtilsSelector_getCurrentValue(struct UtilsSelector *this) {
  return this->dSelectionValueArray[this->dSelectionIndex];
}

/**
 * Gets the formatted string for the option requested.
 * 
 * @param   {struct UtilsSelector *}  this    The instance to be read.
 * @param   {int}                     dIndex  The index of the option requested.
 * @return  {char *}                          The formatted option string.
*/
char *UtilsSelector_getOptionFormatted(struct UtilsSelector *this, int dIndex) {
  
  // In case user does a dumb dumb
  dIndex %= this->dSelectionLimit;

  // Create output buffer
  char *sOutput = calloc(strlen(this->sSelectionArray[dIndex]) + MAX_WRAPPER_LENGTH, sizeof(char));

  // Create the formatted string
  if(!this->bSelectionAvailabilityArray[dIndex]) {
    sprintf(sOutput, this->sDisabledWrapper, this->sSelectionArray[dIndex]);
  } else {
    if(dIndex == this->dSelectionIndex)
      sprintf(sOutput, this->sSelectedWrapper, this->sSelectionArray[dIndex]);
    else 
      sprintf(sOutput, this->sDefaultWrapper, this->sSelectionArray[dIndex]);
  }

  return sOutput;
}

/**
 * Returns how many options are stored by the instance.
 * 
 * @param   {struct UtilsSelector *}  this      The instance to be read. 
 * @return  {int}                               The number of options.
*/
int UtilsSelector_getLength(struct UtilsSelector *this) {
  return this->dSelectionLimit;
}

/**
 * ##########################
 * ###  SELECTOR ACTIONS  ###
 * ##########################
*/

/**
 * Increments the selection index.
 * 
 * @param   {struct UtilsSelector *}  this    The instance to be modified.
*/
void UtilsSelector_increment(struct UtilsSelector *this) {
  if(this->dSelectionAvailable) {
    int oldSelectionIndex = this->dSelectionIndex;

    do {

      // Looped versus non-looped iteration
      if(this->bSelectionLooped) 
        this->dSelectionIndex = (this->dSelectionIndex + 1) % this->dSelectionLimit;
      else if(this->dSelectionIndex - this->dSelectionLimit + 1) 
        this->dSelectionIndex++;
    
    // Skip unavailable options until we reach the end (for not looped) or until we reach an available option (for looped)
    } while(!this->bSelectionAvailabilityArray[this->dSelectionIndex] && 
      (this->dSelectionIndex - this->dSelectionLimit + 1 || this->bSelectionLooped));

    // In case it reached the end of the array and there were no available options (for not looped)
    if(!this->bSelectionLooped)
      if(!(this->dSelectionIndex - this->dSelectionLimit + 1) && !this->bSelectionAvailabilityArray[this->dSelectionIndex])
        this->dSelectionIndex = oldSelectionIndex;
  }
}

/**
 * Decrements the selection index.
 * 
 * @param   {struct UtilsSelector *}  this    The instance to be modified.
*/
void UtilsSelector_decrement(struct UtilsSelector *this) {
  if(this->dSelectionAvailable) {
    int oldSelectionIndex = this->dSelectionIndex;

    do {

      // Looped and not looped iteration
      if(this->bSelectionLooped) 
        this->dSelectionIndex = (this->dSelectionIndex + this->dSelectionLimit - 1) % this->dSelectionLimit;
      else if(this->dSelectionIndex) 
        this->dSelectionIndex--;

    // Skip unavailable options until we reach the end (for not looped) or until we reach an available option (for looped)
    } while(!this->bSelectionAvailabilityArray[this->dSelectionIndex] && 
      (this->dSelectionIndex || this->bSelectionLooped));

    // In case it reached the end of the array and there were no available options (for not looped)
    if(!this->bSelectionLooped)
      if(!this->dSelectionIndex && !this->bSelectionAvailabilityArray[this->dSelectionIndex])
        this->dSelectionIndex = oldSelectionIndex;
  }
}

#endif