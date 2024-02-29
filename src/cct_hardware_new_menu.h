//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_HARDWARE_MENU.H #######################
//-------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _CCT_HARDWARE_MENU_H_
#define _CCT_HARDWARE_MENU_H_

//-- includes to help configurations --//
#include "switches_answers.h"
#include "parameters.h"

    

// Module Exported Functions ---------------------------------------------------
void Cct_Hardware_New_Menu_UpdateTimers (void);
resp_t Cct_Hardware_New_Menu (parameters_typedef * mem, sw_actions_t actions);
void Cct_Hardware_New_Menu_Reset (void);

#endif    /* _CCT_HARDWARE_MENU_H_ */

//--- end of file ---//

