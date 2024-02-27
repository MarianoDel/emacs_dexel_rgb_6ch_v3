//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "cct_manual_colors_menu.h"
#include "cct_utils.h"


//helper modules
#include "tests_ok.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
// -- externals on module
unsigned char menu_selected = 0;
unsigned char menu_state = 0;
unsigned char menu_need_display_update = 0;
unsigned char menu_selection_show = 0;
volatile unsigned short menu_menu_timer = 0;


// Module Auxialiary Functions -------------------------------------------------


// Module Functions for testing ------------------------------------------------
void Test_Index_To_Channel_Mapping (void);



// Module Functions ------------------------------------------------------------


int main(int argc, char *argv[])
{

    Test_Index_To_Channel_Mapping ();
    
}


void Test_Index_To_Channel_Mapping (void)
{
    unsigned char rgb [5];
    // test all 0s
    for (int i = 0; i < 23; i++)
    {
        Cct_Index_To_Channels_Mapping (i, rgb);
        printf("%d %d %d %d %d\n", rgb[0], rgb[1], rgb[2], rgb[3], rgb[4]);
    }

    printf("\nend of test\n");    
    
}





// Module Mocked Functions -----------------------------------------------------
void Display_StartLines (void)
{
}

void Display_ClearLines (unsigned char a)
{
}

void Display_SetLine (unsigned char a)
{
}


void Display_BlankLine (unsigned char a)
{
}

void display_update (void)
{
}
//--- end of file ---//


