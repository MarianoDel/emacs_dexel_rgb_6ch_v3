//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_ENC_DIR_MODE_MENU.C ###################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_enc_dir_mode_menu.h"
#include "options_menu.h"

#include <string.h>
#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
typedef enum {
    ENC_DIR_MENU_INIT = 0,
    ENC_DIR_MENU_SELECT
    
} enc_dir_menu_state_e;

// variables re-use
#define enc_dir_menu_state    menu_state

// Externals -------------------------------------------------------------------
extern unsigned char menu_state;
extern options_menu_st mem_options;


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Funtions -------------------------------------------------------------
void Cct_Enc_Dir_Mode_Menu_Reset (void)
{
    enc_dir_menu_state = ENC_DIR_MENU_INIT;
}


resp_t Cct_Enc_Dir_Mode_Menu (parameters_typedef * mem, sw_actions_t actions)
{
    resp_t resp = resp_continue;

    switch (enc_dir_menu_state)
    {
    case ENC_DIR_MENU_INIT:
        mem_options.argv[0] = "ENCODER DIR CW";
        mem_options.argv[1] = "ENCODER DIR CCW";
        mem_options.argv[2] = "CCT1 MODE";
        mem_options.argv[3] = "CCT2 MODE";
        mem_options.argv[4] = "DMX1 DMX2 MODE";
        // mem_options.argv[4] = "RESET";
        mem_options.options_qtty = 5;
        mem_options.argv[7] = "  Encoder & Mode Menu";        
        OptionsMenuReset();

        enc_dir_menu_state++;
        break;

    case ENC_DIR_MENU_SELECT:
        resp = OptionsMenu(&mem_options, actions);
        
        if (resp == resp_finish)
        {
            switch (mem_options.options_selected)
            {
            case 0:
                // push values to memory
                mem->encoder_direction = 0;
                break;

            case 1:
                mem->encoder_direction = 1;                
                break;

            case 2:
                mem->program_type = CCT1_MODE;
                break;

            case 3:
                mem->program_type = CCT2_MODE;
                break;

            case 4:
                mem->program_type = DMX1_MODE;
                break;
            }

            resp = resp_need_to_save;
        }
        break;        
        
    default:
        enc_dir_menu_state = ENC_DIR_MENU_INIT;
        break;
    }

    return resp;
    
}


//--- end of file ---//
