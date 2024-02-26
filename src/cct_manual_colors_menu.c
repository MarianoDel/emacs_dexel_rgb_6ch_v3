//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_MANUAL_COLORS_MENU.C #####################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_manual_colors_menu.h"
#include "display_utils.h"
#include "cct_utils.h"

#include <string.h>
#include <stdio.h>


// Module Private Types Constants and Macros -----------------------------------
#define COLORS_QTTY    23
char str_colors [COLORS_QTTY][11] = {"2700K",
                                     "3200K",
                                     "4000K",
                                     "4500K",
                                     "5000K",
                                     "5500K",
                                     "6500K",
                                     "Red",
                                     "Green",
                                     "Blue",
                                     "Yellow",
                                     "Cyan",
                                     "Magenta",
                                     "Marine",
                                     "Pink",
                                     "Chocolate",
                                     "Deep Pink",                                     
                                     "Amber",
                                     "Purple",
                                     "Turquoise",
                                     "Orange Red",
                                     "Steel Blue",
                                     "Cherry"};

unsigned char rgb_colors [COLORS_QTTY][5] = {{0,0,0,255,0},
                                             {0,0,0,225,29},
                                             {0,0,0,200,55},
                                             {0,0,0,175,80},
                                             {0,0,0,127,128},
                                             {0,0,0,63,192},
                                             {0,0,0,0,255},
                                             {255,0,0,0,0},
                                             {0,255,0,0,0},
                                             {0,0,255,0,0},
                                             {127,128,0,0,0},
                                             {0,127,128,0,0},
                                             {127,0,128,0,0},
                                             {6,240,9,0,0},                                             
                                             {171,5,79,0,0},
                                             {237,18,0,0,0},
                                             {247,0,8,0,0},
                                             {146,109,0,0,0},
                                             {90,0,165,0,0},
                                             {33,115,107,0,0},
                                             {201,54,0,0,0},
                                             {47,87,121,0,0},
                                             {206,4,45,0,0}};

typedef enum {
    CCT_MANUAL_COLORS_MENU_INIT = 0,
    CCT_MANUAL_COLORS_MENU_CHECK_OPTIONS,
    CCT_MANUAL_COLORS_MENU_CHECK_OPTIONS_WAIT_FREE,    
    CCT_MANUAL_COLORS_MENU_SELECT_LINE1,
    CCT_MANUAL_COLORS_MENU_SELECT_LINE1_WAIT_FREE,
    CCT_MANUAL_COLORS_MENU_SELECT_LINE2,
    CCT_MANUAL_COLORS_MENU_SELECT_LINE2_WAIT_FREE
    
} cct_manual_colors_menu_state_e;

#define TT_SHOW    500
// #define TT_NOT_SHOW    600

// variable re-use
#define cct_selected    menu_selected
#define cct_state    menu_state
#define cct_need_display_update    menu_need_display_update
#define cct_selection_show    menu_selection_show
#define cct_manual_colors_menu_timer    menu_menu_timer

// Externals -------------------------------------------------------------------
extern unsigned char menu_selected;
extern unsigned char menu_state;
extern unsigned char menu_need_display_update;
extern unsigned char menu_selection_show;
extern volatile unsigned short menu_menu_timer;


// Globals ---------------------------------------------------------------------



// Module Private Functions ----------------------------------------------------
void Cct_Index_To_Channels(unsigned char * rgb_table, unsigned char * chnls);


// Module Funtions -------------------------------------------------------------
void Cct_Manual_Colors_Menu_UpdateTimer (void)
{
    if (cct_manual_colors_menu_timer)
        cct_manual_colors_menu_timer--;
}


void Cct_Manual_Colors_Menu_Reset (void)
{
    cct_state = CCT_MANUAL_COLORS_MENU_INIT;
}


