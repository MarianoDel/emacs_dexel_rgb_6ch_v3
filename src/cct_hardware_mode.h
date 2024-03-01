//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_HARDWARE_MODE.H #######################
//-------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _CCT_HARDWARE_MODE_H_
#define _CCT_HARDWARE_MODE_H_

//-- includes to help configurations --//
#include "switches_answers.h"
#include "parameters.h"


#define CCT_HARD_RUNNING_DMX    0
#define CCT_HARD_RUNNING_CCT    1


// Module Exported Functions ---------------------------------------------------
void Cct_Hardware_Mode_UpdateTimers (void);
resp_t Cct_Hardware_Mode (parameters_typedef * mem, sw_actions_t actions);
void Cct_Hardware_Mode_Reset (void);

unsigned char Cct_Hardware_Running_Get (void);
void Cct_Hardware_Running_Set (unsigned char new_val);
#endif    /* _CCT_HARDWARE_MODE_H_ */

//--- end of file ---//

