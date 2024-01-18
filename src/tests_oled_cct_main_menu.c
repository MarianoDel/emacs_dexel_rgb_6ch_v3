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

#include "cct_main_menu.h"


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
void Test_CCT_Main (void);



// Testing Function loop -------------------------------------------------------
gboolean Test_Main_Loop (gpointer user_data)
{
    
    Test_CCT_Main ();
    
    return TRUE;
}


gboolean Test_Timeouts_Loop_1ms (gpointer user_data)
{
    if (timer_standby)
        timer_standby--;

    CCT_Main_Menu_UpdateTimer ();
    
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
    // encoder_actions = selection_dwn_fast;    
    g_mutex_unlock (&mutex);
}


void ccw_button_function (void)
{
    g_mutex_lock (&mutex);
    encoder_actions = selection_up;
    // encoder_actions = selection_up_fast;    
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
void Test_CCT_Main (void)
{
    static unsigned char channel_data = 0;

    //first screen
    if (setup_done == 0)
    {
        setup_done = 1;

        SCREEN_Init();

        CCT_Main_Menu_Reset ();

        display_update_int_state_machine ();
    }

    if (setup_done == 1)
    {
        resp_t resp = resp_continue;

        resp = CCT_Main_Menu (&mem_conf, encoder_actions);
        encoder_actions = do_nothing;

        if (resp != resp_continue)
        {
            printf("something change\n");
        }

        display_update_int_state_machine ();
    }    
}


// Mocked Functions Needed for the tests ---------------------------------------


//--- end of file ---//
