//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### TIM.C #################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "tim.h"
#include "stm32f10x.h"
#include "hard.h"
#include "dmx_transceiver.h"
#include "pwm.h"


// Module Private Types Constants and Macros -----------------------------------
#define START_WITH_CHANNEL_DISABLE    // TIM->ARR = 0


#define RCC_TIM1_CLK     (RCC->APB2ENR & 0x00000800)
#define RCC_TIM1_CLK_ON    (RCC->APB2ENR |= 0x00000800)
#define RCC_TIM1_CLK_OFF    (RCC->APB2ENR &= ~0x00000800)

#define RCC_TIM2_CLK     (RCC->APB1ENR & 0x00000001)
#define RCC_TIM2_CLK_ON    (RCC->APB1ENR |= 0x00000001)
#define RCC_TIM2_CLK_OFF    (RCC->APB1ENR &= ~0x00000001)

#define RCC_TIM3_CLK     (RCC->APB1ENR & 0x00000002)
#define RCC_TIM3_CLK_ON    (RCC->APB1ENR |= 0x00000002)
#define RCC_TIM3_CLK_OFF    (RCC->APB1ENR &= ~0x00000002)

#define RCC_TIM4_CLK     (RCC->APB1ENR & 0x00000004)
#define RCC_TIM4_CLK_ON    (RCC->APB1ENR |= 0x00000004)
#define RCC_TIM4_CLK_OFF    (RCC->APB1ENR &= ~0x00000004)

#define RCC_TIM5_CLK     (RCC->APB1ENR & 0x00000008)
#define RCC_TIM5_CLK_ON    (RCC->APB1ENR |= 0x00000008)
#define RCC_TIM5_CLK_OFF    (RCC->APB1ENR &= ~0x00000008)

#define RCC_TIM6_CLK    (RCC->APB1ENR & 0x00000010)
#define RCC_TIM6_CLK_ON    (RCC->APB1ENR |= 0x00000010)
#define RCC_TIM6_CLK_OFF    (RCC->APB1ENR &= ~0x00000010)

#define RCC_TIM7_CLK    (RCC->APB1ENR & 0x00000020)
#define RCC_TIM7_CLK_ON    (RCC->APB1ENR |= 0x00000020)
#define RCC_TIM7_CLK_OFF    (RCC->APB1ENR &= ~0x00000020)

#define RCC_TIM8_CLK     (RCC->APB2ENR & 0x00002000)
#define RCC_TIM8_CLK_ON    (RCC->APB2ENR |= 0x00002000)
#define RCC_TIM8_CLK_OFF    (RCC->APB2ENR &= ~0x00002000)


// #define RCC_TIM14_CLK    (RCC->APB1ENR & 0x00000100)
// #define RCC_TIM14_CLK_ON    (RCC->APB1ENR |= 0x00000100)
// #define RCC_TIM14_CLK_OFF    (RCC->APB1ENR &= ~0x00000100)

// #define RCC_TIM15_CLK    (RCC->APB2ENR & 0x00010000)
// #define RCC_TIM15_CLK_ON    (RCC->APB2ENR |= 0x00010000)
// #define RCC_TIM15_CLK_OFF    (RCC->APB2ENR &= ~0x00010000)

// #define RCC_TIM16_CLK    (RCC->APB2ENR & 0x00020000)
// #define RCC_TIM16_CLK_ON    (RCC->APB2ENR |= 0x00020000)
// #define RCC_TIM16_CLK_OFF    (RCC->APB2ENR &= ~0x00020000)

// #define RCC_TIM17_CLK    (RCC->APB2ENR & 0x00040000)
// #define RCC_TIM17_CLK_ON    (RCC->APB2ENR |= 0x00040000)
// #define RCC_TIM17_CLK_OFF    (RCC->APB2ENR &= ~0x00040000)

// 40KHz -> 25us -> 1600 tick 15.625ns @ 64MHz
#define VALUE_FOR_LEAST_FREQ    1485    //21.38us tick 15.62ns
#define VALUE_FOR_CONSTANT_OFF    115    //1.8us tick 15.62ns

// Externals -------------------------------------------------------------------
extern volatile unsigned short wait_ms_var;


// Globals ---------------------------------------------------------------------


