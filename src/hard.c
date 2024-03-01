//----------------------------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### HARD.C #########################################################
//----------------------------------------------------------------------
#include "hard.h"
#include "stm32f10x.h"
#include "parameters.h"


#define SWITCHES_TIMER_RELOAD    5
#define SWITCHES_THRESHOLD_FULL	1000		//5 segundos
#define SWITCHES_THRESHOLD_HALF	100		//1 segundo
#define SWITCHES_THRESHOLD_MIN	5		//25 ms

#define ENCODER_COUNTER_ROOF    10
#define ENCODER_COUNTER_THRESHOLD    3

// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
volatile unsigned char switches_timer = 0;



// Module Private Functions ----------------------------------------------------
void UpdateEncoderFilters (void);


// Module Functions ------------------------------------------------------------
void HARD_Timeouts (void)
{
    if (switches_timer)
        switches_timer--;
    
    // UpdateSwitches();    //lo llamo en el main

    UpdateEncoderFilters ();

}



// Module Functions ------------------------------------------------------------




// Encoder Routines ------------------------------------------------------------
unsigned short sw_set_cntr = 0;

resp_sw_t CheckSET (void)
{
    resp_sw_t sw = SW_NO;
    
    if (sw_set_cntr > SWITCHES_THRESHOLD_FULL)
        sw = SW_FULL;
    else if (sw_set_cntr > SWITCHES_THRESHOLD_HALF)
        sw = SW_HALF;
    else if (sw_set_cntr > SWITCHES_THRESHOLD_MIN)
        sw = SW_MIN;

    return sw;    
}


void UpdateSwitches (void)
{
    if (!switches_timer)
    {
        if (EN_SW)
            sw_set_cntr++;
        else if (sw_set_cntr > 50)
            sw_set_cntr -= 50;
        else if (sw_set_cntr > 10)
            sw_set_cntr -= 5;
        else if (sw_set_cntr)
            sw_set_cntr--;
        
        switches_timer = SWITCHES_TIMER_RELOAD;
    }       
}


volatile unsigned char enc_clk_cntr = 0;
volatile unsigned char enc_dt_cntr = 0;
void UpdateEncoderFilters (void)
{
    if (EN_CLK)
    {
        if (enc_clk_cntr < ENCODER_COUNTER_ROOF)
            enc_clk_cntr++;
    }
    else
    {
        if (enc_clk_cntr)
            enc_clk_cntr--;        
    }

    if (EN_DT)
    {
        if (enc_dt_cntr < ENCODER_COUNTER_ROOF)
            enc_dt_cntr++;
    }
    else
    {
        if (enc_dt_cntr)
            enc_dt_cntr--;        
    }
}


unsigned char last_clk = 0;
unsigned char encoder_ccw = 0;
unsigned char encoder_cw = 0;
extern parameters_typedef mem_conf;
void UpdateEncoder (void)
{
    unsigned char current_clk = 0;

    //check if we have rising edge on clk
    if (enc_clk_cntr > ENCODER_COUNTER_THRESHOLD)
        current_clk = 1;
    else
        current_clk = 0;
    
    if ((last_clk == 0) && (current_clk == 1))    //rising edge
    {
        //have a new clock edge
        if (enc_dt_cntr > ENCODER_COUNTER_THRESHOLD)
        {
            if(mem_conf.encoder_direction == 0)    // USE_ENCODER_DIRECT
            {
                //CW
                if (encoder_cw < 1)
                    encoder_cw++;
            }
            else
            {
                //CCW
                if (encoder_ccw < 1)
                    encoder_ccw++;
            }
        }
        else
        {
            if(mem_conf.encoder_direction == 0)    // USE_ENCODER_DIRECT
            {
                //CCW
                if (encoder_ccw < 1)
                    encoder_ccw++;
            }
            else
            {
                //CW
                if (encoder_cw < 1)
                    encoder_cw++;
            }
        }
    }

    if (last_clk != current_clk)
        last_clk = current_clk;
}


unsigned char CheckCCW (void)
{
    unsigned char a = 0;
    
    if (encoder_ccw)
    {
        encoder_ccw--;
        a = 1;
    }
    
    return a;
}


unsigned char CheckCW (void)
{
    unsigned char a = 0;
    
    if (encoder_cw)
    {
        encoder_cw--;
        a = 1;
    }
    
    return a;
}


unsigned char last_was_enter = 0;
sw_actions_t CheckActions (void)
{
    sw_actions_t sw = do_nothing;
    resp_sw_t s_sel = SW_NO;

    s_sel = CheckSET ();
    
    if (CheckCW () > SW_NO)
    {
        if (s_sel > SW_NO)
        {
            sw = selection_up_fast;
            last_was_enter = 2;
        }
        else
            sw = selection_up;
    }

    if (CheckCCW () > SW_NO)
    {
        if (s_sel > SW_NO)
        {
            sw = selection_dwn_fast;
            last_was_enter = 2;
        }
        else
            sw = selection_dwn;
    }
    
    if (s_sel > SW_HALF)
        sw = selection_back;
    else if (s_sel > SW_NO)
    {
        if (!last_was_enter)
            last_was_enter = 1;
    }
    else    // s_sel == SW_NO
    {
        if (last_was_enter == 2)    // fast update
        {
        }
        else if (last_was_enter == 1)    // no fast update
        {
            sw = selection_enter;
        }
        
        last_was_enter = 0;            
    }
    
    return sw;    
}


unsigned char enter_block = 0;
void Hard_Enter_Block (void)
{
    enter_block = 1;
}


void Hard_Enter_UnBlock (void)
{
    enter_block = 0;
}


unsigned char Hard_Enter_Is_Block (void)
{
    return enter_block;
}
// End of Encoder Routines -----------------------------------------------------


//--- end of file ---//
