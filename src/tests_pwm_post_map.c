//-------------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_PWM_POST_MAP.C ######################
//-------------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "pwm.h"
// #include "dsp.h"
// #include "filters_and_offsets.h"
// #include "parameters.h"


// Includes tests helper modules for the tests ---------------------------------
#include "tests_vector_utils.h"


#include <stdio.h>
#include <math.h>
#include <stdint.h>


// Types Constants and Macros --------------------------------------------------
// Select simulation lenght in mseconds
#define SIMULATION_MS    2000

#define VECTOR_LENGTH    SIMULATION_MS
// #define VECTOR_LENGTH    ((int)(SAMPLING_FREQ * SIMULATION_LENGTH))


// Externals -------------------------------------------------------------------
volatile unsigned char pwm_chnls [6];
volatile unsigned short dac_chnls [6];



// Globals ---------------------------------------------------------------------
unsigned short dmx_data_ch1 [VECTOR_LENGTH] = { 0 };
unsigned short dmx_data_ch2 [VECTOR_LENGTH] = { 0 };

unsigned short ena_output_ch1 [VECTOR_LENGTH] = { 0 };
unsigned short pwm_output_ch1 [VECTOR_LENGTH] = { 0 };

unsigned short ena_output_ch2 [VECTOR_LENGTH] = { 0 };
unsigned short pwm_output_ch2 [VECTOR_LENGTH] = { 0 };

// parameters_typedef mem_conf;

// Tests Functions -------------------------------------------------------------
void Test_PWM_Post_Mapping (void);
void Test_PWM_Power_Ctrl (void);
void Test_Dmx_Post_Step (void);
void Test_Dmx_Post_Step_Dowm (void);
void Test_Dmx_Post_Ramp_Ch1 (void);


// Main Function to Test -------------------------------------------------------


// Module Mocked Functions -----------------------------------------------------
void TIM6_Update (void);
void DAC_MUX_Update_by_Int (void);


// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{
    printf("Start of DMX simulations...\n");

    // Test_PWM_Power_Ctrl ();
    Test_PWM_Post_Mapping ();
    // Test_Dmx_Post_Step_Dowm ();    
    // Test_Dmx_Post_Ramp_Ch1 ();        

    return 0;
}


void Test_PWM_Power_Ctrl (void)
{
    printf("\nTest pwm power control\n");

    unsigned short chnls [6] = { 0 };
    
    unsigned short vch1_orig [512] = { 0 };
    unsigned short vch2_orig [512] = { 0 };
    unsigned short vch3_orig [512] = { 0 };
    unsigned short vch4_orig [512] = { 0 };
    unsigned short vch5_orig [512] = { 0 };
    unsigned short vch6_orig [512] = { 0 };

    unsigned short vch1_roof [512] = { 0 };
    unsigned short vch2_roof [512] = { 0 };
    unsigned short vch3_roof [512] = { 0 };
    unsigned short vch4_roof [512] = { 0 };
    unsigned short vch5_roof [512] = { 0 };
    unsigned short vch6_roof [512] = { 0 };
    
    for (int i = 0; i < 512; i++)
    {
        chnls[0] = i;
        chnls[1] = i;
        chnls[2] = i;
        chnls[3] = i;
        chnls[4] = i;
        chnls[5] = i;

        vch1_orig [i] = chnls[0];
        vch2_orig [i] = chnls[1];
        vch3_orig [i] = chnls[2];
        vch4_orig [i] = chnls[3];
        vch5_orig [i] = chnls[4];
        vch6_orig [i] = chnls[5];        
        
        // PWM_Set_PwrCtrl_512 (chnls, 6, 255);
        PWM_Set_PwrCtrl_512 (chnls, 6, 1530);
        // PWM_Set_PwrCtrl_512 (chnls, 6, 765);
        // PWM_Set_PwrCtrl (chnls, 6, 255);        

        vch1_roof [i] = chnls[0];
        vch2_roof [i] = chnls[1];
        vch3_roof [i] = chnls[2];
        vch4_roof [i] = chnls[3];
        vch5_roof [i] = chnls[4];
        vch6_roof [i] = chnls[5];        
    }
    
    ///////////////////////////
    // Backup Data to a file //
    ///////////////////////////
    FILE * file = fopen("data.txt", "w");

    if (file == NULL)
    {
        printf("data file not created!\n");
        return;
    }

    Vector_UShort_To_File (file, "ch1_root", vch1_roof, 512);
    Vector_UShort_To_File (file, "ch1_orig", vch1_orig, 512);
    Vector_UShort_To_File (file, "ch2", vch2_roof, 512);
    Vector_UShort_To_File (file, "ch3", vch3_roof, 512);    
    Vector_UShort_To_File (file, "ch4", vch4_roof, 512);
    Vector_UShort_To_File (file, "ch5", vch5_roof, 512);
    Vector_UShort_To_File (file, "ch6", vch6_roof, 512);
    printf("\nRun by hand python3 simul_outputs.py\n");



}

