//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### DMX_TRANSCEIVER.C #####################
//---------------------------------------------
#include "dmx_transceiver.h"

#include "hard.h"
#include "stm32f10x.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"


#ifdef WITH_RDM
#include "rdm_util.h"
#endif

// Module Configurations -------------------------------------------------------
//TODO: despues poner esto en dmx_transceiver_conf.h y linkear dmx_transceiver.*
#define TIM_DMX    TIM7
#define USART_DMX    USART3

#define DMX_TIM_OneShoot(X)    TIM7_OneShoot(X)
#define DMX_TIM_To_Free_Running    TIM7_To_FreeRunning
#define DMX_TIM_To_One_Shoot    TIM7_To_OneShoot

#define DMX_PIN_To_PushPull    PB10_Pin_To_PushPull
#define DMX_PIN_To_Alternative    PB10_Pin_To_Alternative

#define DMX_Usart_Send(X)    Usart3SendByte(X)
#define DMX_Usart_Disable    Usart3DisableTx


// Module Private Types Constants and Macros -----------------------------------
#define DMX_TIMEOUT	20

typedef enum {
    PCKT_RX_IDLE = 0,
    PCKT_RX_LOOK_FOR_BREAK,
    PCKT_RX_LOOK_FOR_MARK,
    PCKT_RX_LOOK_FOR_START
} pckt_rx_t;


// Externals -------------------------------------------------------------------
extern volatile unsigned char RDM_packet_flag;
extern volatile unsigned char data11[];

extern volatile unsigned char Packet_Detected_Flag;
extern volatile unsigned char DMX_packet_flag;
extern volatile unsigned char RDM_packet_flag;
extern volatile unsigned short DMX_channel_selected;
extern volatile unsigned char DMX_channel_quantity;


// Globals ---------------------------------------------------------------------
volatile pckt_rx_t dmx_signal_state = PCKT_RX_IDLE;
volatile unsigned short current_channel_index = 0;
volatile unsigned char dmx_receive_flag = 0;
volatile unsigned char dmx_timeout_timer = 0;


// Module Private Functions ----------------------------------------------------
#ifdef DMX_BIDIRECTIONAL
inline void UsartSendDMX (void);
#endif



// Module Functions ------------------------------------------------------------
void DMX_EnableRx (void)
{
    //enable driver in rx mode
    SW_RX_TX_RE_NEG;
    //enable the Rx int break detect
    EXTIOn ();
    //enable usart and Rx int
    USART_DMX->CR1 |= USART_CR1_RXNEIE | USART_CR1_UE;

#ifdef DMX_BIDIRECTIONAL
    // config the TIM for freerunning
    DMX_TIM_To_Free_Running();
#endif
}

#ifdef DMX_BIDIRECTIONAL
void DMX_EnableTx (void)
{
    // enable driver in tx mode
    SW_RX_TX_DE;
    // enable usart
    USART_DMX->CR1 |= USART_CR1_UE;
    // config the TIM for OneShoot
    DMX_TIM_To_One_Shoot();
}
#endif


void DMX_Disable (void)
{
    //disable Rx driver -> to tx mode
    SW_RX_TX_DE;
    //disable the Rx int break detect
    EXTIOff ();
    //disable int and driver on usart
    USART_DMX->CR1 &= ~(USART_CR1_TXEIE | USART_CR1_RXNEIE | USART_CR1_UE);
}

void UpdatePackets (void)
{
    if (Packet_Detected_Flag)
    {
        if (data11[0] == 0x00)
            DMX_packet_flag = 1;

        if (data11[0] == 0xCC)
            RDM_packet_flag = 1;

        Packet_Detected_Flag = 0;
    }
}


volatile unsigned char data11_cnt = 0;
void DmxInt_Serial_Handler_Receiver (unsigned char dummy)
{
    if (dmx_receive_flag)
    {
        //TODO: analize this channel 511 is not included!!!
        if (current_channel_index < LAST_DMX_CHANNEL_512)    //else discard silently
        {
            // -- begin old reception
            // data512[current_channel_index] = dummy;            

            // if (current_channel_index >= (DMX_channel_selected + DMX_channel_quantity))
            // {
            //     //copio el inicio del buffer y luego los elegidos
            //     data11[0] = data512[0];
            //     for (unsigned char i = 0; i < DMX_channel_quantity; i++)
            //         data11[i + 1] = data512[(DMX_channel_selected) + i];

            //     //--- Reception end ---//
            //     current_channel_index = 0;
            //     dmx_receive_flag = 0;
            //     Packet_Detected_Flag = 1;
            // }
            
            // current_channel_index++;
            // -- end old reception

            // -- begin new reception
            if (current_channel_index == 0)
            {
                // first byte for rx
                data11[0] = dummy;
                data11_cnt = 1;
            }
            else if (current_channel_index >= (DMX_channel_selected + DMX_channel_quantity - 1))
            {
                // last byte of rx
                data11[data11_cnt] = dummy;
                dmx_receive_flag = 0;
                Packet_Detected_Flag = 1;
            }
            else if (current_channel_index >= (DMX_channel_selected))
            {
                // inner packet bytes
                data11[data11_cnt] = dummy;
                data11_cnt++;                
            }
            current_channel_index++;            
            // -- end new reception
        }
    }
}

