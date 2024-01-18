//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MASTER_SLAVE_MENU.H ##########################
//-------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _CCT_MASTER_SLAVE_MENU_H_
#define _CCT_MASTER_SLAVE_MENU_H_

//-- includes to help configurations --//
#include "switches_answers.h"
#include "parameters.h"

    

// Module Exported Functions ---------------------------------------------------
void CCT_Master_Slave_Menu_UpdateTimer (void);
void CCT_Master_Slave_Menu_Reset (void);
resp_t CCT_Master_Slave_Menu (parameters_typedef *, sw_actions_t);


#endif    /* _CCT_MASTER_SLAVE_MENU_H_ */

//--- end of file ---//

