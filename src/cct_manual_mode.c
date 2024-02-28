//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MANUAL_MODE.C #########################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_manual_mode.h"
#include "cct_manual_cct_menu.h"
#include "cct_manual_static_menu.h"
#include "cct_manual_colors_menu.h"
#include "display_utils.h"

#include "colors_functions.h"

#include <string.h>
#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
typedef enum {
    CCT_MANUAL_MODE_INIT = 0,
    CCT_MANUAL_MODE_CCT_MODE,
    CCT_MANUAL_MODE_STATIC_MODE,
    CCT_MANUAL_MODE_PRESET_MODE
    
} cct_manual_mode_state_e;

// variables re-use
#define cct_manual_state    mode_state
#define cct_manual_effect_timer    mode_effect_timer

// Externals -------------------------------------------------------------------
extern unsigned char mode_state;
extern volatile unsigned short mode_effect_timer;


// Globals ---------------------------------------------------------------------
void (* ptFCct_ManualMenuTT ) (void) = NULL;


// Module Private Functions ----------------------------------------------------


// Module Funtions -------------------------------------------------------------
void Cct_ManualMode_UpdateTimers (void)
{
    if (ptFCct_ManualMenuTT != NULL)
        ptFCct_ManualMenuTT();
    
    if (cct_manual_effect_timer)
        cct_manual_effect_timer--;
}


void Cct_ManualModeReset (void)
{
    cct_manual_state = CCT_MANUAL_MODE_INIT;
}


resp_t Cct_ManualMode (parameters_typedef * mem, sw_actions_t actions)
{
    resp_t resp = resp_continue;
    // unsigned char * ch_val;

    switch (cct_manual_state)
    {
    case CCT_MANUAL_MODE_INIT:

        switch (mem->program_inner_type_in_cct)
        {
        case CCT_MANUAL_CCT_MODE:
            ptFCct_ManualMenuTT = &Cct_Manual_Cct_Menu_UpdateTimer;
            Cct_Manual_Cct_Menu_Reset ();
            cct_manual_state = CCT_MANUAL_MODE_CCT_MODE;
            break;

        case CCT_MANUAL_STATIC_MODE:
            ptFCct_ManualMenuTT = &Cct_Manual_Static_Menu_UpdateTimer;
            Cct_Manual_Static_Menu_Reset ();
            cct_manual_state = CCT_MANUAL_MODE_STATIC_MODE;
            break;
            
        case CCT_MANUAL_PRESET_MODE:
            ptFCct_ManualMenuTT = &Cct_Manual_Colors_Menu_UpdateTimer;            
            Cct_Manual_Colors_Menu_Reset ();
            cct_manual_state = CCT_MANUAL_MODE_PRESET_MODE;
            break;
            
            // default to cct dimmer mode
        default:
            mem->program_inner_type_in_cct = CCT_MANUAL_CCT_MODE;
            ptFCct_ManualMenuTT = &Cct_Manual_Cct_Menu_UpdateTimer;
            Cct_Manual_Cct_Menu_Reset ();
            cct_manual_state = CCT_MANUAL_MODE_CCT_MODE;
            break;
        }
        break;

    case CCT_MANUAL_MODE_CCT_MODE:
        resp = Cct_Manual_Cct_Menu (mem, actions);

        if (resp == resp_finish)
        {
            cct_manual_state = CCT_MANUAL_MODE_INIT;
            resp = resp_need_to_save;
        }
        break;
        
    case CCT_MANUAL_MODE_STATIC_MODE:        
        //resp_change translates to resp_change_all_up in this mode
        resp = Cct_Manual_Static_Menu (mem, actions);

        if (resp == resp_change)
            resp = resp_change_all_up;

        break;

    case CCT_MANUAL_MODE_PRESET_MODE:
        //resp_change translates to resp_change_all_up in this mode        
        resp = Cct_Manual_Colors_Menu (mem, actions);

        if (resp == resp_change)
            resp = resp_change_all_up;
        
        break;
        
    default:
        cct_manual_state = CCT_MANUAL_MODE_INIT;
        break;
    }

    return resp;
    
}


//--- end of file ---//
