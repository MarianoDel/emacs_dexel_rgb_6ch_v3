//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### GPIO.C ################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "stm32f10x.h"
#include "gpio.h"
#include "hard.h"


// Configurations --------------------------------------------------------------
// #define USE_EXTERNAL_INTS



// Private Types Constants and Macros ------------------------------------------
//--- RCC clkEnable ---//
#define RCC_GPIOA_CLK    (RCC->APB2ENR & 0x00000004)
#define RCC_GPIOA_CLKEN    (RCC->APB2ENR |= 0x00000004)
#define RCC_GPIOA_CLKDIS    (RCC->APB2ENR &= ~0x00000004)

#define RCC_GPIOB_CLK    (RCC->APB2ENR & 0x00000008)
#define RCC_GPIOB_CLKEN    (RCC->APB2ENR |= 0x00000008)
#define RCC_GPIOB_CLKDIS    (RCC->APB2ENR &= ~0x00000008)

#define RCC_GPIOC_CLK    (RCC->APB2ENR & 0x00000010)
#define RCC_GPIOC_CLKEN    (RCC->APB2ENR |= 0x00000010)
#define RCC_GPIOC_CLKDIS    (RCC->APB2ENR &= ~0x00000010)

#define RCC_GPIOD_CLK    (RCC->APB2ENR & 0x00000020)
#define RCC_GPIOD_CLKEN    (RCC->APB2ENR |= 0x00000020)
#define RCC_GPIOD_CLKDIS    (RCC->APB2ENR &= ~0x00000020)

#define RCC_AFIO_CLK    (RCC->APB2ENR & 0x00000001)
#define RCC_AFIO_CLKEN    (RCC->APB2ENR |= 0x00000001)
#define RCC_AFIO_CLKDIS    (RCC->APB2ENR &= ~0x00000001)


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Functions ------------------------------------------------------------


//--- Tamper config ---//
void Tamper_Config(void)
{
	unsigned long temp;

	//--- GPIOB ---//
	//--- Clock ---//
	if (!(RCC->APB2ENR & 0x00000008))
		RCC->APB2ENR |= 0x00000008;

	//--- Config pines ---//
	temp = GPIOB->CRH;
	temp &= 0xFF0FFFFF;
	temp |= 0x00800000;
	GPIOB->CRH = temp;
	GPIOB->BSRR = 0x00002000;
}





//------- GPIO REGISTERS ----------//
//
//	GPIOx->CRL	pin 7 - 0
//	3  2  1  0
//	CNF0  MODE0
//	GPIOx->CRH	pin 15 - 8
//
//      En hexa me queda cada nibble es igual a la configuracion de ese pin
//      nibble0 configura completo pin0
//
//	MODEx 00 Input (reset)
//	MODEx 01 Output 10MHz
//	MODEx 10 Output 2MHz
//	MODEx 11 Output 50MHz
//
//	Input Mode
//      las funciones alternativas de los pines estan directamente conectadas al
//      periferico en el modo input
//      CNFx 00 Analog
//      CNFx 01 Floating (reset)
//      CNFx 10 Input (pull up / dwn)
//      CNFx 11 Reserved
//
//	Output Mode
//      CNFx 00 Push Pull
//      CNFx 01 Open Drain
//      CNFx 10 Alternate func Push Pull
//      CNFx 11 Alternate func Open Drain
//
//      Pull-Up Pull-Dwn si el pin es Input el registro ODR coloca pull-up (1) o pull-dwn (0)
//      GPIOx->ODR 0xNNNN, 1 bit por pin
//

