//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_DMX_TRANSCEIVER.C ###############
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "dmx_transceiver.h"


//helper modules
#include "tests_ok.h"
#include "tests_vector_utils.h"

#include <stdio.h>
#include <string.h>


// Globals externals for module to test ----------------------------------------
volatile unsigned char RDM_packet_flag;
volatile unsigned char data11[11];
volatile unsigned char Packet_Detected_Flag;
volatile unsigned char DMX_packet_flag;
volatile unsigned char RDM_packet_flag;
volatile unsigned short DMX_channel_selected;
volatile unsigned char DMX_channel_quantity;


// Externals from module to test -----------------------------------------------
extern volatile unsigned short current_channel_index;


// Globals ---------------------------------------------------------------------
unsigned char data512 [512] = { 0 };
int data512_cnt = 0;

// Module Auxialiary Functions -------------------------------------------------


// Module Functions for testing ------------------------------------------------
void Test_Verify_Dmx_Data (void);
void Test_Dmx_Send_Data (void);


// Module Functions ------------------------------------------------------------


int main (int argc, char *argv[])
{

    DMX_channel_selected = 500;
    DMX_channel_quantity = 6;
    Test_Verify_Dmx_Data ();

    Test_Dmx_Send_Data ();

}


void Test_Verify_Dmx_Data (void)
{
    printf("Test on DMX_channel_selected: ");
    if ((DMX_channel_selected < 1) ||
        (DMX_channel_selected > 511))
    {
        PrintERR();
        printf ("error on selected channel: %d\n", DMX_channel_selected);
    }
    else
    {
        PrintOK();
        printf ("selected channel: %d\n", DMX_channel_selected);
    }

    printf("Test on DMX_channel_quantity: ");
    if ((DMX_channel_quantity < 1) ||
        (DMX_channel_quantity > 6))
    {
        PrintERR();
        printf ("error on channel quantity: %d\n", DMX_channel_quantity);
    }
    else
    {
        PrintOK();
        printf ("selected channel quantity: %d\n", DMX_channel_quantity);
    }

    printf("Test on selected and quantity: ");
    if ((DMX_channel_selected + DMX_channel_quantity - 1) > 512)
    {
        PrintERR();
        printf ("error on channel selected + quantity: %d\n",
                DMX_channel_quantity + DMX_channel_selected);
        printf ("begin in channel %d last channel: %d\n",
                DMX_channel_selected,
                DMX_channel_selected + DMX_channel_quantity - 1);
        
    }
    else
    {
        PrintOK();
        printf ("channel selected + quantity: %d\n",
                DMX_channel_quantity + DMX_channel_selected);
        printf ("begin in channel %d last channel: %d\n",
                DMX_channel_selected,
                DMX_channel_selected + DMX_channel_quantity - 1);
        
    }
}


void Test_Dmx_Send_Data (void)
{
    printf ("\n\nTesting DMX send data\n");
    data11[0] = 0;
    data11[1] = 255;
    data11[2] = 255;
    data11[3] = 255;
    data11[4] = 127;
    data11[5] = 127;
    data11[6] = 127;

    current_channel_index = 0;

    int line_cnt = 0;
    for (int i = 0; i < 512; i++)
    {
        DmxInt_Serial_Handler_Transmitter ();
        if (line_cnt == 0)
        {
            printf("\nindex: %d %d", i, data512[i]);
            line_cnt++;
        }
        else if (line_cnt < 20)
        {
            printf(" %d", data512[i]);
            line_cnt++;

            if (line_cnt == 20)
                line_cnt = 0;
        }
    }
    printf("\n");

    // Print_Array_Uchar (data512, 512, 500);
    
}


// Module Mocked Functions
void EXTIOn (void)
{
    printf("EXTI is ON!\n");
}


void EXTIOff (void)
{
    printf("EXTI is Off!\n");
}


void Usart3SendByte (unsigned char a)
{
    data512[data512_cnt] = a;
    data512_cnt++;
}


void Usart3DisableTx (void)
{
    printf("\nUsart 3 Disabled!\n");
}

void PB10_Pin_To_PushPull (void)
{
    printf("PB10 to pushpull\n");
}

void PB10_Pin_To_Alternative (void)
{
    printf("PB10 to alternative\n");
}

void TIM7_To_FreeRunning (void)
{
    printf("TIM7 to free running!\n");
}


void TIM7_To_OneShoot (void)
{
    printf("TIM7 to one shoot!\n");
}

void OneShootTIM7 (unsigned short a)
{
    printf("TIM7 one shoot to %dus\n", a);
}
// void TIM6_Update (unsigned short new_val)
// {
//     timer_cnt = 0;
//     timer_overflow = new_val;
// }

// void TIM_Deactivate_Channels (unsigned char deact_chnls)
// {
//     printf("deactivate: 0x%02x\n", deact_chnls);
// }


// void TIM_Activate_Channels (unsigned char act_chnls)
// {
//     printf("activate: 0x%02x\n", act_chnls);
// }

//--- end of file ---//