void Test_PWM_Post_Mapping (void)
{
    unsigned short chnls [6] = { 0 };

    printf("\nTest pwm post-mapping\n from 0 to 511");

    unsigned short vch1_orig [512] = { 0 };
    unsigned short vch2_orig [512] = { 0 };
    unsigned short vch3_orig [512] = { 0 };
    unsigned short vch4_orig [512] = { 0 };
    unsigned short vch5_orig [512] = { 0 };
    unsigned short vch6_orig [512] = { 0 };

    unsigned short ch1_pwm [512] = { 0 };
    unsigned short ch2_pwm [512] = { 0 };
    unsigned short ch3_pwm [512] = { 0 };
    unsigned short ch4_pwm [512] = { 0 };
    unsigned short ch5_pwm [512] = { 0 };
    unsigned short ch6_pwm [512] = { 0 };

    unsigned short ch1_dac [512] = { 0 };
    unsigned short ch2_dac [512] = { 0 };
    unsigned short ch3_dac [512] = { 0 };
    unsigned short ch4_dac [512] = { 0 };
    unsigned short ch5_dac [512] = { 0 };
    unsigned short ch6_dac [512] = { 0 };

    unsigned short pwm_ch;
    unsigned short dac_ch;
    for (int i = 0; i < 512; i++)
    {
        chnls[0] = i;
        chnls[1] = i;
        chnls[2] = i;
        chnls[3] = i;
        chnls[4] = i;
        chnls[5] = i;

        vch1_orig[i] = i;
        vch2_orig[i] = i;
        vch3_orig[i] = i;
        vch4_orig[i] = i;
        vch5_orig[i] = i;
        vch6_orig[i] = i;
        
        PWM_Map_Post_Filter(
            *(chnls + 0) << 3,
            &pwm_ch,
            &dac_ch);
        
        ch1_pwm [i] = pwm_ch;
        ch1_dac [i] = dac_ch;
    }

    // ShowVectorUShort("\nVector data input:\n", to_map, 600);
    // ShowVectorUShort("\nVector ena pwm:\n", vena, 600);    
    // ShowVectorUShort("\nVector dac out:\n", vdac, 600);

    ///////////////////////////
    // Backup Data to a file //
    ///////////////////////////
    FILE * file = fopen("data.txt", "w");

    if (file == NULL)
    {
        printf("data file not created!\n");
        return;
    }

    Vector_UShort_To_File (file, "ch1_data", vch1_orig, 512);
    Vector_UShort_To_File (file, "ch1_pwm", ch1_pwm, 512);    
    Vector_UShort_To_File (file, "ch1_dac", ch1_dac, 512);

    printf("\nRun by hand python3 simul_pwm_pre_data.py\n");

}


// void Test_Dmx_Post_Step (void)
// {
//     printf("\nTest dmx Step with post-mapping\n");
    
//     mem_conf.channels_operation_mode = CCT1_MODE;
//     mem_conf.max_current_channels[0] = 255;
//     mem_conf.max_current_channels[1] = 255;
    
//     // step 255
//     for (int i = 0; i < VECTOR_LENGTH; i++)
//     {
//         dmx_data_ch1 [i] = 255;
//         dmx_data_ch2 [i] = 0;
//     }
    
//     UpdateFiltersTest_Reset ();
//     for (int i = 0; i < VECTOR_LENGTH; i++)
//     {
//         unsigned char ch_dmx_val [2];
//         ch_dmx_val[0] = dmx_data_ch1[i];
//         ch_dmx_val[1] = dmx_data_ch2[i];

//         FiltersAndOffsets_Post_Mapping_SM (ch_dmx_val);
//         Update_PWM_Counters();
//     }

//     ShowVectorUShort("\nVector dmx data ch1:\n", dmx_data_ch1, 100);
//     ShowVectorUShort("\nVector dmx data ch2:\n", dmx_data_ch2, 100);    
//     ShowVectorUShort("\nVector ena outp ch1:\n", ena_output_ch1, 100);
//     ShowVectorUShort("\nVector pwm outp ch1:\n", pwm_output_ch1, 100);    
//     ShowVectorUShort("\nVector ena outp ch2:\n", ena_output_ch2, 100);
//     ShowVectorUShort("\nVector pwm outp ch2:\n", pwm_output_ch2, 100);    

