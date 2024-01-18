//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MANUAL_COLORS_MENU.C #####################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_manual_colors_menu.h"
#include "display_utils.h"
#include "cct_utils.h"

#include <string.h>
#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
char str_colors [20][11] = {"2700K",
                            "3200K",
                            "4000K",
                            "4500K",
                            "5000K",
                            "5500K",
                            "6500K",
                            "Yellow",
                            "Cyan",
                            "Magenta",
                            "Purple",
                            "Pink",
                            "Chocolate",
                            "Salmon",
                            "Amber",
                            "Peach",
                            "Turquoise",
                            "Aqua",
                            "Steel Blue",
                            "Cherry"};

typedef enum {
    CCT_MANUAL_COLORS_MENU_INIT = 0,
    CCT_MANUAL_COLORS_MENU_CHECK_OPTIONS,
    CCT_MANUAL_COLORS_MENU_CHECK_OPTIONS_WAIT_FREE,    
    CCT_MANUAL_COLORS_MENU_SELECT_LINE1,
    CCT_MANUAL_COLORS_MENU_SELECT_LINE2
    
} cct_manual_colors_menu_state_e;

#define TT_SHOW    500
// #define TT_NOT_SHOW    600

// variable re-use
#define cct_selected    menu_selected
#define cct_state    menu_state
#define cct_need_display_update    menu_need_display_update
#define cct_selection_show    menu_selection_show
#define cct_manual_colors_menu_timer    menu_menu_timer

// Externals -------------------------------------------------------------------
extern unsigned char menu_selected;
extern unsigned char menu_state;
extern unsigned char menu_need_display_update;
extern unsigned char menu_selection_show;
extern volatile unsigned short menu_menu_timer;


// Globals ---------------------------------------------------------------------



// Module Private Functions ----------------------------------------------------


// Module Funtions -------------------------------------------------------------
void CCT_Manual_Colors_Menu_UpdateTimer (void)
{
    if (cct_manual_colors_menu_timer)
        cct_manual_colors_menu_timer--;
}


void CCT_Manual_Colors_Menu_Reset (void)
{
    cct_state = CCT_MANUAL_COLORS_MENU_INIT;
}


extern void display_update (void);
resp_t CCT_Manual_Colors_Menu (parameters_typedef * mem, sw_actions_t actions)
{
    static unsigned char showing = 0;
    resp_t resp = resp_continue;
    char s_temp[ALL_LINE_LENGTH_NULL];
    unsigned char dim_int;
    unsigned char dim_dec;
    unsigned short cct_int;    

    switch (cct_state)
    {
    case CCT_MANUAL_COLORS_MENU_INIT:
        Display_StartLines ();
        Display_ClearLines();

        // line 1
        GetPercentage (mem->fixed_channels[0], &dim_int, &dim_dec);
        sprintf(s_temp, "Dimmer: %3d.%01d%%", dim_int, dim_dec);
        Display_SetLine1(s_temp);

        // line 2
        sprintf(s_temp, "%2d -> %s",
                mem->fixed_channels[1] + 1,
                &str_colors[mem->fixed_channels[1]][0]);
        Display_SetLine2(s_temp);

        // bottom line
        Display_SetLine8("         Preset Color");
        
        cct_need_display_update = 1;
        cct_state++;
        break;

    case CCT_MANUAL_COLORS_MENU_CHECK_OPTIONS:
        if (actions == selection_enter)
        {
            cct_state++;
        }
        break;

    case CCT_MANUAL_COLORS_MENU_CHECK_OPTIONS_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state++;
            showing = 1;
        }
        break;
        
    case CCT_MANUAL_COLORS_MENU_SELECT_LINE1:        
        resp = CCT_Utils_Update_Actions_Values (actions, &mem->fixed_channels[0]);

        if (actions == selection_enter)
        {
            cct_state++;
        }

        if (actions != do_nothing)
        {
            cct_manual_colors_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_manual_colors_menu_timer)
        {
            Display_BlankLine1();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                GetPercentage (mem->fixed_channels[0], &dim_int, &dim_dec);
                sprintf(s_temp, "Dimmer: %3d.%01d%%", dim_int, dim_dec);
                Display_SetLine1(s_temp);
            }
            
            cct_manual_colors_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;

    case CCT_MANUAL_COLORS_MENU_SELECT_LINE2:
        // resp = CCT_Utils_Update_Actions_Values (actions, &mem->fixed_channels[1]);

        if (actions == selection_dwn_fast)
        {
            if (mem->fixed_channels[1] >= 4)
                mem->fixed_channels[1] -= 4;
            else
                mem->fixed_channels[1] = 0;

            resp = resp_change;
        }
        else if (actions == selection_dwn)
        {
            if (mem->fixed_channels[1])
                mem->fixed_channels[1] -= 1;

            resp = resp_change;        
        }
        else if (actions == selection_up_fast)
        {
            if (mem->fixed_channels[1] <= 19 - 4)
                mem->fixed_channels[1] += 4;
            else
                mem->fixed_channels[1] = 19;

            resp = resp_change;        
        }
        else if (actions == selection_up)
        {
            if (mem->fixed_channels[1] <= 19 - 1)
                mem->fixed_channels[1] += 1;

            resp = resp_change;        
        }
        
        if (actions == selection_enter)
        {
            cct_state = CCT_MANUAL_COLORS_MENU_CHECK_OPTIONS;
        }

        if (actions != do_nothing)
        {
            cct_manual_colors_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_manual_colors_menu_timer)
        {
            Display_BlankLine2();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                sprintf(s_temp, "%2d -> %s",
                        mem->fixed_channels[1] + 1,
                        &str_colors[mem->fixed_channels[1]][0]);
                Display_SetLine2(s_temp);
            }
            
            cct_manual_colors_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;        
        
    default:
        cct_state = CCT_MANUAL_COLORS_MENU_INIT;
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
