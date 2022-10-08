//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### USART.C ################################
//---------------------------------------------

// Includes --------------------------------------------------------------------
#include "usart.h"
#include "stm32f10x.h"
#include "dmx_transceiver.h"
  
#include <stdio.h>
#include <string.h>

// Module Configs --------------------------------------------------------------
// #define USE_USART1
// #define USE_USART2
#define USE_USART3
#define USE_UART4
// #define USE_UART5

#define SIZEOF_PC_TXDATA        512
#define SIZEOF_PC_RXDATA        512
#define SIZEOF_TXDATA        256
#define SIZEOF_RXDATA        256

//---- Common Defines --------------------
// fck is PCLK1 for USART 2,3,4,5 PCLK2 for USART1
// baud = fck / (16 * USARTDIV)
// USARTDIV 0xMMMF Mantissa MMM Fraction F/16
//
// For Xtal use and freq 72MHz (PCKL1 36MHz PCKL2 72MHz)
#define USART_XTAL_PCKL1_9600    0x0EA6
#define USART_XTAL_PCKL2_9600    0x1D4C

// For Internal RC at 64MHz (PCKL1 32MHz PCKL2 64MHz)
#define USART_RC_PCKL1_9600    0x0D04
#define USART_RC_PCKL2_9600    0x1A0A

#define USART_RC_PCKL1_250000    0x0080


#define USART1_9600        USART_RC_PCKL2_9600
#define USART2_9600        USART_RC_PCKL1_9600
#define USART3_9600        USART_RC_PCKL1_9600
#define UART4_9600         USART_RC_PCKL1_9600
#define UART5_9600         USART_RC_PCKL1_9600

#define USART3_250000    USART_RC_PCKL1_250000

// Module Private Types Constants and Macros -----------------------------------
#define RCC_USART1_CLK    (RCC->APB2ENR & 0x00004000)
#define RCC_USART1_CLKEN    (RCC->APB2ENR |= 0x00004000)
#define RCC_USART1_CLKDIS    (RCC->APB2ENR &= ~0x00004000)

#define RCC_USART2_CLK    (RCC->APB1ENR & 0x00020000)
#define RCC_USART2_CLKEN    (RCC->APB1ENR |= 0x00020000)
#define RCC_USART2_CLKDIS    (RCC->APB1ENR &= ~0x00020000)

#define RCC_USART3_CLK    (RCC->APB1ENR & 0x00040000)
#define RCC_USART3_CLKEN    (RCC->APB1ENR |= 0x00040000)
#define RCC_USART3_CLKDIS    (RCC->APB1ENR &= ~0x00040000)

#define RCC_UART4_CLK    (RCC->APB1ENR & 0x00080000)
#define RCC_UART4_CLKEN    (RCC->APB1ENR |= 0x00080000)
#define RCC_UART4_CLKDIS    (RCC->APB1ENR &= ~0x00080000)

#define RCC_UART5_CLK    (RCC->APB1ENR & 0x00100000)
#define RCC_UART5_CLKEN    (RCC->APB1ENR |= 0x00100000)
#define RCC_UART5_CLKDIS    (RCC->APB1ENR &= ~0x00100000)


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
//--- Private variables ---//
//--- USART1 ---//
#ifdef USE_USART1
volatile unsigned char * ptx1;
volatile unsigned char * ptx1_pckt_index;
volatile unsigned char * prx1;
volatile unsigned char tx1buff[SIZEOF_PC_TXDATA];
volatile unsigned char rx1buff[SIZEOF_PC_RXDATA];
volatile unsigned char usart1_have_data;
#endif

//--- USART2 ---//
#ifdef USE_USART2
volatile unsigned char * ptx2;
volatile unsigned char * ptx2_pckt_index;
volatile unsigned char * prx2;
volatile unsigned char tx2buff[SIZEOF_TXDATA];
volatile unsigned char rx2buff[SIZEOF_RXDATA];
volatile unsigned char usart2_have_data;
#endif

//--- USART3 ---//
#ifdef USE_USART3
// volatile unsigned char * ptx3;
// volatile unsigned char * ptx3_pckt_index;
// volatile unsigned char * prx3;
// volatile unsigned char tx3buff[SIZEOF_TXDATA];
// volatile unsigned char rx3buff[SIZEOF_RXDATA];
// volatile unsigned char usart3_have_data;
// used with dmx
#endif

