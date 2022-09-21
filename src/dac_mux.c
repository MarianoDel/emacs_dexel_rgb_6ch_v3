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

}


void DAC_MUX_Update_by_Int (unsigned short * values)
{
    if (dac_mux_cnt < 5)
        dac_mux_cnt++;
    else
        dac_mux_cnt = 0;

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

}


void DAC_MUX_Timeouts (void)
{
    if (dac_mux_ena)
        dac_mux_ena--;
}

//--- end of file ---//

