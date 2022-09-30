//------------------------------------------------
// #### PROJECT: O3 TREATMENT - Custom Board #####
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.C #########################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "test_functions.h"
#include "hard.h"
#include "stm32f10x.h"
#include "tim.h"
#include "dac.h"
#include "dac_mux.h"
#include "pwm.h"
#include "dmx_transceiver.h"
#include "usart.h"
#include "dsp.h"
#include "i2c.h"

#include "screen.h"
#include "ssd1306_display.h"
#include "dmx_menu.h"

#include "parameters.h"
#include "flash_program.h"

#include <stdio.h>
#include <string.h>

// Externals -------------------------------------------------------------------
extern volatile unsigned char pwm_chnls[];
extern volatile unsigned short timer_standby;

extern volatile unsigned short DMX_channel_selected;
extern volatile unsigned char DMX_channel_quantity;
extern volatile unsigned char DMX_packet_flag;

extern parameters_typedef * pmem;
extern parameters_typedef mem_conf;

// Globals ---------------------------------------------------------------------


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------
void TF_Led_On_Ctrl_Fan (void);
void TF_Encoder_Sw (void);
void TF_Encoder_CheckSET (void);
void TF_Encoder_CheckCW (void);
void TF_Encoder_CheckCCW (void);

void TF_Pwm_Channel_1 (void);
void TF_Pwm_Channel_2 (void);
void TF_Pwm_Channel_2_50_Percent (void);
void TF_Pwm_Channel_3 (void);
void TF_Pwm_Channel_4 (void);
void TF_Pwm_Channel_5 (void);
void TF_Pwm_Channel_6 (void);
void TF_Pwm_All_Channels (void);
void TF_Pwm_All_Channels_50_Percent (void);
void TF_Pwm_All_Channels_Up_Down (void);

void TF_Dac1 (void);
void TF_Int_Timer5 (void);

void TF_Complete_Channel_1 (void);

void TF_All_Chain_For_Channel_1 (void);
void TF_All_Chain_For_Channel_1_Max (void);

void TF_Dac_Mux (void);
void TF_Dac_Mux_All_Channels (void);
void TF_All_Chain_For_Channel_1_With_Dac_Mux (void);

void TF_Dmx_Input_Break_Detect (void);
void TF_Dmx_Input_Packet_Detect (void);
void TF_Dmx_All_Channels (void);
void TF_Dmx_All_Channels_5ms_filter (void);

void TF_I2C_Send_Data (void);
void TF_I2C_Check_Address (void);
void TF_Oled_Screen (void);
void TF_Oled_Screen_Int (void);
void TF_Dmx_All_Channels_5ms_filter_with_Oled (void);

void TF_Uart4_Tx (void);
void TF_Flash_Configurations (void);
void TF_Flash_Configurations_With_Uart4 (void);


// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // TF_Led_On_Ctrl_Fan ();
    // TF_Encoder_Sw ();
    // TF_Encoder_CheckSET ();
    // TF_Encoder_CheckCW ();
    // TF_Encoder_CheckCCW ();

    // TF_Pwm_Channel_1 ();
    // TF_Pwm_Channel_2 ();
    // TF_Pwm_Channel_2_50_Percent ();
    // TF_Pwm_Channel_3 ();
    // TF_Pwm_Channel_4 ();
    // TF_Pwm_Channel_5 ();
    // TF_Pwm_Channel_6 ();
    // TF_Pwm_All_Channels ();
    // TF_Pwm_All_Channels_50_Percent ();
    // TF_Pwm_All_Channels_Up_Down ();

    // TF_Dac1 ();
    // TF_Complete_Channel_1 ();

    // TF_All_Chain_For_Channel_1 ();
    // TF_All_Chain_For_Channel_1_Max ();
    // TF_Dac_Mux ();
    // TF_Dac_Mux_All_Channels ();
    // TF_All_Chain_For_Channel_1_With_Dac_Mux ();

    // TF_Dmx_Input_Break_Detect ();
    // TF_Dmx_Input_Packet_Detect ();
    // TF_Dmx_All_Channels ();
    // TF_Dmx_All_Channels_5ms_filter ();

    // TF_I2C_Send_Data ();
    // TF_I2C_Check_Address ();
    // TF_Oled_Screen ();
    // TF_Oled_Screen_Int ();
    // TF_Oled_Screen_Int2 ();
    // TF_Dmx_All_Channels_5ms_filter_with_Oled ();

    // TF_Uart4_Tx ();
    TF_Flash_Configurations ();
    // TF_Flash_Configurations_With_Uart4 ();
    
}


void TF_Led_On_Ctrl_Fan (void)
{
    while (1)
    {
        if (CTRL_FAN)
            CTRL_FAN_OFF;
        else
            CTRL_FAN_ON;

        Wait_ms(1000);
    }
}