//--- UART4 ---//
#ifdef USE_UART4
volatile unsigned char * ptx4;
volatile unsigned char * ptx4_pckt_index;
volatile unsigned char * prx4;
volatile unsigned char tx4buff[SIZEOF_TXDATA];
volatile unsigned char rx4buff[SIZEOF_RXDATA];
volatile unsigned char uart4_have_data;
#endif

//--- UART5 ---//
#ifdef USE_UART5
volatile unsigned char * ptx5;
volatile unsigned char * ptx5_pckt_index;
volatile unsigned char * prx5;
volatile unsigned char tx5buff[SIZEOF_TXDATA];
volatile unsigned char rx5buff[SIZEOF_RXDATA];
volatile unsigned char uart5_have_data;
#endif


// Module Functions ------------------------------------------------------------

//---- USART1 Functions ----
#ifdef USE_USART1
void Usart1Config(void)
{
    unsigned long temp;
    
    //---- Clk USART1 ----
    if (!RCC_USART1_CLK)
        RCC_USART1_CLKEN;

    //---- Acomodo punteros ----
    ptx1 = tx1buff;
    ptx1_pckt_index = tx1buff;
    prx1 = rx1buff;

    //---- Configuro velocidad y opciones del puerto
    USART1->BRR = USART1_9600;
    // USART1->CR2 |= USART_CR2_STOP_1;	//2 bits stop
    // USART1->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
    // USART1->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE;	//SIN TX
    USART1->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;	//para pruebas TX

    //---- Configuro salidas alternativas ----
    //----GPIOA----//
    //----TX:PA9 RX:PA10----//
    temp = GPIOA->CRH;
    temp &= 0xFFFFF00F;
    // temp |= 0x000004B0;
    temp |= 0x000008B0;    //PA10 input pullup - pulldwn
    GPIOA->CRH = temp;
    GPIOA->ODR |= 0x0400;    //PA10 pullup
        
    //---- Habilito Int y prioridad ----
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART1_IRQn, 7);
}

void Usart1Send (char * send)
{
    unsigned char i;

    i = strlen(send);
    Usart1SendUnsigned((unsigned char *) send, i);
}

void Usart1SendUnsigned (unsigned char * send, unsigned char size)
{
    if ((ptx1_pckt_index + size) < &tx1buff[SIZEOF_PC_TXDATA])
    {
        memcpy((unsigned char *)ptx1_pckt_index, send, size);
        ptx1_pckt_index += size;
        USART1->CR1 |= USART_CR1_TXEIE;
    }
}

unsigned char Usart1ReadBuffer (unsigned char * bout, unsigned short max_len)
{
    unsigned int len;

    len = prx1 - rx1buff;

    if (len < max_len)
    {
        //el prx1 siempre llega adelantado desde la int, lo corto con un 0
        *prx1 = '\0';
        prx1++;
        len += 1;
        memcpy(bout, (unsigned char *) rx1buff, len);
    }
    else
    {
        memcpy(bout, (unsigned char *) rx1buff, len);
        len = max_len;
    }

    //ajusto punteros de rx luego de la copia
    prx1 = rx1buff;

    return (unsigned char) len;
}