// Module Functions ------------------------------------------------------------

// Channel 5 on ver 3.2
// Channel 1 ver 3.1 3.0 -- out TIM_OUT_CH1 -> TIM1_CH1 @ PA8, in TIM1_CH2 @ PA9
void TIM_1_Init (void)
{
    if (!RCC_TIM1_CLK)
        RCC_TIM1_CLK_ON;

    // timer configuration
    // TIM1->CR1 = 0x00 | TIM_CR1_ARPE;        //clk int / 1 , auto preload enable
    TIM1->CR1 = 0x00;        //clk int / 1

    //Reset mode, trigger with TI2
    TIM1->SMCR |= TIM_SMCR_SMS_2 |
        TIM_SMCR_TS_2 | TIM_SMCR_TS_1;
    
    //CH1 output PWM mode 2 (channel active TIM1->CNT > TIM1->CCR1)
    //CH2 input map IC2->TI2; filtered / 4 N = 6
    // primera prueba 0.44A
    // TIM1->CCMR1 = 0x0070 |
    //     TIM_CCMR1_CC2S_0 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_1;

    // segunda prueba 0.39A
    TIM1->CCMR1 = 0x0070 |
        TIM_CCMR1_CC2S_0 | TIM_CCMR1_IC2F_2;

    // tercer prueba 0.31A
    // TIM1->CCMR1 = 0x0070;
        
    TIM1->CCMR2 = 0x0000;
    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;    // CH1 enable, polarity reversal

#ifdef START_WITH_CHANNEL_DISABLE
    TIM1->ARR = 0;    // start with channel disable
    TIM1->CR1 |= TIM_CR1_ARPE;    // auto preload enable    
#else
    TIM1->ARR = VALUE_FOR_LEAST_FREQ;
#endif
 
    TIM1->CNT = 0;
    TIM1->PSC = 0;

    TIM1->BDTR |= TIM_BDTR_MOE;    
    
    // Enable timer ver UDIS
    TIM1->CCR1 = VALUE_FOR_CONSTANT_OFF;
    TIM1->CR1 |= TIM_CR1_CEN;

#ifdef START_WITH_CHANNEL_DISABLE
    unsigned int temp = 0;
    temp = GPIOA->CRH; 
    temp &= 0xFFFFFFF0;    //PA8 TIM1_CH1 alternative push-pull 2MHz
    temp |= 0x0000000A;
    GPIOA->CRH = temp;
#endif
    
}


// Channel 4 on ver 3.2
// Channel 2 ver 3.1 3.0 -- out TIM_OUT_CH2 -> TIM8_CH1 @ PC6, in TIM8_CH2 @ PC7
void TIM_8_Init (void)
{
    if (!RCC_TIM8_CLK)
        RCC_TIM8_CLK_ON;

    // timer configuration
    // TIM8->CR1 = 0x00 | TIM_CR1_ARPE;        //clk int / 1 , auto preload enable
    TIM8->CR1 = 0x00;    //clk int / 1    

    //Reset mode, trigger with TI2
    TIM8->SMCR |= TIM_SMCR_SMS_2 |
        TIM_SMCR_TS_2 | TIM_SMCR_TS_1;
    
    //CH1 output PWM mode 2 (channel active TIM8->CNT > TIM8->CCR1)
    //CH2 input map IC2->TI2; filtered / 4 N = 6
    // TIM8->CCMR1 = 0x0070 |
    //     TIM_CCMR1_CC2S_0 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_1;

    // less filter 0.39A?
    TIM8->CCMR1 = 0x0070 |
        TIM_CCMR1_CC2S_0 | TIM_CCMR1_IC2F_2;
    
    TIM8->CCMR2 = 0x0000;
    TIM8->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;    // CH1 enable, polarity reversal
    
#ifdef START_WITH_CHANNEL_DISABLE
    TIM8->ARR = 0;    // start with channel disable
    TIM8->CR1 |= TIM_CR1_ARPE;     //auto preload enable    
#else
    TIM8->ARR = VALUE_FOR_LEAST_FREQ;
#endif

    TIM8->CNT = 0;
    TIM8->PSC = 0;

    TIM8->BDTR |= TIM_BDTR_MOE;    
    
    // Enable timer ver UDIS
    TIM8->CCR1 = VALUE_FOR_CONSTANT_OFF;
    TIM8->CR1 |= TIM_CR1_CEN;

#ifdef START_WITH_CHANNEL_DISABLE
    unsigned int temp = 0;
    temp = GPIOC->CRL;
    temp &= 0xF0FFFFFF;    //PC6 TIM8_CH1 alternative push-pull 2MHz
    temp |= 0x0A000000;
    GPIOC->CRL = temp;
#endif
    
}

