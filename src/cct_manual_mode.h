//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MANUAL_MODE.H #########################
//-------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _CCT_MANUAL_MODE_H_
#define _CCT_MANUAL_MODE_H_

//-- includes to help configurations --//
#include "switches_answers.h"
#include "parameters.h"

    

// Module Exported Functions ---------------------------------------------------
resp_t Cct_ManualMode (parameters_typedef *, sw_actions_t);
void Cct_ManualModeReset (void);
void Cct_ManualMode_UpdateTimers (void);

#endif    /* _CCT_MANUAL_MODE_H_ */

//--- end of file ---//

