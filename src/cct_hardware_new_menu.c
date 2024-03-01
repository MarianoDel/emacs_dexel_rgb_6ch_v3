//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_HARDWARE_NEW_MENU.C ###################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_hardware_new_menu.h"
#include "display_utils.h"
#include "ssd1306_display.h"
#include "cct_utils.h"

#include <string.h>
#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
char str_hard_modes [5][20] = {"CURRENT IN CHANNELS",
                               "CURRENT LIMIT",
                               "TEMP CONFIG",
                               "ENC DIR / DMX MENU",
                               "VERSION"};

typedef enum {
    CCT_HARDWARE_MENU_INIT = 0,
    CCT_HARDWARE_MENU_CHECK_OPTIONS,
    CCT_HARDWARE_MENU_CHECK_OPTIONS_WAIT_FREE,
    CCT_HARDWARE_MENU_SELECT_OPT1
    
} cct_hardware_menu_state_e;

#define TT_SHOW    500

// variable re-use
#define cct_hard_selected    menu_selected
#define cct_hard_state    menu_state
#define cct_hard_need_display_update    menu_need_display_update
#define cct_hard_selection_show    menu_selection_show
#define cct_hard_menu_timer    menu_menu_timer

// Externals -------------------------------------------------------------------
extern unsigned char menu_selected;
extern unsigned char menu_state;
extern unsigned char menu_need_display_update;
extern unsigned char menu_selection_show;
extern volatile unsigned short menu_menu_timer;



// Globals ---------------------------------------------------------------------
// void (* ptFCctHardNewMenu ) (void) = NULL;


// Module Private Functions ----------------------------------------------------


// Module Funtions -------------------------------------------------------------
void Cct_Hardware_New_Menu_UpdateTimers (void)
{
    if (cct_hard_menu_timer)
        cct_hard_menu_timer--;
    // if (ptFCctHardNewMenu != NULL)
    //     ptFCctHardNewMenu();
    
}


void Cct_Hardware_New_Menu_Reset (void)
{
    cct_hard_state = CCT_HARDWARE_MENU_INIT;
}


resp_t Cct_Hardware_New_Menu (parameters_typedef * mem, sw_actions_t actions)
{
    static unsigned char showing = 0;
    resp_t resp = resp_continue;
    char s_temp[ALL_LINE_LENGTH_NULL];

    switch (cct_hard_state)
    {
    case CCT_HARDWARE_MENU_INIT:
        // call former menu
        
        cct_hard_state++;
        break;

    case CCT_HARDWARE_MENU_CHECK_OPTIONS:
        if (actions == selection_enter)
        {
            cct_hard_state++;
        }
        break;

    case CCT_HARDWARE_MENU_CHECK_OPTIONS_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_hard_state++;
            showing = 1;
        }
        break;
        
    case CCT_HARDWARE_MENU_SELECT_OPT1:
        // resp = CCT_Utils_Update_Actions_Values (actions, &mem->fixed_channels[0]);

        if ((actions == selection_dwn) ||
            (actions == selection_dwn_fast))
        {
            if (mem->program_inner_type_in_cct > CCT_DMX_MODE)
                mem->program_inner_type_in_cct -= 1;

        }
        else if ((actions == selection_up) ||
                 (actions == selection_up_fast))
        {
            if (mem->program_inner_type_in_cct < CCT_MANUAL_PRESET_MODE)
                mem->program_inner_type_in_cct += 1;

        }
        
        if (actions == selection_enter)
        {
            cct_hard_state = CCT_HARDWARE_MENU_CHECK_OPTIONS;
            resp = resp_change;
        }

        if (actions != do_nothing)
        {
            cct_hard_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_hard_menu_timer)
        {
            Display_BlankLine3();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                sprintf(s_temp, "%s",
                        &str_hard_modes[mem->program_inner_type_in_cct - CCT_DMX_MODE][0]);
                Display_SetLine3(s_temp);
            }
            
            cct_hard_menu_timer = TT_SHOW;
            cct_hard_need_display_update = 1;
        }
        break;
        
    default:
        cct_hard_state = CCT_HARDWARE_MENU_INIT;
        break;
    }

    if (cct_hard_need_display_update)
    {
        display_update();
        cct_hard_need_display_update = 0;
    }

    return resp;
}


//--- end of file ---//
