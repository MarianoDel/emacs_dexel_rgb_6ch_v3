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

//----- Board Configuration -------------------//
//--- Hardware ------------------//
#define HARDWARE_VERSION_3_0        // first version of new board

//--- Software ------------------//
// #define SOFTWARE_VERSION_1_2		
// #define SOFTWARE_VERSION_1_1	     //habla contra pc o rpi con nuevo protocolo camilla
#define SOFTWARE_VERSION_1_0        //habla contra rpi con programa magneto y traduce a micros potencia

//-------- Type of Program (depending on software version) ----------------

//-------- Type of Program and Features ----------------

//-------- Kind of Reports Sended ----------------

//-------- Others Configurations depending on the formers ------------

//-------- Hysteresis Conf ------------------------

//-------- PWM Conf ------------------------



//-------- End Of Defines For Configuration ------




//--- Hardware & Software Messages ------------------//
#ifdef HARDWARE_VERSION_3_0
#define HARD "Hardware Version: 3.0\r\n"
#endif
#ifdef SOFTWARE_VERSION_2_2
#define SOFT "Software Version: 2.2\r\n"
#endif
#ifdef SOFTWARE_VERSION_1_0
#define SOFT "Software Version: 1.0\r\n"
#endif
#ifdef SOFTWARE_VERSION_1_1
#define SOFT "Software Version: 1.1\r\n"
#endif
//--- Type of Program Announcement ----------------

//--- End of Hardware & Software Messages ------------------//



// Module Exported Constants ---------------------------------------------------

// Gpios Configuration ---------------------------------------------------------
#ifdef HARDWARE_VERSION_3_0

//--- Port C ---//
// PC0
#define EN_SW    ((GPIOC->ODR & 0x0001) != 0)

// PC1
#define EN_DT    ((GPIOC->ODR & 0x0002) != 0)

// PC2
#define EN_CLK    ((GPIOC->ODR & 0x0004) != 0)

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
#define DMX_INPUT    ((GPIOB->ODR & 0x0002) != 0)

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


#endif    /* _HARD_H_ */

// //--- end of file ---//
