//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MANUAL_CCT_MENU.C #####################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_manual_cct_menu.h"
#include "display_utils.h"
#include "cct_utils.h"

#include <string.h>
#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
typedef enum {
    CCT_MANUAL_CCT_MENU_INIT = 0,
    CCT_MANUAL_CCT_MENU_CHECK_OPTIONS,
    CCT_MANUAL_CCT_MENU_CHECK_OPTIONS_WAIT_FREE,    
    CCT_MANUAL_CCT_MENU_SELECTING,
    CCT_MANUAL_CCT_MENU_SELECTING_WAIT_FREE,    
    CCT_MANUAL_CCT_MENU_CHANGING,
    CCT_MANUAL_CCT_MENU_CHANGING_WAIT_FREE,    
    CCT_MANUAL_CCT_MENU_SELECTED,
    CCT_MANUAL_CCT_MENU_WAIT_FREE
    
} cct_manual_cct_menu_state_e;

#define TT_SHOW    500
#define TT_NOT_SHOW    500

// variable re-use
#define cct_selected    menu_selected
#define cct_state    menu_state
#define cct_need_display_update    menu_need_display_update
#define cct_selection_show    menu_selection_show
#define cct_manual_cct_menu_timer    menu_menu_timer

// Externals -------------------------------------------------------------------
extern unsigned char menu_selected;
extern unsigned char menu_state;
extern unsigned char menu_need_display_update;
extern unsigned char menu_selection_show;
extern volatile unsigned short menu_menu_timer;


// Globals ---------------------------------------------------------------------



// Module Private Functions ----------------------------------------------------


// Module Funtions -------------------------------------------------------------
void ColorsMenu_UpdateTimer (void)
{
    if (cct_manual_cct_menu_timer)
        cct_manual_cct_menu_timer--;
}

void CCT_Manual_Cct_Menu_Reset (void)
{
    cct_state = CCT_MANUAL_CCT_MENU_INIT;
}


