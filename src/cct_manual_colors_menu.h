//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MANUAL_COLORS_MENU.H ##########################
//-------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _CCT_MANUAL_COLORS_MENU_H_
#define _CCT_MANUAL_COLORS_MENU_H_

//-- includes to help configurations --//
#include "switches_answers.h"
#include "parameters.h"

    

// Module Exported Functions ---------------------------------------------------
void Cct_Manual_Colors_Menu_UpdateTimer (void);
void Cct_Manual_Colors_Menu_Reset (void);
resp_t Cct_Manual_Colors_Menu (parameters_typedef *, sw_actions_t);
void Cct_Index_To_Channels_Mapping (unsigned char index, unsigned char * chnls);

#endif    /* _CCT_MANUAL_COLORS_MENU_H_ */

//--- end of file ---//

