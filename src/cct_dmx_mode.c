//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_DMX_MODE.C ###############################
//-------------------------------------------------

// Includes --------------------------------------------------------------------
#include "cct_dmx_mode.h"
#include "cct_dmx_menu.h"
#include "cct_utils.h"
#include "cct_manual_colors_menu.h"

#include "flash_program.h"
#include "parameters.h"

#include <stdio.h>
#include <string.h>


// Private Types Constants and Macros ------------------------------------------
typedef enum {
    CCT_DMX_MODE_INIT = 0,
    CCT_DMX_MODE_RUNNING

} cct_dmx_mode_e;


#define CCT_DMX_PKT_TYPE    0
#define CCT_DMX_CH1    1
#define CCT_DMX_CH2    2
#define CCT_DMX_CH3    3
#define CCT_DMX_CH4    4
#define CCT_DMX_CH5    5
// #define CCT_DMX_CCT_CH    6
#define CCT_DMX_CCT_CH    7
#define CCT_DMX_STB_CH    6
// #define CCT_DMX_STB_CH    7
#define CCT_DMX_CLR_CH    8
#define CCT_DMX_DIM_CH    9
// #define CCT_DMX_DIM_CH    6


#define TT_MENU_TIMEOUT    30000
#define TT_DMX_RECEIVING    1000

// dmx effects
#define DMX_NO_EFFECT    0
#define DMX_CCT_EFFECT    1
#define DMX_STB_EFFECT    2
#define DMX_CLR_EFFECT    3

// variables re-use
#define cct_dmx_mode_state    mode_state
#define cct_dmx_mode_effect_timer    mode_effect_timer


// Externals -------------------------------------------------------------------
extern volatile unsigned char data11[];

// -- externals re-used
extern unsigned char mode_state;
extern volatile unsigned short mode_effect_timer;


//del main para dmx
extern volatile unsigned char Packet_Detected_Flag;
extern volatile unsigned short DMX_channel_selected;

extern parameters_typedef mem_conf;


// Globals ---------------------------------------------------------------------
unsigned char cct_dmx_end_of_packet_update = 0;


//-- timers del modulo --------------------
volatile unsigned short cct_dmx_mode_enable_menu_timer = 0;
// volatile unsigned short cct_dmx_mode_dmx_receiving_timer = 0;


// Module Private Functions ----------------------------------------------------
void Cct_DMXMode_ChannelsDimmer (unsigned char *, unsigned char *);
void Cct_DMXMode_ChannelsStrobe (unsigned char *);
void Cct_DMXMode_ChannelsStrobeSet (unsigned char *);
// void CCT_DMXMode_ChannelsEffect (unsigned char *, unsigned char *);
unsigned char Cct_DMXMode_MapEffect (unsigned char);
unsigned char Cct_DMXMode_MapSpeed (unsigned char);


// Module Functions ------------------------------------------------------------
void Cct_DMXMode_UpdateTimers (void)
{
    if (cct_dmx_mode_enable_menu_timer)
        cct_dmx_mode_enable_menu_timer--;

    if (cct_dmx_mode_effect_timer)
        cct_dmx_mode_effect_timer--;
    
    // if (cct_dmx_mode_dmx_receiving_timer)
    //     cct_dmx_mode_dmx_receiving_timer--;

}

void Cct_DMXMode_Reset (void)
{
    cct_dmx_mode_state = CCT_DMX_MODE_INIT;
}