extern void display_update (void);
resp_t CCT_Manual_Cct_Menu (parameters_typedef * mem, sw_actions_t actions)
{
    resp_t resp = resp_continue;
    char s_temp[ALL_LINE_LENGTH_NULL];
    unsigned char dim_int;
    unsigned char dim_dec;
    unsigned short cct_int;

    switch (cct_state)
    {
    case CCT_MANUAL_CCT_MENU_INIT:
        Display_StartLines ();
        Display_ClearLines();

        GetPercentage (mem->fixed_channels[0], &dim_int, &dim_dec);
        sprintf(s_temp, "Dimmer: %3d.%01d%%", dim_int, dim_dec);
        Display_SetLine1(s_temp);

        cct_int = GetCct (mem->fixed_channels[1], CCT_MODE_2700_6500);
        sprintf(s_temp, "CCT: %dK", cct_int);
        Display_SetLine2(s_temp);

        sprintf(s_temp, "Green: %3d", mem->fixed_channels[2]);
        Display_SetLine3(s_temp);

        Display_SetLine8("           Manual CCT");

        cct_need_display_update = 1;
        cct_state++;
        break;

    case CCT_MANUAL_CCT_MENU_CHECK_OPTIONS:
        if (actions == selection_enter)
        {
        }
        break;

    case CCT_MANUAL_CCT_MENU_CHECK_OPTIONS_WAIT_FREE:
        if (actions == do_nothing)
        {
            cct_state++;
        }
        break;
        
    case CCT_MANUAL_CCT_MENU_SELECTING:
        // if (actions == selection_dwn)
        // {
        //     if (cct_selected > 0)
        //     {
        //         // clean last option
        //         sprintf(s_temp, "%d", mem->program_inner_type_speed);
        //         ColorsMenu_Options(0, colors_selected, s_temp);

        //         colors_selected--;

        //         // set new option
        //         ColorsMenu_Options(1, colors_selected, "EXIT");

        //         colors_need_display_update = 1;
        //     }
        // }
        
        // if (actions == selection_up)
        // {
        //     if (colors_selected < 1)
        //     {
        //         // clean last option
        //         ColorsMenu_Options(0, colors_selected, "EXIT");
                
        //         colors_selected++;

        //         // set new option                
        //         sprintf(s_temp, "%d", mem->program_inner_type_speed);
        //         ColorsMenu_Options(1, colors_selected, s_temp);

        //         colors_need_display_update = 1;
        //     }
        // }

        // if (actions == selection_enter)
        // {
        //     if (!colors_selected)
        //         colors_state = CCT_MANUAL_CCT_MENU_WAIT_FREE;
        //     else
        //     {
        //         sprintf(s_temp, "%d", mem->program_inner_type_speed);
        //         ColorsMenu_Options(0, colors_selected, s_temp);
        //         colors_state++;
        //     }

        //     colors_need_display_update = 1;
        // }
        
        break;

    case CCT_MANUAL_CCT_MENU_SELECTING_WAIT_FREE:
        // if (actions == do_nothing)
        // {
        //     colors_selection_show = 1;
        //     cct_manual_cct_menu_timer = TT_SHOW;            
        //     colors_state++;
        // }
        
        break;

    case CCT_MANUAL_CCT_MENU_CHANGING:
        // if (actions == selection_dwn)
        // {
        //     unsigned char * p_speed = &mem->program_inner_type_speed;

        //     if (*p_speed)
        //     {
        //         *p_speed -= 1;
        //         sprintf(s_temp, "%d", *p_speed);
        //         ColorsMenu_Options(0, colors_selected, s_temp);

        //         resp = resp_change;

        //         colors_selection_show = 1;
        //         cct_manual_cct_menu_timer = TT_SHOW;
        //         colors_need_display_update = 1;
        //     }
        // }
        
        // if (actions == selection_up)
        // {
        //     unsigned char * p_speed = &mem->program_inner_type_speed;

        //     if (*p_speed < 9)
        //     {
        //         *p_speed += 1;
        //         sprintf(s_temp, "%d", *p_speed);
        //         ColorsMenu_Options(0, colors_selected, s_temp);

        //         resp = resp_change;                

        //         colors_selection_show = 1;
        //         cct_manual_cct_menu_timer = TT_SHOW;
        //         colors_need_display_update = 1;
        //     }
        // }

        // if (colors_selection_show)
        // {
        //     if (!cct_manual_cct_menu_timer)
        //     {
        //         colors_selection_show = 0;
        //         cct_manual_cct_menu_timer = TT_NOT_SHOW;
        //         ColorsMenu_Options(0, colors_selected, "");
        //         colors_need_display_update = 1;
        //     }
        // }
        // else
        // {
        //     if (!cct_manual_cct_menu_timer)
        //     {
        //         unsigned char * p_speed = &mem->program_inner_type_speed;
                
        //         colors_selection_show = 1;
        //         cct_manual_cct_menu_timer = TT_SHOW;
        //         sprintf(s_temp, "%d", *p_speed);
        //         ColorsMenu_Options(0, colors_selected, s_temp);
        //         colors_need_display_update = 1;
        //     }
        // }

        // if (actions == selection_enter)
        // {
        //     unsigned char * p_speed = &mem->program_inner_type_speed;            

        //     sprintf(s_temp, "%d", *p_speed);
        //     ColorsMenu_Options(1, colors_selected, s_temp);
            
        //     colors_need_display_update = 1;
        //     colors_state++;
        // }        
        break;

    case CCT_MANUAL_CCT_MENU_CHANGING_WAIT_FREE:
        // if (actions == do_nothing)
        // {
        //     colors_state = CCT_MANUAL_CCT_MENU_SELECTING;            
        //     resp = resp_need_to_save;
        // }
        break;

    case CCT_MANUAL_CCT_MENU_SELECTED:
        // colors_state++;
        break;

    case CCT_MANUAL_CCT_MENU_WAIT_FREE:
        // if (actions == do_nothing)
        // {
        //     colors_state = CCT_MANUAL_CCT_MENU_INIT;
        //     resp = resp_finish;            
        // }
        break;
        
    default:
        // colors_state = CCT_MANUAL_CCT_MENU_INIT;
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
