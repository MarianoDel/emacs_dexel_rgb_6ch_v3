//----------------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### FILTERS_AND_OFFSETS.C ########################
//----------------------------------------------------

// Includes --------------------------------------------------------------------
#include "filters_and_offsets.h"
#include "dsp.h"
#include "parameters.h"
#include "pwm.h"
// #include "tim.h"


// Module Private Types & Macros -----------------------------------------------


// Externals -------------------------------------------------------------------
extern volatile unsigned short dac_chnls [];
extern volatile unsigned char pwm_chnls[];

// Globals ---------------------------------------------------------------------
ma16_u16_data_obj_t st_sp1;
ma16_u16_data_obj_t st_sp2;
ma16_u16_data_obj_t st_sp3;
ma16_u16_data_obj_t st_sp4;
ma16_u16_data_obj_t st_sp5;
ma16_u16_data_obj_t st_sp6;

volatile unsigned char ch_dmx_val [6] = { 0 };
volatile unsigned short limit_output [6] = { 0 };

volatile unsigned char filters_enable_outputs = 0;
// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void FiltersAndOffsets_Channels_to_Backup (unsigned char * channels)
{
    // __disable_irq();
    *(ch_dmx_val + 0) = *(channels + 0);
    *(ch_dmx_val + 1) = *(channels + 1);
    *(ch_dmx_val + 2) = *(channels + 2);
    *(ch_dmx_val + 3) = *(channels + 3);
    *(ch_dmx_val + 4) = *(channels + 4);
    *(ch_dmx_val + 5) = *(channels + 5);
    // __enable_irq();
}


void FiltersAndOffsets_Enable_Outputs (void)
{
    filters_enable_outputs = 1;
}


void FiltersAndOffsets_Disable_Outputs (void)
{
    filters_enable_outputs = 0;    
}


typedef enum {
    FILTERS_BKP_CHANNELS,
    FILTERS_LIMIT_EACH_CHANNEL,
    FILTERS_LIMIT_ALL_CHANNELS,
    FILTERS_OUTPUTS_CH1_CH3,
    FILTERS_OUTPUTS_CH4_CH6
    
} filters_and_offsets_e;