#define timer_address    cct_dmx_mode_enable_menu_timer
unsigned char cct_dmx_address_show = 0;
unsigned char cct_dmx_effect = 0;
resp_t Cct_DMXMode (unsigned char * ch_val, sw_actions_t action)
{
    resp_t resp = resp_continue;
    
    
    switch (cct_dmx_mode_state)
    {
    case CCT_DMX_MODE_INIT:
        Cct_DMX_MenuReset();
        cct_dmx_effect = DMX_NO_EFFECT;
        // DMXModeMenu_ChangeAddressReset();
        cct_dmx_address_show = 1;
        cct_dmx_mode_state++;
        break;

    case CCT_DMX_MODE_RUNNING:

        //update del dmx - generalmente a 40Hz -
        //desde el dmx al sp
        if (Packet_Detected_Flag)
        {
            Packet_Detected_Flag = 0;

            //le aviso al menu que se estan recibiendo paquetes dmx
            // cct_dmx_mode_dmx_receiving_timer = TT_DMX_RECEIVING;            

            if (data11[CCT_DMX_PKT_TYPE] == 0x00)    //dmx packet
            {
                // check if we have an effect
                if (data11[CCT_DMX_CCT_CH] > 10)
                {
                    if (cct_dmx_effect != DMX_CCT_EFFECT)
                        cct_dmx_effect = DMX_CCT_EFFECT;

                    int temp_color = data11[CCT_DMX_CCT_CH] - 10;
                    temp_color = temp_color * 1041;
                    temp_color = temp_color / 1000;
                    if (temp_color > 255)
                        temp_color = 255;

                    *(ch_val + 0) = 0;
                    *(ch_val + 1) = 0;
                    *(ch_val + 2) = 0;
                    // *(ch_val + 3) = Cct_Utils_Dim_Color (
                    //     data11[CCT_DMX_DIM_CH],
                    //     255 - temp_color);

                    // *(ch_val + 4) = Cct_Utils_Dim_Color (
                    //     data11[CCT_DMX_DIM_CH],
                    //     temp_color);

                    *(ch_val + 3) = Cct_Utils_Dim_Color (
                        255,
                        255 - temp_color);

                    *(ch_val + 4) = Cct_Utils_Dim_Color (
                        255,
                        temp_color);
                    
                    cct_dmx_end_of_packet_update = 1;
                    resp = resp_change;
                    break;                    
                }
                else if (data11[CCT_DMX_STB_CH] > 10)
                {                    
                    if (cct_dmx_effect != DMX_STB_EFFECT)
                        cct_dmx_effect = DMX_STB_EFFECT;

                    // dim the channels
                    // for (int i = 0; i < 5; i++)
                    // {
                    //     *(ch_val + i) = Cct_Utils_Dim_Color (
                    //         data11[CCT_DMX_DIM_CH],
                    //         data11[CCT_DMX_CH1 + i]);
                    // }
                    for (int i = 0; i < 5; i++)
                    {
                        *(ch_val + i) = Cct_Utils_Dim_Color (
                            255,
                            data11[CCT_DMX_CH1 + i]);
                    }
                    
                    // map the channels values to strobe
                    Cct_DMXMode_ChannelsStrobeSet((unsigned char *) &data11[CCT_DMX_CH1]);
                    
                    // map the speed in this mode
                    mem_conf.program_inner_type_speed = Cct_DMXMode_MapSpeed(data11[CCT_DMX_STB_CH]);
                    
                }
                else if (data11[CCT_DMX_CLR_CH] > 10)
                {                    
                    if (cct_dmx_effect != DMX_CLR_EFFECT)
                        cct_dmx_effect = DMX_CLR_EFFECT;

                    // map the color
                    unsigned char rgb_color[5] = { 0 };
                    int index = data11[CCT_DMX_CLR_CH] / 10;
                    index -= 1;

                    Cct_Index_To_Channels_Mapping (index, rgb_color);
                    
                    // for (int i = 0; i < 5; i++)
                    // {
                    //     *(ch_val + i) = Cct_Utils_Dim_Color (
                    //         data11[CCT_DMX_DIM_CH],
                    //         rgb_color[i]);
                    // }
                    for (int i = 0; i < 5; i++)
                    {
                        *(ch_val + i) = Cct_Utils_Dim_Color (
                            255,
                            rgb_color[i]);
                    }

                    // map the channels values to strobe
                    Cct_DMXMode_ChannelsStrobeSet((unsigned char *) &data11[CCT_DMX_CH1]);
                    
                    // map the speed in this mode
                    mem_conf.program_inner_type_speed = Cct_DMXMode_MapSpeed(data11[CCT_DMX_STB_CH]);
                    
                }
                else    // we are in DMX mode with grandmaster
                {
                    Cct_DMXMode_ChannelsDimmer(ch_val, (unsigned char *) data11);
                    cct_dmx_end_of_packet_update = 1;
                    resp = resp_change;
                    break;
                }

                cct_dmx_end_of_packet_update = 1;                
            }
        }

        if (cct_dmx_end_of_packet_update)
        {
            dmx_menu_data_t cct_dmx_st;
            cct_dmx_st.dmx_first_chnl = &mem_conf.dmx_first_channel;
            cct_dmx_st.mode = CCT_DMX_MODE;
            cct_dmx_st.pchannels = (const unsigned char *) &(data11[1]);
            cct_dmx_st.chnls_qtty = mem_conf.dmx_channel_quantity;
            if (cct_dmx_address_show)
                cct_dmx_st.show_addres = 1;
            else
                cct_dmx_st.show_addres = 0;

            resp = Cct_DMX_Menu (&cct_dmx_st);
            if (resp == resp_finish)
                cct_dmx_end_of_packet_update = 0;
            
        }
        break;

    default:
        cct_dmx_mode_state = CCT_DMX_MODE_INIT;
        break;
    }


    //Effects in modes
    switch (cct_dmx_effect)
    {
    case DMX_CLR_EFFECT:
        // if (!cct_dmx_mode_effect_timer)
        // {
        //     resp = Colors_Fading_Pallete (ch_val);
        //     // if (resp == resp_finish)
        //     //     resp = resp_continue;

        //     resp = resp_change;
        //     cct_dmx_mode_effect_timer = 10 - mem_conf.program_inner_type_speed;
        // }
        break;

    // case CCT_DMX_INNER_GRADUAL_MODE:
    //     if (!cct_dmx_mode_effect_timer)
    //     {
    //         resp = Colors_Fading_Shuffle_Pallete (ch_val);
    //         // if (resp == resp_finish)
    //         //     resp = resp_continue;

    //         resp = resp_change;
    //         cct_dmx_mode_effect_timer = 10 - mem_conf.program_inner_type_speed;
    //     }
    //     break;

    case DMX_STB_EFFECT:
        if (!cct_dmx_mode_effect_timer)
        {
            Cct_DMXMode_ChannelsStrobe(ch_val);

            resp = resp_change;

            if (mem_conf.program_inner_type_speed > 4)
            {
                //from 128ms to 60ms
                //     5        9
                unsigned char fast = mem_conf.program_inner_type_speed - 5;
                cct_dmx_mode_effect_timer = 128 - fast * 17;
            }
            else
            {
                //from 728ms to 144ms
                //     0        4
                cct_dmx_mode_effect_timer = 800 - mem_conf.program_inner_type_speed * 164;
            }
        }
        break;

    case DMX_NO_EFFECT:
    case DMX_CCT_EFFECT:
    default:
        // do nothing in here
        break;
    }


    //look for a change in address if we are not changing colors
    if (resp != resp_change)
    {
        dmx_menu_address_data_t cct_dmx_addr_st;
        cct_dmx_addr_st.dmx_address = mem_conf.dmx_first_channel;
        cct_dmx_addr_st.dmx_channels_qtty = mem_conf.dmx_channel_quantity + 4;
        cct_dmx_addr_st.actions = action;
        cct_dmx_addr_st.timer = (unsigned short *) &timer_address;
        cct_dmx_addr_st.address_show = &cct_dmx_address_show;
        resp = DMXModeMenu_ChangeAddress(&cct_dmx_addr_st);

        if (resp == resp_change)
        {
            // change the DMX address
            DMX_channel_selected = cct_dmx_addr_st.dmx_address;
            mem_conf.dmx_first_channel = DMX_channel_selected;
        
            // force a display update
            cct_dmx_end_of_packet_update = 1;
        }

        if (resp == resp_finish)
        {
            //end of changing ask for a memory save
            resp = resp_need_to_save;
        
        }
    }
    return resp;
    
}
            

