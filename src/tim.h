//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### TIM.H #################################
//---------------------------------------------
#ifndef _TIMER_H_
#define _TIMER_H_


// Module Exported Types Constants and Macros ----------------------------------
#define EnableTimer6    (TIM6->CR1 |= TIM_CR1_CEN)
#define DisableTimer6    (TIM6->CR1 &= ~TIM_CR1_CEN)

#define EnableTimer7    (TIM7->CR1 |= TIM_CR1_CEN)
#define DisableTimer7    (TIM7->CR1 &= ~TIM_CR1_CEN)


// Module Exported Functions ---------------------------------------------------
void TIM_1_Init (void);
void TIM_2_Init (void);
void TIM_3_Init (void);
void TIM_4_Init (void);
void TIM_5_Init (void);
void TIM_8_Init (void);

void TIM6_Init(void);
void TIM6_IRQHandler (void);
void TIM6_Update (unsigned short new_val);

void TIM7_Init(void);
void TIM7_IRQHandler (void);
void TIM7_To_OneShoot (void);
void TIM7_To_FreeRunning (void);
void TIM7_OneShoot (unsigned short a);

void Wait_ms (unsigned short);

void TIM_Deactivate_Channels (unsigned char deact_chnls);
void TIM_Activate_Channels (unsigned char act_chnls);


#endif /* _TIMER_H_ */


//--- end of file ---//