void TF_Encoder_Sw (void)
{
    while (1)
    {
        if (EN_SW)
            LED_ON;
        else
            LED_OFF;
    }
}


void TF_Encoder_CheckSET (void)
{
    while (1)
    {
        if (CheckSET() > SW_NO)
            LED_ON;
        else
            LED_OFF;

        UpdateSwitches();
    }
}


void TF_Encoder_CheckCW (void)
{
    while (1)
    {
        if (CheckCW())
            timer_standby = 50;

        if (timer_standby)
            LED_ON;
        else
            LED_OFF;

        UpdateEncoder ();
    }
}


void TF_Encoder_CheckCCW (void)
{
    while (1)
    {
        if (CheckCCW())
            timer_standby = 50;

        if (timer_standby)
            LED_ON;
        else
            LED_OFF;

        UpdateEncoder ();
    }
}


void TF_Pwm_Channel_1 (void)
{
    TIM_1_Init();
    
    while (1)
    {
        if (CTRL_FAN)
            CTRL_FAN_OFF;
        else
            CTRL_FAN_ON;

        Wait_ms(1000);
    }    
}


void TF_Pwm_Channel_2 (void)
{
    TIM_8_Init();
    
    while (1)
    {
        if (CTRL_FAN)
            CTRL_FAN_OFF;
        else
            CTRL_FAN_ON;

        Wait_ms(1000);
    }    
}


void TF_Pwm_Channel_2_50_Percent (void)
{
    TIM_8_Init();

    TIM6_Init();
    pwm_chnls[0] = 0;
    pwm_chnls[1] = 127;
    pwm_chnls[2] = 0;
    pwm_chnls[3] = 0;
    pwm_chnls[4] = 0;
    pwm_chnls[5] = 0;
    
    while (1)
    {
        Wait_ms(1000);
    }    
}

void TF_Pwm_Channel_3 (void)
{
    TIM_2_Init();
    
    while (1)
    {
        if (CTRL_FAN)
            CTRL_FAN_OFF;
        else
            CTRL_FAN_ON;

        Wait_ms(1000);
    }    
}


void TF_Pwm_Channel_4 (void)
{
    TIM_3_Init();
    
    while (1)
    {
        if (CTRL_FAN)
            CTRL_FAN_OFF;
        else
            CTRL_FAN_ON;

        Wait_ms(1000);
    }    
}


void TF_Pwm_Channel_5 (void)
{
    TIM_4_Init();
    
    while (1)
    {
        if (CTRL_FAN)
            CTRL_FAN_OFF;
        else
            CTRL_FAN_ON;

        Wait_ms(1000);
    }    
}


void TF_Pwm_Channel_6 (void)
{
    TIM_5_Init();
    
    while (1)
    {
        if (CTRL_FAN)
            CTRL_FAN_OFF;
        else
            CTRL_FAN_ON;

        Wait_ms(1000);
    }    
}


void TF_Pwm_All_Channels (void)
{
    // enable timers for channels
    TIM_1_Init();    //ch1
    TIM_8_Init();    //ch2
    TIM_2_Init();    //ch3
    TIM_3_Init();    //ch4
    TIM_4_Init();    //ch5
    TIM_5_Init();    //ch6
    
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        Wait_ms(1000);
    }    
}


void TF_Pwm_All_Channels_50_Percent (void)
{
    // enable timers for channels
    TIM_1_Init();    //ch1
    TIM_8_Init();    //ch2
    TIM_2_Init();    //ch3
    TIM_3_Init();    //ch4
    TIM_4_Init();    //ch5
    TIM_5_Init();    //ch6

    TIM6_Init();
    pwm_chnls[0] = 127;
    pwm_chnls[1] = 127;
    pwm_chnls[2] = 127;
    pwm_chnls[3] = 127;
    pwm_chnls[4] = 127;
    pwm_chnls[5] = 127;
    
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        Wait_ms(1000);
    }    
}

void TF_Pwm_All_Channels_Up_Down (void)
{
    // enable timers for channels
    TIM_1_Init();    //ch1
    TIM_8_Init();    //ch2
    TIM_2_Init();    //ch3
    TIM_3_Init();    //ch4
    TIM_4_Init();    //ch5
    TIM_5_Init();    //ch6

    TIM6_Init();
    pwm_chnls[0] = 0;
    pwm_chnls[1] = 0;
    pwm_chnls[2] = 0;
    pwm_chnls[3] = 0;
    pwm_chnls[4] = 0;
    pwm_chnls[5] = 0;
    
    while (1)
    {
        // Go to max current with pwm
        for (int i = 0; i < 256; i++)
        {
            pwm_chnls[0] = i;
            pwm_chnls[1] = i;
            pwm_chnls[2] = i;
            pwm_chnls[3] = i;
            pwm_chnls[4] = i;
            pwm_chnls[5] = i;
            Wait_ms(5);
        }

        Wait_ms(5000);

        // min from pwm
        for (int i = 255; i > 0; i--)
        {
            pwm_chnls[0] = i;
            pwm_chnls[1] = i;
            pwm_chnls[2] = i;
            pwm_chnls[3] = i;
            pwm_chnls[4] = i;
            pwm_chnls[5] = i;
            Wait_ms(5);
        }

        Wait_ms(5000);        
        
    }
}


