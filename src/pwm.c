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


// Externals -------------------------------------------------------------------


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


// // void PWM_Set_PwrCtrl (unsigned char * ch_dmx_val, unsigned char chnls_qtty, unsigned short max_power)
// // {
// //     unsigned short total_dmx = 0;

// //     //cuantos en total
// //     for (unsigned char i = 0; i < chnls_qtty; i++)
// //         total_dmx += *(ch_dmx_val + i);

// //     if (total_dmx > max_power)
// //     {
// //         unsigned int new = 0;
// //         for (unsigned char i = 0; i < chnls_qtty; i++)
// //         {
// //             // si el canal tiene algo
// //             if (*(ch_dmx_val + i))
// //             {
// //                 new = *(ch_dmx_val + i) * max_power;
// //                 new = new / total_dmx;

// //                 // no dejo que se apaguen los canales
// //                 if (new)
// //                     *(ch_dmx_val + i) = (unsigned char) new;
// //                 else
// //                     *(ch_dmx_val + i) = 1;
                
// //             }
// //         }
// //     }
// // }


// // get dmx_data from 0 to 255
// // answer pwm_ena 0 to 4096
// // answer pwm_ch 0 to 4096
// void PWM_Map_Pre_Filter (unsigned char dmx_data, unsigned short * pwm_ena, unsigned short * pwm_ch)
// {
//     unsigned char dmx_ena = 0;
//     unsigned short dmx_ch = 0;
    
//     if (dmx_data > 4)
//     {
//         dmx_ena = 4;
//         dmx_ch = dmx_data - 4;
//     }
//     else
//     {
//         dmx_ena = dmx_data;
//         dmx_ch = 0;
//     }

//     // *pwm_ena = dmx_ena * 1024;
//     *pwm_ena = dmx_ena << 10;
    
//     dmx_ch = dmx_ch * 164;
//     dmx_ch = dmx_ch / 10;
//     *pwm_ch = dmx_ch;
    
// }


// get dmx_filtered from 0 to 4095
// answer pwm_ena 0 to 255
// answer dac_ch 0 to 4095
void PWM_Map_Post_Filter (unsigned short dmx_filtered, unsigned short * pwm_ena, unsigned short * pwm_ch)
{
    unsigned short dmx_ena = 0;
    unsigned int dmx_ch = 0;
    
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
void PWM_Timer_Handler (void)
{
    if (int_edge < edges)
    {
        // deactivate channels in this instance
        PWM_Deactivate_Channels (deactivate[int_edge]);

        // set the next timer
        int_edge++;
        PWM_Timer_Update(vtime[int_edge]);
    }
    else
    {
        // start the next cycle loop
        if (!pwm_update_block)
            PWM_Timer_Setup ();
        
        int_edge = 0;
        PWM_Timer_Update(vtime[0]);
        PWM_Activate_Channels (activate);
    }
}


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
    unsigned short remain = 255 * 10;
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

        if (least == 255)
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
            diff = last_pwm * 10 - account;
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
    *least = 255;
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


//--- end of file ---//

