#ifndef _CONFIG_H
#define	_CONFIG_H

#include <xc.h>


// CONFIG1
#pragma config FEXTOSC  = OFF
#pragma config RSTOSC   = HFINT1
#pragma config CLKOUTEN = OFF
#pragma config CSWEN    = ON
#pragma config FCMEN    = ON

// CONFIG2
#pragma config MCLRE    = ON
#pragma config PWRTE    = OFF
#pragma config LPBOREN  = OFF
#pragma config BOREN    = OFF
#pragma config BORV     = LO
#pragma config ZCD      = OFF
#pragma config PPS1WAY  = OFF
#pragma config STVREN   = ON

// CONFIG3
#pragma config WDTCPS   = WDTCPS_31
#pragma config WDTE     = OFF
#pragma config WDTCWS   = WDTCWS_7
#pragma config WDTCCS   = SC

// CONFIG4
#pragma config BBSIZE   = BB512
#pragma config BBEN     = OFF
#pragma config SAFEN    = OFF
#pragma config WRTAPP   = OFF
#pragma config WRTB     = OFF
#pragma config WRTC     = OFF
#pragma config WRTSAF   = OFF
#pragma config LVP      = ON

// CONFIG5
#pragma config CP       = OFF


#ifndef di
    #define di() GIE=0
#endif


#ifndef ei
    #define ei() GIE=1
#endif



#endif
