/* 
 * File:   initializeHardware.h
 * Author: thaus
 *
 * Created on 2014. sije?anj 08, 14:34
 */

#ifndef INITIALIZEHARDWARE_H
#define	INITIALIZEHARDWARE_H

// desired frequency Fosc, note that Fp = Fcy = Fosc /2 = 40 Mhz
#define FOSC 80000000
#define DEBUG_UART 0

#include <p33Fxxxx.h>
#include "../../peripheral/timer/timerFunctions.h"

void setUpPorts();

#endif	/* INITIALIZEHARDWARE_H */

