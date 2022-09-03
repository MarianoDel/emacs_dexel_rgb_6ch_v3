//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### USART.H ################################
//---------------------------------------------
#ifndef _USART_H_
#define _USART_H_


// Module Exported Types Constants and Macros ----------------------------------


// Module Exported Functions ---------------------------------------------------
void Usart1Config (void);
void Usart1Send (char *);
void Usart1SendUnsigned (unsigned char *, unsigned char);
unsigned char Usart1ReadBuffer (unsigned char *, unsigned short);
void USART1_IRQHandler (void);

void Usart2Config (void);
void Usart2Send (char *);
void Usart2SendUnsigned (unsigned char *, unsigned char);
unsigned char Usart2ReadBuffer (unsigned char *, unsigned short);
void USART2_IRQHandler (void);

void Usart3Config (void);
void Usart3Send (char *);
void Usart3SendUnsigned (unsigned char *, unsigned char);
unsigned char Usart3ReadBuffer (unsigned char *, unsigned short);
void USART3_IRQHandler (void);

void Uart4Config (void);
void Uart4Send (char *);
void Uart4SendUnsigned (unsigned char *, unsigned char);
unsigned char Uart4ReadBuffer (unsigned char *, unsigned short);
void UART4_IRQHandler (void);

void Uart5Config (void);
void Uart5Send (char *);
void Uart5SendUnsigned (unsigned char *, unsigned char);
unsigned char Uart5ReadBuffer (unsigned char *, unsigned short);
void UART5_IRQHandler (void);

#endif