void TF_Dac1 (void)
{
    DAC_Config();

    while (1)
    {
        LED_OFF;
        DAC_Output(0);
        Wait_ms(4000);

        LED_ON;
        DAC_Output(4095);
        Wait_ms(4000);
    }
}


void TF_Complete_Channel_1 (void)
{
    TIM_1_Init();
    DAC_Config();

    DAC_Output(0);    
    while (1)
    {
        // LED_OFF;
        // DAC_Output(0);
        // Wait_ms(4000);

        // LED_ON;
        // DAC_Output(4095);
        // Wait_ms(4000);
    }    
}


void TF_All_Chain_For_Channel_1 (void)
{
    unsigned char pwm_ena = 0;
    unsigned short dac_ch = 0;
    
    TIM_1_Init();
    DAC_Config();

    CTRL_C2_OFF;
    CTRL_C1_ON;
    DAC_Output(0);

    TIM6_Init();
    // TIM7_Init();

    pwm_chnls[0] = 0;
    pwm_chnls[1] = 0;
    pwm_chnls[2] = 0;
    pwm_chnls[3] = 0;
    pwm_chnls[4] = 0;
    pwm_chnls[5] = 0;

    while (1)
    {
        // Go to max current with pwm
        for (int i = 0; i < 256; i++)
        {
            pwm_chnls[0] = i;
            pwm_chnls[1] = 0;
            pwm_chnls[2] = 0;
            pwm_chnls[3] = 0;
            pwm_chnls[4] = 0;
            pwm_chnls[5] = 0;
            Wait_ms(5);
        }

        Wait_ms(5000);

        // Max current with dac
        for (int i = 0 ; i < 4096; i++)
        {
            DAC_Output(i);
            Wait_ms(1);
        }

        Wait_ms(5000);

        // Go to min from dac
        for (int i = 4095 ; i >= 0; i--)
        {
            DAC_Output(i);
            Wait_ms(1);
        }

        Wait_ms(5000);

        // min from pwm

        for (int i = 255; i > 0; i--)
        {
            pwm_chnls[0] = i;
            pwm_chnls[1] = 0;
            pwm_chnls[2] = 0;
            pwm_chnls[3] = 0;
            pwm_chnls[4] = 0;
            pwm_chnls[5] = 0;
            Wait_ms(5);
        }

        Wait_ms(5000);        
        
    }


    while (1)
    {
        // Ascending to Max; 4095 . 5ms = 20s
        for (int i = 0; i < 4095; i++)
        {
            // All Chain for Channel 1
            // No filter
            PWM_Map_Post_Filter (i, &pwm_ena, &dac_ch);
            DAC_Output(dac_ch);
            // pwm_saved[0] = pwm_ena;
            // pwm_saved[1] = 0;
            // pwm_saved[2] = 0;
            // pwm_saved[3] = 0;
            // pwm_saved[4] = 0;
            // pwm_saved[5] = 0;
            
            PWM_Timer_Arrange_Vector ();
            Wait_ms(5);
        }       

        // Wait 2 secs
        Wait_ms(2000);

        // Decending to Min
        for (int i = 4095; i > 1; i--)
        {
            // All Chain for Channel 1
            // No filter
            PWM_Map_Post_Filter (i, &pwm_ena, &dac_ch);
            DAC_Output(dac_ch);
            // pwm_saved[0] = pwm_ena;
            // pwm_saved[1] = 0;
            // pwm_saved[2] = 0;
            // pwm_saved[3] = 0;
            // pwm_saved[4] = 0;
            // pwm_saved[5] = 0;
            
            PWM_Timer_Arrange_Vector ();
            Wait_ms(5);
        }       

        // Wait 2 secs
        Wait_ms(2000);
        
    }
}


void TF_All_Chain_For_Channel_1_Max (void)
{
    TIM_1_Init();
    DAC_Config();

    CTRL_C2_OFF;
    CTRL_C1_ON;
    DAC_Output(0);

    TIM6_Init();
    // TIM7_Init();

    pwm_chnls[0] = 255;
    pwm_chnls[1] = 0;
    pwm_chnls[2] = 0;
    pwm_chnls[3] = 0;
    pwm_chnls[4] = 0;
    pwm_chnls[5] = 0;

    DAC_Output(4095);    

    while (1);
}


