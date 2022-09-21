//---------------------------------------------------------
// #### DEXEL PROJ 6CH BIDIRECTIONAL v3 - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### MAIN.C ############################################
//---------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "stm32f10x.h"
#include "hard.h"
#include "startup_clocks.h"
#include "tim.h"
#include "usart.h"
#include "adc.h"
#include "gpio.h"
#include "dma.h"
#include "i2c.h"
// #include "flash_program.h"
#include "dac.h"
#include "dac_mux.h"

#include "dsp.h"
#include "test_functions.h"
#include "dmx_transceiver.h"

#include "manager.h"

// linked modules
#include "parameters.h"
#include "screen.h"
#include "ssd1306_display.h"


#include <stdio.h>

// Externals -------------------------------------------------------------------
// -- for the adc
volatile unsigned short adc_ch [ADC_CHANNEL_QUANTITY];
volatile unsigned char seq_ready;

volatile unsigned short wait_ms_var = 0;
// -- for dmx
volatile unsigned char data11[SIZEOF_DMX_DATA11];
volatile unsigned char Packet_Detected_Flag = 0;
volatile unsigned char DMX_packet_flag = 0;
volatile unsigned char RDM_packet_flag = 0;
volatile unsigned short DMX_channel_selected = 0;
volatile unsigned char DMX_channel_quantity = 0;

// -- for pwm channels and dimmer channels
volatile unsigned char pwm_chnls [6] = { 0 };
volatile unsigned short dac_chnls [6] = { 0 };

// -- for manager timers
volatile unsigned short timer_standby = 0;

// -- for the timeouts in the modes ----
void (* ptFTT ) (void) = NULL;


// Globals ---------------------------------------------------------------------
// parameters_typedef * pmem = (parameters_typedef *) (unsigned int *) FLASH_PAGE_FOR_BKP;	//en flash

parameters_typedef mem_conf;



// Private Functions -----------------------------------------------------------
void SysTickError (void);
void TimingDelay_Decrement(void);

void DisableIrqs (void);
void EnableIrqs (void);


// Module Functions ------------------------------------------------------------
int main (void)
{
    // System Clock already configured at this point
    
    // Gpio Configuration.
    GpioInit();

    // Systick Timer Activation
    if (SysTick_Config(64000))
        SysTickError();

    // Hardware Tests Functions
    // TF_Hardware_Tests ();    

    // Hardware Inits. ---------------------------
    // enable timers for channels
    TIM_1_Init();    //ch1
    TIM_8_Init();    //ch2
    TIM_2_Init();    //ch3
    TIM_3_Init();    //ch4
    TIM_4_Init();    //ch5
    TIM_5_Init();    //ch6

    // enable DAC dimmer
    DAC_Config();
    DAC_Output(0);

    // enable PWM dimmer and DMX transceiver timer
    TIM6_Init();
    TIM7_Init();

    // DMX defaults
    Usart3Config ();
    DMX_channel_selected = 1;    //TODO: check default mem config line 141
    DMX_channel_quantity = 6;

    // Start filters
    FiltersAndOffsets_Filters_Reset ();

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

    // Default mem config
    mem_conf.max_power = 1530;
    mem_conf.dmx_first_channel = 1;
    mem_conf.dmx_channel_quantity = 6;
    mem_conf.max_current_channels[0] = 255;
    mem_conf.max_current_channels[1] = 255;
    mem_conf.max_current_channels[2] = 255;
    mem_conf.max_current_channels[3] = 255;
    mem_conf.max_current_channels[4] = 255;
    mem_conf.max_current_channels[5] = 255;    


    // main program
    while (1)
    {
        Manager(&mem_conf);
    }

}

//--- End of Main ---//


void TimingDelay_Decrement(void)
{
    if (wait_ms_var)
        wait_ms_var--;

    if (timer_standby)
        timer_standby--;

    Manager_Timeouts ();

    // Modes Menus Timers
    if (ptFTT != NULL)
        ptFTT();

    // DAC_MUX_Timeouts();

    Dmx_Timeouts ();

    FiltersAndOffsets_Calc_SM (&mem_conf);
    
}


void SysTickError (void)
{
    //Capture systick error...
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        for (unsigned char i = 0; i < 255; i++)
        {
            asm ("nop \n\t"
                 "nop \n\t"
                 "nop \n\t" );
        }
    }
}


void DisableIrqs (void)
{
    __disable_irq;
}


void EnableIrqs (void)
{
    __enable_irq;
}

//--- end of file ---//
