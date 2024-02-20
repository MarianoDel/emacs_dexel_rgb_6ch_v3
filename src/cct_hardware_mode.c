//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### HARDWARE_MODE.C ###########################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "hardware_mode.h"
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
    HARDWARE_MODE_INIT = 0,
    HARDWARE_MODE_SELECT,
    HARDWARE_MODE_CURRENTS,
    HARDWARE_MODE_LIMIT,
    HARDWARE_MODE_CHANNELS,
    HARDWARE_MODE_TEMP,
    HARDWARE_MODE_ENC_DIR_CCT,
    HARDWARE_MODE_VERSION
    
} hardware_mode_state_e;

// variables re-use
#define hardware_mode_state    mode_state

// Externals -------------------------------------------------------------------
extern unsigned char mode_state;
extern options_menu_st mem_options;


// Globals ---------------------------------------------------------------------
void (* ptFCctHardMenuTT ) (void) = NULL;

// Module Private Functions ----------------------------------------------------


// Module Funtions -------------------------------------------------------------
void Cct_HardwareMode_UpdateTimers (void)
{
    if (ptFCctHardMenuTT != NULL)
        ptFCctHardMenuTT();
    
}


void Cct_HardwareModeReset (void)
{
    hardware_mode_state = HARDWARE_MODE_INIT;
}


resp_t Cct_HardwareMode (parameters_typedef * mem, sw_actions_t actions)
{
    resp_t resp = resp_continue;

    switch (hardware_mode_state)
    {
    case HARDWARE_MODE_INIT:
        mem_options.argv[0] = "CURRENT IN CHANNELS";
        mem_options.argv[1] = "CURRENT LIMIT";
        mem_options.argv[2] = "CHANNELS SELECTION";
        mem_options.argv[3] = "TEMP CONFIG";
        mem_options.argv[4] = "ENC DIR / CCT MODE";
        mem_options.argv[5] = "VERSION";
        mem_options.argv[6] = "EXIT";        
        mem_options.options_qtty = 7;
        mem_options.argv[7] = "        Hardware Mode";
        OptionsMenuReset();

        hardware_mode_state++;
        break;

    case HARDWARE_MODE_SELECT:
        resp = OptionsMenu(&mem_options, actions);
        
        if (resp == resp_finish)
        {
            resp = resp_continue;
            
            switch (mem_options.options_selected)
            {
            case 0:
                hardware_mode_state = HARDWARE_MODE_CURRENTS;
                ptFCctHardMenuTT = &CurrentMenu_UpdateTimer;
                CurrentMenuReset();
                break;

            case 1:
                hardware_mode_state = HARDWARE_MODE_LIMIT;
                ptFCctHardMenuTT = &LimitsMenu_UpdateTimer;                
                LimitsMenuReset();
                break;

            case 2:
                hardware_mode_state = HARDWARE_MODE_CHANNELS;
                ptFCctHardMenuTT = &ChannelsMenu_UpdateTimer;                
                ChannelsMenuReset();
                break;

            case 3:
                hardware_mode_state = HARDWARE_MODE_TEMP;
                ptFCctHardMenuTT = &TempMenu_UpdateTimer;                
                TempMenuReset();
                break;

            case 4:
                hardware_mode_state = HARDWARE_MODE_ENC_DIR_CCT;
                Cct_Enc_Dir_Mode_Menu_Reset ();
                break;

            case 5:
                hardware_mode_state = HARDWARE_MODE_VERSION;
                ptFCctHardMenuTT = &VersionMenu_UpdateTimer;
                VersionMenuReset();
                break;
                
            case 6:
                hardware_mode_state = HARDWARE_MODE_INIT;
                ptFCctHardMenuTT = NULL;
                resp = resp_finish;
                break;
                
            default:
                hardware_mode_state = HARDWARE_MODE_INIT;
                ptFCctHardMenuTT = NULL;
                break;
                
            }
        }
        break;

    case HARDWARE_MODE_CURRENTS:
        resp = CurrentMenu(mem, actions);

        if (resp == resp_finish)
        {
            hardware_mode_state = HARDWARE_MODE_INIT;
            resp = resp_continue;
        }
        break;

    case HARDWARE_MODE_LIMIT:
        resp = LimitsMenu(mem, actions);

        if (resp == resp_finish)
        {
            hardware_mode_state = HARDWARE_MODE_INIT;
            resp = resp_continue;
        }
        break;

    case HARDWARE_MODE_CHANNELS:
        resp = ChannelsMenu(mem, actions);

        if (resp == resp_finish)
        {
            hardware_mode_state = HARDWARE_MODE_INIT;
            resp = resp_continue;
        }
        break;

    case HARDWARE_MODE_TEMP:
        resp = TempMenu(mem, actions);

        if (resp == resp_finish)
        {
            hardware_mode_state = HARDWARE_MODE_INIT;
            resp = resp_continue;
        }
        break;

    case HARDWARE_MODE_ENC_DIR_CCT:
        resp = Cct_Enc_Dir_Mode_Menu (mem, actions);

        if (resp == resp_finish)
        {
            hardware_mode_state = HARDWARE_MODE_INIT;
            resp = resp_continue;
        }
        break;
        
    case HARDWARE_MODE_VERSION:
        resp = VersionMenu(mem, actions);

        if (resp == resp_finish)
        {
            hardware_mode_state = HARDWARE_MODE_INIT;
            resp = resp_continue;
        }
        break;
        
    default:
        hardware_mode_state = HARDWARE_MODE_INIT;
        break;
    }

    return resp;
    
}


resp_t Cct_HardwareMode_New (parameters_typedef * mem, sw_actions_t actions)
{
    return resp_continue;
}
//--- end of file ---//
