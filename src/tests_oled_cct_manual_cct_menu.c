//----------------------------------------------------
// Tests for gtk lib
// Implementation of tests_lcd_application.h functions
//----------------------------------------------------

#include <gtk/gtk.h>
#include <stdint.h>
#include "tests_oled_application.h"

// Application Includes needed for this test
#include "screen.h"
#include "parameters.h"
#include "switches_answers.h"

#include "cct_manual_cct_menu.h"


// Module Types Constants and Macros -------------------------------------------


// Externals -- Access to the tested Module ------------------------------------
// unsigned char menu_state;
// options_menu_st mem_options;
// extern volatile unsigned short show_select_timer;


// Globals -- Externals for the tested Module ----------------------------------
parameters_typedef mem_conf;
// dmx_menu_data_t dmx_data;
sw_actions_t encoder_actions;
unsigned short dmx_channel = 1;
unsigned char dmx_channel_data [9] = { 0 };
// -- Externals for the menues
unsigned char menu_state = 0;
unsigned char menu_selected = 0;
unsigned char menu_need_display_update = 0;
unsigned char menu_selection_show = 0;
volatile unsigned short menu_menu_timer = 0;


// Globals ---------------------------------------------------------------------
static GMutex mutex;
unsigned int timer_standby = 0;


// Teting Functions ------------------------------------------------------------
void Test_CCT_DMX_Always_Increment (void);



// Testing Function loop -------------------------------------------------------
gboolean Test_Main_Loop (gpointer user_data)
{
    
    Test_CCT_DMX_Always_Increment ();
    
    return TRUE;
}


gboolean Test_Timeouts_Loop_1ms (gpointer user_data)
{
    if (timer_standby)
        timer_standby--;
    // //timeout lcd_utils internal
    // if (show_select_timer)
    //     show_select_timer--;

    return TRUE;
}


gboolean Test_Timeouts_Loop_1000ms (gpointer user_data)
{
    return TRUE;
}


// Module Implemantation of buttons functions
void cw_button_function (void)
{
    g_mutex_lock (&mutex);
    encoder_actions = selection_dwn;
    g_mutex_unlock (&mutex);
}


void ccw_button_function (void)
{
    g_mutex_lock (&mutex);
    encoder_actions = selection_up;
    g_mutex_unlock (&mutex);
}


void set_button_function (void)
{
    g_mutex_lock (&mutex);
    encoder_actions = selection_enter;
    g_mutex_unlock (&mutex);
}


// Module Testing functions ----------------------------------------------------
int setup_done = 0;
void Test_CCT_DMX_Always_Increment (void)
{
    static unsigned char channel_data = 0;

    //first screen
    if (setup_done == 0)
    {
        setup_done = 1;

        SCREEN_Init();

        CCT_Manual_Cct_Menu_Reset ();

        // dmx_data.dmx_first_chnl = &dmx_channel;
        // dmx_data.pchannels = (const unsigned char *) &dmx_channel_data;
        // dmx_data.chnls_qtty = 9;
        // dmx_data.show_addres = 1;
        // dmx_data.mode = DMX1_MODE;        
            
        display_update_int_state_machine ();
    }

    if (setup_done == 1)
    {
        resp_t resp = resp_continue;

        resp = CCT_Manual_Cct_Menu (&mem_conf, encoder_actions);
        encoder_actions = do_nothing;

        if (resp != resp_continue)
        {
            if (!timer_standby)
            {
                timer_standby = 500;
                if (channel_data < 255)
                    channel_data++;
                else
                    channel_data = 0;

                for (int i = 0; i < 9; i++)
                    dmx_channel_data[i] = channel_data;
            }
        }

        display_update_int_state_machine ();
    }    
}


// Mocked Functions Needed for the tests ---------------------------------------


//--- end of file ---//