//the dmx line is high when idle, going to zero on the start of packet
//the time that remains in zero is the signal break (87us to 4800us) are valid times
//the time in one is the signal mark (8us aprox.) this time its not checked
//after that the serial bytes start to comming on 250Kbits, can have idle times between them
void DmxInt_Break_Handler (void)
{
    unsigned short aux;
    //si no esta con el USART detecta el flanco	PONER TIMEOUT ACA?????
    if ((dmx_receive_flag == 0) || (dmx_timeout_timer == 0))
        //if (dmx_receive_flag == 0)
    {
        switch (dmx_signal_state)
        {
        case PCKT_RX_IDLE:
            if (!(DMX_INPUT))
            {
                //Activo timer en Falling.
                TIM_DMX->CNT = 0;
                TIM_DMX->CR1 |= 0x0001;
                dmx_signal_state++;
            }
            break;

        case PCKT_RX_LOOK_FOR_BREAK:
            if (DMX_INPUT)
            {
                //Desactivo timer en Rising.
                aux = TIM_DMX->CNT;

                //reviso BREAK
                //if (((tim_counter_65ms) || (aux > 88)) && (tim_counter_65ms <= 20))
                //if ((aux > 87) && (aux < 210))	//Consola STARLET 6
                //if ((aux > 87) && (aux < 2000))		//Consola marca CODE tiene break 1.88ms
                if ((aux > 87) && (aux < 4600))		//Consola marca CODE modelo A24 tiene break 4.48ms fecha 11-04-17
                {
                    dmx_signal_state++;
                    current_channel_index = 0;
                    dmx_timeout_timer = DMX_TIMEOUT;		//activo el timeout para esperar un MARK valido
                }
                else	//falso disparo
                    dmx_signal_state = PCKT_RX_IDLE;
            }
            else	//falso disparo
                dmx_signal_state = PCKT_RX_IDLE;

            TIM_DMX->CR1 &= 0xFFFE;
            break;

        case PCKT_RX_LOOK_FOR_MARK:
            if ((!(DMX_INPUT)) && (dmx_timeout_timer))	//termino Mark after break
            {
                //ya tenia el serie habilitado
                dmx_receive_flag = 1;
                EXTIOff();    //dejo 20ms del paquete sin INT
            }
            else	//falso disparo
            {
                //termine por timeout
                dmx_receive_flag = 0;
            }
            dmx_signal_state = PCKT_RX_IDLE;
            break;

        default:
            dmx_signal_state = PCKT_RX_IDLE;
            break;
        }
    }
}


unsigned char Dmx_Get_Packet_Init_Flag (void)
{
    return dmx_receive_flag;
}


// unsigned char Dmx_Get_Packet_Detect_Flag (void)
// {
//     return DMX_packet_flag;
// }


void Dmx_Timeouts (void)
{
    //after a start of pck rx, leave 20ms the int in off
    if (dmx_timeout_timer)
        dmx_timeout_timer--;
    else
        EXTIOn();
    
}


#if defined DMX_BIDIRECTIONAL
typedef enum {
    SEND_BREAK,
    SEND_MARK,
    SEND_DMX_DATA
    
} dmx_tx_state_e;


void DmxInt_Serial_Handler_Transmitter (void)
{
    // -- begin new transmission
    if (current_channel_index == 0)
    {
        // first byte to tx
        DMX_Usart_Send (0);
        data11_cnt = 1;
    }
    else if (current_channel_index < DMX_channel_selected)
    {
        // inner bytes
        DMX_Usart_Send (0);        
    }
    else if (current_channel_index <= (DMX_channel_selected + (DMX_channel_quantity - 1)))
    {
        // packet bytes
        DMX_Usart_Send (data11[data11_cnt]);
        data11_cnt++;
    }
    else if (current_channel_index < 512)
    {
        // remaining bytes
        DMX_Usart_Send (0);
    }
    else
        DMX_Usart_Disable();
    
    current_channel_index++;            

}


// Send a DMX packet
// packet must be on data11
// PCKT_INIT called by user, starts the state machine
// PCKT_UPDATE state machine automation
// one inited, it called itself by OneShoot and Usart
volatile dmx_tx_state_e dmx_state = SEND_BREAK;
void SendDMXPacket (unsigned char new_func)
{
    if (!(USART_DMX->CR1 & USART_CR1_UE))
        return;
    
    if (new_func == PCKT_INIT)
        dmx_state = SEND_BREAK;
    
    switch (dmx_state)
    {
    case SEND_BREAK:
        SW_RX_TX_DE;
        DMX_PIN_To_PushPull();
        DMX_TX_PIN_OFF;
        DMX_TIM_OneShoot(200);
        dmx_state++;
        break;

    case SEND_MARK:
        DMX_TX_PIN_ON;
        DMX_TIM_OneShoot(8);        
        dmx_state++;
        break;

    case SEND_DMX_DATA:
        DMX_PIN_To_Alternative();
        UsartSendDMX();
        break;

    default:
        dmx_state = SEND_BREAK;
        break;
    }
}


__attribute__((always_inline)) void UsartSendDMX (void)
{
    current_channel_index = 0;
    USART_DMX->CR1 |= USART_CR1_TXEIE;
}

#else //no DMX_BIDIRECTIONAL
void DmxInt_Serial_Handler_Transmitter (void)
{
}
#endif

#ifdef DMX_WITH_RDM
//revisa si existe paquete RDM y que hacer con el mismo
//
void UpdateRDMResponder(void)
{
    RDMKirnoHeader * p_header;

    p_header = (RDMKirnoHeader *) data11;
    if (RDM_packet_flag)
    {
        //voy a revisar si el paquete tiene buen checksum
        if (RDMUtil_VerifyChecksumK((unsigned char *)data11, data11[1]) == true)
        {
            LED_ON;
            //reviso si es unicast
            if (RDMUtil_IsUnicast(p_header->dest_uid) == true)
                LED_OFF;
        }
        else
            LED_OFF;
        RDM_packet_flag = 0;
    }
}
#endif


//--- end of file ---//
