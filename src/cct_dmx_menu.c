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

        if (pmenu_data->show_addres)
            sprintf(s_temp, "ADDR: %03d", *pmenu_data->dmx_first_chnl);
        else
            strcpy(s_temp, "ADDR:");
        
        Display_SetLine1(s_temp);
        Display_SetLine8("             DMX CCT");
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


// typedef enum {
//     DO_NOTHING = 0,
//     TO_CHANGE_WAIT_FREE,
//     CHANGING,
//     TO_DO_NOTHING_WAIT_FREE,
//     TO_CLEAN_OUT
    
// } dmx_address_e;


// dmx_address_e dmx_address_state = DO_NOTHING;
// void DMXModeMenu_ChangeAddressReset (void)
// {
//     dmx_address_state = DO_NOTHING;
// }

// #define TT_SHOW_ADDRESS    500
// #define CNTR_TO_OUT    16
// unsigned char dmx_address_cntr_out = 0;
// resp_t DMXModeMenu_ChangeAddress (dmx_menu_address_data_t * data)
// {
//     resp_t resp = resp_continue;
//     sw_actions_t action = data->actions;
//     unsigned short * address = &data->dmx_address;
//     unsigned char channels = data->dmx_channels_qtty;
//     unsigned short * timer_address = data->timer;
//     unsigned char * address_show = data->address_show;
    
//     switch (dmx_address_state)
//     {
//     case DO_NOTHING:
//         if (action == selection_enter)
//             dmx_address_state++;
        
//         break;

//     case TO_CHANGE_WAIT_FREE:
//         if (action == do_nothing)
//         {
//             dmx_address_cntr_out = CNTR_TO_OUT;
//             dmx_address_state++;
//         }
//         break;
            
//     case CHANGING:
        
//         if (action == selection_up)
//         {
//             if (*address < (512 - channels))
//             {
//                 *address += 1;
//                 *address_show = 1;
                
//                 //force the display change
//                 resp = resp_change;

                
//                 *timer_address = TT_SHOW_ADDRESS;
//                 dmx_address_cntr_out = CNTR_TO_OUT;
//             }
//         }
        
//         if (action == selection_dwn)
//         {
//             if (*address > 1)
//             {
//                 *address -= 1;
//                 *address_show = 1;
                
//                 //force the display change
//                 resp = resp_change;


//                 *timer_address = TT_SHOW_ADDRESS;
//                 dmx_address_cntr_out = CNTR_TO_OUT;                
//             }
//         }

//         if (action == selection_enter)
//             dmx_address_state++;

//         if (!*timer_address)
//         {
//             if (*address_show)
//                 *address_show = 0;
//             else
//                 *address_show = 1;

//             if (dmx_address_cntr_out)
//                 dmx_address_cntr_out--;
            
//             *timer_address = TT_SHOW_ADDRESS;
//         }

//         if (!dmx_address_cntr_out)
//             dmx_address_state = TO_CLEAN_OUT;
        
//         break;

//     case TO_DO_NOTHING_WAIT_FREE:
//         if (action == do_nothing)
//             dmx_address_state++;
        
//         break;

//     case TO_CLEAN_OUT:
//         resp = resp_need_to_save;
//         *address_show = 1;
//         dmx_address_state = DO_NOTHING;
//         break;
        
//     default:
//         dmx_address_state = DO_NOTHING;
//         break;            
//     }
    
//     return resp;
// }


//--- end of file ---//