void TF_Dac_Mux (void)
{
    unsigned short dac_chnls [6] = { 0 };
    
    TIM_1_Init();
    
    DAC_Config();
    DAC_Output(0);

    TIM6_Init();

    pwm_chnls[0] = 255;
    pwm_chnls[1] = 0;
    pwm_chnls[2] = 0;
    pwm_chnls[3] = 0;
    pwm_chnls[4] = 0;
    pwm_chnls[5] = 0;

    dac_chnls[0] = 4095;
    dac_chnls[1] = 2048;
    dac_chnls[2] = 1024;
    dac_chnls[3] = 512;
    dac_chnls[4] = 256;
    dac_chnls[5] = 0;

    while (1)
    {
        DAC_MUX_Update(dac_chnls);
    }
}


void TF_Dac_Mux_All_Channels (void)
{
    unsigned short dac_chnls [6] = { 0 };

    // enable timers for channels
    TIM_1_Init();    //ch1
    TIM_8_Init();    //ch2
    TIM_2_Init();    //ch3
    TIM_3_Init();    //ch4
    TIM_4_Init();    //ch5
    TIM_5_Init();    //ch6
    
    DAC_Config();
    DAC_Output(0);

    TIM6_Init();

    pwm_chnls[0] = 255;
    pwm_chnls[1] = 255;
    pwm_chnls[2] = 255;
    pwm_chnls[3] = 255;
    pwm_chnls[4] = 255;
    pwm_chnls[5] = 255;

    dac_chnls[0] = 4095;
    dac_chnls[1] = 2048;
    dac_chnls[2] = 1024;
    dac_chnls[3] = 512;
    dac_chnls[4] = 256;
    dac_chnls[5] = 0;

    while (1)
    {
        DAC_MUX_Update(dac_chnls);
    }
}


void TF_All_Chain_For_Channel_1_With_Dac_Mux (void)
{
    unsigned short dac_chnls [6] = { 0 };
    
    TIM_1_Init();
    DAC_Config();

    DAC_Output(0);

    TIM6_Init();

    pwm_chnls[0] = 0;
    pwm_chnls[1] = 0;
    pwm_chnls[2] = 0;
    pwm_chnls[3] = 0;
    pwm_chnls[4] = 0;
    pwm_chnls[5] = 0;

    dac_chnls[0] = 0;
    dac_chnls[1] = 0;
    dac_chnls[2] = 0;
    dac_chnls[3] = 0;
    dac_chnls[4] = 0;
    dac_chnls[5] = 0;

    
    int i = 0;
    int stage = 0;
    while (1)
    {
        // Go to max current with pwm        
        if (stage == 0)
        {
            if (i < 256)
            {
                if (!timer_standby)
                {
                    pwm_chnls[0] = i;
                    pwm_chnls[1] = 0;
                    pwm_chnls[2] = 0;
                    pwm_chnls[3] = 0;
                    pwm_chnls[4] = 0;
                    pwm_chnls[5] = 0;

                    i++;
                    timer_standby = 5;
                }
            }
            else
            {
                stage = 1;
                i = 0;
                timer_standby = 5000;                
            }
        }

        // Wait 5 secs
        if (stage == 1)
        {
            if (!timer_standby)
                stage = 2;

        }
        
        // Max current with dac
        if (stage == 2)
        {
            if (i < 4095)
            {
                if (!timer_standby)
                {
                    dac_chnls[0] = i;
                    dac_chnls[1] = 0;
                    dac_chnls[2] = 0;
                    dac_chnls[3] = 0;
                    dac_chnls[4] = 0;
                    dac_chnls[5] = 0;

                    i++;
                    timer_standby = 5;
                }
            }
            else
            {
                stage = 3;
                i = 4095;
                timer_standby = 5000;
            }
        }

        // Wait 5 secs
        if (stage == 3)
        {
            if (!timer_standby)
                stage = 4;

        }

        // Go to min from dac
        if (stage == 4)
        {
            if (i > 1)
            {
                if (!timer_standby)
                {
                    dac_chnls[0] = i;
                    dac_chnls[1] = 0;
                    dac_chnls[2] = 0;
                    dac_chnls[3] = 0;
                    dac_chnls[4] = 0;
                    dac_chnls[5] = 0;

                    i--;
                    timer_standby = 5;
                }
            }
            else
            {
                stage = 5;
                i = 255;
                timer_standby = 5000;
            }
        }

        // Wait 5 secs
        if (stage == 5)
        {
            if (!timer_standby)
                stage = 6;

        }
        
        // min from pwm
        if (stage == 6)
        {
            if (i > 1)
            {
                if (!timer_standby)
                {
                    pwm_chnls[0] = i;
                    pwm_chnls[1] = 0;
                    pwm_chnls[2] = 0;
                    pwm_chnls[3] = 0;
                    pwm_chnls[4] = 0;
                    pwm_chnls[5] = 0;

                    i--;
                    timer_standby = 5;
                }
            }
            else
            {
                stage = 7;
                i = 0;
                timer_standby = 5000;
            }
        }

        // Wait 5 secs
        if (stage == 7)
        {
            if (!timer_standby)
                stage = 0;

        }

        DAC_MUX_Update(dac_chnls);        
        
    }
}


