//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MASTER_SLAVE_MODE.C ###################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_master_slave_mode.h"
#include "cct_master_slave_menu.h"
#include "cct_manual_cct_menu.h"
#include "cct_manual_static_menu.h"
#include "cct_manual_colors_menu.h"
#include "display_utils.h"
#include "dmx_transceiver.h"

#include <string.h>
#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
typedef enum {
    CCT_MASTER_SLAVE_MODE_INIT = 0,
    CCT_MASTER_SLAVE_SELECTING,
    CCT_MASTER_SLAVE_MODE_IN_CCT_MODE,    
    CCT_MASTER_SLAVE_MODE_IN_STATIC_MODE,    
    CCT_MASTER_SLAVE_MODE_IN_PRESET_MODE,
    CCT_MASTER_SLAVE_MODE_IN_SLAVE
    
} cct_master_slave_mode_state_e;

// variables re-use
#define cct_master_slave_state    mode_state
#define cct_master_effect_timer    mode_effect_timer


// Externals -------------------------------------------------------------------
extern volatile unsigned char data11[];
extern volatile unsigned char Packet_Detected_Flag;

extern unsigned char mode_state;
extern volatile unsigned short mode_effect_timer;


// Globals ---------------------------------------------------------------------
void (* ptFCct_MasterMenuTT ) (void) = NULL;


// Module Private Functions ----------------------------------------------------
void Cct_MasterResetInnerMode (parameters_typedef *);

// Module Funtions -------------------------------------------------------------
void Cct_MasterSlaveMode_UpdateTimers (void)
{
    if (ptFCct_MasterMenuTT != NULL)
        ptFCct_MasterMenuTT();

    // if (cct_master_effect_timer)
    //     cct_master_effect_timer--;
}


void Cct_MasterSlaveModeReset (void)
{
    cct_master_slave_state = CCT_MASTER_SLAVE_MODE_INIT;
}


// resp_change will update colors
// resp_change_all_up will update colors and save selections
// resp_need_to_save will save selections
resp_t Cct_MasterSlaveMode (parameters_typedef * mem, sw_actions_t actions)
{
    resp_t resp = resp_continue;
    unsigned char * ch_val;

    switch (cct_master_slave_state)
    {
    case CCT_MASTER_SLAVE_MODE_INIT:

        // check first time
        // if ((mem->program_inner_type_in_cct < CCT_MASTER_SLAVE_CCT_MODE) ||
        //     (mem->program_inner_type_in_cct > CCT_MASTER_SLAVE_SLAVE_MODE))
        // {
        //     mem->program_inner_type_in_cct = CCT_MASTER_SLAVE_CCT_MODE;
        //     resp = resp_need_to_save;    // saves this default selection
        // }

        switch (mem->program_inner_type_in_cct)
        {
        case CCT_MASTER_SLAVE_CCT_MODE:
            ptFCct_MasterMenuTT = &Cct_Manual_Cct_Menu_UpdateTimer;
            Cct_Manual_Cct_Menu_Reset ();
            cct_master_slave_state = CCT_MASTER_SLAVE_MODE_IN_CCT_MODE;
            
            DMX_EnableTx();
            break;

        case CCT_MASTER_SLAVE_STATIC_MODE:
            ptFCct_MasterMenuTT = &Cct_Manual_Static_Menu_UpdateTimer;
            Cct_Manual_Static_Menu_Reset ();
            cct_master_slave_state = CCT_MASTER_SLAVE_MODE_IN_STATIC_MODE;
            
            DMX_EnableTx();            
            break;
            
        case CCT_MASTER_SLAVE_PRESET_MODE:
            ptFCct_MasterMenuTT = &Cct_Manual_Colors_Menu_UpdateTimer;
            Cct_Manual_Colors_Menu_Reset ();
            cct_master_slave_state = CCT_MASTER_SLAVE_MODE_IN_PRESET_MODE;
            
            DMX_EnableTx();            
            break;
            
        case CCT_MASTER_SLAVE_SLAVE_MODE:
            // ptFCct_MasterMenuTT = &SlaveMenu_UpdateTimer;    //this is not nedded
            // SlaveMenuReset();
            cct_master_slave_state = CCT_MASTER_SLAVE_MODE_IN_SLAVE;

            //packet reception enable
            DMX_EnableRx();            
            break;
            
        default:           // go to selection menu
            ptFCct_MasterMenuTT = &Cct_Master_Slave_Menu_UpdateTimer;
            Cct_Master_Slave_Menu_Reset ();
            cct_master_slave_state = CCT_MASTER_SLAVE_SELECTING;
            break;
        }
        break;

    case CCT_MASTER_SLAVE_SELECTING:
        resp = Cct_Master_Slave_Menu (mem, actions);

        if (resp == resp_change)
        {
            cct_master_slave_state = CCT_MASTER_SLAVE_MODE_INIT;
            resp = resp_need_to_save;
        }
        break;

    case CCT_MASTER_SLAVE_MODE_IN_CCT_MODE:
        resp = Cct_Manual_Cct_Menu (mem, actions);

        // if (resp == resp_finish)
        // {
        //     cct_master_slave_state = CCT_MASTER_SLAVE_MODE_INIT;
        //     resp = resp_need_to_save;
        // }
        break;
        
    case CCT_MASTER_SLAVE_MODE_IN_STATIC_MODE:        
        //resp_change translates to resp_change_all_up in this mode
        resp = Cct_Manual_Static_Menu (mem, actions);

        if (resp == resp_change)
            resp = resp_change_all_up;
        
        break;
        
    case CCT_MASTER_SLAVE_MODE_IN_PRESET_MODE:
        //resp_change translates to resp_change_all_up in this mode        
        resp = Cct_Manual_Colors_Menu (mem, actions);

        if (resp == resp_change)
            resp = resp_change_all_up;
        
        break;
        
    case CCT_MASTER_SLAVE_MODE_IN_SLAVE:
        //update del dmx - generalmente a 40Hz -
        if (Packet_Detected_Flag)
        {
            Packet_Detected_Flag = 0;

            if (data11[0] == 0x00)    //dmx packet
            {
                //update the colors channels
                for (unsigned char i = 0; i < 5; i++)
                    mem->fixed_channels[i] = data11[i + 1];
                
                resp = resp_change;
                break;
            }
        }
        break;
        
    default:
        cct_master_slave_state = CCT_MASTER_SLAVE_MODE_INIT;
        break;
    }

    return resp;
    
}


//--- end of file ---//
