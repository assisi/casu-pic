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

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif
#include "../../peripheral/timer/timerFunctions.h"
#include "../../peripheral/gpio/digitalIO.h"


extern digitalPin PELTIER;
extern digitalPin PELTIER_EN;

void setUpPorts();

#endif	/* INITIALIZEHARDWARE_H */

