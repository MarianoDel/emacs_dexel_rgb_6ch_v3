//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_HARDWARE_MODE.C #######################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_hardware_mode.h"
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
    CCT_HARDWARE_MODE_INIT = 0,
    CCT_HARDWARE_MODE_SELECT,
    CCT_HARDWARE_MODE_CURRENTS,
    CCT_HARDWARE_MODE_LIMIT,
    CCT_HARDWARE_MODE_CHANNELS,
    CCT_HARDWARE_MODE_TEMP,
    CCT_HARDWARE_MODE_ENC_DIR_CCT,
    CCT_HARDWARE_MODE_VERSION,
    CCT_HARDWARE_MODE_CHANNELS_FIXED
    
} cct_hardware_mode_state_e;

// variables re-use
#define cct_hardware_mode_state    mode_state

// Externals -------------------------------------------------------------------
extern unsigned char mode_state;
extern options_menu_st mem_options;


// Globals ---------------------------------------------------------------------
void (* ptFCctHardMenuTT ) (void) = NULL;

// Module Private Functions ----------------------------------------------------


// Module Funtions -------------------------------------------------------------
void Cct_Hardware_Mode_UpdateTimers (void)
{
    if (ptFCctHardMenuTT != NULL)
        ptFCctHardMenuTT();
    
}


void Cct_Hardware_Mode_Reset (void)
{
    cct_hardware_mode_state = CCT_HARDWARE_MODE_INIT;
}


resp_t Cct_Hardware_Mode (parameters_typedef * mem, sw_actions_t actions)
{
    resp_t resp = resp_continue;

    switch (cct_hardware_mode_state)
    {
    case CCT_HARDWARE_MODE_INIT:
        mem_options.argv[0] = "CURRENT IN CHANNELS";
        mem_options.argv[1] = "CURRENT LIMIT";
        mem_options.argv[2] = "CHANNELS SELECTION";
        mem_options.argv[3] = "TEMP CONFIG";
        if (Cct_Hardware_Running_Get() != CCT_HARD_RUNNING_CCT)
            mem_options.argv[4] = "ENC DIR / GO TO CCT";
        else
            mem_options.argv[4] = "ENC DIR / GO TO DMX";
        mem_options.argv[5] = "VERSION";
        mem_options.argv[6] = "EXIT";        
        mem_options.options_qtty = 7;
        mem_options.argv[7] = "        Hardware Mode";
        OptionsMenuReset();

        cct_hardware_mode_state++;
        break;

    case CCT_HARDWARE_MODE_SELECT:
        resp = OptionsMenu(&mem_options, actions);
        
        if (resp == resp_finish)
        {
            resp = resp_continue;
            
            switch (mem_options.options_selected)
            {
            case 0:
                cct_hardware_mode_state = CCT_HARDWARE_MODE_CURRENTS;
                ptFCctHardMenuTT = &CurrentMenu_UpdateTimer;
                CurrentMenuReset();
                break;

            case 1:
                cct_hardware_mode_state = CCT_HARDWARE_MODE_LIMIT;
                ptFCctHardMenuTT = &LimitsMenu_UpdateTimer;                
                LimitsMenuReset();
                break;

            case 2:
                if (Cct_Hardware_Running_Get() == CCT_HARD_RUNNING_CCT)
                {
                    cct_hardware_mode_state = CCT_HARDWARE_MODE_CHANNELS_FIXED;
                    ptFCctHardMenuTT = &ChannelsMenu_UpdateTimer;
                    ChannelsMenuReset();
                }
                else
                {
                    cct_hardware_mode_state = CCT_HARDWARE_MODE_CHANNELS;
                    ptFCctHardMenuTT = &ChannelsMenu_UpdateTimer;                
                    ChannelsMenuReset();
                }
                break;

            case 3:
                cct_hardware_mode_state = CCT_HARDWARE_MODE_TEMP;
                ptFCctHardMenuTT = &TempMenu_UpdateTimer;                
                TempMenuReset();
                break;

            case 4:
                cct_hardware_mode_state = CCT_HARDWARE_MODE_ENC_DIR_CCT;
                Cct_Enc_Dir_Mode_Menu_Reset ();
                break;

            case 5:
                cct_hardware_mode_state = CCT_HARDWARE_MODE_VERSION;
                ptFCctHardMenuTT = &VersionMenu_UpdateTimer;
                VersionMenuReset();
                break;
                
            case 6:
                cct_hardware_mode_state = CCT_HARDWARE_MODE_INIT;
                ptFCctHardMenuTT = NULL;
                resp = resp_finish;
                break;
                
            default:
                cct_hardware_mode_state = CCT_HARDWARE_MODE_INIT;
                ptFCctHardMenuTT = NULL;
                break;
                
            }
        }
        break;

    case CCT_HARDWARE_MODE_CURRENTS:
        resp = CurrentMenu(mem, actions);

        if (resp == resp_finish)
        {
            cct_hardware_mode_state = CCT_HARDWARE_MODE_INIT;
            resp = resp_continue;
        }
        break;

    case CCT_HARDWARE_MODE_LIMIT:
        resp = LimitsMenu(mem, actions);

        if (resp == resp_finish)
        {
            cct_hardware_mode_state = CCT_HARDWARE_MODE_INIT;
            resp = resp_continue;
        }
        break;

    case CCT_HARDWARE_MODE_CHANNELS:
        resp = ChannelsMenu(mem, actions);

        if (resp == resp_finish)
        {
            cct_hardware_mode_state = CCT_HARDWARE_MODE_INIT;
            resp = resp_continue;
        }
        break;

    case CCT_HARDWARE_MODE_TEMP:
        resp = TempMenu(mem, actions);

        if (resp == resp_finish)
        {
            cct_hardware_mode_state = CCT_HARDWARE_MODE_INIT;
            resp = resp_continue;
        }
        break;

    case CCT_HARDWARE_MODE_ENC_DIR_CCT:
        resp = Cct_Enc_Dir_Mode_Menu (mem, actions);

        if (resp == resp_finish)
        {
            cct_hardware_mode_state = CCT_HARDWARE_MODE_INIT;
            resp = resp_continue;
        }
        break;
        
    case CCT_HARDWARE_MODE_VERSION:
        resp = VersionMenu(mem, actions);

        if (resp == resp_finish)
        {
            cct_hardware_mode_state = CCT_HARDWARE_MODE_INIT;
            resp = resp_continue;
        }
        break;

    case CCT_HARDWARE_MODE_CHANNELS_FIXED:
        resp = ChannelsMenuFixed ();

        if (resp == resp_finish)
        {
            cct_hardware_mode_state = CCT_HARDWARE_MODE_INIT;
            resp = resp_continue;
        }
        break;
        
        
    default:
        cct_hardware_mode_state = CCT_HARDWARE_MODE_INIT;
        break;
    }

    return resp;
    
}


unsigned char running_on_cct = 0;
unsigned char Cct_Hardware_Running_Get (void)
{
    return running_on_cct;
}


void Cct_Hardware_Running_Set (unsigned char new_val)
{
    running_on_cct = new_val;
}

//--- end of file ---//
