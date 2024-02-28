//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MAIN_MENU.C #####################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_main_menu.h"
#include "display_utils.h"
#include "cct_utils.h"

#include <string.h>
#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
char str_main_modes [5][14] = {"DMX",
                               "MASTER/SLAVE",
                               "MANUAL CCT",
                               "MANUAL STATIC",
                               "MANUAL PRESET"};


typedef enum {
    CCT_MAIN_MENU_INIT = 0,
    CCT_MAIN_MENU_CHECK_OPTIONS,
    CCT_MAIN_MENU_CHECK_OPTIONS_WAIT_FREE,    
    CCT_MAIN_MENU_SELECT_OPT1
    
} cct_main_menu_state_e;

#define TT_SHOW    500
// #define TT_NOT_SHOW    600

// variable re-use
#define cct_selected    menu_selected
#define cct_state    menu_state
#define cct_need_display_update    menu_need_display_update
#define cct_selection_show    menu_selection_show
#define cct_main_menu_timer    menu_menu_timer

// Externals -------------------------------------------------------------------
extern unsigned char menu_selected;
extern unsigned char menu_state;
extern unsigned char menu_need_display_update;
extern unsigned char menu_selection_show;
extern volatile unsigned short menu_menu_timer;


// Globals ---------------------------------------------------------------------



// Module Private Functions ----------------------------------------------------


// Module Funtions -------------------------------------------------------------
void Cct_Main_Menu_UpdateTimer (void)
{
    if (cct_main_menu_timer)
        cct_main_menu_timer--;
}


void Cct_Main_Menu_Reset (void)
{
    cct_state = CCT_MAIN_MENU_INIT;
}


extern void display_update (void);
resp_t Cct_Main_Menu (parameters_typedef * mem, sw_actions_t actions)
{
    static unsigned char showing = 0;
    resp_t resp = resp_continue;
    char s_temp[ALL_LINE_LENGTH_NULL];

    switch (cct_state)
    {
    case CCT_MAIN_MENU_INIT:
        Display_StartLines ();
        Display_ClearLines();

        // line 1
        Display_SetLine1("MODE:");

        // line 2
        if ((mem->program_inner_type_in_cct < CCT_DMX_MODE) ||
            (mem->program_inner_type_in_cct > CCT_MANUAL_PRESET_MODE))
            mem->program_inner_type_in_cct = CCT_DMX_MODE;
        
        sprintf(s_temp, "%s",
                &str_main_modes[mem->program_inner_type_in_cct - CCT_DMX_MODE][0]);
        Display_SetLine3(s_temp);

        // bottom line
        Display_SetLine8("            Main Menu");        
        
        cct_need_display_update = 1;
        cct_state++;
        break;

    case CCT_MAIN_MENU_CHECK_OPTIONS:
        if (actions == selection_enter)
        {
            cct_state++;
        }
        break;

    case CCT_MAIN_MENU_CHECK_OPTIONS_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state++;
            showing = 1;
        }
        break;
        
    case CCT_MAIN_MENU_SELECT_OPT1:
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
            cct_state = CCT_MAIN_MENU_CHECK_OPTIONS;
            resp = resp_change;
        }

        if (actions != do_nothing)
        {
            cct_main_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_main_menu_timer)
        {
            Display_BlankLine3();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                sprintf(s_temp, "%s",
                        &str_main_modes[mem->program_inner_type_in_cct - CCT_DMX_MODE][0]);
                Display_SetLine3(s_temp);
            }
            
            cct_main_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;
        
    default:
        cct_state = CCT_MAIN_MENU_INIT;
        break;
    }

    if (cct_need_display_update)
    {
        display_update();
        cct_need_display_update = 0;
    }

    return resp;
    
}


//--- end of file ---//
