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
// #include "options_menu.h"

#include "dmx_menu.h"


// Module Types Constants and Macros -------------------------------------------


// Externals -- Access to the tested Module ------------------------------------
// unsigned char menu_state;
// options_menu_st mem_options;
// extern volatile unsigned short show_select_timer;


// Globals -- Externals for the tested Module ----------------------------------
// parameters_typedef mem_conf;
dmx_menu_data_t dmx_data;
sw_actions_t encoder_actions;
unsigned short dmx_channel = 1;
unsigned char dmx_channel_data [6] = { 0 };


// Globals ---------------------------------------------------------------------
static GMutex mutex;
unsigned int timer_standby = 0;


// Teting Functions ------------------------------------------------------------
void Test_DMX1_Always_Increment (void);
void Test_DMX2_Address_Change (void);



// Testing Function loop -------------------------------------------------------
gboolean Test_Main_Loop (gpointer user_data)
{
    
    // Test_DMX1_Always_Increment ();
    
    Test_DMX2_Address_Change ();
    
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
void Test_DMX1_Always_Increment (void)
{
    static unsigned char channel_data = 0;

    //first screen
    if (setup_done == 0)
    {
        setup_done = 1;

        SCREEN_Init();

        DMXModeMenuReset ();

        dmx_data.dmx_first_chnl = &dmx_channel;
        dmx_data.pchannels = (const unsigned char *) &dmx_channel_data;
        dmx_data.chnls_qtty = 6;
        dmx_data.show_addres = 1;
        dmx_data.mode = DMX1_MODE;        
            
        display_update_int_state_machine ();
    }

    if (setup_done == 1)
    {
        resp_t resp = resp_continue;

        resp = DMXModeMenu (&dmx_data);        
        // resp = ManualMenu (&mem_conf, encoder_actions);
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

                for (int i = 0; i < 6; i++)
                    dmx_channel_data[i] = channel_data;
            }
        }

        display_update_int_state_machine ();
    }    
}


void Test_DMX2_Address_Change (void)
{
    static unsigned char channel_data = 0;
    static int add_cnt = 0;

    //first screen
    if (setup_done == 0)
    {
        setup_done = 1;

        SCREEN_Init();

        DMXModeMenuReset ();

        dmx_data.dmx_first_chnl = &dmx_channel;
        dmx_data.pchannels = (const unsigned char *) &dmx_channel_data;
        dmx_data.chnls_qtty = 6;
        dmx_data.show_addres = 1;
        dmx_data.mode = DMX2_MODE;        
            
        display_update_int_state_machine ();
    }

    if (setup_done == 1)
    {
        resp_t resp = resp_continue;

        resp = DMXModeMenu (&dmx_data);        
        // resp = ManualMenu (&mem_conf, encoder_actions);
        encoder_actions = do_nothing;

        if (resp != resp_continue)
        {
            if (!timer_standby)
            {
                timer_standby = 250;
                if (dmx_data.show_addres == 0)
                    dmx_data.show_addres = 1;
                else
                    dmx_data.show_addres = 0;

                if (add_cnt < 1)
                    add_cnt++;
                else
                {
                    add_cnt = 0;
                    if (*dmx_data.dmx_first_chnl < (511 - dmx_data.chnls_qtty))
                    {
                        dmx_channel++;
                        // printf("first channel: %d\n", dmx_channel);
                    }
                    else
                        dmx_channel = 0;
                }
            }
        }
        
        display_update_int_state_machine ();
    }    
}


// Mocked Functions Needed for the tests ---------------------------------------


//--- end of file ---//