// Channel 1 on ver 3.2
// Channel 3 ver 3.1 3.0 -- out TIM_OUT_CH3 -> TIM2_CH3 @ PA2, in TIM2_CH1 @ PA0
void TIM_2_Init (void)
{
    if (!RCC_TIM2_CLK)
        RCC_TIM2_CLK_ON;

    // timer configuration
    // TIM2->CR1 = 0x00 | TIM_CR1_ARPE;        //clk int / 1 , auto preload enable
    TIM2->CR1 = 0x00;        //clk int / 1    

    //Reset mode, trigger with TI1
    TIM2->SMCR |= TIM_SMCR_SMS_2 |
        TIM_SMCR_TS_2 | TIM_SMCR_TS_0;
    
    //CH3 output PWM mode 2 (channel active TIM2->CNT > TIM2->CCR3)
    //CH1 input map IC1->TI1; filtered / 4 N = 6
    TIM2->CCMR1 = 0x0000 |
        TIM_CCMR1_CC1S_0 | TIM_CCMR1_IC1F_2 | TIM_CCMR1_IC1F_1;

    TIM2->CCMR2 = 0x0070;

    TIM2->CCER |= TIM_CCER_CC3E | TIM_CCER_CC3P;    // CH3 enable, polarity reversal

#ifdef START_WITH_CHANNEL_DISABLE
    TIM2->ARR = 0;    // start with channel disable
    TIM2->CR1 |= TIM_CR1_ARPE;        //clk int / 1 , auto preload enable
#else
    TIM2->ARR = VALUE_FOR_LEAST_FREQ;
#endif

    TIM2->CNT = 0;
    TIM2->PSC = 0;

    // Enable timer ver UDIS
    TIM2->CCR3 = VALUE_FOR_CONSTANT_OFF;
    TIM2->CR1 |= TIM_CR1_CEN;

#ifdef START_WITH_CHANNEL_DISABLE
    unsigned int temp = 0;
    temp = GPIOA->CRL;    
    temp &= 0xFFFFF0FF;    // PA2 TIM2_CH3 alternative push-pull 2MHz
    temp |= 0x00000A00;
    GPIOA->CRL = temp;
#endif
    
}

// Channel 3 on ver 3.2
// Channel 4 ver 3.1 3.0 -- out TIM_OUT_CH4 -> TIM3_CH1 @ PA6, in TIM3_CH2 @ PA7
void TIM_3_Init (void)
{
    if (!RCC_TIM3_CLK)
        RCC_TIM3_CLK_ON;

    // timer configuration
    // TIM3->CR1 = 0x00 | TIM_CR1_ARPE;        //clk int / 1 , auto preload enable
    TIM3->CR1 = 0x00;        //clk int / 1 

    //Reset mode, trigger with TI2
    TIM3->SMCR |= TIM_SMCR_SMS_2 |
        TIM_SMCR_TS_2 | TIM_SMCR_TS_1;
    
    //CH1 output PWM mode 2 (channel active TIM3->CNT > TIM3->CCR1)
    //CH2 input map IC2->TI2; filtered / 4 N = 6
    TIM3->CCMR1 = 0x0070 |
        TIM_CCMR1_CC2S_0 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_1;

    TIM3->CCMR2 = 0x0000;
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;    // CH1 enable, polarity reversal

#ifdef START_WITH_CHANNEL_DISABLE
    TIM3->ARR = 0;    // start with channel disable
    TIM3->CR1 |= TIM_CR1_ARPE;        //clk int / 1 , auto preload enable
#else
    TIM3->ARR = VALUE_FOR_LEAST_FREQ;
#endif

    TIM3->CNT = 0;
    TIM3->PSC = 0;

    // Enable timer ver UDIS
    TIM3->CCR1 = VALUE_FOR_CONSTANT_OFF;
    TIM3->CR1 |= TIM_CR1_CEN;

#ifdef START_WITH_CHANNEL_DISABLE
    unsigned int temp = 0;
    temp = GPIOA->CRL;    
    temp &= 0xF0FFFFFF;    //PA6 TIM3_CH1 alternative push-pull 2MHz
    temp |= 0x0A000000;
    GPIOA->CRL = temp;
#endif

}