void USART1_IRQHandler (void)
{
    unsigned char dummy;

    /* USART in Receive mode --------------------------------------------------*/
    if (USART1->SR & USART_SR_RXNE)
    {
        dummy = USART1->DR & 0x0FF;

        if (prx1 < &rx1buff[SIZEOF_PC_RXDATA - 1])
        {
            // if ((dummy == '\n') || (dummy == '\r') || (dummy == 26))		//26 es CTRL-Z
            if ((dummy == '\n') || (dummy == 26))		//26 es CTRL-Z                
            {
                *prx1 = '\0';
                usart1_have_data = 1;
                // if (LED)
                // 	LED_OFF;
                // else
                // 	LED_ON;

            }
            else
            {
                *prx1 = dummy;
                prx1++;
            }
        }
        else
            prx1 = rx1buff;    //soluciona problema bloqueo con garbage
    }

    /* USART in Transmit mode -------------------------------------------------*/
    if (USART1->CR1 & USART_CR1_TXEIE)
    {
        if (USART1->SR & USART_SR_TXE)
        {
            if ((ptx1 < &tx1buff[SIZEOF_PC_TXDATA]) && (ptx1 < ptx1_pckt_index))
            {
                USART1->DR = *ptx1;
                ptx1++;
            }
            else
            {
                ptx1 = tx1buff;
                ptx1_pckt_index = tx1buff;
                USART1->CR1 &= ~USART_CR1_TXEIE;
            }
        }
    }

    if ((USART1->SR & USART_SR_ORE) || (USART1->SR & USART_SR_NE) || (USART1->SR & USART_SR_FE))
    {
        dummy = USART1->SR;
        dummy = USART1->DR;
    }
}
#endif    //USE_USART1

//---- USART2 Functions ----
#ifdef USE_USART2
void Usart2Config(void)
{
    unsigned long temp;
    //---- Clk USART2 ----
    if (!RCC_USART2_CLK)
        RCC_USART2_CLKEN;

    //---- Acomodo punteros ----
    ptx2 = tx2buff;
    ptx2_pckt_index = tx2buff;
    prx2 = rx2buff;

    //---- Configuro velocidad y opciones del puerto
    USART2->BRR = USART2_9600;
    // USART2->CR2 |= USART_CR2_STOP_1;	//2 bits stop
    // USART2->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
    // USART2->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE;	//SIN TX
    USART2->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;	//para pruebas TX

    //---- Configuro salidas alternativas ----
    //----GPIOA----//
    //----TX:PA2 RX:PA3----//
    temp = GPIOA->CRL;
    temp &= 0xFFFF00FF;
    temp |= 0x00004B00;
    GPIOA->CRL = temp;
    

    //---- Habilito Int y prioridad ----
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn, 7);
}

void Usart2Send (char * send)
{
    unsigned char i;

    i = strlen(send);
    Usart2SendUnsigned((unsigned char *) send, i);
}

void Usart2SendUnsigned (unsigned char * send, unsigned char size)
{
    if ((ptx2_pckt_index + size) < &tx2buff[SIZEOF_TXDATA])
    {
        memcpy((unsigned char *)ptx2_pckt_index, send, size);
        ptx2_pckt_index += size;
        USART2->CR1 |= USART_CR1_TXEIE;
    }
}

unsigned char Usart2ReadBuffer (unsigned char * bout, unsigned short max_len)
{
    unsigned int len;

    len = prx2 - rx2buff;

    if (len < max_len)
    {
        //prx2 points to the '\0' end of line                
        len += 1;
        memcpy(bout, (unsigned char *) rx2buff, len);
    }
    else
    {
        memcpy(bout, (unsigned char *) rx2buff, len);
        len = max_len;
    }

    //ajusto punteros de rx luego de la copia
    prx2 = rx2buff;

    return (unsigned char) len;
}

void USART2_IRQHandler (void)
{
    unsigned char dummy;

    /* USART in Receive mode --------------------------------------------------*/
    if (USART2->SR & USART_SR_RXNE)
    {
        dummy = USART2->DR & 0x0FF;

        //para usar solo one-wire reviso si no estoy transmitiendo yo
        if (USART2->SR & USART_SR_TC)
        {
            if (prx2 < &rx2buff[SIZEOF_RXDATA - 1])
            {
                // if ((dummy == '\n') || (dummy == '\r') || (dummy == 26))		//26 es CTRL-Z
                if ((dummy == '\n') || (dummy == 26))		//26 es CTRL-Z                
                {
                    *prx2 = '\0';
                    usart2_have_data = 1;
                    // if (LED)
                    // 	LED_OFF;
                    // else
                    // 	LED_ON;

                }
                else
                {
                    *prx2 = dummy;
                    prx2++;
                }
            }
            else
                prx2 = rx2buff;    //soluciona problema bloqueo con garbage
        }
    }

    /* USART in Transmit mode -------------------------------------------------*/
    if (USART2->CR1 & USART_CR1_TXEIE)
    {
        if (USART2->SR & USART_SR_TXE)
        {
            if ((ptx2 < &tx2buff[SIZEOF_TXDATA]) && (ptx2 < ptx2_pckt_index))
            {
                USART2->DR = *ptx2;
                ptx2++;
            }
            else
            {
                ptx2 = tx2buff;
                ptx2_pckt_index = tx2buff;
                USART2->CR1 &= ~USART_CR1_TXEIE;
            }
        }
    }

    if ((USART2->SR & USART_SR_ORE) || (USART2->SR & USART_SR_NE) || (USART2->SR & USART_SR_FE))
    {
        dummy = USART2->SR;
        dummy = USART2->DR;
    }
}
#endif    //USE_USART2

