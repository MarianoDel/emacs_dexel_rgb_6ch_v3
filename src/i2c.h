//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### I2C.H #################################
//---------------------------------------------
#ifndef _I2C_H_
#define _I2C_H_

// Config Defines --------------------------------------------------------------
#define I2C_ADDRESS_OWN    0x01
#define I2C_ADDRESS_SLV    0x3C    //para 32?
// #define I2C_ADDRESS_SLV    0x3D    //para 64?
#define I2C_ADDRESS_MSK    0x7F



// Common Defines --------------------------------------------------------------
#define I2C_CR2_NBYTES_Pos           (16U)                                     
#define I2C_CR2_NBYTES_Msk           (0xFFU << I2C_CR2_NBYTES_Pos)             /*!< 0x00FF0000 */
#define I2C_CR2_RELOAD_Pos           (24U)                                     
#define I2C_CR2_RELOAD_Msk           (0x1U << I2C_CR2_RELOAD_Pos)              /*!< 0x01000000 */
#define I2C_CR2_AUTOEND_Pos          (25U)                                     
#define I2C_CR2_AUTOEND_Msk          (0x1U << I2C_CR2_AUTOEND_Pos)             /*!< 0x02000000 */
#define I2C_CR2_SADD_Pos             (0U)                                      
#define I2C_CR2_SADD_Msk             (0x3FFU << I2C_CR2_SADD_Pos)              /*!< 0x000003FF */

// Module Exported Functions ---------------------------------------------------
void I2C1_Init (void);
void I2C1_SendByte (unsigned char, unsigned char);

void I2C2_Init (void);
void I2C2_SendByte (unsigned char, unsigned char);


#endif    /* _I2C_H_ */

//--- end of file ---//

