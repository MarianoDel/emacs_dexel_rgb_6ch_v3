//----------------------------------------------------------
// #### DEXEL PROJ 6CH BIDIRECTIONAL v3 - Custom Board #####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### DAC_MUX.C ##########################################
//----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "dac_mux.h"
#include "hard.h"
#include "dac.h"
#include "stm32f10x.h"


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
unsigned char dac_mux_ena = 0;
unsigned char dac_mux_cnt = 0;

// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------


void DAC_MUX_Update (unsigned short * values)
{
    if (dac_mux_ena)
        return;

    dac_mux_ena = 1;    // 1ms update
    
    if (dac_mux_cnt < 5)
        dac_mux_cnt++;
    else
        dac_mux_cnt = 0;

#ifdef HARDWARE_VERSION_3_2
    // mux to not used output
    GPIOA->BSRR = 0x00001C00;
    
    DAC_Output(*(values + dac_mux_cnt));

    // Open the proper switch
    unsigned char mask = 0x07 & (~dac_mux_cnt);
    GPIOA->BSRR = (mask << (10 + 16));

#else    // ver 3.1 & ver 3.0
    
    // Close all switches
    CTRL_C1_OFF;
    CTRL_C2_OFF;
    CTRL_C3_OFF;
    CTRL_C4_OFF;
    CTRL_C5_OFF;
    CTRL_C6_OFF;
    
    DAC_Output(*(values + dac_mux_cnt));

    // Open the proper switch
    if (dac_mux_cnt == 0)
        CTRL_C1_ON;
    else if (dac_mux_cnt == 1)
        CTRL_C2_ON;
    else if (dac_mux_cnt == 2)
        CTRL_C3_ON;
    else if (dac_mux_cnt == 3)
        CTRL_C4_ON;
    else if (dac_mux_cnt == 4)
        CTRL_C5_ON;
    else if (dac_mux_cnt == 5)
        CTRL_C6_ON;
    
#endif
}


unsigned char v_mux [] = {0, 2, 4, 1, 3, 5};
void DAC_MUX_Update_by_Int (unsigned short * values)
{
    if (dac_mux_cnt < 5)
        dac_mux_cnt++;
    else
        dac_mux_cnt = 0;


#ifdef HARDWARE_VERSION_3_2
    // mux to not used output
    GPIOA->BSRR = 0x00001C00;
    
    DAC_Output(*(values + dac_mux_cnt));

    // Open the proper switch
    unsigned char mask = 0x07 & (~dac_mux_cnt);
    GPIOA->BSRR = (mask << (10 + 16));

#else    // ver 3.1 & ver 3.0
    
    // Close all switches
    CTRL_C1_OFF;
    CTRL_C2_OFF;
    CTRL_C3_OFF;
    CTRL_C4_OFF;
    CTRL_C5_OFF;
    CTRL_C6_OFF;

    unsigned char ch_to_send = v_mux[dac_mux_cnt];
    DAC_Output(*(values + ch_to_send));

    // Open the proper switch
    if (ch_to_send == 0)
        CTRL_C1_ON;
    else if (ch_to_send == 1)
        CTRL_C2_ON;
    else if (ch_to_send == 2)
        CTRL_C3_ON;
    else if (ch_to_send == 3)
        CTRL_C4_ON;
    else if (ch_to_send == 4)
        CTRL_C5_ON;
    else if (ch_to_send == 5)
        CTRL_C6_ON;

#endif
}


void DAC_MUX_Timeouts (void)
{
    if (dac_mux_ena)
        dac_mux_ena--;
}

//--- end of file ---//

