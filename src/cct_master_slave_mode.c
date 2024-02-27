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
#include "cct_master_menu.h"
#include "slave_menu.h"
#include "fixed_menu.h"
#include "colors_menu.h"
#include "display_utils.h"
#include "dmx_transceiver.h"

#include "colors_functions.h"

#include <string.h>
#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
typedef enum {
    CCT_MASTER_SLAVE_MODE_INIT = 0,
    CCT_MASTER_SLAVE_MODE_CHECK_INNER_MODE_0,
    CCT_MASTER_SLAVE_MODE_CHECK_INNER_MODE_1,    
    CCT_MASTER_SLAVE_MODE_GLOBAL_MENU,
    CCT_MASTER_SLAVE_MODE_IN_COLORS_FIXED,
    CCT_MASTER_SLAVE_MODE_IN_COLORS_SKIPPING,
    CCT_MASTER_SLAVE_MODE_IN_COLORS_GRADUAL,
    CCT_MASTER_SLAVE_MODE_IN_COLORS_STROBE,
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

    if (cct_master_effect_timer)
        cct_master_effect_timer--;
}


void Cct_MasterSlaveModeReset (void)
{
    cct_master_slave_state = CCT_MASTER_SLAVE_MODE_INIT;
}


resp_t Cct_MasterSlaveMode (parameters_typedef * mem, sw_actions_t actions)
{
    resp_t resp = resp_continue;
    unsigned char * ch_val;

    switch (cct_master_slave_state)
    {
    case CCT_MASTER_SLAVE_MODE_INIT:

        switch (mem->program_inner_type_in_cct)
        {
        case CCT_MASTER_INNER_FIXED_MODE:
            ptFCct_MasterMenuTT = &FixedMenu_UpdateTimer;            
            FixedMenuReset();
            cct_master_slave_state = CCT_MASTER_SLAVE_MODE_IN_COLORS_FIXED;
            DMX_EnableTx();
            break;

        case CCT_MASTER_INNER_SKIPPING_MODE:
            ptFCct_MasterMenuTT = &ColorsMenu_UpdateTimer;
            ColorsMenuReset();
            cct_master_slave_state = CCT_MASTER_SLAVE_MODE_IN_COLORS_SKIPPING;
            DMX_EnableTx();            
            break;
            
        case CCT_MASTER_INNER_GRADUAL_MODE:
            ptFCct_MasterMenuTT = &ColorsMenu_UpdateTimer;
            ColorsMenuReset();
            cct_master_slave_state = CCT_MASTER_SLAVE_MODE_IN_COLORS_GRADUAL;
            DMX_EnableTx();            
            break;
            
        case CCT_MASTER_INNER_STROBE_MODE:
            ptFCct_MasterMenuTT = &ColorsMenu_UpdateTimer;
            ColorsMenuReset();
            cct_master_slave_state = CCT_MASTER_SLAVE_MODE_IN_COLORS_STROBE;
            DMX_EnableTx();            
            break;

        case CCT_MASTER_INNER_SLAVE:
            // ptFCct_MasterMenuTT = &SlaveMenu_UpdateTimer;    //this is not nedded
            SlaveMenuReset();
            cct_master_slave_state = CCT_MASTER_SLAVE_MODE_IN_SLAVE;
            DMX_EnableRx();
            break;
            
        default:
            ptFCct_MasterMenuTT = NULL;
            Cct_MasterSlaveMenuReset ();
            cct_master_slave_state = CCT_MASTER_SLAVE_MODE_CHECK_INNER_MODE_0;
            break;
        }
        break;

    case CCT_MASTER_SLAVE_MODE_CHECK_INNER_MODE_0:
        resp = Cct_MasterSlaveMenu (mem, actions);

        if (resp == resp_finish)
        {
            if (mem->program_inner_type == CCT_MASTER_INNER_SLAVE)
            {
                cct_master_slave_state = CCT_MASTER_SLAVE_MODE_INIT;
                resp = resp_need_to_save;
            }
            else
            {
                Cct_MasterMenuReset();
                cct_master_slave_state++;
            }
        }
        break;

    case CCT_MASTER_SLAVE_MODE_CHECK_INNER_MODE_1:
        resp = Cct_MasterMenu (mem, actions);

        if (resp == resp_finish)
        {
            cct_master_slave_state = CCT_MASTER_SLAVE_MODE_INIT;
            resp = resp_need_to_save;
        }
        break;
        
    case CCT_MASTER_SLAVE_MODE_IN_COLORS_FIXED:
        
        //resp_change translates to resp_change_all_up in this mode, resp_finish end of this mode
        resp = FixedMenu(mem, actions);

        if (resp == resp_change)
            resp = resp_change_all_up;

        if (resp == resp_finish)
        {
            Cct_MasterResetInnerMode(mem);
            resp = resp_change;
        }
        break;

    case CCT_MASTER_SLAVE_MODE_IN_COLORS_SKIPPING:

        //resp_change do nothing, resp_finish end of this mode, resp_need_to_save goes straight up
        resp = ColorsMenu (mem, actions);

        if (resp == resp_finish)
        {
            Cct_MasterResetInnerMode(mem);            
            resp = resp_change;
            break;
        }

        // speed change, save it
        if (resp == resp_need_to_save)
            break;

        if (!cct_master_effect_timer)
        {
            ch_val = mem->fixed_channels;

            resp = Colors_Fading_Pallete (ch_val);
            if (resp == resp_finish)
                resp = resp_continue;

            cct_master_effect_timer = 10 - mem->program_inner_type_speed;
            resp = resp_change;
        }
        break;

    case CCT_MASTER_SLAVE_MODE_IN_COLORS_GRADUAL:

        //resp_change do nothing, resp_finish end of this mode, resp_need_to_save goes straight up
        resp = ColorsMenu (mem, actions);

        if (resp == resp_finish)
        {
            Cct_MasterResetInnerMode(mem);
            resp = resp_change;
            break;
        }

        // speed change, save it
        if (resp == resp_need_to_save)
            break;
        
        if (!cct_master_effect_timer)
        {
            ch_val = mem->fixed_channels;

            resp = Colors_Fading_Shuffle_Pallete (ch_val);
            if (resp == resp_finish)
                resp = resp_continue;

            cct_master_effect_timer = 10 - mem->program_inner_type_speed;
            resp = resp_change;
        }

        break;

    case CCT_MASTER_SLAVE_MODE_IN_COLORS_STROBE:

        //resp_change do nothing, resp_finish end of this mode, resp_need_to_save goes straight up
        resp = ColorsMenu (mem, actions);

        if (resp == resp_finish)
        {
            Cct_MasterResetInnerMode(mem);
            resp = resp_change;
            break;
        }

        // speed change, save it
        if (resp == resp_need_to_save)
            break;

        if (!cct_master_effect_timer)
        {
            ch_val = mem->fixed_channels;

            resp = Colors_Strobe_Pallete (ch_val);
            if (resp == resp_finish)
                resp = resp_continue;

            // cct_master_effect_timer = 2000 - mem->program_inner_type_speed * 200;
            cct_master_effect_timer = 1000 - mem->program_inner_type_speed * 100;                        
            resp = resp_change;
        }
        break;

    case CCT_MASTER_SLAVE_MODE_IN_SLAVE:
        //update del dmx - generalmente a 40Hz -
        if (Packet_Detected_Flag)
        {
            Packet_Detected_Flag = 0;

            if (data11[0] == 0x00)    //dmx packet
            {
                //update the colors channels
                for (unsigned char i = 0; i < 6; i++)
                    mem->fixed_channels[i] = data11[i + 1];
                
                resp = resp_change;
                break;
            }
        }
        
        resp = SlaveMenu (mem, actions);

        if (resp == resp_finish)
        {
            Cct_MasterResetInnerMode(mem);            
            resp = resp_change;
        }
        break;
        
    default:
        cct_master_slave_state = CCT_MASTER_SLAVE_MODE_INIT;
        break;
    }

    return resp;
    
}


void Cct_MasterResetInnerMode (parameters_typedef * mem)
{
    mem->program_inner_type = CCT_MASTER_NO_INNER_MODE;
    cct_master_slave_state = CCT_MASTER_SLAVE_MODE_INIT;
    
    //colors reset
    for (unsigned char i = 0; i < 6; i++)
        mem->fixed_channels[i] = 0;

    DMX_Disable();
        
}
//--- end of file ---//