// Channel 6 on ver 3.2
// Channel 5 ver 3.1 3.0 -- out TIM_OUT_CH5 -> TIM4_CH1 @ PB6, in TIM4_CH2 @ PB7
void TIM_4_Init (void)
{
    if (!RCC_TIM4_CLK)
        RCC_TIM4_CLK_ON;

    // timer configuration
    // TIM4->CR1 = 0x00 | TIM_CR1_ARPE;        //clk int / 1 , auto preload enable
    TIM4->CR1 = 0x00;        //clk int / 1 

    //Reset mode, trigger with TI2
    TIM4->SMCR |= TIM_SMCR_SMS_2 |
        TIM_SMCR_TS_2 | TIM_SMCR_TS_1;
    
    //CH1 output PWM mode 2 (channel active TIM4->CNT > TIM4->CCR1)
    //CH2 input map IC2->TI2; filtered / 4 N = 6
    TIM4->CCMR1 = 0x0070 |
        TIM_CCMR1_CC2S_0 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_1;

    TIM4->CCMR2 = 0x0000;
    TIM4->CCER |= TIM_CCER_CC1E | TIM_CCER_CC1P;    // CH1 enable, polarity reversal
    
#ifdef START_WITH_CHANNEL_DISABLE
    TIM4->ARR = 0;    // start with channel disable
    TIM4->CR1 |= TIM_CR1_ARPE;        //auto preload enable    
#else
    TIM4->ARR = VALUE_FOR_LEAST_FREQ;
#endif

    TIM4->CNT = 0;
    TIM4->PSC = 0;

    // Enable timer ver UDIS
    TIM4->CCR1 = VALUE_FOR_CONSTANT_OFF;
    TIM4->CR1 |= TIM_CR1_CEN;

#ifdef START_WITH_CHANNEL_DISABLE
    unsigned int temp = 0;
    temp = GPIOB->CRL;    
    temp &= 0xF0FFFFFF;    //PB6 TIM4_CH1 alternative push-pull 2MHz
    temp |= 0x0A000000;
    GPIOB->CRL = temp;
#endif
    
}

// Channel 2 on ver 3.2
// Channel 6 ver 3.1 3.0 -- out TIM_OUT_CH6 -> TIM5_CH4 @ PA3, in TIM5_CH2 @ PA1
void TIM_5_Init (void)
{
    if (!RCC_TIM5_CLK)
        RCC_TIM5_CLK_ON;

    // timer configuration
    // TIM5->CR1 = 0x00 | TIM_CR1_ARPE;        //clk int / 1 , auto preload enable
    TIM5->CR1 = 0x00;        //clk int / 1 

    //Reset mode, trigger with TI2
    TIM5->SMCR |= TIM_SMCR_SMS_2 |
        TIM_SMCR_TS_2 | TIM_SMCR_TS_1;
    
    //CH4 output PWM mode 2 (channel active TIM5->CNT > TIM5->CCR4)
    //CH2 input map IC2->TI2; filtered / 4 N = 6
    TIM5->CCMR1 = 0x0000 |
        TIM_CCMR1_CC2S_0 | TIM_CCMR1_IC2F_2 | TIM_CCMR1_IC2F_1;

    TIM5->CCMR2 = 0x7000;
    TIM5->CCER |= TIM_CCER_CC4E | TIM_CCER_CC4P;    // CH4 enable, polarity reversal

#ifdef START_WITH_CHANNEL_DISABLE
    TIM5->ARR = 0;    // start with channel disable
    TIM5->CR1 |= TIM_CR1_ARPE;        //auto preload enable    
#else
    TIM5->ARR = VALUE_FOR_LEAST_FREQ;
#endif

    TIM5->CNT = 0;
    TIM5->PSC = 0;

    // Enable timer ver UDIS
    TIM5->CCR4 = VALUE_FOR_CONSTANT_OFF;
    TIM5->CR1 |= TIM_CR1_CEN;

#ifdef START_WITH_CHANNEL_DISABLE
    unsigned int temp = 0;
    temp = GPIOA->CRL;    
    temp &= 0xFFFF0FFF;    //PA3 TIM5_CH4 alternative push-pull 2MHz   
    temp |= 0x0000A000;
    GPIOA->CRL = temp;
#endif
    
}


