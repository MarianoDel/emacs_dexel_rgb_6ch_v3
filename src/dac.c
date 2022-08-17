//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### DAC.C #################################
//---------------------------------------------
#include "dac.h"
#include "stm32f10x.h"


// Module Private Types Constants and Macros -----------------------------------
#define RCC_DAC_CLK    (RCC->APB1ENR & RCC_APB1ENR_DACEN)
#define RCC_DAC_CLK_ON    (RCC->APB1ENR |= RCC_APB1ENR_DACEN)
#define RCC_DAC_CLK_OFF    (RCC->APB1ENR &= ~RCC_APB1ENR_DACEN)


// Externals -------------------------------------------------------------------


// Global ----------------------------------------------------------------------


// Module Functions ------------------------------------------------------------
void DAC_Config (void)
{
    // DAC1 clock enable
    if (!RCC_DAC_CLK)
        RCC_DAC_CLK_ON;

    // Enable DAC1, no trigger with buffer
    // DAC->CR |= DAC_CR_BOFF1 | DAC_CR_EN1;
    DAC->CR |= DAC_CR_EN1;

}


void DAC_Output (unsigned short new_val)
{
    DAC->DHR12R1 = new_val;
}


unsigned short DAC_Buffer (void)
{
    return (unsigned short) DAC->DOR1;
}


//---- end of file ----//