void TF_Dmx_Input_Break_Detect (void)
{
    TIM7_Init();

    DMX_channel_selected = 1;
    DMX_channel_quantity = 6;

    DMX_EnableRx ();

    while (1)
    {
        if ((Dmx_Get_Packet_Init_Flag()) &&
            (!timer_standby))
        {
            LED_ON;
            timer_standby = 2;
        }

        if (!timer_standby)
            LED_OFF;
    }
}


void TF_Dmx_Input_Packet_Detect (void)
{
    TIM7_Init();

    Usart3Config ();

    DMX_channel_selected = 1;
    DMX_channel_quantity = 6;

    DMX_EnableRx ();

    while (1)
    {
        if ((DMX_packet_flag) &&
            (!timer_standby))
        {
            LED_ON;
            timer_standby = 10;
            DMX_packet_flag = 0;
        }

        if (!timer_standby)
            LED_OFF;

        UpdatePackets();
    }
}


extern volatile unsigned char data11[];
void TF_Dmx_All_Channels (void)
{
    unsigned short dac_chnls [6] = { 0 };

    // enable timers for channels
    TIM_1_Init();    //ch1
    TIM_8_Init();    //ch2
    TIM_2_Init();    //ch3
    TIM_3_Init();    //ch4
    TIM_4_Init();    //ch5
    TIM_5_Init();    //ch6

    DAC_Config();
    DAC_Output(0);

    TIM6_Init();
    TIM7_Init();

    pwm_chnls[0] = 0;
    pwm_chnls[1] = 0;
    pwm_chnls[2] = 0;
    pwm_chnls[3] = 0;
    pwm_chnls[4] = 0;
    pwm_chnls[5] = 0;

    dac_chnls[0] = 0;
    dac_chnls[1] = 0;
    dac_chnls[2] = 0;
    dac_chnls[3] = 0;
    dac_chnls[4] = 0;
    dac_chnls[5] = 0;

    Usart3Config ();

    DMX_channel_selected = 1;
    DMX_channel_quantity = 6;

    DMX_EnableRx ();
    
    while (1)
    {
        if ((DMX_packet_flag) &&
            (!timer_standby))
        {
            LED_ON;
            timer_standby = 10;
            DMX_packet_flag = 0;

            unsigned short dmx_input = 0;
            // channel 1
            // dmx_input = MA16_U16Circular (&st_sp1, data11[1] << 4);
            // PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[0], &dac_chnls[0]);            
            dmx_input = data11[1] << 4;
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[0], &dac_chnls[0]);
            // channel 2
            dmx_input = data11[2] << 4;
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[1], &dac_chnls[1]);
            // channel 3
            dmx_input = data11[3] << 4;
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[2], &dac_chnls[2]);
            // channel 4
            dmx_input = data11[4] << 4;
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[3], &dac_chnls[3]);
            // channel 5
            dmx_input = data11[5] << 4;
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[4], &dac_chnls[4]);
            // channel 6
            dmx_input = data11[6] << 4;
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[5], &dac_chnls[5]);
            
        }

        if (!timer_standby)
            LED_OFF;

        UpdatePackets();
        DAC_MUX_Update(dac_chnls);
    }
}