// color index in cct_temp_color
extern void display_update (void);
resp_t Cct_Manual_Colors_Menu (parameters_typedef * mem, sw_actions_t actions)
{
    static unsigned char showing = 0;
    resp_t resp = resp_continue;
    char s_temp[ALL_LINE_LENGTH_NULL];
    unsigned char dim_int;
    unsigned char dim_dec;
    
    switch (cct_state)
    {
    case CCT_MANUAL_COLORS_MENU_INIT:
        Display_StartLines ();
        Display_ClearLines();

        // line 1
        GetPercentage (mem->cct_dimmer, &dim_int, &dim_dec);
        sprintf(s_temp, "Dimmer: %3d.%01d%%", dim_int, dim_dec);
        Display_SetLine1(s_temp);

        // line 2
        sprintf(s_temp, "%2d -> %s",
                mem->cct_temp_color + 1,
                &str_colors[mem->cct_temp_color][0]);
        Display_SetLine2(s_temp);

        // bottom line
        Display_SetLine8("         Preset Color");

        // update all colors
        Cct_Index_To_Channels(&rgb_colors[mem->cct_temp_color][0],&mem->dimmed_channels[0]);

        // // line 3 & 4 FOR DEBUG dimmed_channels
        // Display_BlankLine3();
        // sprintf(s_temp, "%d %d %d",
        //         mem->dimmed_channels[0],
        //         mem->dimmed_channels[1],
        //         mem->dimmed_channels[2]);
        // Display_SetLine3(s_temp);
        // Display_BlankLine4();
        // sprintf(s_temp, "%d %d",
        //         mem->dimmed_channels[3],
        //         mem->dimmed_channels[4]);
        // Display_SetLine4(s_temp);
        
        for (int i = 0; i < 5; i++)
        {
            mem->fixed_channels[i] = Cct_Utils_Dim_Color (
                mem->cct_dimmer,
                mem->dimmed_channels[i]);
        }

        // line 3 & 4 FOR DEBUG fixed_channels
        Display_BlankLine3();
        sprintf(s_temp, "%d %d %d",
                mem->fixed_channels[0],
                mem->fixed_channels[1],
                mem->fixed_channels[2]);
        Display_SetLine3(s_temp);
        Display_BlankLine4();
        sprintf(s_temp, "%d %d",
                mem->fixed_channels[3],
                mem->fixed_channels[4]);
        Display_SetLine4(s_temp);
        
        resp = resp_change;        
                
        cct_need_display_update = 1;
        cct_state++;
        break;

    case CCT_MANUAL_COLORS_MENU_CHECK_OPTIONS:
        if (actions == selection_enter)
        {
            cct_state++;
        }
        break;

    case CCT_MANUAL_COLORS_MENU_CHECK_OPTIONS_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state++;
            showing = 1;
        }
        break;
        
    case CCT_MANUAL_COLORS_MENU_SELECT_LINE1:        
        resp = Cct_Utils_Update_Actions_Values (actions, &mem->cct_dimmer);

        // colors change
        if (resp == resp_change)
        {
            Cct_Index_To_Channels(&rgb_colors[mem->cct_temp_color][0],&mem->dimmed_channels[0]);

            for (int i = 0; i < 5; i++)
            {
                mem->fixed_channels[i] = Cct_Utils_Dim_Color (
                    mem->cct_dimmer,
                    mem->dimmed_channels[i]);
            }

            // line 3 & 4 FOR DEBUG fixed_channels
            Display_BlankLine3();
            sprintf(s_temp, "%d %d %d",
                    mem->fixed_channels[0],
                    mem->fixed_channels[1],
                    mem->fixed_channels[2]);
            Display_SetLine3(s_temp);
            Display_BlankLine4();
            sprintf(s_temp, "%d %d",
                    mem->fixed_channels[3],
                    mem->fixed_channels[4]);
            Display_SetLine4(s_temp);
            
        }
        
        if (actions == selection_enter)
        {
            cct_state++;
        }

        if (actions != do_nothing)
        {
            cct_manual_colors_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_manual_colors_menu_timer)
        {
            Display_BlankLine1();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                GetPercentage (mem->cct_dimmer, &dim_int, &dim_dec);
                sprintf(s_temp, "Dimmer: %3d.%01d%%", dim_int, dim_dec);
                Display_SetLine1(s_temp);
            }
            
            cct_manual_colors_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;

    case CCT_MANUAL_COLORS_MENU_SELECT_LINE1_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state++;
            showing = 1;
        }
        break;

    case CCT_MANUAL_COLORS_MENU_SELECT_LINE2:
        // resp = CCT_Utils_Update_Actions_Values (actions, &mem->fixed_channels[1]);

        // if (actions == selection_dwn_fast)
        // {
        //     if (mem->cct_temp_color >= 4)
        //         mem->cct_temp_color -= 4;
        //     else
        //         mem->cct_temp_color = 0;

        //     resp = resp_change;
        // }
        // else if (actions == selection_dwn)
        // {
        //     if (mem->cct_temp_color)
        //         mem->cct_temp_color -= 1;

        //     resp = resp_change;        
        // }
        // else if (actions == selection_up_fast)
        // {
        //     if (mem->cct_temp_color <= 19 - 4)
        //         mem->cct_temp_color += 4;
        //     else
        //         mem->cct_temp_color = 19;

        //     resp = resp_change;        
        // }
        // else if (actions == selection_up)
        // {
        //     if (mem->cct_temp_color <= 19 - 1)
        //         mem->cct_temp_color += 1;

        //     resp = resp_change;        
        // }

        if ((actions == selection_dwn) ||
            (actions == selection_dwn_fast))
        {
            if (mem->cct_temp_color)
                mem->cct_temp_color -= 1;

            resp = resp_change;        
        }
        else if ((actions == selection_up) ||
                 (actions == selection_up_fast))
        {
            if (mem->cct_temp_color < COLORS_QTTY - 1)
                mem->cct_temp_color += 1;

            resp = resp_change;        
        }
        
        // colors change
        if (resp == resp_change)
        {
            Cct_Index_To_Channels(&rgb_colors[mem->cct_temp_color][0],&mem->dimmed_channels[0]);

            // // line 3 & 4 FOR DEBUG dimmed_channels
            // Display_BlankLine3();
            // sprintf(s_temp, "%d %d %d",
            //         mem->dimmed_channels[0],
            //         mem->dimmed_channels[1],
            //         mem->dimmed_channels[2]);
            // Display_SetLine3(s_temp);
            // Display_BlankLine4();
            // sprintf(s_temp, "%d %d",
            //         mem->dimmed_channels[3],
            //         mem->dimmed_channels[4]);
            // Display_SetLine4(s_temp);
            
            for (int i = 0; i < 5; i++)
            {
                mem->fixed_channels[i] = Cct_Utils_Dim_Color (
                    mem->cct_dimmer,
                    mem->dimmed_channels[i]);
            }

            // line 3 & 4 FOR DEBUG fixed_channels
            Display_BlankLine3();
            sprintf(s_temp, "%d %d %d",
                    mem->fixed_channels[0],
                    mem->fixed_channels[1],
                    mem->fixed_channels[2]);
            Display_SetLine3(s_temp);
            Display_BlankLine4();
            sprintf(s_temp, "%d %d",
                    mem->fixed_channels[3],
                    mem->fixed_channels[4]);
            Display_SetLine4(s_temp);
            
        }
        
        if (actions == selection_enter)
        {
            cct_state++;
        }

        if (actions != do_nothing)
        {
            cct_manual_colors_menu_timer = 0;
            showing = 0;
        }
        
        if (!cct_manual_colors_menu_timer)
        {
            Display_BlankLine2();

            if (showing)
                showing = 0;
            else
            {
                showing = 1;
                sprintf(s_temp, "%2d -> %s",
                        mem->cct_temp_color + 1,
                        &str_colors[mem->cct_temp_color][0]);
                Display_SetLine2(s_temp);
            }
            
            cct_manual_colors_menu_timer = TT_SHOW;
            cct_need_display_update = 1;
        }
        break;        
        
    case CCT_MANUAL_COLORS_MENU_SELECT_LINE2_WAIT_FREE:
        if (actions == do_nothing)    // change start or end selections
        {
            cct_state = CCT_MANUAL_COLORS_MENU_CHECK_OPTIONS;
            showing = 1;
        }
        break;

    default:
        cct_state = CCT_MANUAL_COLORS_MENU_INIT;
        break;
    }

    if (cct_need_display_update)
    {
        display_update();
        cct_need_display_update = 0;
    }

    return resp;
    
}


// void Cct_Index_To_Channels(&rgb_colors[mem->cct_temp_color][0],&mem->dimmed_channels[0])
void Cct_Index_To_Channels(unsigned char * rgb_table, unsigned char * chnls)
{
    for (int i = 0; i < 5; i++)
        *(chnls + i) = *(rgb_table + i);
}
//--- end of file ---//