//     ///////////////////////////
//     // Backup Data to a file //
//     ///////////////////////////
//     FILE * file = fopen("data.txt", "w");

//     if (file == NULL)
//     {
//         printf("data file not created!\n");
//         return;
//     }

//     Vector_UShort_To_File (file, "dmx_ch1", dmx_data_ch1, VECTOR_LENGTH);
//     Vector_UShort_To_File (file, "dmx_ch2", dmx_data_ch2, VECTOR_LENGTH);    
//     Vector_UShort_To_File (file, "ena_ch1", ena_output_ch1, VECTOR_LENGTH);
//     Vector_UShort_To_File (file, "pwm_ch1", pwm_output_ch1, VECTOR_LENGTH);    
//     Vector_UShort_To_File (file, "ena_ch2", ena_output_ch2, VECTOR_LENGTH);
//     Vector_UShort_To_File (file, "pwm_ch2", pwm_output_ch2, VECTOR_LENGTH);    

//     printf("\nRun by hand python3 simul_pwm_pre_data.py\n");

// }




// void Test_Dmx_Post_Step_Dowm (void)
// {
//     printf("\nTest dmx Step Down with post-mapping\n");
    
//     mem_conf.channels_operation_mode = CCT1_MODE;
//     mem_conf.max_current_channels[0] = 255;
//     mem_conf.max_current_channels[1] = 255;
    
//     // step up 255
//     for (int i = 0; i < (VECTOR_LENGTH/2); i++)
//     {
//         dmx_data_ch1 [i] = 255;
//         dmx_data_ch2 [i] = 0;
//     }

//     // step down 0
//     for (int i = (VECTOR_LENGTH/2); i < VECTOR_LENGTH; i++)
//     {
//         dmx_data_ch1 [i] = 0;
//         dmx_data_ch2 [i] = 0;
//     }
    
//     UpdateFiltersTest_Reset ();
//     for (int i = 0; i < VECTOR_LENGTH; i++)
//     {
//         unsigned char ch_dmx_val [2];
//         ch_dmx_val[0] = dmx_data_ch1[i];
//         ch_dmx_val[1] = dmx_data_ch2[i];

//         FiltersAndOffsets_Post_Mapping_SM (ch_dmx_val);
//         Update_PWM_Counters();
//     }

//     ShowVectorUShort("\nVector dmx data ch1:\n", dmx_data_ch1, 100);
//     ShowVectorUShort("\nVector dmx data ch2:\n", dmx_data_ch2, 100);    
//     ShowVectorUShort("\nVector ena outp ch1:\n", ena_output_ch1, 100);
//     ShowVectorUShort("\nVector pwm outp ch1:\n", pwm_output_ch1, 100);    
//     ShowVectorUShort("\nVector ena outp ch2:\n", ena_output_ch2, 100);
//     ShowVectorUShort("\nVector pwm outp ch2:\n", pwm_output_ch2, 100);    

//     ///////////////////////////
//     // Backup Data to a file //
//     ///////////////////////////
//     FILE * file = fopen("data.txt", "w");

//     if (file == NULL)
//     {
//         printf("data file not created!\n");
//         return;
//     }

//     Vector_UShort_To_File (file, "dmx_ch1", dmx_data_ch1, VECTOR_LENGTH);
//     Vector_UShort_To_File (file, "dmx_ch2", dmx_data_ch2, VECTOR_LENGTH);    
//     Vector_UShort_To_File (file, "ena_ch1", ena_output_ch1, VECTOR_LENGTH);
//     Vector_UShort_To_File (file, "pwm_ch1", pwm_output_ch1, VECTOR_LENGTH);    
//     Vector_UShort_To_File (file, "ena_ch2", ena_output_ch2, VECTOR_LENGTH);
//     Vector_UShort_To_File (file, "pwm_ch2", pwm_output_ch2, VECTOR_LENGTH);    

//     printf("\nRun by hand python3 simul_pwm_pre_data.py\n");

// }


// void Test_Dmx_Post_Ramp_Ch1 (void)
// {
//     printf("\nTest dmx Ramp with post-mapping\n");
    
//     mem_conf.channels_operation_mode = CCT1_MODE;
//     mem_conf.max_current_channels[0] = 255;
//     mem_conf.max_current_channels[1] = 255;
    
//     // ramp 255
//     for (int i = 0; i < VECTOR_LENGTH; i++)
//     {
//         if (i <= 255)
//         {
//             dmx_data_ch1 [i] = i;
//             dmx_data_ch2 [i] = 0;
//         }
//         else
//         {
//             dmx_data_ch1 [i] = 255;
//             dmx_data_ch2 [i] = 0;
//         }
//     }
    
