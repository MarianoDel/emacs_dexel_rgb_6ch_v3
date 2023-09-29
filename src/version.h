//-------------------------------------------------
// #### DEXEL 6CH BIDIRECTIONAL - Custom Board ####
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### VERSION.H #################################
//-------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _VERSION_H_
#define _VERSION_H_


// Exported Constants ----------------------------------------------------------
#define HARDWARE_VERSION_3_2    // 09-2023 5 prototipes
// #define HARDWARE_VERSION_3_1    // 05-2023 60 boards production
// #define HARDWARE_VERSION_3_0    // 10-2022 new prototipe
// #define HARDWARE_VERSION_2_3    // 04-2022 production boards 214 units
// #define HARDWARE_VERSION_2_2    // 2020 prototypes 4 units, & production boards 112 units
// #define HARDWARE_VERSION_2_0    // 2020 prototypes 4 units
// #define HARDWARE_VERSION_1_0    // 2018 prototypes 4 units


#define SOFTWARE_VERSION_1_4    // ntc detection, encoder direction, current temp
// #define SOFTWARE_VERSION_1_3    // abstraction layer on manager.c
// #define SOFTWARE_VERSION_1_2    // temp meas with ntc 1k
// #define SOFTWARE_VERSION_1_1    //changes on pwm frequency 4KHz -> 8KHz 12-2021
// #define SOFTWARE_VERSION_1_0    //original version delivered 02-2021


//--- Hardware Welcome Code ------------------//
#ifdef HARDWARE_VERSION_3_2
#define HARD "Hardware Version: 3.2\n"
#define HARD_VER "3.2"
#endif
#ifdef HARDWARE_VERSION_3_1
#define HARD "Hardware Version: 3.1\n"
#define HARD_VER "3.1"
#endif
#ifdef HARDWARE_VERSION_3_0
#define HARD "Hardware Version: 3.0\n"
#define HARD_VER "3.0"
#endif

//--- Software Welcome Code ------------------//
#ifdef SOFTWARE_VERSION_1_4
#define SOFT "Software Version: 1.4\n"
#define SOFT_VER "1.4"
#endif
#ifdef SOFTWARE_VERSION_1_3
#define SOFT "Software Version: 1.3\n"
#define SOFT_VER "1.3"
#endif
#ifdef SOFTWARE_VERSION_1_2
#define SOFT "Software Version: 1.2\n"
#define SOFT_VER "1.2"
#endif
#ifdef SOFTWARE_VERSION_1_1
#define SOFT "Software Version: 1.1\n"
#define SOFT_VER "1.1"
#endif
#ifdef SOFTWARE_VERSION_1_0
#define SOFT "Software Version: 1.0\n"
#define SOFT_VER "1.0"
#endif


#endif    /* _VERSION_H_ */

//--- end of file ---//