extern ma16_u16_data_obj_t st_sp1;
extern ma16_u16_data_obj_t st_sp2;
extern ma16_u16_data_obj_t st_sp3;
extern ma16_u16_data_obj_t st_sp4;
extern ma16_u16_data_obj_t st_sp5;
extern ma16_u16_data_obj_t st_sp6;
void TF_Dmx_All_Channels_5ms_filter (void)
{
    unsigned short dac_chnls [6] = { 0 };
    unsigned short dmx_getted [6] = { 0 };

    // enable timers for channels
    TIM_1_Init();    //ch1
    TIM_8_Init();    //ch2
    TIM_2_Init();    //ch3
    TIM_3_Init();    //ch4
    TIM_4_Init();    //ch5
    TIM_5_Init();    //ch6

    DAC_Config();
    DAC_Output(0);

    TIM6_Init();
    TIM7_Init();

    pwm_chnls[0] = 0;
    pwm_chnls[1] = 0;
    pwm_chnls[2] = 0;
    pwm_chnls[3] = 0;
    pwm_chnls[4] = 0;
    pwm_chnls[5] = 0;

    dac_chnls[0] = 0;
    dac_chnls[1] = 0;
    dac_chnls[2] = 0;
    dac_chnls[3] = 0;
    dac_chnls[4] = 0;
    dac_chnls[5] = 0;

    Usart3Config ();

    DMX_channel_selected = 1;
    DMX_channel_quantity = 6;

    DMX_EnableRx ();

    MA16_U16Circular_Reset (&st_sp1);
    MA16_U16Circular_Reset (&st_sp2);
    MA16_U16Circular_Reset (&st_sp3);
    MA16_U16Circular_Reset (&st_sp4);
    MA16_U16Circular_Reset (&st_sp5);
    MA16_U16Circular_Reset (&st_sp6);    
    
    while (1)
    {
        if (!timer_standby)
        {
            unsigned short dmx_input = 0;

            // channel 1
            dmx_input = MA16_U16Circular (&st_sp1, dmx_getted[0]);
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[0], &dac_chnls[0]);
            // channel 2
            dmx_input = MA16_U16Circular (&st_sp2, dmx_getted[1]);
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[1], &dac_chnls[1]);
            // channel 3
            dmx_input = MA16_U16Circular (&st_sp3, dmx_getted[2]);            
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[2], &dac_chnls[2]);
            // channel 4
            dmx_input = MA16_U16Circular (&st_sp4, dmx_getted[3]);
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[3], &dac_chnls[3]);
            // channel 5
            dmx_input = MA16_U16Circular (&st_sp5, dmx_getted[4]);            
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[4], &dac_chnls[4]);
            // channel 6
            dmx_input = MA16_U16Circular (&st_sp6, dmx_getted[5]);
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[5], &dac_chnls[5]);
            
            timer_standby = 5;
                        
        }
        
        if (DMX_packet_flag)
        {
            DMX_packet_flag = 0;

            if (LED)
                LED_OFF;
            else
                LED_ON;
            
            for (int i = 0; i < 6; i++)
                dmx_getted[i] = data11[i+1] << 4;
            
        }

        UpdatePackets();
        DAC_MUX_Update(dac_chnls);
    }
}


void TF_I2C_Send_Data (void)
{
    I2C1_Init();
    
    while (1)
    {
        LED_ON;
        I2C1_SendByte (I2C_ADDRESS_SLV, 0x55);
        LED_OFF;
        Wait_ms (30);
    }
    
}


void TF_I2C_Check_Address (void)
{
    LED_OFF;
    
    I2C1_Init();
    
    while (1)
    {
        for (unsigned char i = 0; i < 128; i++)
        {
            // addr is right aligned
            // b0 = 0 for write b0 = 1 for read
            unsigned char addr = (i << 1) & 0xFE;    
            if (I2C1_SendAddr(addr) == 1)
            {
                LED_ON;
                break;
            }
            
            Wait_ms(1);
            LED_OFF;
            Wait_ms(9);
            // Wait_ms(10);
        }

        Wait_ms(999);
        LED_OFF;
        
    }
}


extern uint8_t SSD1306_buffer[];
void TF_Oled_Screen (void)
{
    // OLED Init
    Wait_ms(500);    //for supply stability
    I2C1_Init();
    Wait_ms(10);

    //primer pantalla
    LED_ON;
    SCREEN_Init();
    LED_OFF;
    
    while (1)
    {
        LED_ON;
        display_contrast (255);        
        SCREEN_ShowText2(
            "Primera  ",
            " Pantalla",
            "         ",
            "         "
            );

        LED_OFF;
        Wait_ms(5000);

        LED_ON;
        display_contrast (10);
        SCREEN_ShowText2(
            "         ",
            "         ",
            "Segunda  ",
            " Pantalla"
            );
        LED_OFF;
        Wait_ms(5000);

        display_contrast (255);        
        LED_ON;
        display_invert(1);
        SCREEN_ShowText2(
            "Third    ",
            "  Screen ",
            "         ",
            "         "
            );
        LED_OFF;
        Wait_ms(5000);

        LED_ON;
        display_invert(0);
        SCREEN_ShowText2(
            "         ",
            "         ",            
            "Forth    ",
            "  Screen "
            );
        LED_OFF;
        Wait_ms(5000);
        
        

    }
}


void TF_Oled_Screen_Int (void)
{
    // OLED Init
    Wait_ms(500);    //for supply stability
    I2C1_Init();
    Wait_ms(10);

    //primer pantalla
    // LED_ON;
    // SCREEN_Init();
    // LED_OFF;

    // pruebo falla en la int, salta por STOPF?
    // no, reentra antes de generar el stop, si no tengo START no hago nada
    while (1)
    {
        unsigned short dummy = I2C1->SR1;
        dummy += 1;
        dummy = I2C1->SR2;
        dummy += 1;
        Wait_ms(1);
        LED_ON;
        display_contrast (10);
        Wait_ms(1);
        LED_OFF;
        Wait_ms(5000);
    }

}


