/* 
 * File:   initializeHardware.h
 * Author: thaus
 *
 * Created on 2014. sije?anj 08, 14:34
 */

#ifndef INITIALIZEHARDWARE_H
#define	INITIALIZEHARDWARE_H

// desired frequency Fosc, note that Fp = Fcy = Fosc /2 = 40 Mhz
#define FOSC 40000000
#define DEBUG_UART 0

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif

#include "../peripheral/timer/timerFunctions.h"
#include "../peripheral/gpio/digitalIO.h"
#include "../pic-to-beaglebone/pic2beaglebone.h"

#define ACC_F 0
#define ACC_R 1
#define ACC_B 2
#define ACC_L 3

/*
extern digitalPin LED2R;
extern digitalPin LED2G;
extern digitalPin LED2B;
*/

/* Digital pins used as chip select pins for SPI comm*/
extern digitalPin aSlaveF;
extern digitalPin aSlaveR;
extern digitalPin aSlaveB;
extern digitalPin aSlaveL;

/*Define chip select pins for adt7320 sensors used*/
extern digitalPin tSlaveF;
extern digitalPin tSlaveR;
extern digitalPin tSlaveB;
extern digitalPin tSlaveL;
extern digitalPin slaveVib;

extern digitalPin MUXRST;   //I2C mux

extern digitalPin PEL_L;    //Peltier
extern digitalPin PEL_H;

void setUpPorts();
int PIDcontroller_t(float temp);


#endif	/* INITIALIZEHARDWARE_H */

