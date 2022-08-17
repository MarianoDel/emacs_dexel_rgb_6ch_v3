//---------------------------------------------------------
// #### Part of Startup Files for STM32F103RC
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### STARTUP_CLOCKS.H ############################################
//---------------------------------------------------------

#ifndef _STARTUP_CLOCKS_H_
#define _STARTUP_CLOCKS_H_


//--- Defines for conf clocks -------
// Select Clock Source
#define HSI_INTERNAL_RC
// #define HSE_CRYSTAL_OSC

// Select Sysclk frequency
#define SYSCLK_FREQ_72MHz
// #define SYSCLK_FREQ_48MHz
// #define SYSCLK_FREQ_8MHz

// Select Xtal frequency
// #define CRYSTAL_8MHZ
// #define CRYSTAL_12MHZ


//--- Exported Module Functions ----
void SystemInit (void);


#endif    /* _STARTUP_CLOCKS_H_ */

//--- end of file ---//
