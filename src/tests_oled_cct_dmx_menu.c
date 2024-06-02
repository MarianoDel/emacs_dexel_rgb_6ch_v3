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

#include "cct_dmx_menu.h"


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
unsigned char dmx_channel_data [9] = { 0 };

#include "adc.h"
#include "temperatures.h"
volatile unsigned short adc_ch [ADC_CHANNEL_QUANTITY];

// Globals ---------------------------------------------------------------------
static GMutex mutex;
unsigned int timer_standby = 0;


// Teting Functions ------------------------------------------------------------
void Test_Cct_DMX_Always_Increment (void);
void Test_Cct_DMX_Address_Change (void);


// Testing Function loop -------------------------------------------------------
gboolean Test_Main_Loop (gpointer user_data)
{
    
    // Test_Cct_DMX_Always_Increment ();

    Test_Cct_DMX_Address_Change ();
    
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
    encoder_actions = selection_dwn_fast;
    g_mutex_unlock (&mutex);
}


void ccw_button_function (void)
{
    g_mutex_lock (&mutex);
    encoder_actions = selection_up_fast;
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
void Test_Cct_DMX_Always_Increment (void)
{
    static unsigned char channel_data = 0;

    //first screen
    if (setup_done == 0)
    {
        setup_done = 1;

        SCREEN_Init();

        Cct_DMX_MenuReset ();

        dmx_data.dmx_first_chnl = &dmx_channel;
        dmx_data.pchannels = (const unsigned char *) &dmx_channel_data;
        dmx_data.chnls_qtty = 9;
        dmx_data.show_addres = 1;
        dmx_data.mode = CCT_DMX_MODE;
            
        display_update_int_state_machine ();
    }

    if (setup_done == 1)
    {
        resp_t resp = resp_continue;

        resp = Cct_DMX_Menu (&dmx_data);        
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

                for (int i = 0; i < 9; i++)
                    dmx_channel_data[i] = channel_data;
            }
        }

        display_update_int_state_machine ();
    }    
}


int cct_dmx_address_show = 0;
int cct_counter_out = 0;
int cct_dmx_end_of_packet_update = 0;
void Test_Cct_DMX_Address_Change (void)
{
    static unsigned char channel_data = 0;

    //first screen
    if (setup_done == 0)
    {
        setup_done = 1;

        SCREEN_Init();

        Cct_DMX_MenuReset ();

        dmx_data.dmx_first_chnl = &dmx_channel;
        dmx_data.pchannels = (const unsigned char *) &dmx_channel_data;
        dmx_data.chnls_qtty = 9;
        dmx_data.show_addres = 1;
        dmx_data.mode = CCT_DMX_MODE;

        cct_dmx_address_show = 1;
            
        display_update_int_state_machine ();
    }

    if (setup_done == 1)
    {
        resp_t resp = resp_continue;

        // show address & temp flag
        dmx_data.show_addres = 0;
        if (cct_dmx_address_show)
            // cct_dmx_st.show_addres |= 0x01;
            dmx_data.show_addres |= 0x01;        

        if (cct_counter_out)
            // cct_dmx_st.show_addres |= 0x02;
            dmx_data.show_addres |= 0x02;        
            
        resp = Cct_DMX_Menu (&dmx_data);

        // // clean encoders actions
        // encoder_actions = do_nothing;
        
        if (resp == resp_finish)
        {
            // update until all data will show
            if (cct_dmx_end_of_packet_update)
                cct_dmx_end_of_packet_update--;
        }
        
        display_update_int_state_machine ();

        //look for a change in address if we are not changing colors
        if (resp != resp_change)
        {
            dmx_menu_address_data_t cct_dmx_addr_st;
            // cct_dmx_addr_st.dmx_address = &dmx_channel;
            cct_dmx_addr_st.dmx_address = dmx_channel;            
            cct_dmx_addr_st.dmx_channels_qtty = 9;
            cct_dmx_addr_st.actions = encoder_actions;
            // cct_dmx_addr_st.timer = (unsigned short *) &timer_address;
            cct_dmx_addr_st.timer = (unsigned short *) &timer_standby;            
            cct_dmx_addr_st.address_show = &cct_dmx_address_show;
            resp = DMXModeMenu_ChangeAddress(&cct_dmx_addr_st);

            // clean encoders actions
            encoder_actions = do_nothing;
            
            if (resp == resp_change)
            {
                // change the DMX address
                dmx_channel = cct_dmx_addr_st.dmx_address;
                printf("address change: %d\n", dmx_channel);

                // force a display update
                cct_dmx_end_of_packet_update = 1;

            }

            // ask for temp indication
            if (resp == resp_working)
            {
                cct_counter_out = 1;
            }

            if (resp == resp_need_to_save)
            {
                // end of temp indication
                cct_counter_out = 0;
            }
        }
    }
}


void Hard_Enter_Block (void)
{
}

void Hard_Enter_UnBlock (void)
{
}

// Mocked Functions Needed for the tests ---------------------------------------


//--- end of file ---//
