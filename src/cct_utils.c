//----------------------------------------------------------
// #### DEXEL PROJ 6CH BIDIRECTIONAL v3 - Custom Board #####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_UTILS.C ########################################
//----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_utils.h"


#include <stdio.h>
#include <string.h>
// Module Private Types & Macros -----------------------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
// change values from actions with update flagged
resp_t CCT_Utils_Update_Actions_Values (sw_actions_t actions, unsigned char * value)
{
    resp_t resp = resp_continue;
    
    if (actions == selection_dwn_fast)
    {
        if (*value >= 10)
            *value -= 10;
        else
            *value = 0;

        resp = resp_change;
    }
    else if (actions == selection_dwn)
    {
        if (*value)
            *value -= 1;

        resp = resp_change;        
    }
    else if (actions == selection_up_fast)
    {
        if (*value <= 245)
            *value += 10;
        else
            *value = 255;

        resp = resp_change;        
    }
    else if (actions == selection_up)
    {
        if (*value <= 254)
            *value += 1;

        resp = resp_change;        
    }

    return resp;
}


// get the percentage from 0 - 255 values
void GetPercentage (unsigned char dmx_value, unsigned char * val_int, unsigned char * val_dec)
{
    unsigned int calc = 0;
    
    if (dmx_value == 0)
    {
        *val_int = 0;
        *val_dec = 0;
    }
    else if (dmx_value == 255)
    {
        *val_int = 100;
        *val_dec = 0;
    }
    else
    {
        calc = dmx_value * 1000;
        calc = calc / 255;        
        *val_int = calc / 10;
        *val_dec = calc - *val_int * 10;
    }
}


unsigned short GetCct (unsigned char value, unsigned char cct_mode)
{
    int calc;
    unsigned short scale = 2500;    // CCT_MODE_3200_5700
    
    if (cct_mode == CCT_MODE_2700_6500)
        scale = 3800;

    calc = scale * value;
    calc >>= 8;

    unsigned char comp_2500 = 0;
    unsigned char comp_3800 = 0;
    if (value > (128 + 64))
    {
        comp_2500 = 10;
        comp_3800 = 15;
    }
    else if (value > 128)
    {
        comp_2500 = 5;
        comp_3800 = 8;
    }
    else if (value > 64)
    {
        comp_2500 = 2;
        comp_3800 = 4;
    }

    if (cct_mode == CCT_MODE_2700_6500)
        calc = calc + comp_3800 + 2700;
    else
        calc = calc + comp_2500 + 3200;

    return (unsigned short) calc;
    
}


//--- end of file ---//

