//----------------------------------------------------------
// #### DEXEL PROJ 6CH BIDIRECTIONAL v3 - Custom Board #####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_UTILS.H ########################################
//----------------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef __CCT_UTILS_H_
#define __CCT_UTILS_H_

#include "switches_answers.h"

// Exported Types Constants and Macros -----------------------------------------
#define CCT_MODE_2700_6500    0
#define CCT_MODE_3200_5700    1


// Exported Functions ----------------------------------------------------------
void GetPercentage (unsigned char dmx_value, unsigned char * val_int, unsigned char * val_dec);
unsigned short GetCct (unsigned char value, unsigned char cct_mode);
resp_t Cct_Utils_Update_Actions_Values (sw_actions_t actions, unsigned char * values);
unsigned char Cct_Utils_Dim_Color (unsigned char dim_value, unsigned char color_value);


#endif    /* __CCT_UTILS_H_ */

//--- end of file ---//

