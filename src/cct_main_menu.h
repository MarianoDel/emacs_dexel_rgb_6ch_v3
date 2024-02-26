//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MAIN_MENU.H ##########################
//-------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _CCT_MAIN_MENU_H_
#define _CCT_MAIN_MENU_H_

//-- includes to help configurations --//
#include "switches_answers.h"
#include "parameters.h"

    

// Module Exported Functions ---------------------------------------------------
void Cct_Main_Menu_UpdateTimer (void);
void Cct_Main_Menu_Reset (void);
resp_t Cct_Main_Menu (parameters_typedef *, sw_actions_t);


#endif    /* _CCT_MAIN_MENU_H_ */

//--- end of file ---//

