//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MANUAL_STATIC_MENU.C #####################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_manual_static_menu.h"
#include "display_utils.h"
#include "cct_utils.h"

#include <string.h>
#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
typedef enum {
    CCT_MANUAL_STATIC_MENU_INIT = 0,
    CCT_MANUAL_STATIC_MENU_CHECK_OPTIONS,
    CCT_MANUAL_STATIC_MENU_CHECK_OPTIONS_WAIT_FREE,    
    CCT_MANUAL_STATIC_MENU_SELECT_OPT1,
    CCT_MANUAL_STATIC_MENU_SELECT_OPT1_WAIT_FREE,    
    CCT_MANUAL_STATIC_MENU_SELECT_OPT2,
    CCT_MANUAL_STATIC_MENU_SELECT_OPT2_WAIT_FREE,
    CCT_MANUAL_STATIC_MENU_SELECT_OPT3,
    CCT_MANUAL_STATIC_MENU_SELECT_OPT3_WAIT_FREE,    
    CCT_MANUAL_STATIC_MENU_SELECT_OPT4,
    CCT_MANUAL_STATIC_MENU_SELECT_OPT4_WAIT_FREE,    
    CCT_MANUAL_STATIC_MENU_SELECT_OPT5,
    CCT_MANUAL_STATIC_MENU_SELECT_OPT5_WAIT_FREE,    
    CCT_MANUAL_STATIC_MENU_SELECT_OPT6,
    CCT_MANUAL_STATIC_MENU_SELECT_OPT6_WAIT_FREE,    
    CCT_MANUAL_STATIC_MENU_SELECT_OPT7,
    CCT_MANUAL_STATIC_MENU_SELECT_OPT7_WAIT_FREE
    
} cct_manual_static_menu_state_e;

#define TT_SHOW    500
// #define TT_NOT_SHOW    600

// variable re-use
#define cct_selected    menu_selected
#define cct_state    menu_state
#define cct_need_display_update    menu_need_display_update
#define cct_selection_show    menu_selection_show
#define cct_manual_static_menu_timer    menu_menu_timer

// Externals -------------------------------------------------------------------
extern unsigned char menu_selected;
extern unsigned char menu_state;
extern unsigned char menu_need_display_update;
extern unsigned char menu_selection_show;
extern volatile unsigned short menu_menu_timer;


// Globals ---------------------------------------------------------------------
volatile unsigned short cct_manual_static_strobe_timer = 0;


// Module Private Functions ----------------------------------------------------


// Module Funtions -------------------------------------------------------------
void Cct_Manual_Static_Menu_UpdateTimer (void)
{
    if (cct_manual_static_menu_timer)
        cct_manual_static_menu_timer--;

    if (cct_manual_static_strobe_timer)
        cct_manual_static_strobe_timer--;
}


void Cct_Manual_Static_Menu_Reset (void)
{
    cct_state = CCT_MANUAL_STATIC_MENU_INIT;
}


