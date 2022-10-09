//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### GPIO.H ################################
//---------------------------------------------

#ifndef _GPIO_H_
#define _GPIO_H_


// Module Exported Functions ---------------------------------------------------
void GpioInit(void);
void EXTIOn (void);
void EXTIOff (void);

void PB10_Pin_To_Alternative (void);
void PB10_Pin_To_PushPull (void);

#endif    /* _GPIO_H_ */

//--- end of file ---//