void TF_Oled_Screen_Int2 (void)
{
    // OLED Init
    Wait_ms(500);    //for supply stability
    I2C1_Init();
    Wait_ms(10);

    //primer pantalla
    LED_ON;
    SCREEN_Init();
    LED_OFF;

    int a = 1;

    while (1)
    {
        if ((a == 1) &&
            (!timer_standby))
        {
            LED_ON;
            display_contrast (255);        
            SCREEN_ShowText2(
                "Primera  ",
                " Pantalla",
                "         ",
                "         "
                );

            LED_OFF;
            timer_standby = 5000;
            a++;
        }

        if ((a == 2) &&
            (!timer_standby))
        {        
            LED_ON;
            display_contrast (10);
            SCREEN_ShowText2(
                "         ",
                "         ",
                "Segunda  ",
                " Pantalla"
                );
            LED_OFF;
            timer_standby = 5000;
            a++;
        }

        if ((a == 3) &&
            (!timer_standby))
        {
            LED_ON;
            display_contrast (255);        
            display_invert(1);
            SCREEN_ShowText2(
                "Third    ",
                "  Screen ",
                "         ",
                "         "
                );
            LED_OFF;
            timer_standby = 5000;
            a++;
        }


        if ((a == 4) &&
            (!timer_standby))
        {        
            LED_ON;
            display_invert(0);
            SCREEN_ShowText2(
                "         ",
                "         ",            
                "Forth    ",
                "  Screen "
                );
            LED_OFF;
            timer_standby = 5000;
            a = 1;            
        }

        display_update_int_state_machine ();
    }
}


// all channels plus oled
void TF_Dmx_All_Channels_5ms_filter_with_Oled (void)
{
    unsigned short dac_chnls [6] = { 0 };
    unsigned short dmx_getted [6] = { 0 };
    unsigned char dmx_to_show [6] = { 0 };    

    // enable timers for channels
    TIM_1_Init();    //ch1
    TIM_8_Init();    //ch2
    TIM_2_Init();    //ch3
    TIM_3_Init();    //ch4
    TIM_4_Init();    //ch5
    TIM_5_Init();    //ch6

    DAC_Config();
    DAC_Output(0);

    TIM6_Init();
    TIM7_Init();

    pwm_chnls[0] = 0;
    pwm_chnls[1] = 0;
    pwm_chnls[2] = 0;
    pwm_chnls[3] = 0;
    pwm_chnls[4] = 0;
    pwm_chnls[5] = 0;

    dac_chnls[0] = 0;
    dac_chnls[1] = 0;
    dac_chnls[2] = 0;
    dac_chnls[3] = 0;
    dac_chnls[4] = 0;
    dac_chnls[5] = 0;

    Usart3Config ();

    DMX_channel_selected = 1;
    DMX_channel_quantity = 6;

    DMX_EnableRx ();

    MA16_U16Circular_Reset (&st_sp1);
    MA16_U16Circular_Reset (&st_sp2);
    MA16_U16Circular_Reset (&st_sp3);
    MA16_U16Circular_Reset (&st_sp4);
    MA16_U16Circular_Reset (&st_sp5);
    MA16_U16Circular_Reset (&st_sp6);    

    // OLED Init
    Wait_ms(500);    //for supply stability
    I2C1_Init();
    Wait_ms(10);

    //primer pantalla
    LED_ON;
    SCREEN_Init();
    LED_OFF;

    SCREEN_ShowText2(
        "Kirno    ",
        "   Tech  ",
        "Smart    ",
        "   Driver"
        );
    timer_standby = 1300;
    
    while (timer_standby)
        display_update_int_state_machine();

    unsigned char update_menu = 0;
    while (1)
    {
        if (!timer_standby)
        {
            unsigned short dmx_input = 0;

            // channel 1
            dmx_input = MA16_U16Circular (&st_sp1, dmx_getted[0]);
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[0], &dac_chnls[0]);
            // channel 2
            dmx_input = MA16_U16Circular (&st_sp2, dmx_getted[1]);
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[1], &dac_chnls[1]);
            // channel 3
            dmx_input = MA16_U16Circular (&st_sp3, dmx_getted[2]);            
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[2], &dac_chnls[2]);
            // channel 4
            dmx_input = MA16_U16Circular (&st_sp4, dmx_getted[3]);
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[3], &dac_chnls[3]);
            // channel 5
            dmx_input = MA16_U16Circular (&st_sp5, dmx_getted[4]);            
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[4], &dac_chnls[4]);
            // channel 6
            dmx_input = MA16_U16Circular (&st_sp6, dmx_getted[5]);
            PWM_Map_Post_Filter (dmx_input, (unsigned char *)&pwm_chnls[5], &dac_chnls[5]);
            
            timer_standby = 5;
                        
        }
        
        if (DMX_packet_flag)
        {
            DMX_packet_flag = 0;

            if (LED)
                LED_OFF;
            else
                LED_ON;
            
            for (int i = 0; i < 6; i++)
            {
                dmx_getted[i] = data11[i+1] << 4;
                dmx_to_show[i] = data11[i+1];
            }

            update_menu = 1;
        }

        if (update_menu)
        {
            unsigned short dmx_channel = 1;
            
            resp_t resp = resp_continue;
            dmx_menu_data_t dmx1_st;
            dmx1_st.dmx_first_chnl = &dmx_channel;
            dmx1_st.mode = 0;    //DMX1_MODE
            dmx1_st.pchannels = dmx_to_show;
            dmx1_st.chnls_qtty = 6;
            dmx1_st.show_addres = 1;

            resp = DMXModeMenu(&dmx1_st);
            if (resp == resp_finish)
                update_menu = 0;
        }
        display_update_int_state_machine();

        UpdatePackets();
        DAC_MUX_Update(dac_chnls);
    }
}