//---- USART3 Functions ----
#ifdef USE_USART3
void Usart3Config(void)
{
    //-- USART3 Clk
    if (!RCC_USART3_CLK)
        RCC_USART3_CLKEN;

    //-- Pointers init
    // ptx3 = tx3buff;
    // ptx3_pckt_index = tx3buff;
    // prx3 = rx3buff;

    //-- Baudrate and port options
    USART3->BRR = USART3_250000;
    USART3->CR2 |= USART_CR2_STOP_1;	//2 stop bits
    // USART3->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
    // USART3->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE;	//SIN TX
    USART3->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;

    //---- Configuro salidas alternativas ----
    //----GPIOB----//
    //----TX:PB10 RX:PB11----//
    // unsigned long temp;
    // temp = GPIOB->CRH;
    // temp &= 0xFFFF00FF;
    // temp |= 0x00004B00;
    // GPIOB->CRH = temp;

    //---- Habilito Int y prioridad ----
    NVIC_EnableIRQ(USART3_IRQn);
    NVIC_SetPriority(USART3_IRQn, 5);
}


void Usart3SendByte (unsigned char byte)
{
    USART3->DR = byte;
}


void Usart3DisableTx (void)
{
    USART3->CR1 &= ~USART_CR1_TXEIE;
}

// void Usart3Send (char * send)
// {
//     unsigned char i;

//     i = strlen(send);
//     Usart3SendUnsigned((unsigned char *) send, i);
// }

// void Usart3SendUnsigned (unsigned char * send, unsigned char size)
// {
//     if ((ptx3_pckt_index + size) < &tx3buff[SIZEOF_TXDATA])
//     {
//         memcpy((unsigned char *)ptx3_pckt_index, send, size);
//         ptx3_pckt_index += size;
//         USART3->CR1 |= USART_CR1_TXEIE;
//     }
// }

// unsigned char Usart3ReadBuffer (unsigned char * bout, unsigned short max_len)
// {
//     unsigned int len;

//     len = prx3 - rx3buff;

//     if (len < max_len)
//     {
//         //prx3 points to the '\0' end of line        
//         len += 1;
//         memcpy(bout, (unsigned char *) rx3buff, len);
//     }
//     else
//     {
//         memcpy(bout, (unsigned char *) rx3buff, len);
//         len = max_len;
//     }

//     //ajusto punteros de rx luego de la copia
//     prx3 = rx3buff;

//     return (unsigned char) len;
// }

void USART3_IRQHandler (void)
{
    unsigned short dummy;
    
    // USART in Receiver mode --------------------------------------------------
    if (USART3->SR & USART_SR_RXNE)
    {
        dummy = (unsigned short) USART3->DR & 0x00FF;
        DmxInt_Serial_Handler_Receiver (dummy);
    }

    // USART in Transmiter mode -------------------------------------------------
    if (USART3->CR1 & USART_CR1_TXEIE)
    {
        if (USART3->SR & USART_SR_TXE)
        {
            DmxInt_Serial_Handler_Transmitter ();
        }
    }

    if ((USART3->SR & USART_SR_ORE) || (USART3->SR & USART_SR_NE) || (USART3->SR & USART_SR_FE))
    {
        dummy = USART3->SR;
        dummy = USART3->DR;
    }
}
#endif    //USE_USART3

