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

/*Mux ports*/
#define muxPort0 8
#define muxPort1 9
#define muxPort2 10
#define muxPort3 11
#define muxPort4 12
#define muxPort5 13
#define muxPort6 14
#define muxPort7 15

/*Map mux ports to proxy sensors */
#define proxyF  muxPort1
#define proxyFR muxPort0
#define proxyBR muxPort7
#define proxyB muxPort6
#define proxyBL muxPort5
#define proxyFl muxPort2
#define proxyT muxPort4

extern digitalPin LED2R;
extern digitalPin LED2G;
extern digitalPin LED2B;
extern digitalPin I2Crst;

extern digitalPin PELTIER;   //SPI select
extern digitalPin PELTIER_EN;

void setUpPorts();

#endif	/* INITIALIZEHARDWARE_H */

