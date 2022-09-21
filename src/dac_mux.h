//----------------------------------------------------------
// #### DEXEL PROJ 6CH BIDIRECTIONAL v3 - Custom Board #####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### DAC_MUX.H ##########################################
//----------------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef __DAC_MUX_H_
#define __DAC_MUX_H_


// Exported Types Constants and Macros -----------------------------------------



// Exported Functions ----------------------------------------------------------
void DAC_MUX_Update (unsigned short * values);
void DAC_MUX_Update_by_Int (unsigned short * values);
void DAC_MUX_Timeouts (void);



#endif    /* __DAC_MUX_H_ */

//--- end of file ---//