//-- GPIO Configuration --------------------
void GpioInit (void)
{
    unsigned long temp;

    //--- GPIO Clocks ---//
    if (!RCC_GPIOA_CLK)
        RCC_GPIOA_CLKEN;

    if (!RCC_GPIOB_CLK)
        RCC_GPIOB_CLKEN;

    if (!RCC_GPIOC_CLK)
        RCC_GPIOC_CLKEN;

    if (!RCC_GPIOD_CLK)
        RCC_GPIOD_CLKEN;

    //Clock Enable for alternative functions
    if (!RCC_AFIO_CLK)
        RCC_AFIO_CLKEN;

    //--- GPIOA Low Side ------------------//
    //PA0 TIM2_CH1 alternative input
    //PA1 TIM5_CH2 alternative input
    //PA2 TIM2_CH3 alternative push-pull 2MHz
    //PA3 TIM5_CH4 alternative push-pull 2MHz
    //PA4 DAC_OUT1 (analog input)
    //PA5 analog I_Sense_2
    //PA6 TIM3_CH1 alternative push-pull 2MHz
    //PA7 TIM3_CH2 alternative input
    temp = GPIOA->CRL;    
    temp &= 0x00F00000;   
    temp |= 0x4A00AA44;
    GPIOA->CRL = temp;

    //--- GPIOA High Side ------------------//
    //PA8 TIM1_CH1 alternative push-pull 2MHz
    //PA9 TIM1_CH2 alternative input
    //PA10 alternative Rx Usart1
    //PA11 PWM2002 alternative push-pull 50MHz
    //PA12 NC
    //PA13 NC
    //PA14 NC
    //PA15 NC
    temp = GPIOA->CRH; 
    temp &= 0xFFFFFF00;
    temp |= 0x0000004A;
    GPIOA->CRH = temp;

    //--- GPIOA Pull-Up Pull-Dwn ------------------//
#ifdef USE_PULLUP_ON_RX_INPUT
    temp = GPIOA->ODR;
    temp &= 0xFBFF;    //PA10 pull-up
    temp |= 0x0400;
    GPIOA->ODR = temp;
#endif
        
    //--- GPIOB Low Side -------------------//
    //PB0 EN_CH1
    //PB1 EN_CH2
    //PB2 EN_CH3
    //PB3 NC
    //PB4 NC
    //PB5 NC
    //PB6 TIM4_CH1 alternative push-pull 2MHz
    //PB7 TIM4_CH2 alternative input
    temp = GPIOB->CRL;    
    temp &= 0x00FFF000;
    temp |= 0x4A000222;
    GPIOB->CRL = temp;

    //--- GPIOB High Side -------------------//
    //PB8 PWM404
    //PB9 PWMN4
    //PB10 alternative Tx Usart3
    //PB11 alternative Rx Usart3
    //PB12 LED3
    //PB13 EN_CH4
    //PB14 NC
    //PB15 NC    
    temp = GPIOB->CRH;
    temp &= 0xFF000000;
    temp |= 0x00224BBB;
    GPIOB->CRH = temp;    
    
    //--- GPIOC Low Side -------------------//
    //PC0 EN_SW
    //PC1 EN_DT
    //PC2 EN_CLK
    //PC3 CTRL_FAN
    //PC4 
    //PC5 
    //PC6 TIM8_CH1 alternative push-pull 2MHz
    //PC7 TIM8_CH2 alternative input
    temp = GPIOC->CRL;
    temp &= 0x00FF0FFF;
    temp |= 0x4A002000;
    GPIOC->CRL = temp;

    //--- GPIOC High Side -------------------//
    //PC8 CTRL_C2
    //PC9 CTRL_C1
    //PC10 alternative Tx Uart4
    //PC11 alternativo Rx Uart4
    //PC12 alternativo Tx Uart5
    //PC13 BUZZER
    //PC14 NC
    //PC15 NC
    temp = GPIOC->CRH;    
    temp &= 0xFF000000;    
    temp |= 0x002B4B22;
    GPIOC->CRH = temp;

    //--- GPIOD Low Side -------------------//
    //PD0 NC
    //PD1 NC
    //PD2 alternative Rx Uart5
    //PD3 No implemented
    //PD4 No implemented
    //PD5 No implemented
    //PD6 No implemented
    //PD7 No implemented
    temp = GPIOD->CRL;  
    temp &= 0xFFFFF0FF;    
    temp |= 0x00000400;
    GPIOD->CRL = temp;

#ifdef USE_EXTERNAL_INTS
    //Interrupt en PA4 y PA5
    if (!RCC_AFIO_CLK)
        RCC_AFIO_CLKEN;

    //Select Port A & Pin0 for external interrupt
    temp = AFIO->EXTICR[0];
    temp &= ~AFIO_EXTICR1_EXTI0;
    temp |= AFIO_EXTICR1_EXTI0_PA;
    AFIO->EXTICR[0] = (unsigned short) temp;
    
    // EXTI->IMR |= 0x00000001; 			//Corresponding mask bit for interrupts EXTI0
    EXTI->EMR |= 0x00000000; 			//Corresponding mask bit for events
    EXTI->RTSR |= 0x00000001; 			//Interrupt line on rising edge
    EXTI->FTSR |= 0x00000000; 			//Interrupt line on falling edge

    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_SetPriority(EXTI0_IRQn, 2);

#endif
}

#ifdef USE_EXTERNAL_INTS
inline void EXTIOff (void)
{
    EXTI->IMR &= ~0x00000001;
}

inline void EXTIOn (void)
{
    EXTI->IMR |= 0x00000001;
}
#endif

//--- end of file ---//
