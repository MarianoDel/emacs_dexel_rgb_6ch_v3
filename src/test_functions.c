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
#include "pwm.h"
#include "dac_mux.h"

#include <stdio.h>


// Externals -------------------------------------------------------------------
extern volatile unsigned short pwm_chnls[];

// Globals ---------------------------------------------------------------------


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------
void TF_Led_On_Ctrl_Fan (void);
void TF_Pwm_Channel_1 (void);
void TF_Pwm_Channel_2 (void);
void TF_Pwm_Channel_3 (void);
void TF_Pwm_Channel_4 (void);
void TF_Pwm_Channel_5 (void);
void TF_Pwm_Channel_6 (void);
void TF_Dac1 (void);
void TF_Int_Timer5 (void);

void TF_Complete_Channel_1 (void);

void TF_All_Chain_For_Channel_1 (void);
void TF_All_Chain_For_Channel_1_Max (void);

void TF_Dac_Mux (void);


// Module Functions ------------------------------------------------------------
void TF_Hardware_Tests (void)
{
    // TF_Led_On_Ctrl_Fan ();
    // TF_Pwm_Channel_1 ();
    // TF_Pwm_Channel_2 ();
    // TF_Pwm_Channel_3 ();
    // TF_Pwm_Channel_4 ();
    // TF_Pwm_Channel_5 ();
    // TF_Pwm_Channel_6 ();
    // TF_Dac1 ();
    // TF_Complete_Channel_1 ();

    // TF_All_Chain_For_Channel_1 ();
    // TF_All_Chain_For_Channel_1_Max ();
    TF_Dac_Mux ();
    
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
    unsigned short pwm_ena = 0;
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

// void TF_Int_Timer5 (void)
// {
//     TIM5_Init();
    
//     while (1)
//     {
//         Wait_ms(1000);
//     }    
// }

//--- end of file ---//
