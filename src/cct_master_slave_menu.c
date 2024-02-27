//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MASTER_SLAVE_MENU.C #####################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_master_slave_menu.h"
#include "display_utils.h"
#include "cct_utils.h"

#include <string.h>
#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
char str_global_mode [2][7] = {"MASTER",
                               "SLAVE"};

char str_modes [3][7] = {"Cct",
                         "Static",
                         "Preset"};


typedef enum {
    CCT_MASTER_SLAVE_MENU_INIT = 0,
    CCT_MASTER_SLAVE_MENU_CHECK_OPTIONS,
    CCT_MASTER_SLAVE_MENU_CHECK_OPTIONS_WAIT_FREE,    
    CCT_MASTER_SLAVE_MENU_SELECT_OPT1,
    CCT_MASTER_SLAVE_MENU_SELECT_OPT2    
    
} cct_master_slave_menu_state_e;

#define TT_SHOW    500
// #define TT_NOT_SHOW    600

// variable re-use
#define cct_selected    menu_selected
#define cct_state    menu_state
#define cct_need_display_update    menu_need_display_update
#define cct_selection_show    menu_selection_show
#define cct_master_slave_menu_timer    menu_menu_timer

// Externals -------------------------------------------------------------------
extern unsigned char menu_selected;
extern unsigned char menu_state;
extern unsigned char menu_need_display_update;
extern unsigned char menu_selection_show;
extern volatile unsigned short menu_menu_timer;


// Globals ---------------------------------------------------------------------



// Module Private Functions ----------------------------------------------------


// Module Funtions -------------------------------------------------------------
void CCT_Master_Slave_Menu_UpdateTimer (void)
{
    if (cct_master_slave_menu_timer)
        cct_master_slave_menu_timer--;
}


void CCT_Master_Slave_Menu_Reset (void)
{
    cct_state = CCT_MASTER_SLAVE_MENU_INIT;
}


extern void display_update (void);
resp_t CCT_Master_Slave_Menu (parameters_typedef * mem, sw_actions_t actions)
{
    static unsigned char showing = 0;
    resp_t resp = resp_continue;
    char s_temp[ALL_LINE_LENGTH_NULL];

    switch (cct_state)
    {
    case CCT_MASTER_SLAVE_MENU_INIT:
        Display_StartLines ();
        Display_ClearLines();

        // line 1
        Display_SetLine1("MODE:");

        // line 2
        sprintf(s_temp, "%s", &str_global_mode[mem->program_type][0]);
        Display_SetLine2(s_temp);

        // line 3
        if (mem->program_type == 0)    // suppose master
        {
            sprintf(s_temp, "%s", &str_modes[mem->program_inner_type][0]);
            Display_SetLine3(s_temp);
        }
        
        // bottom line
        Display_SetLine8("    Master/Slave Menu");
        
        cct_need_display_update = 1;
        cct_state++;
        break;

    case CCT_MASTER_SLAVE_MENU_CHECK_OPTIONS:
        if (actions == selection_enter)
        {
            cct_state++;
        }
        break;

    case CCT_MASTER_SLAVE_MENU_CHECK_OPTIONS_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state++;
            showing = 1;
        }
        break;

    case CCT_MASTER_SLAVE_MENU_SELECT_OPT1:
        // resp = CCT_Utils_Update_Actions_Values (actions, &mem->fixed_channels[0]);

        if ((actions == selection_dwn) ||
            (actions == selection_dwn_fast))
        {
            if (mem->program_type)
                mem->program_type -= 1;

            //here we are sure in MASTER
            Display_BlankLine3();
            sprintf(s_temp, "%s", &str_modes[mem->program_inner_type][0]);
            Display_SetLine3(s_temp);

        }
        else if ((actions == selection_up) ||
                 (actions == selection_up_fast))
        {
            if (!mem->program_type)
                mem->program_type += 1;

            //here we are sure in SLAVE
            Display_BlankLine3();
        }
        
        if (actions == selection_enter)
        {
            if (!mem->program_type)
                cct_state++;
            else
                cct_state = CCT_MASTER_SLAVE_MENU_CHECK_OPTIONS;
            
            resp = resp_change;
        }

        if (actions != do_nothing)
        {
            cct_master_slave_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_master_slave_menu_timer)
        {
            Display_BlankLine2();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                sprintf(s_temp, "%s", &str_global_mode[mem->program_type][0]);
                Display_SetLine2(s_temp);
            }
            
            cct_master_slave_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;

    case CCT_MASTER_SLAVE_MENU_SELECT_OPT2:
        // resp = CCT_Utils_Update_Actions_Values (actions, &mem->fixed_channels[0]);

        if ((actions == selection_dwn) ||
            (actions == selection_dwn_fast))
        {
            if (mem->program_inner_type)
                mem->program_inner_type -= 1;

        }
        else if ((actions == selection_up) ||
                 (actions == selection_up_fast))
        {
            if (mem->program_inner_type <= 1)
                mem->program_inner_type += 1;

        }
        
        if (actions == selection_enter)
        {
            cct_state = CCT_MASTER_SLAVE_MENU_CHECK_OPTIONS;
            resp = resp_change;
        }

        if (actions != do_nothing)
        {
            cct_master_slave_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_master_slave_menu_timer)
        {
            Display_BlankLine3();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                sprintf(s_temp, "%s", &str_modes[mem->program_inner_type][0]);
                Display_SetLine3(s_temp);
            }
            
            cct_master_slave_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;
        
    default:
        cct_state = CCT_MASTER_SLAVE_MENU_INIT;
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
