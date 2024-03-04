//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_DMX_MENU.C ############################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_dmx_menu.h"
#include "display_utils.h"
#include "parameters.h"
#include "ssd1306_gfx.h"

#include <stdio.h>
#include <string.h>


// Private Types Constants and Macros ------------------------------------------
typedef enum {
    CCT_DMX_MENU_INIT = 0,
    CCT_DMX_MENU_CH1_CH6,
    CCT_DMX_MENU_CH2_CH7,
    CCT_DMX_MENU_CH3_CH8,
    CCT_DMX_MENU_CH4_CH9,
    CCT_DMX_MENU_CH5,    
    CCT_DMX_MENU_UPDATE_DISPLAY

} cct_dmx_menu_e;


// Externals -------------------------------------------------------------------
// -- for current temp --
#include "adc.h"
#include "temperatures.h"
extern volatile unsigned short adc_ch [];


// Globals ---------------------------------------------------------------------
static cct_dmx_menu_e cct_dmx_menu_state = CCT_DMX_MENU_INIT;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Cct_DMX_MenuReset (void)
{
    cct_dmx_menu_state = CCT_DMX_MENU_INIT;
}


extern unsigned char display_is_free (void);
extern void display_update (void);
resp_t Cct_DMX_Menu (dmx_menu_data_t * pmenu_data)
{
    resp_t resp = resp_continue;
    char s_temp[21];    //20 caracteres por linea + '\0'

    switch (cct_dmx_menu_state)
    {
    case CCT_DMX_MENU_INIT:
        
        if (!display_is_free())
            break;
        
        Display_StartLines ();
        Display_ClearLines();

        if (pmenu_data->show_addres & 0x01)
            sprintf(s_temp, "ADDR: %03d", *pmenu_data->dmx_first_chnl);
        else
            strcpy(s_temp, "ADDR:");

        Display_SetLine1(s_temp);

        if (pmenu_data->show_addres & 0x02)
        {
            // show temp here
            char curr_temp = Temp_TempToDegreesExtended(Temp_Channel);
            sprintf(s_temp, "CURR T: %dC", curr_temp);
            Display_SetLine8(s_temp);
        }
        else
        {
            Display_SetLine8("             DMX CCT");
        }
        
        cct_dmx_menu_state++;
        break;

    case CCT_DMX_MENU_CH1_CH6:
        memset(s_temp, 0, sizeof(s_temp));

        sprintf(s_temp, "RED: %3d     CCT: %3d",
                *((pmenu_data->pchannels) + 0),
                *((pmenu_data->pchannels) + 5));
        
        Display_BlankLine3();
        Display_SetLine3(s_temp);            
        cct_dmx_menu_state++;
        break;

    case CCT_DMX_MENU_CH2_CH7:
        memset(s_temp, 0, sizeof(s_temp));

        sprintf(s_temp, "BLU: %3d     STB: %3d",        
                *((pmenu_data->pchannels) + 1),
                *((pmenu_data->pchannels) + 6));

        Display_BlankLine4();
        Display_SetLine4(s_temp);                    
        cct_dmx_menu_state++;
        break;

    case CCT_DMX_MENU_CH3_CH8:
        memset(s_temp, 0, sizeof(s_temp));

        sprintf(s_temp, "GRN: %3d     CLR: %3d",        
                *((pmenu_data->pchannels) + 2),
                *((pmenu_data->pchannels) + 7));
        
        Display_BlankLine5();
        Display_SetLine5(s_temp);            
        cct_dmx_menu_state++;
        break;

    case CCT_DMX_MENU_CH4_CH9:
        memset(s_temp, 0, sizeof(s_temp));

        sprintf(s_temp, " WW: %3d     DIM: %3d",        
                *((pmenu_data->pchannels) + 3),
                *((pmenu_data->pchannels) + 8));
        
        Display_BlankLine6();
        Display_SetLine6(s_temp);            
        cct_dmx_menu_state++;
        break;

    case CCT_DMX_MENU_CH5:
        memset(s_temp, 0, sizeof(s_temp));

        sprintf(s_temp, " CW: %3d             ",
                *((pmenu_data->pchannels) + 4));
        
        Display_BlankLine7();
        Display_SetLine7(s_temp);            
        cct_dmx_menu_state++;
        break;
        
    case CCT_DMX_MENU_UPDATE_DISPLAY:
        display_update();
        cct_dmx_menu_state = CCT_DMX_MENU_INIT;
        resp = resp_finish;
        break;
        
    default:
        cct_dmx_menu_state = CCT_DMX_MENU_INIT;
        break;
    }

    return resp;    
}


//--- end of file ---//