filters_and_offsets_e filters_sm = FILTERS_BKP_CHANNELS;
void FiltersAndOffsets_Calc_SM (parameters_typedef * pmem)
{
    unsigned int calc = 0;    

    if (!filters_enable_outputs)
        return;
    
    switch (filters_sm)
    {
    case FILTERS_BKP_CHANNELS:
        *(limit_output + 0) = *(ch_dmx_val + 0);
        *(limit_output + 1) = *(ch_dmx_val + 1);
        *(limit_output + 2) = *(ch_dmx_val + 2);
        *(limit_output + 3) = *(ch_dmx_val + 3);
        *(limit_output + 4) = *(ch_dmx_val + 4);
        *(limit_output + 5) = *(ch_dmx_val + 5);
        filters_sm++;
        break;

    case FILTERS_LIMIT_EACH_CHANNEL:
        calc = limit_output[0] * pmem->max_current_channels[0];
        calc >>= 7;
        limit_output[0] = (unsigned short) calc;

        calc = limit_output[1] * pmem->max_current_channels[1];
        calc >>= 7;
        limit_output[1] = (unsigned short) calc;

        calc = limit_output[2] * pmem->max_current_channels[2];
        calc >>= 7;
        limit_output[2] = (unsigned short) calc;

        calc = limit_output[3] * pmem->max_current_channels[3];
        calc >>= 7;
        limit_output[3] = (unsigned short) calc;

        calc = limit_output[4] * pmem->max_current_channels[4];
        calc >>= 7;
        limit_output[4] = (unsigned short) calc;

        calc = limit_output[5] * pmem->max_current_channels[5];
        calc >>= 7;
        limit_output[5] = (unsigned short) calc;

        filters_sm++;
        break;

    case FILTERS_LIMIT_ALL_CHANNELS:    // 0 to 511
        PWM_Set_PwrCtrl_512 (limit_output,
                             pmem->dmx_channel_quantity,
                             pmem->max_power);
        filters_sm++;
        break;

    case FILTERS_OUTPUTS_CH1_CH3:
        if (pmem->program_inner_type == DMX2_INNER_STROBE_MODE)
        {
            // outputs without filter
            // channel 1
            PWM_Map_Post_Filter (
                *(limit_output + 0) << 3,
                (unsigned char *)&pwm_chnls[0],
                (unsigned short *)&dac_chnls[0]);            

            // channel 2
            PWM_Map_Post_Filter (
                *(limit_output + 1) << 3,
                (unsigned char *)&pwm_chnls[1],
                (unsigned short *)&dac_chnls[1]);

            // channel 3
            PWM_Map_Post_Filter (
                *(limit_output + 2) << 3,
                (unsigned char *)&pwm_chnls[2],
                (unsigned short *)&dac_chnls[2]);
            
        }
        else
        {
            unsigned short ch_filter = 0;

            // channel 1
            ch_filter = MA16_U16Circular (
                &st_sp1,
                *(limit_output + 0) << 3);
            PWM_Map_Post_Filter (
                ch_filter,
                (unsigned char *)&pwm_chnls[0],
                (unsigned short *)&dac_chnls[0]);

            // channel 2
            ch_filter = MA16_U16Circular (
                &st_sp2,
                *(limit_output + 1) << 3);
            PWM_Map_Post_Filter (
                ch_filter,
                (unsigned char *)&pwm_chnls[1],
                (unsigned short *)&dac_chnls[1]);

            // channel 3
            ch_filter = MA16_U16Circular (
                &st_sp3,
                *(limit_output + 2) << 3);
            PWM_Map_Post_Filter (
                ch_filter,
                (unsigned char *)&pwm_chnls[2],
                (unsigned short *)&dac_chnls[2]);
        }

        filters_sm++;
        break;

    case FILTERS_OUTPUTS_CH4_CH6:
        if (pmem->program_inner_type == DMX2_INNER_STROBE_MODE)
        {
            // outputs without filter
            // channel 4
            PWM_Map_Post_Filter (
                *(limit_output + 3) << 3,
                (unsigned char *)&pwm_chnls[3],
                (unsigned short *)&dac_chnls[3]);

            // channel 5
            PWM_Map_Post_Filter (
                *(limit_output + 4) << 3,
                (unsigned char *)&pwm_chnls[4],
                (unsigned short *)&dac_chnls[4]);

            // channel 6
            PWM_Map_Post_Filter (
                *(limit_output + 5) << 3,
                (unsigned char *)&pwm_chnls[5],
                (unsigned short *)&dac_chnls[5]);
        }
        else
        {
            unsigned short ch_filter = 0;

            // channel 4
            ch_filter = MA16_U16Circular (
                &st_sp4,
                *(limit_output + 3) << 3);
            PWM_Map_Post_Filter (
                ch_filter,
                (unsigned char *)&pwm_chnls[3],
                (unsigned short *)&dac_chnls[3]);

            // channel 5
            ch_filter = MA16_U16Circular (
                &st_sp5,
                *(limit_output + 4) << 3);
            PWM_Map_Post_Filter (
                ch_filter,
                (unsigned char *)&pwm_chnls[4],
                (unsigned short *)&dac_chnls[4]);

            // channel 6
            ch_filter = MA16_U16Circular (
                &st_sp6,
                *(limit_output + 5) << 3);
            PWM_Map_Post_Filter (
                ch_filter,
                (unsigned char *)&pwm_chnls[5],
                (unsigned short *)&dac_chnls[5]);
        }

        filters_sm = FILTERS_BKP_CHANNELS;
        break;
        
    default:
        filters_sm = FILTERS_BKP_CHANNELS;
        break;
    }
}


void FiltersAndOffsets_Filters_Reset (void)
{
    MA16_U16Circular_Reset(&st_sp1);
    MA16_U16Circular_Reset(&st_sp2);
    MA16_U16Circular_Reset(&st_sp3);
    MA16_U16Circular_Reset(&st_sp4);
    MA16_U16Circular_Reset(&st_sp5);
    MA16_U16Circular_Reset(&st_sp6);
}


void FiltersAndOffsets_Channels_Reset (void)
{
    for (int i = 0; i < 6; i++)
    {
        ch_dmx_val [i] = 0;
        limit_output [i] = 0;
        pwm_chnls [i] = 0;
        dac_chnls [i] = 0;
    }
}


//--- end of file ---//