//---- UART4 Functions ----
#ifdef USE_UART4
void Uart4Config(void)
{
    unsigned long temp;
    
    //---- Clk UART4 ----
    if (!RCC_UART4_CLK)
        RCC_UART4_CLKEN;

    //---- Acomodo punteros ----
    ptx4 = tx4buff;
    ptx4_pckt_index = tx4buff;
    prx4 = rx4buff;

    //---- Configuro velocidad y opciones del puerto
    UART4->BRR = UART4_9600;
    // UART4->CR2 |= USART_CR2_STOP_1;	//2 bits stop
    // UART4->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
    // UART4->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE;	//SIN TX
    UART4->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;	//para pruebas TX

    //---- Configuro salidas alternativas ----
    //----GPIOA----//
    //----TX:PC10 RX:PC11----//
    temp = GPIOC->CRH;
    temp &= 0xFFFF00FF;
    temp |= 0x00004B00;
    GPIOC->CRH = temp;
    

    //---- Habilito Int y prioridad ----
    NVIC_EnableIRQ(UART4_IRQn);
    NVIC_SetPriority(UART4_IRQn, 7);
}

void Uart4Send (char * send)
{
    unsigned char i;

    i = strlen(send);
    Uart4SendUnsigned((unsigned char *) send, i);
}

void Uart4SendUnsigned (unsigned char * send, unsigned char size)
{
    if ((ptx4_pckt_index + size) < &tx4buff[SIZEOF_TXDATA])
    {
        memcpy((unsigned char *)ptx4_pckt_index, send, size);
        ptx4_pckt_index += size;
        UART4->CR1 |= USART_CR1_TXEIE;
    }
}

unsigned char Uart4ReadBuffer (unsigned char * bout, unsigned short max_len)
{
    unsigned int len;

    len = prx4 - rx4buff;

    if (len < max_len)
    {
        //prx4 points to the '\0' end of line        
        len += 1;
        memcpy(bout, (unsigned char *) rx4buff, len);
    }
    else
    {
        memcpy(bout, (unsigned char *) rx4buff, len);
        len = max_len;
    }

    //ajusto punteros de rx luego de la copia
    prx4 = rx4buff;

    return (unsigned char) len;
}


unsigned char Uart4HaveData (void)
{
    return uart4_have_data;
}


void Uart4HaveDataReset (void)
{
    uart4_have_data = 0;
}


void UART4_IRQHandler (void)
{
    unsigned short dummy;

    // USART in Receiver mode --------------------------------------------------
    if (UART4->SR & USART_SR_RXNE)
    {
        dummy = UART4->DR & 0x0FF;

        //para usar solo one-wire reviso si no estoy transmitiendo yo
        if (UART4->SR & USART_SR_TC)
        {
            if (prx4 < &rx4buff[SIZEOF_RXDATA - 1])
            {
                // if ((dummy == '\n') || (dummy == '\r') || (dummy == 26))		//26 es CTRL-Z
                if ((dummy == '\n') || (dummy == 26))		//26 es CTRL-Z                
                {
                    *prx4 = '\0';
                    uart4_have_data = 1;
                    // if (LED)
                    // 	LED_OFF;
                    // else
                    // 	LED_ON;

                }
                else
                {
                    *prx4 = dummy;
                    prx4++;
                }
            }
            else
                prx4 = rx4buff;    //soluciona problema bloqueo con garbage
        }
    }

    // USART in Transmit mode -------------------------------------------------
    if (UART4->CR1 & USART_CR1_TXEIE)
    {
        if (UART4->SR & USART_SR_TXE)
        {
            if ((ptx4 < &tx4buff[SIZEOF_TXDATA]) && (ptx4 < ptx4_pckt_index))
            {
                UART4->DR = *ptx4;
                ptx4++;
            }
            else
            {
                ptx4 = tx4buff;
                ptx4_pckt_index = tx4buff;
                UART4->CR1 &= ~USART_CR1_TXEIE;
            }
        }
    }

    if ((UART4->SR & USART_SR_ORE) || (UART4->SR & USART_SR_NE) || (UART4->SR & USART_SR_FE))
    {
        dummy = UART4->SR;
        dummy = UART4->DR;
    }
}
#endif    //USE_UART4