void TF_Uart4_Tx (void)
{
    Uart4Config ();
    Wait_ms (100);

    while (1)
    {
        Uart4Send("Test Uart4 at 9600bps\n");
        Wait_ms(2000);
        
    }
    
}


void TF_Flash_Configurations (void)
{
    memset(&mem_conf, 0, sizeof(mem_conf));
    
    mem_conf.max_power = 1530;
    mem_conf.dmx_first_channel = 1;
    mem_conf.dmx_channel_quantity = 6;
    mem_conf.max_current_channels[0] = 128;
    mem_conf.max_current_channels[1] = 128;
    mem_conf.max_current_channels[2] = 128;
    mem_conf.max_current_channels[3] = 128;
    mem_conf.max_current_channels[4] = 128;
    mem_conf.max_current_channels[5] = 128;

    Flash_WriteConfigurations();

    int error = 0;
    int error_cnt = 0;    
    unsigned char * p_orig = (unsigned char *) &mem_conf;
    unsigned char * p_dest = (unsigned char *) pmem;
    for (int i = 0; i < sizeof(mem_conf); i++)
    {
        if (*(p_orig + i) != *(p_dest + i))
        {
            error = 1;            
            break;
        }
        error_cnt++;
    }

    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        if (error)
            Wait_ms(500);
        else
            Wait_ms(50);        

        // // check good bytes
        // for (int i = 0; i < error_cnt; i++)
        // {
        //     LED_ON;
        //     Wait_ms(100);
        //     LED_OFF;
        //     Wait_ms(100);
        // }

        // Wait_ms(2000);
    }
}


void TF_Flash_Configurations_With_Uart4 (void)
{
    char buff [100];

    Uart4Config ();
    Wait_ms (100);

    memset(&mem_conf, 0, sizeof(mem_conf));
    
    mem_conf.max_power = 1530;
    mem_conf.dmx_first_channel = 1;
    mem_conf.dmx_channel_quantity = 6;
    mem_conf.max_current_channels[0] = 128;
    mem_conf.max_current_channels[1] = 128;
    mem_conf.max_current_channels[2] = 128;
    mem_conf.max_current_channels[3] = 128;
    mem_conf.max_current_channels[4] = 128;
    mem_conf.max_current_channels[5] = 128;

    Flash_WriteConfigurations();

    int error = 0;
    int error_cnt = 0;    
    unsigned char * p_orig = (unsigned char *) &mem_conf;
    unsigned char * p_dest = (unsigned char *) pmem;
    for (int i = 0; i < sizeof(mem_conf); i++)
    {
        if (*(p_orig + i) != *(p_dest + i))
        {
            error = 1;            
            break;
        }
        error_cnt++;
    }

    sprintf(buff, "error: %d error_cnt: %d sizeof(mem): %d\n",
            error,
            error_cnt,
            sizeof(mem_conf));
    Uart4Send(buff);
    Wait_ms(100);

    int end_of_line = 0;
    for (int i = 0; i < sizeof(mem_conf); i++)
    {
        if (!(i % 8))
        {
            sprintf(buff, "i: %d ", i);
            Uart4Send(buff);
        }

        sprintf(buff, "0x%02x ", *(p_orig + i));
        Uart4Send(buff);

        if (end_of_line == 7)
        {
            end_of_line = 0;
            Uart4Send("\n");
        }
        else
            end_of_line++;

        Wait_ms(10);
        
    }

    Uart4Send("\nend of buffer\n");
    while (1);
}
//--- end of file ---//
