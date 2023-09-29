//----------------------------------------------------------
// #### DEXEL PROJ 6CH BIDIRECTIONAL v3 - Custom Board #####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### PWM.C ##############################################
//----------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "pwm.h"
#include "hard.h"
#include "tim.h"
#include "dac_mux.h"

// #include "stm32f10x.h"    //quitar luego de pruebas

// Externals -------------------------------------------------------------------
extern volatile unsigned char pwm_chnls [];
extern volatile unsigned short dac_chnls [];

// Globals ---------------------------------------------------------------------


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------
void PWM_Timer_Update (unsigned short next_time);
void PWM_Timer_Setup (void);
void PWM_Deactivate_Channels (unsigned char deact_chnls);
void PWM_Activate_Channels (unsigned char act_chnls);
void PWM_Find_Least_Value_With_Mask (unsigned char *pwm,
                                     unsigned char qtty,
                                     unsigned char *least,
                                     unsigned char *posi,
                                     unsigned char mask);


// Module Functions ------------------------------------------------------------
// void PWMChannelsReset (void)
// {
//     PWM_Update_CH1(DUTY_NONE);
//     PWM_Update_CH2(DUTY_NONE);
// #ifdef HARDWARE_VERSION_1_1
//     ENA_CH1_OFF;
//     ENA_CH2_OFF;
// #endif
// }


// unsigned short PWM_Map_From_Dmx (unsigned char dmx_val)
// {
//     unsigned int pwm = 0;

// #if (DUTY_100_PERCENT == 1000)    
//     if (dmx_val)
//     {
//         pwm = dmx_val * 391;
//         pwm = pwm / 100;
//         pwm += 4;
//     }
// #endif
// #if (DUTY_100_PERCENT == 4000)
//     if (dmx_val)
//     {
//         pwm = dmx_val * 157;
//         pwm = pwm / 10;
//         pwm += 13;
//     }
// #endif

//     return (unsigned short) pwm;

// }


void PWM_Set_PwrCtrl_512 (unsigned short * p_ch_val, unsigned char chnls_qtty, unsigned short max_power)
{
    unsigned short total_dmx = 0;

    // how many in total
    for (unsigned char i = 0; i < chnls_qtty; i++)
        total_dmx += *(p_ch_val + i);

    if (total_dmx > (max_power << 1))
    {
        unsigned int new = 0;
        for (unsigned char i = 0; i < chnls_qtty; i++)
        {
            // if its something on the channel
            if (*(p_ch_val + i))
            {
                new = *(p_ch_val + i) * (max_power << 1);
                new = new / total_dmx;

                // no dejo que se apaguen los canales
                if (new)
                    *(p_ch_val + i) = (unsigned short) new;
                else
                    *(p_ch_val + i) = 1;
                
            }
        }
    }
}


void PWM_Set_PwrCtrl (unsigned char * p_ch_val, unsigned char chnls_qtty, unsigned short max_power)
{
    unsigned short total_dmx = 0;

    // how many in total
    for (unsigned char i = 0; i < chnls_qtty; i++)
        total_dmx += *(p_ch_val + i);

    if (total_dmx > max_power)
    {
        unsigned int new = 0;
        for (unsigned char i = 0; i < chnls_qtty; i++)
        {
            // if its something on the channel
            if (*(p_ch_val + i))
            {
                new = *(p_ch_val + i) * max_power;
                new = new / total_dmx;

                // no dejo que se apaguen los canales
                if (new)
                    *(p_ch_val + i) = (unsigned char) new;
                else
                    *(p_ch_val + i) = 1;
                
            }
        }
    }
}



// #define OLD_DAC_OFFSET
#define NEW_DAC_OFFSET
// get dmx_filtered from 0 to 4095
// answer pwm_ena 0 to 255
// answer dac_ch 0 to 4095
void PWM_Map_Post_Filter (unsigned short dmx_filtered, unsigned char * pwm_ena, unsigned short * pwm_ch)
{
    unsigned short dmx_ena = 0;
    unsigned int dmx_ch = 0;

#if (defined OLD_DAC_OFFSET)
    if (dmx_filtered > 511)
    {
        dmx_ena = 511;
        dmx_ch = dmx_filtered - 511;
    }
    else
    {
        dmx_ena = dmx_filtered;
        dmx_ch = 0;
    }

    *pwm_ena = dmx_ena >> 1;
    
    dmx_ch = dmx_ch * 114;
    dmx_ch = dmx_ch / 100;
    
#elif (defined NEW_DAC_OFFSET)
    if (dmx_filtered > 511)
    {
        dmx_ena = 511;
        dmx_ch = dmx_filtered;        
    }
    else
    {
        dmx_ena = dmx_filtered;
        dmx_ch = 511;        
    }

    *pwm_ena = dmx_ena >> 1;

#ifdef HARDWARE_VERSION_3_2
    // for ver 3.2 Toff = 1.8us Rsense = 0.1 mux = 74hc4051 (20% reduction)
    dmx_ch = dmx_ch * 206;
    dmx_ch >>= 8;
    // end of for ver 3.2 Toff = 1.8us Rsense = 0.1 mux = 74hc4051
#endif
    
#else
#error "Define DAC_OFFSET on pwm.c"
#endif

    // rooftop
    if (dmx_ch > 4095)
        dmx_ch = 4095;
    
    *pwm_ch = (unsigned short) dmx_ch;
    
}


