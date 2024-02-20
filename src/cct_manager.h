//----------------------------------------------------------
// #### DEXEL PROJ 6CH BIDIRECTIONAL v3 - Custom Board #####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MANAGER.H ######################################
//----------------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef __CCT_MANAGER_H_
#define __CCT_MANAGER_H_

#include "parameters.h"

// Exported Types Constants and Macros -----------------------------------------
void Cct_Manager (parameters_typedef * pmem);
void Cct_Manager_Timeouts (void);
void Cct_Manager_Ntc_Set (void);
void Cct_Manager_Ntc_Reset (void);


// Exported Functions ----------------------------------------------------------


#endif    /* __CCT_MANAGER_H_ */

//--- end of file ---//

