//----------------------------------------------------------
// #### DEXEL PROJ 6CH BIDIRECTIONAL v3 - Custom Board #####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### PWM.H ##############################################
//----------------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef __PWM_H_
#define __PWM_H_


// Exported Types Constants and Macros -----------------------------------------
#define CH1_OFFSET    0
#define CH2_OFFSET    1
#define CH3_OFFSET    2
#define CH4_OFFSET    3
#define CH5_OFFSET    4
#define CH6_OFFSET    5

#define CH1_FLAG    0x01
#define CH2_FLAG    0x02
#define CH3_FLAG    0x04
#define CH4_FLAG    0x08
#define CH5_FLAG    0x10
#define CH6_FLAG    0x20

#define CHNLS_ALL    (CH1_FLAG | CH2_FLAG | CH3_FLAG | CH4_FLAG | CH5_FLAG | CH6_FLAG)

#define PWM_INT_MAX    255
#define PWM_TIMER_MULTIPLIER    20

// #define POWER_CONTROL_GENERAL_THRESHOLD    512    // este es un valor total de dmx no pwm

// #define EnablePreload_PWM1    EnablePreload_TIM3_CH1
// #define EnablePreload_PWM2    EnablePreload_TIM3_CH2

// #define DisablePreload_PWM1    DisablePreload_TIM3_CH1
// #define DisablePreload_PWM2    DisablePreload_TIM3_CH2

// #define PWM_Update_CH1(X)    Update_TIM3_CH1(X)
// #define PWM_Update_CH2(X)    Update_TIM3_CH2(X)

// #define EnablePreload_ENA1    EnablePreload_TIM1_CH2
// #define EnablePreload_ENA2    EnablePreload_TIM1_CH4

// #define DisablePreload_ENA1    DisablePreload_TIM1_CH2
// #define DisablePreload_ENA2    DisablePreload_TIM1_CH4

// #define PWM_Update_ENA1(X)    Update_TIM1_CH2(X)
// #define PWM_Update_ENA2(X)    Update_TIM1_CH4(X)



// Exported Functions ----------------------------------------------------------
void PWM_Map_Post_Filter (unsigned short dmx_filtered, unsigned short * pwm_ena, unsigned short * pwm_ch);

void PWMChannelsReset (void);
unsigned short PWM_Map_From_Dmx (unsigned char);
// void PWM_Set_PwrCtrl (unsigned char *, unsigned char, unsigned short);
void PWM_Map_Pre_Filter (unsigned char dmx_data, unsigned short * pwm_ena, unsigned short * pwm_ch);
void PWM_Map_Post_Filter (unsigned short dmx_filtered, unsigned short * pwm_ena, unsigned short * pwm_ch);

void PWM_Timer_Handler (void);
void PWM_Timer_Arrange_Vector (void);


#endif    /* __PWM_H_ */

//--- end of file ---//

