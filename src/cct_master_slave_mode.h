//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MASTER_SLAVE_MODE.H ###################
//-------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _CCT_MASTER_SLAVE_MODE_H_
#define _CCT_MASTER_SLAVE_MODE_H_

//-- includes to help configurations --//
#include "switches_answers.h"
#include "parameters.h"

    

// Module Exported Functions ---------------------------------------------------
resp_t Cct_MasterSlaveMode (parameters_typedef *, sw_actions_t);
void Cct_MasterSlaveModeReset (void);
void Cct_MasterSlaveMode_UpdateTimers (void);

#endif    /* _CCT_MASTER_SLAVE_MODE_H_ */

//--- end of file ---//

