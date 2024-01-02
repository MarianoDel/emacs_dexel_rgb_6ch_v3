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
#include "options_menu.h"


// Module Types Constants and Macros -------------------------------------------


// Externals -- Access to the tested Module ------------------------------------
unsigned char menu_state;
options_menu_st mem_options;
// extern volatile unsigned short show_select_timer;


// Globals -- Externals for the tested Module ----------------------------------
parameters_typedef mem_conf;
sw_actions_t encoder_actions;


// Globals ---------------------------------------------------------------------
static GMutex mutex;
int setup_done = 0;
unsigned int timer_standby = 0;


// Testing Function loop -------------------------------------------------------
gboolean Test_Main_Loop (gpointer user_data)
{
    //first screen
    if (setup_done == 0)
    {
        setup_done = 1;

        SCREEN_Init();

        ManualMenuReset ();

        // SCREEN_ShowText2(
        //     "Kirno    ",
        //     "   Tech  ",
        //     "Smart    ",
        //     "   Driver"
        //     );
        
        // timer_standby = 1300;
        
        display_update_int_state_machine ();
    }

    if (setup_done == 1)
    {
        if (timer_standby)
            display_update_int_state_machine ();
        else
            setup_done = 2;
    }

    if (setup_done == 2)
    {
        resp_t resp = resp_continue;
        
        resp = ManualMenu (&mem_conf, encoder_actions);
        encoder_actions = do_nothing;

        if (resp != resp_continue)
            printf("something was selected!!!\n");

        display_update_int_state_machine ();
    }

    
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

// Nedded by menues module provided by hard module
// void UpdateSwitches (void)
// {
// }

// resp_sw_t Check_SW_SEL (void)
// {
//     resp_sw_t sw = SW_NO;
    
//     g_mutex_lock (&mutex);

//     if (switch_actions == selection_enter)
//         sw = SW_MIN;
    
//     g_mutex_unlock (&mutex);
    
//     return sw;    
// }

// unsigned char Check_SW_DWN (void)
// {
//     unsigned char a = 0;
    
//     g_mutex_lock (&mutex);

//     if (switch_actions == selection_dwn)
//         a = 1;
    
//     g_mutex_unlock (&mutex);
    
//     return a;
// }


// unsigned char Check_SW_UP (void)
// {
//     unsigned char a = 0;
    
//     g_mutex_lock (&mutex);

//     if (switch_actions == selection_up)
//         a = 1;
    
//     g_mutex_unlock (&mutex);
    
//     return a;
// }


//--- end of file ---//