// New Pwm Int functions
volatile unsigned char pwm_update_block = 0;
volatile unsigned short vtime[7];
volatile unsigned short vtime_saved[7];
volatile unsigned char deactivate [6];
volatile unsigned char pwm_saved [6];
volatile unsigned char deactivate_saved [6];
volatile unsigned char activate = 0;
volatile unsigned char activate_saved = 0;
volatile unsigned char edges = 0;
volatile unsigned char edges_saved = 0;
volatile unsigned char int_edge = 0;

// void PWM_Timer_Handler (void)
// {
//     if (int_edge < edges)
//     {
//         // deactivate channels in this instance
//         PWM_Deactivate_Channels (deactivate[int_edge]);

//         // set the next timer
//         int_edge++;
//         PWM_Timer_Update(vtime[int_edge]);
//     }
//     else
//     {
//         // start the next cycle loop
//         if (!pwm_update_block)
//             PWM_Timer_Setup ();
        
//         int_edge = 0;
//         PWM_Timer_Update(vtime[0]);
//         PWM_Activate_Channels (activate);
//         // if (LED)
//         //     LED_OFF;
//         // else
//         //     LED_ON;
//     }
// }


void PWM_Timer_Update (unsigned short next_time)
{
    TIM6_Update (next_time);
}


void PWM_Timer_Setup (void)
{
    // get the saved data
    for (int i = 0; i < 6; i++)
    {
        deactivate[i] = deactivate_saved[i];
        vtime[i] = vtime_saved[i];
    }        
    vtime[6] = vtime_saved[6];
    edges = edges_saved;
    activate = activate_saved;
}


void PWM_Deactivate_Channels (unsigned char deact_chnls)
{
    TIM_Deactivate_Channels (deact_chnls);
}


void PWM_Activate_Channels (unsigned char act_chnls)
{
    TIM_Activate_Channels (act_chnls);
}


void PWM_Timer_Arrange_Vector (void)
{
    // block the int update
    pwm_update_block = 1;
    
    // clean the vectors
    for (unsigned char i = 0; i < 6; i++)
    {
        vtime_saved[i] = 0;
        deactivate_saved[i] = 0;
    }
    vtime_saved[6] = 0;
    
    // find the sequence
    unsigned char least = 0;
    unsigned char posi = 0;    
    unsigned char masked = 0;
    unsigned char last_pwm = 0;
    unsigned short remain = PWM_INT_MAX * PWM_TIMER_MULTIPLIER;
    unsigned short account = 0;
    unsigned char edges_cnt = 0;

    for (unsigned char i = 0; i < 6; i++)
    {
        PWM_Find_Least_Value_With_Mask((unsigned char *)pwm_saved, 6, &least, &posi, masked);
        masked |= (1 << posi);

        // check the boundary
        if (least == 0)
        {
            activate_saved &= ~(1 << posi);
            last_pwm = 0;
            continue;
        }
        else
            activate_saved |= (1 << posi);

        if (least == PWM_INT_MAX)
        {
            deactivate_saved[edges_cnt] &= ~(1 << posi);
            continue;
        }
        
        if (last_pwm != least) 
        {
            unsigned short diff = 0;    

            if (last_pwm)    // first loop? or non previus zero
                edges_cnt++;
            
            last_pwm = least;
            diff = last_pwm * PWM_TIMER_MULTIPLIER - account;
            vtime_saved[edges_cnt] = diff;
            account += diff;
            deactivate_saved[edges_cnt] |= (1 << posi);
        }
        else
        {
            deactivate_saved[edges_cnt] |= (1 << posi);
        }
    }

    if (edges_cnt)
    {
        vtime_saved[edges_cnt + 1] = remain - account;
        edges_cnt++;
    }
    else
    {
        // check if we have something
        if (deactivate_saved[0])
        {
            vtime_saved[edges_cnt + 1] = remain - account;
            edges_cnt++;
        }
        else    // sure was all zeroes
            vtime_saved[edges_cnt] = remain - account;
    }
    
    edges_saved = edges_cnt;

    // unblock the int update
    pwm_update_block = 0;
}

