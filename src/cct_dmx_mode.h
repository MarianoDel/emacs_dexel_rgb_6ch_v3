//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_DMX_MODE.H ###############################
//-------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _CCT_DMX_MODE_H_
#define _CCT_DMX_MODE_H_

//-- includes to help configurations --//
#include "switches_answers.h"


// Exported Types Constants and Macros -----------------------------------------



// Exported Module Functions ---------------------------------------------------
resp_t Cct_DMXMode (unsigned char *, sw_actions_t);
void Cct_DMXMode_Reset (void);
void Cct_DMXMode_UpdateTimers (void);


#endif    /* _CCT_DMX_MODE_H_ */

//--- end of file ---//