void TIM6_Init(void)
{
    //---- Clk ----//
    if (!RCC_TIM6_CLK)
        RCC_TIM6_CLK_ON;

    //--- Config ----//
    TIM6->CR1 |= TIM_CR1_URS;
    // TIM6->ARR = 2550; //2550 -> 2KHz
    // TIM6->ARR = 5110; //5110 -> 1KHz
    TIM6->ARR = PWM_INT_MAX * PWM_TIMER_MULTIPLIER; //5110 -> 1KHz    
    TIM6->CNT = 0;
    TIM6->PSC = 13;
    // TIM6->PSC = 25;    
    // TIM6->PSC = 71;
    // TIM6->PSC = 719;

    // Int on Update
    TIM6->DIER |= TIM_DIER_UIE;

    //Habilito NVIC
    //Interrupcion timer6.
    NVIC_EnableIRQ(TIM6_IRQn);
    NVIC_SetPriority(TIM6_IRQn, 5);

    TIM6->CR1 |= TIM_CR1_CEN;
}


void TIM6_Update (unsigned short new_val)
{
    TIM6->CNT = 0;
    TIM6->ARR = new_val;
}


void TIM6_IRQHandler (void)
{
    // avoid int reentrant
    TIM6->SR = 0;
    TIM6->ARR = PWM_INT_MAX * PWM_TIMER_MULTIPLIER; //5110 -> 1KHz
    
    PWM_Timer_Handler();
    
    // low the flag
    if (TIM6->SR & TIM_SR_UIF)
        TIM6->SR &= ~(TIM_SR_UIF);    
}


// TIM7 is free running with 1us tick
void TIM7_Init (void)
{
    //---- Clk ----//
    if (!RCC_TIM7_CLK)
        RCC_TIM7_CLK_ON;

    //--- Config ----//
    TIM7->ARR = 0xFFFF;
    TIM7->CNT = 0;
    TIM7->PSC = 63;
    TIM7->EGR = TIM_EGR_UG; //update registers

    // Enable timer
    // TIM7->DIER |= TIM_DIER_UIE;    //int on update
    TIM7->CR1 |= TIM_CR1_CEN;

    // NVIC Enable
    // Int for timer7.
    NVIC_EnableIRQ(TIM7_IRQn);
    NVIC_SetPriority(TIM7_IRQn, 10);    
}


void TIM7_IRQHandler (void)	// one shoot mode
{
    // avoid int reentrant, low flag
    TIM7->SR = 0;

    SendDMXPacket(PCKT_UPDATE);
}


void TIM7_To_OneShoot (void)
{
    TIM7->CR1 &= ~(TIM_CR1_CEN);    // disable timer
    TIM7->DIER |= TIM_DIER_UIE;
    
    TIM7->ARR = 0;    //leave it stand by
    // enable timer with one pulse mode, int only on overflow
    TIM7->CR1 |= TIM_CR1_CEN | TIM_CR1_OPM | TIM_CR1_URS;    
}


void TIM7_To_FreeRunning (void)
{
    TIM7->CR1 &= ~(TIM_CR1_CEN | TIM_CR1_OPM);    // disable timer and one pulse
    TIM7->DIER &= ~(TIM_DIER_UIE);    // disable int
    
    TIM7->ARR = 0xFFFF;    //set to free running
    TIM7->CR1 |= TIM_CR1_CEN;    // enable timer
}


void TIM7_OneShoot (unsigned short a)
{
    TIM7->CNT = 0;
    TIM7->ARR = a;
    TIM7->CR1 |= TIM_CR1_CEN;
}


void Wait_ms (unsigned short a)
{
    wait_ms_var = a;

    while (wait_ms_var);
}