#define STB_FLAG_RUNNING    0
#define STB_FLAG_END    1
#define STB_FLAG_ENDED    2
extern void display_update (void);
resp_t Cct_Manual_Static_Menu (parameters_typedef * mem, sw_actions_t actions)
{
    static unsigned char showing = 0;
    static unsigned char strobe_flag = STB_FLAG_ENDED;
    static unsigned char strobe_flag_on = 0;    
    resp_t resp = resp_continue;
    char s_temp[ALL_LINE_LENGTH_NULL];

    switch (cct_state)
    {
    case CCT_MANUAL_STATIC_MENU_INIT:
        Display_StartLines ();
        Display_ClearLines();

        // line 1
        sprintf(s_temp, "RED: %3d", mem->dimmed_channels[0]);
        Display_SetLine1(s_temp);

        // line 2
        sprintf(s_temp, "GRN: %3d", mem->dimmed_channels[1]);
        Display_SetLine2(s_temp);

        // line 3
        sprintf(s_temp, "BLU: %3d", mem->dimmed_channels[2]);
        Display_SetLine3(s_temp);

        // line 4
        sprintf(s_temp, " WW: %3d", mem->dimmed_channels[3]);
        Display_SetLine4(s_temp);

        // line 5
        sprintf(s_temp, " CW: %3d", mem->dimmed_channels[4]);
        Display_SetLine5(s_temp);

        // line 6
        sprintf(s_temp, "STB: %3d", mem->cct_strobe);
        Display_SetLine6(s_temp);

        // line 7
        sprintf(s_temp, "DIM: %3d", mem->cct_dimmer);
        // sprintf(s_temp, "Dimmer: %3d", mem->cct_dimmer);        
        Display_SetLine7(s_temp);
        
        // bottom line
        Display_SetLine8("        Manual Static");        

        // update all colors
        for (int i = 0; i < 5; i++)
        {
            mem->fixed_channels[i] = Cct_Utils_Dim_Color (
                mem->cct_dimmer,
                mem->dimmed_channels[i]);
        }
        resp = resp_change;
        
        cct_need_display_update = 1;
        cct_state++;
        break;

    case CCT_MANUAL_STATIC_MENU_CHECK_OPTIONS:
        if (actions == selection_enter)
        {
            cct_state++;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_CHECK_OPTIONS_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state++;
            showing = 1;
        }
        break;
        
    case CCT_MANUAL_STATIC_MENU_SELECT_OPT1:
        resp = Cct_Utils_Update_Actions_Values (actions, &mem->dimmed_channels[0]);

        // update this color
        if (resp == resp_change)
        {
            mem->fixed_channels[0] = Cct_Utils_Dim_Color (
                mem->cct_dimmer,
                mem->dimmed_channels[0]);
        }

        if (actions == selection_enter)
        {
            cct_state++;
        }

        if (actions != do_nothing)
        {
            cct_manual_static_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_manual_static_menu_timer)
        {
            Display_BlankLine1();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                sprintf(s_temp, "RED: %3d", mem->dimmed_channels[0]);
                Display_SetLine1(s_temp);
            }
            
            cct_manual_static_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_SELECT_OPT1_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state++;
            showing = 1;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_SELECT_OPT2:
        resp = Cct_Utils_Update_Actions_Values (actions, &mem->dimmed_channels[1]);

        // update this color
        if (resp == resp_change)
        {
            mem->fixed_channels[1] = Cct_Utils_Dim_Color (
                mem->cct_dimmer,
                mem->dimmed_channels[1]);
        }

        if (actions == selection_enter)
        {
            cct_state++;
        }

        if (actions != do_nothing)
        {
            cct_manual_static_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_manual_static_menu_timer)
        {
            Display_BlankLine2();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                sprintf(s_temp, "GRN: %3d", mem->dimmed_channels[1]);
                Display_SetLine2(s_temp);
            }
            
            cct_manual_static_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_SELECT_OPT2_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state++;
            showing = 1;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_SELECT_OPT3:
        resp = Cct_Utils_Update_Actions_Values (actions, &mem->dimmed_channels[2]);

        // update this color
        if (resp == resp_change)
        {
            mem->fixed_channels[2] = Cct_Utils_Dim_Color (
                mem->cct_dimmer,
                mem->dimmed_channels[2]);
        }

        if (actions == selection_enter)
        {
            cct_state++;
        }

        if (actions != do_nothing)
        {
            cct_manual_static_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_manual_static_menu_timer)
        {
            Display_BlankLine3();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                sprintf(s_temp, "BLU: %3d", mem->dimmed_channels[2]);
                Display_SetLine3(s_temp);
            }
            
            cct_manual_static_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_SELECT_OPT3_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state++;
            showing = 1;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_SELECT_OPT4:
        resp = Cct_Utils_Update_Actions_Values (actions, &mem->dimmed_channels[3]);

        // update this color
        if (resp == resp_change)
        {
            mem->fixed_channels[3] = Cct_Utils_Dim_Color (
                mem->cct_dimmer,
                mem->dimmed_channels[3]);
        }

        if (actions == selection_enter)
        {
            cct_state++;
        }

        if (actions != do_nothing)
        {
            cct_manual_static_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_manual_static_menu_timer)
        {
            Display_BlankLine4();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                sprintf(s_temp, " WW: %3d", mem->dimmed_channels[3]);
                Display_SetLine4(s_temp);
            }
            
            cct_manual_static_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_SELECT_OPT4_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state++;
            showing = 1;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_SELECT_OPT5:
        resp = Cct_Utils_Update_Actions_Values (actions, &mem->dimmed_channels[4]);

        // update this color
        if (resp == resp_change)
        {
            mem->fixed_channels[4] = Cct_Utils_Dim_Color (
                mem->cct_dimmer,
                mem->dimmed_channels[4]);
        }

        if (actions == selection_enter)
        {
            cct_state++;
        }

        if (actions != do_nothing)
        {
            cct_manual_static_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_manual_static_menu_timer)
        {
            Display_BlankLine5();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                sprintf(s_temp, " CW: %3d", mem->dimmed_channels[4]);
                Display_SetLine5(s_temp);
            }
            
            cct_manual_static_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_SELECT_OPT5_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state++;
            showing = 1;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_SELECT_OPT6:
        resp = Cct_Utils_Update_Actions_Values (actions, &mem->cct_strobe);

        if (resp == resp_change)
        {
            resp = resp_continue;
            
            if (mem->cct_strobe)
            {
                // disable fast filters
                mem->program_inner_type = DMX2_INNER_STROBE_MODE;
                strobe_flag = STB_FLAG_RUNNING;
            }
            else
            {
                // enable filters
                mem->program_inner_type = MANUAL_NO_INNER_MODE;
                strobe_flag = STB_FLAG_END;
            }
        }

        if (actions == selection_enter)
        {
            cct_state++;
        }

        if (actions != do_nothing)
        {
            cct_manual_static_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_manual_static_menu_timer)
        {
            Display_BlankLine6();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                sprintf(s_temp, "STB: %3d", mem->cct_strobe);
                Display_SetLine6(s_temp);
            }
            
            cct_manual_static_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_SELECT_OPT6_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state++;
            showing = 1;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_SELECT_OPT7:
        resp = Cct_Utils_Update_Actions_Values (actions, &mem->cct_dimmer);

        // update all colors if not in strobe, else the strobe func will update the dimmer
        if (resp == resp_change)
        {
            if (strobe_flag != STB_FLAG_RUNNING)
            {
                for (int i = 0; i < 5; i++)
                {
                    mem->fixed_channels[i] = Cct_Utils_Dim_Color (
                        mem->cct_dimmer,
                        mem->dimmed_channels[i]);
                }
            }
            else
                resp = resp_continue;
        }
        
        if (actions == selection_enter)
        {
            cct_state++;
        }

        if (actions != do_nothing)
        {
            cct_manual_static_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_manual_static_menu_timer)
        {
            Display_BlankLine7();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                sprintf(s_temp, "DIM: %3d", mem->cct_dimmer);
                Display_SetLine7(s_temp);
            }
            
            cct_manual_static_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;

    case CCT_MANUAL_STATIC_MENU_SELECT_OPT7_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state = CCT_MANUAL_STATIC_MENU_CHECK_OPTIONS;
        }
        break;
        
        
    default:
        cct_state = CCT_MANUAL_STATIC_MENU_INIT;
        break;
    }

    if (cct_need_display_update)
    {
        display_update();
        cct_need_display_update = 0;
    }

    // Strobe functions
    if (strobe_flag == STB_FLAG_RUNNING)
    {
        if (!cct_manual_static_strobe_timer)
        {
            if (!strobe_flag_on)
            {
                // update all colors
                for (int i = 0; i < 5; i++)
                {
                    mem->fixed_channels[i] = Cct_Utils_Dim_Color (
                        mem->cct_dimmer,
                        mem->dimmed_channels[i]);
                }
                strobe_flag_on = 1;
            }
            else
            {
                for (int i = 0; i < 5; i++)
                    mem->fixed_channels[i] = 0;

                strobe_flag_on = 0;
            }
        
            cct_manual_static_strobe_timer = 1060 - mem->cct_strobe * 4;
            resp = resp_change;
        }
    }
    else if (strobe_flag == STB_FLAG_END)
    {
        // update all colors
        for (int i = 0; i < 5; i++)
        {
            mem->fixed_channels[i] = Cct_Utils_Dim_Color (
                mem->cct_dimmer,
                mem->dimmed_channels[i]);
        }
        cct_manual_static_strobe_timer = 0;
        resp = resp_change;
        strobe_flag = STB_FLAG_ENDED;
    }

    return resp;
    
}


//--- end of file ---//
