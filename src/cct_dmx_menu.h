//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### CCT_DMX_MENU.C ############################
//-------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _CCT_DMX_MENU_H_
#define _CCT_DMX_MENU_H_

//-- includes to help configurations --//
#include "switches_answers.h"


// Module Exported Types Constants and Macros ----------------------------------
typedef struct {
    const unsigned short * dmx_first_chnl;
    const unsigned char * pchannels;
    unsigned char chnls_qtty;    
    unsigned char show_addres;
    unsigned char mode;
    
} dmx_menu_data_t;


typedef struct {
    unsigned short dmx_address;
    unsigned char dmx_channels_qtty;
    sw_actions_t actions;
    unsigned short * timer;
    unsigned char * address_show;
    
} dmx_menu_address_data_t;


// Module Exported Functions ---------------------------------------------------
void Cct_DMX_MenuReset (void);
resp_t Cct_DMX_Menu (dmx_menu_data_t *);
void DMXModeMenu_ChangeAddressReset (void);
resp_t DMXModeMenu_ChangeAddress (dmx_menu_address_data_t *);


#endif    /* _CCT_DMX_MENU_H_ */

//--- end of file ---//
