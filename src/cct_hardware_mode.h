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

    

// Module Exported Functions ---------------------------------------------------
void Cct_HardwareMode_UpdateTimers (void);
resp_t Cct_HardwareMode (parameters_typedef * mem, sw_actions_t actions);
void Cct_HardwareModeReset (void);

resp_t Cct_HardwareMode_New (parameters_typedef * mem, sw_actions_t actions);

#endif    /* _CCT_HARDWARE_MODE_H_ */

//--- end of file ---//