//     UpdateFiltersTest_Reset ();
//     for (int i = 0; i < VECTOR_LENGTH; i++)
//     {
//         unsigned char ch_dmx_val [2];
//         ch_dmx_val[0] = dmx_data_ch1[i];
//         ch_dmx_val[1] = dmx_data_ch2[i];

//         FiltersAndOffsets_Post_Mapping_SM (ch_dmx_val);
//         Update_PWM_Counters();
//     }

//     ShowVectorUShort("\nVector dmx data ch1:\n", dmx_data_ch1, 100);
//     ShowVectorUShort("\nVector dmx data ch2:\n", dmx_data_ch2, 100);    
//     ShowVectorUShort("\nVector ena outp ch1:\n", ena_output_ch1, 100);
//     ShowVectorUShort("\nVector pwm outp ch1:\n", pwm_output_ch1, 100);    
//     ShowVectorUShort("\nVector ena outp ch2:\n", ena_output_ch2, 100);
//     ShowVectorUShort("\nVector pwm outp ch2:\n", pwm_output_ch2, 100);    

//     ///////////////////////////
//     // Backup Data to a file //
//     ///////////////////////////
//     FILE * file = fopen("data.txt", "w");

//     if (file == NULL)
//     {
//         printf("data file not created!\n");
//         return;
//     }

//     Vector_UShort_To_File (file, "dmx_ch1", dmx_data_ch1, VECTOR_LENGTH);
//     Vector_UShort_To_File (file, "dmx_ch2", dmx_data_ch2, VECTOR_LENGTH);    
//     Vector_UShort_To_File (file, "ena_ch1", ena_output_ch1, VECTOR_LENGTH);
//     Vector_UShort_To_File (file, "pwm_ch1", pwm_output_ch1, VECTOR_LENGTH);    
//     Vector_UShort_To_File (file, "ena_ch2", ena_output_ch2, VECTOR_LENGTH);
//     Vector_UShort_To_File (file, "pwm_ch2", pwm_output_ch2, VECTOR_LENGTH);    

//     printf("\nRun by hand python3 simul_pwm_pre_data.py\n");

// }


// void UpdateFiltersTest_Reset (void)
// {
//     MA32_U16Circular_Reset(&st_sp1);
//     MA32_U16Circular_Reset(&st_sp2);
//     MA32_U16Circular_Reset(&st_ena1);
//     MA32_U16Circular_Reset(&st_ena2);    
// }


// int pwm_cntr = 0;
// int ch1_update = 0;
// int ch2_update = 0;
// int ena1_update = 0;
// int ena2_update = 0;
// void Update_PWM_Counters (void)
// {
//     if (pwm_cntr)
//     {
//         if (!ch1_update)
//             pwm_output_ch1[pwm_cntr] = pwm_output_ch1[pwm_cntr - 1];
//         else
//             ch1_update = 0;

//         if (!ch2_update)        
//             pwm_output_ch2[pwm_cntr] = pwm_output_ch2[pwm_cntr - 1];
//         else
//             ch2_update = 0;

//         if (!ena1_update)        
//             ena_output_ch1[pwm_cntr] = ena_output_ch1[pwm_cntr - 1];
//         else
//             ena1_update = 0;
        
//         if (!ena2_update)        
//             ena_output_ch2[pwm_cntr] = ena_output_ch2[pwm_cntr - 1];
//         else
//             ena2_update = 0;
        
//     }
//     pwm_cntr++;
// }


// void Update_TIM3_CH1 (unsigned short new_pwm)
// {
//     pwm_output_ch1[pwm_cntr] = new_pwm;
//     ch1_update = 1;
// }


// void Update_TIM3_CH2 (unsigned short new_pwm)
// {
//     pwm_output_ch2[pwm_cntr] = new_pwm;
//     ch2_update = 1;    
// }


// void Update_TIM1_CH2 (unsigned short new_pwm)
// {
//     ena_output_ch1[pwm_cntr] = new_pwm;
//     ena1_update = 1;    
// }


// void Update_TIM1_CH4 (unsigned short new_pwm)
// {
//     ena_output_ch2[pwm_cntr] = new_pwm;
//     ena2_update = 1;    
// }

// Module Mocked Functions -----------------------------------------------------
void TIM_Deactivate_Channels (unsigned char deact_chnls)
{
    printf("deactivate: 0x%02x\n", deact_chnls);
}


void TIM_Activate_Channels (unsigned char act_chnls)
{
    printf("activate: 0x%02x\n", act_chnls);
}

void TIM6_Update (void)
{
}


void DAC_MUX_Update_by_Int (void)
{
}

//--- end of file ---//