void Cct_DMXMode_ChannelsDimmer (unsigned char * ch_out, unsigned char * ch_in)
{
    for (int i = 0; i < 5; i++)
    {
        *(ch_out + i) = Cct_Utils_Dim_Color (
            ch_in[CCT_DMX_DIM_CH],
            ch_in[CCT_DMX_CH1 + i]);
    }
}


// unsigned char ch_strobe_ref [6] = { 0 };
extern unsigned char ch_strobe_ref [6];
void Cct_DMXMode_ChannelsStrobeSet (unsigned char * ch_in)
{
    for (int i = 0; i < 5; i++)
        ch_strobe_ref[i] = ch_in[i];
}


// unsigned char last_strobe_was_on = 0;
extern unsigned char last_strobe_was_on;
void Cct_DMXMode_ChannelsStrobe (unsigned char * ch_out)
{
    if (last_strobe_was_on)
    {
        last_strobe_was_on = 0;
        for (int i = 0; i < 5; i++)
            ch_out[i] = 0;
    }
    else
    {
        last_strobe_was_on = 1;
        for (int i = 0; i < 5; i++)
            ch_out[i] = ch_strobe_ref[i];
    }
}


unsigned char Cct_DMXMode_MapEffect (unsigned char eff)
{
    if (eff < 127)
        return 0;
    else
        return 1;
}


unsigned char Cct_DMXMode_MapSpeed (unsigned char speed)
{
    unsigned char out_speed = 0;
    
    if (speed > 230)
        out_speed = 9;
    else if (speed > 205)
        out_speed = 8;
    else if (speed > 180)
        out_speed = 7;
    else if (speed > 155)
        out_speed = 6;
    else if (speed > 130)
        out_speed = 5;
    else if (speed > 105)
        out_speed = 4;
    else if (speed > 80)
        out_speed = 3;
    else if (speed > 55)
        out_speed = 2;
    else if (speed > 20)
        out_speed = 1;

    return out_speed;
}

// unsigned char Cct_Low_Temp_To_Channel (unsigned char temp, unsigned char dimmer)
// {
//     unsigned char offset;
    
//     offset = 0;
//     calc = 255 - mem->cct_temp_color;
//     if (calc)
//         offset = 1;
    
//     calc = calc * mem->cct_dimmer;
//     calc >>= 8;
//     calc += offset;
    
//     return (unsigned char) calc;
    
//     mem->fixed_channels[3] = calc + offset;
    
//     offset = 0;            
//     calc = mem->cct_temp_color;            
//     if (calc)
//         offset = 1;
            
//     calc = calc * mem->cct_dimmer;
//     calc >>= 8;
//     mem->fixed_channels[4] = calc + offset;
// }

//--- end of file ---//
