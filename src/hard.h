//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### HARD.H #################################
//---------------------------------------------

#ifndef _HARD_H_
#define _HARD_H_


// Defines For Configuration ---------------------------------------------------
//---- Configuration for Hardware Versions -------
#include "version.h"

//----- Board Configuration -------------------//
//---- Features Configuration ----------------
// #define WITH_BIDIRECTIONAL

//--- Check the temp sensor ---//
#define USE_OVERTEMP_PROT

//--- if applies check for ntc connection ---//
#define USE_NTC_DETECTION

//--- Check the voltage sensor ---//
#define USE_VOLTAGE_PROT

//--- Uses of CTRL_FAN Ouput as FAN or signaling ---//
#define USE_CTRL_FAN_FOR_TEMP_CTRL

// --- Encoder Motion Direction --- //
#define USE_ENCODER_DIRECT    //dt one on rising clk is CW (clockwise)
// #define USE_ENCODER_INVERT    //dt one on rising clk is CCW (counter-clockwise)

//------ Configuration for Firmware-Channels -----
#define WHITE_AS_IN_RGB		//el blanco lo forma con los 3 colores
//#define WHITE_AS_WHITE	//el blanco tiene leds blancos individuales

// --- UsartDebug Configs --- //
// #define USART_DEBUG_MODE
#define UsartDebug    Uart4Send    //use uart4 as debug serial console

//-------- PWM Conf ------------------------


//-- Configuration for some Analog things ------
//---- Voltage Sensor
#define MIN_PWR_SUPPLY    VOLTS_20
#define MAX_PWR_SUPPLY    VOLTS_50

#define VOLTS_20    730
#define VOLTS_50    1825
//---- LM335 or NTC1K measurement Temperatures
#include "temperatures.h"

//-------- End Of Defines For Configuration ------


//-- Sanity Checks ----------------------------------
#if (!defined HARDWARE_VERSION_3_0)
#error "Not HARD version selected on version.h"
#endif

#if (!defined TEMP_SENSOR_LM335) && \
    (!defined TEMP_SENSOR_NTC1K)
#error "Not Temp Sensor selected on temperatures.h"
#endif

//-- End of Sanity Checks ---------------------------





// Module Exported Constants ---------------------------------------------------
typedef enum {
    SW_NO = 0,
    SW_MIN,
    SW_HALF,
    SW_FULL
    
} resp_sw_t;


// Gpios Configuration ---------------------------------------------------------
#ifdef HARDWARE_VERSION_3_0

//--- Port C ---//
// PC0
#define EN_SW    ((GPIOC->IDR & 0x0001) == 0)

// PC1
#define EN_DT    ((GPIOC->IDR & 0x0002) == 0)

// PC2
#define EN_CLK    ((GPIOC->IDR & 0x0004) == 0)

// PC3
#define CTRL_FAN    ((GPIOC->ODR & 0x0008) != 0)
#define CTRL_FAN_ON    (GPIOC->BSRR = 0x00000008)
#define CTRL_FAN_OFF    (GPIOC->BSRR = 0x00080000)
#define LED    CTRL_FAN    
#define LED_ON    CTRL_FAN_ON 
#define LED_OFF    CTRL_FAN_OFF

// PC8
#define CTRL_C2    ((GPIOC->ODR & 0x0100) != 0)
#define CTRL_C2_ON    (GPIOC->BSRR = 0x00000100)
#define CTRL_C2_OFF    (GPIOC->BSRR = 0x01000000)

// PC9
#define CTRL_C1    ((GPIOC->ODR & 0x0200) != 0)
#define CTRL_C1_ON    (GPIOC->BSRR = 0x00000200)
#define CTRL_C1_OFF    (GPIOC->BSRR = 0x02000000)

//--- Port A ---//
//PA4 SPI_CS
#define SPI_CS    ((GPIOA->ODR & 0x0010) == 0)
#define SPI_CS_OFF    (GPIOA->BSRR = 0x00000010)
#define SPI_CS_ON    (GPIOA->BSRR = 0x00100000)

//PA8 Alternative TIM1_CH1
#define SW ((GPIOA->ODR & 0x0100) != 0)
#define SW_ON (GPIOA->BSRR = 0x00000100)
#define SW_OFF (GPIOA->BSRR = 0x01000000)

// PA10
#define CTRL_C4    ((GPIOA->ODR & 0x0400) != 0)
#define CTRL_C4_ON    (GPIOA->BSRR = 0x00000400)
#define CTRL_C4_OFF    (GPIOA->BSRR = 0x04000000)

// PA11
#define CTRL_C5    ((GPIOA->ODR & 0x0800) != 0)
#define CTRL_C5_ON    (GPIOA->BSRR = 0x00000800)
#define CTRL_C5_OFF    (GPIOA->BSRR = 0x08000000)

// PA12
#define CTRL_C6    ((GPIOA->ODR & 0x1000) != 0)
#define CTRL_C6_ON    (GPIOA->BSRR = 0x00001000)
#define CTRL_C6_OFF    (GPIOA->BSRR = 0x10000000)


//--- Port B ---//
// PB1 
#define DMX_INPUT    ((GPIOB->IDR & 0x0002) != 0)

// PB2
#define SW_RX_TX    ((GPIOB->ODR & 0x0004) != 0)
#define SW_RX_TX_DE    (GPIOB->BSRR = 0x00000004)
#define SW_RX_TX_RE_NEG    (GPIOB->BSRR = 0x00040000)

// PB8 

// PB10, PB11 Alternative Usart 3 Tx Rx
    
// PB15
#define CTRL_C3    ((GPIOB->ODR & 0x8000) != 0)
#define CTRL_C3_ON    (GPIOB->BSRR = 0x00008000)
#define CTRL_C3_OFF    (GPIOB->BSRR = 0x80000000)



#endif //HARDWARE_VERSION_3_0


// Module Exported Functions ---------------------------------------------------
void HARD_Timeouts (void);
resp_sw_t CheckSET (void);
unsigned char CheckCCW (void);
unsigned char CheckCW (void);
void UpdateSwitches (void);
void UpdateEncoder (void);


#endif    /* _HARD_H_ */

// //--- end of file ---//
