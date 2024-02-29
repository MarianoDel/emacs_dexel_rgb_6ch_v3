//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### HARDWARE_MENU.C ###########################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_hardware_new_menu.h"
#include "current_menu.h"
#include "limits_menu.h"
#include "channels_menu.h"
#include "temp_menu.h"
#include "cct_enc_dir_mode_menu.h"
#include "version_menu.h"
#include "options_menu.h"

#include <string.h>
#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
typedef enum {
    CCT_HARDWARE_MENU_INIT = 0,
    CCT_HARDWARE_MENU_SELECT,
    CCT_HARDWARE_MENU_CURRENTS,
    CCT_HARDWARE_MENU_LIMIT,
    CCT_HARDWARE_MENU_CHANNELS,
    CCT_HARDWARE_MENU_TEMP,
    CCT_HARDWARE_MENU_ENC_DIR_CCT,
    CCT_HARDWARE_MENU_VERSION
    
} cct_hardware_menu_state_e;

// variables re-use
#define cct_hardware_menu_state    menu_state

// Externals -------------------------------------------------------------------
extern unsigned char menu_state;
extern options_menu_st mem_options;


// Globals ---------------------------------------------------------------------
void (* ptFCctHardNewMenu ) (void) = NULL;

// Module Private Functions ----------------------------------------------------


// Module Funtions -------------------------------------------------------------
void Cct_Hardware_New_Menu_UpdateTimers (void)
{
    if (ptFCctHardNewMenu != NULL)
        ptFCctHardNewMenu();
    
}


void Cct_Hardware_New_Menu_Reset (void)
{
    cct_hardware_menu_state = CCT_HARDWARE_MENU_INIT;
}


resp_t Cct_Hardware_New_Menu (parameters_typedef * mem, sw_actions_t actions)
{
    resp_t resp = resp_continue;

    switch (cct_hardware_menu_state)
    {
    case CCT_HARDWARE_MENU_INIT:
        mem_options.argv[0] = "CURRENT IN CHANNELS";
        mem_options.argv[1] = "CURRENT LIMIT";
        mem_options.argv[2] = "CHANNELS SELECTION";
        mem_options.argv[3] = "TEMP CONFIG";
        mem_options.argv[4] = "ENC DIR / CCT MENU";
        mem_options.argv[5] = "VERSION";
        mem_options.argv[6] = "EXIT";        
        mem_options.options_qtty = 7;
        mem_options.argv[7] = "        Cct_Hardware Menu";
        OptionsMenuReset();

        cct_hardware_menu_state++;
        break;

    case CCT_HARDWARE_MENU_SELECT:
        resp = OptionsMenu(&mem_options, actions);
        
        if (resp == resp_finish)
        {
            resp = resp_continue;
            
            switch (mem_options.options_selected)
            {
            case 0:
                cct_hardware_menu_state = CCT_HARDWARE_MENU_CURRENTS;
                ptFCctHardNewMenu = &CurrentMenu_UpdateTimer;
                CurrentMenuReset();
                break;

            case 1:
                cct_hardware_menu_state = CCT_HARDWARE_MENU_LIMIT;
                ptFCctHardNewMenu = &LimitsMenu_UpdateTimer;                
                LimitsMenuReset();
                break;

            case 2:
                cct_hardware_menu_state = CCT_HARDWARE_MENU_CHANNELS;
                ptFCctHardNewMenu = &ChannelsMenu_UpdateTimer;                
                ChannelsMenuReset();
                break;

            case 3:
                cct_hardware_menu_state = CCT_HARDWARE_MENU_TEMP;
                ptFCctHardNewMenu = &TempMenu_UpdateTimer;                
                TempMenuReset();
                break;

            case 4:
                cct_hardware_menu_state = CCT_HARDWARE_MENU_ENC_DIR_CCT;
                Cct_Enc_Dir_Mode_Menu_Reset ();
                break;

            case 5:
                cct_hardware_menu_state = CCT_HARDWARE_MENU_VERSION;
                ptFCctHardNewMenu = &VersionMenu_UpdateTimer;
                VersionMenuReset();
                break;
                
            case 6:
                cct_hardware_menu_state = CCT_HARDWARE_MENU_INIT;
                ptFCctHardNewMenu = NULL;
                resp = resp_finish;
                break;
                
            default:
                cct_hardware_menu_state = CCT_HARDWARE_MENU_INIT;
                ptFCctHardNewMenu = NULL;
                break;
                
            }
        }
        break;

    case CCT_HARDWARE_MENU_CURRENTS:
        resp = CurrentMenu(mem, actions);

        if (resp == resp_finish)
        {
            cct_hardware_menu_state = CCT_HARDWARE_MENU_INIT;
            resp = resp_continue;
        }
        break;

    case CCT_HARDWARE_MENU_LIMIT:
        resp = LimitsMenu(mem, actions);

        if (resp == resp_finish)
        {
            cct_hardware_menu_state = CCT_HARDWARE_MENU_INIT;
            resp = resp_continue;
        }
        break;

    case CCT_HARDWARE_MENU_CHANNELS:
        resp = ChannelsMenu(mem, actions);

        if (resp == resp_finish)
        {
            cct_hardware_menu_state = CCT_HARDWARE_MENU_INIT;
            resp = resp_continue;
        }
        break;

    case CCT_HARDWARE_MENU_TEMP:
        resp = TempMenu(mem, actions);

        if (resp == resp_finish)
        {
            cct_hardware_menu_state = CCT_HARDWARE_MENU_INIT;
            resp = resp_continue;
        }
        break;

    case CCT_HARDWARE_MENU_ENC_DIR_CCT:
        resp = Cct_Enc_Dir_Mode_Menu (mem, actions);

        if (resp == resp_finish)
        {
            cct_hardware_menu_state = CCT_HARDWARE_MENU_INIT;
            resp = resp_continue;
        }
        break;
        
    case CCT_HARDWARE_MENU_VERSION:
        resp = VersionMenu(mem, actions);

        if (resp == resp_finish)
        {
            cct_hardware_menu_state = CCT_HARDWARE_MENU_INIT;
            resp = resp_continue;
        }
        break;
        
    default:
        cct_hardware_menu_state = CCT_HARDWARE_MENU_INIT;
        break;
    }

    return resp;
    
}


//--- end of file ---//