//---- UART5 Functions ----
#ifdef USE_UART5
void Uart5Config(void)
{
    unsigned long temp;
    
    //---- Clk UART5 ----
    if (!RCC_UART5_CLK)
        RCC_UART5_CLKEN;

    //---- Acomodo punteros ----
    ptx5 = tx5buff;
    ptx5_pckt_index = tx5buff;
    prx5 = rx5buff;

    //---- Configuro velocidad y opciones del puerto
    UART5->BRR = UART5_9600;
    // UART5->CR2 |= USART_CR2_STOP_1;	//2 bits stop
    // UART5->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
    // UART5->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE;	//SIN TX
    UART5->CR1 = USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;	//para pruebas TX

    //---- Configuro salidas alternativas ----
    //----GPIOC/D----//
    //----TX:PC12 RX:PD3----//
    temp = GPIOC->CRH;
    temp &= 0xFFF0FFFF;
    temp |= 0x000B0000;
    GPIOC->CRH = temp;
    
    temp = GPIOD->CRL;
    temp &= 0xFFFFF0FF;
    temp |= 0x00000400;
    GPIOD->CRL = temp;

        
    //---- Habilito Int y prioridad ----
    NVIC_EnableIRQ(UART5_IRQn);
    NVIC_SetPriority(UART5_IRQn, 7);
}

void Uart5Send (char * send)
{
    unsigned char i;

    i = strlen(send);
    Uart5SendUnsigned((unsigned char *) send, i);
}

void Uart5SendUnsigned (unsigned char * send, unsigned char size)
{
    if ((ptx5_pckt_index + size) < &tx5buff[SIZEOF_TXDATA])
    {
        memcpy((unsigned char *)ptx5_pckt_index, send, size);
        ptx5_pckt_index += size;
        UART5->CR1 |= USART_CR1_TXEIE;
    }
}

unsigned char Uart5ReadBuffer (unsigned char * bout, unsigned short max_len)
{
    unsigned int len;

    len = prx5 - rx5buff;

    if (len < max_len)
    {
        //prx5 points to the '\0' end of line
        len += 1;
        memcpy(bout, (unsigned char *) rx5buff, len);
    }
    else
    {
        memcpy(bout, (unsigned char *) rx5buff, len);
        len = max_len;
    }

    // pointer adjust
    prx5 = rx5buff;

    return (unsigned char) len;
}

void UART5_IRQHandler (void)
{
    unsigned char dummy;

    /* USART in Receive mode --------------------------------------------------*/
    if (UART5->SR & USART_SR_RXNE)
    {
        dummy = UART5->DR & 0x0FF;

        //para usar solo one-wire reviso si no estoy transmitiendo yo
        if (UART5->SR & USART_SR_TC)
        {
            if (prx5 < &rx5buff[SIZEOF_RXDATA - 1])
            {
                // if ((dummy == '\n') || (dummy == '\r') || (dummy == 26))		//26 es CTRL-Z
                if ((dummy == '\n') || (dummy == 26))		//26 es CTRL-Z                
                {
                    *prx5 = '\0';
                    usart5_have_data = 1;
                    // if (LED)
                    // 	LED_OFF;
                    // else
                    // 	LED_ON;

                }
                else
                {
                    *prx5 = dummy;
                    prx5++;
                }
            }
            else
                prx5 = rx5buff;    //soluciona problema bloqueo con garbage
        }
    }

    /* USART in Transmit mode -------------------------------------------------*/
    if (UART5->CR1 & USART_CR1_TXEIE)
    {
        if (UART5->SR & USART_SR_TXE)
        {
            if ((ptx5 < &tx5buff[SIZEOF_TXDATA]) && (ptx5 < ptx5_pckt_index))
            {
                UART5->DR = *ptx5;
                ptx5++;
            }
            else
            {
                ptx5 = tx5buff;
                ptx5_pckt_index = tx5buff;
                UART5->CR1 &= ~USART_CR1_TXEIE;
            }
        }
    }

    if ((UART5->SR & USART_SR_ORE) || (UART5->SR & USART_SR_NE) || (UART5->SR & USART_SR_FE))
    {
        dummy = UART5->SR;
        dummy = UART5->DR;
    }
}
#endif    //USE_UART5


//--- end of file ---//