void PWM_Find_Least_Value_With_Mask (unsigned char *pwm,
                                     unsigned char qtty,
                                     unsigned char *least,
                                     unsigned char *posi,
                                     unsigned char mask)
{
    unsigned char i = 0;

    // *least = *pwm;
    *least = PWM_INT_MAX;
    for (i = 0; i < qtty; i++)
    {
        // if ((*least > *(pwm + i)) &&
        if ((*least >= *(pwm + i)) &&            
            (!(mask & (1 << i))))
        {
            *least = *(pwm + i);
            *posi = i;
        }
    }
}


// call it at least at PWM_INT_MAX * PWM_TIMER_MULTIPLIER; //5110 -> 1KHz
volatile unsigned short pwm_int_cnt = 0;
void PWM_Timer_Handler (void)
{
    unsigned char least_next = PWM_INT_MAX;
    unsigned char pwm_ch = 0;

    if (pwm_int_cnt < PWM_INT_MAX)
    {
        // deactivate channels in this instance
        unsigned char deactivate = 0;

        // Channel 1
        pwm_ch = pwm_chnls[CH1_OFFSET];
        if (pwm_int_cnt >= pwm_ch)
            deactivate |= (1 << CH1_OFFSET);
        else if (least_next > pwm_ch)
            least_next = pwm_ch;

        // Channel 2
        pwm_ch = pwm_chnls[CH2_OFFSET];
        if (pwm_int_cnt >= pwm_ch)
            deactivate |= (1 << CH2_OFFSET);
        else if (least_next > pwm_ch)
            least_next = pwm_ch;

        // Channel 3
        pwm_ch = pwm_chnls[CH3_OFFSET];
        if (pwm_int_cnt >= pwm_ch)
            deactivate |= (1 << CH3_OFFSET);
        else if (least_next > pwm_ch)
            least_next = pwm_ch;

        // Channel 4
        pwm_ch = pwm_chnls[CH4_OFFSET];
        if (pwm_int_cnt >= pwm_ch)
            deactivate |= (1 << CH4_OFFSET);
        else if (least_next > pwm_ch)
            least_next = pwm_ch;

        // Channel 5
        pwm_ch = pwm_chnls[CH5_OFFSET];
        if (pwm_int_cnt >= pwm_ch)
            deactivate |= (1 << CH5_OFFSET);
        else if (least_next > pwm_ch)
            least_next = pwm_ch;

        // Channel 6
        pwm_ch = pwm_chnls[CH6_OFFSET];
        if (pwm_int_cnt >= pwm_ch)
            deactivate |= (1 << CH6_OFFSET);
        else if (least_next > pwm_ch)
            least_next = pwm_ch;
        
        PWM_Deactivate_Channels (deactivate);

        // set the next counter and timer
        // pwm_int_cnt += least_next;
        unsigned char diff = least_next - pwm_int_cnt;
        pwm_int_cnt = least_next;
        PWM_Timer_Update(diff * PWM_TIMER_MULTIPLIER);
    }
    else
    {
        // activate channels in this instance
        unsigned char activate = 0;
        
        // Channel 1
        pwm_ch = pwm_chnls[CH1_OFFSET];
        if (pwm_ch)
        {
            activate |= (1 << CH1_OFFSET);
            if (least_next > pwm_ch)
                least_next = pwm_ch;
        }

        // Channel 2
        pwm_ch = pwm_chnls[CH2_OFFSET];
        if (pwm_ch)
        {
            activate |= (1 << CH2_OFFSET);
            if (least_next > pwm_ch)
                least_next = pwm_ch;
        }

        // Channel 3
        pwm_ch = pwm_chnls[CH3_OFFSET];
        if (pwm_ch)
        {
            activate |= (1 << CH3_OFFSET);
            if (least_next > pwm_ch)
                least_next = pwm_ch;
        }

        // Channel 4
        pwm_ch = pwm_chnls[CH4_OFFSET];
        if (pwm_ch)
        {
            activate |= (1 << CH4_OFFSET);
            if (least_next > pwm_ch)
                least_next = pwm_ch;
        }

        // Channel 5
        pwm_ch = pwm_chnls[CH5_OFFSET];
        if (pwm_ch)
        {
            activate |= (1 << CH5_OFFSET);
            if (least_next > pwm_ch)
                least_next = pwm_ch;
        }

        // Channel 6
        pwm_ch = pwm_chnls[CH6_OFFSET];
        if (pwm_ch)
        {
            activate |= (1 << CH6_OFFSET);
            if (least_next > pwm_ch)
                least_next = pwm_ch;
        }
        
        PWM_Activate_Channels (activate);

        // call Dac Mux here
        DAC_MUX_Update_by_Int ((unsigned short *) dac_chnls);
        
        pwm_int_cnt = least_next;
        PWM_Timer_Update(least_next * PWM_TIMER_MULTIPLIER);

    }
}


//--- end of file ---//

