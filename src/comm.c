//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### COMM.C ################################
//---------------------------------------------

/* Includes ------------------------------------------------------------------*/
#include "comm.h"
#include "usart.h"
#include "fixed_menu.h"

#include <string.h>
#include <stdio.h>



// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
char buffMessages [100];


// Module Private Functions ----------------------------------------------------
void InterpretarMsg (char *);


// Module Functions ------------------------------------------------------------
void UpdateCommunications (void)
{
    if (Uart4HaveData())
    {
        Uart4HaveDataReset();
        Uart4ReadBuffer((unsigned char *) buffMessages, sizeof(buffMessages));
        InterpretarMsg(buffMessages);
    }
}


void InterpretarMsg (char * msg)
{
    unsigned char send_nok = 0;
    
    //-- Colors Settings
    if (strncmp(msg, "red", sizeof("red") - 1) == 0)        
    {
        FixedMenu_SetColors(FIXED_RED);
    }

    else if (strncmp(msg, "blue", sizeof("blue") - 1) == 0)
    {
        FixedMenu_SetColors(FIXED_BLUE);        
    }

    else if (strncmp(msg, "green", sizeof("green") - 1) == 0)
    {
        FixedMenu_SetColors(FIXED_GREEN);        
    }

    else if (strncmp(msg, "warm", sizeof("warm") - 1) == 0)
    {
        FixedMenu_SetColors(FIXED_WARM);        
    }

    else if (strncmp(msg, "cold", sizeof("cold") - 1) == 0)
    {
        FixedMenu_SetColors(FIXED_COLD);        
    }

    else
        send_nok = 1;

    if (send_nok)
        Uart4Send("nok\n");
    else
        Uart4Send("ok\n");
    
    
}

//--- end of file ---//