// Special Functions -----------------------------------------------------------
void TIM_Deactivate_Channels (unsigned char deact_chnls)
{
#ifdef HARDWARE_VERSION_3_2
    
    if (deact_chnls & CH1_FLAG)
        TIM2->ARR = 0;
    if (deact_chnls & CH2_FLAG)
        TIM5->ARR = 0;
    if (deact_chnls & CH3_FLAG)
        TIM3->ARR = 0;
    if (deact_chnls & CH4_FLAG)
        TIM8->ARR = 0;
    if (deact_chnls & CH5_FLAG)
        TIM1->ARR = 0;
    if (deact_chnls & CH6_FLAG)
        TIM4->ARR = 0;
    
#else    // ver 3.1 & ver 3.0
    
    if (deact_chnls & CH1_FLAG)
        TIM1->ARR = 0;
    if (deact_chnls & CH2_FLAG)
        TIM8->ARR = 0;
    if (deact_chnls & CH3_FLAG)
        TIM2->ARR = 0;
    if (deact_chnls & CH4_FLAG)
        TIM3->ARR = 0;
    if (deact_chnls & CH5_FLAG)
        TIM4->ARR = 0;
    if (deact_chnls & CH6_FLAG)
        TIM5->ARR = 0;    
#endif
    
}


void TIM_Activate_Channels (unsigned char act_chnls)
{
#ifdef HARDWARE_VERSION_3_2
    
    if ((act_chnls & CH1_FLAG) && (!TIM2->ARR))
    {
        TIM2->ARR = VALUE_FOR_LEAST_FREQ;
        TIM2->EGR |= TIM_EGR_UG;    
    }

    if ((act_chnls & CH2_FLAG) && (!TIM5->ARR))
    {
        TIM5->ARR = VALUE_FOR_LEAST_FREQ;
        TIM5->EGR |= TIM_EGR_UG;    
    }

    if ((act_chnls & CH3_FLAG) && (!TIM3->ARR))
    {
        TIM3->ARR = VALUE_FOR_LEAST_FREQ;
        TIM3->EGR |= TIM_EGR_UG;    
    }

    if ((act_chnls & CH4_FLAG) && (!TIM8->ARR))
    {
        TIM8->ARR = VALUE_FOR_LEAST_FREQ;
        TIM8->EGR |= TIM_EGR_UG;    
    }

    if ((act_chnls & CH5_FLAG) && (!TIM1->ARR))
    {
        TIM1->ARR = VALUE_FOR_LEAST_FREQ;
        TIM1->EGR |= TIM_EGR_UG;    
    }

    if ((act_chnls & CH6_FLAG) && (!TIM4->ARR))
    {
        TIM4->ARR = VALUE_FOR_LEAST_FREQ;
        TIM4->EGR |= TIM_EGR_UG;    
    }
    
#else    // ver 3.1 & ver 3.0

    if ((act_chnls & CH1_FLAG) && (!TIM1->ARR))
    {
        TIM1->ARR = VALUE_FOR_LEAST_FREQ;
        TIM1->EGR |= TIM_EGR_UG;    
    }

    if ((act_chnls & CH2_FLAG) && (!TIM8->ARR))
    {
        TIM8->ARR = VALUE_FOR_LEAST_FREQ;
        TIM8->EGR |= TIM_EGR_UG;    
    }

    if ((act_chnls & CH3_FLAG) && (!TIM2->ARR))
    {
        TIM2->ARR = VALUE_FOR_LEAST_FREQ;
        TIM2->EGR |= TIM_EGR_UG;    
    }

    if ((act_chnls & CH4_FLAG) && (!TIM3->ARR))
    {
        TIM3->ARR = VALUE_FOR_LEAST_FREQ;
        TIM3->EGR |= TIM_EGR_UG;    
    }

    if ((act_chnls & CH5_FLAG) && (!TIM4->ARR))
    {
        TIM4->ARR = VALUE_FOR_LEAST_FREQ;
        TIM4->EGR |= TIM_EGR_UG;    
    }

    if ((act_chnls & CH6_FLAG) && (!TIM5->ARR))
    {
        TIM5->ARR = VALUE_FOR_LEAST_FREQ;
        TIM5->EGR |= TIM_EGR_UG;    
    }

#endif
}
