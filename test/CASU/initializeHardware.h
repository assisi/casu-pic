/*! \file initializeHardware.h
 *  \brief Declarations of functions used for initializing devices (MCU ports, MCU peripheral devices).
 */

#ifndef INITIALIZEHARDWARE_H
#define	INITIALIZEHARDWARE_H

// 
/*! Desired frequency Fosc.
 * Note that CPU frequency is Fp = Fcy = Fosc /2.
 */
#define FOSC 80000000

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif

#include "../../peripheral/timer/timerFunctions.h"
#include "../../peripheral/gpio/digitalIO.h"
#include "../../pic-to-beaglebone/pic2beaglebone.h"

/*! Digital pin used for controlling red LED.
 */
extern digitalPin LED2R;

/*! Digital pin used for controlling green LED.
 */
extern digitalPin LED2G;

/*! Digital pin used for controlling blue LED.
 */
extern digitalPin LED2B;

/*! Digital pin used as SPI select pin (active low) for digital-to-analog converter (DAC), which is used for generating Peltier referent value.
 */
extern digitalPin PELTIER;

/*! Digital pin used for disabling/enabling (low/high) Peltier driver.
 */
extern digitalPin PELTIER_EN;

/*! Digital pin used for reseting I2C multiplexer (active low).
 */
extern digitalPin MUXRST; 

/*! Function sets MCU ports as digital or analog. Initializes digital pins and peripheral devices (I2C, timer).
 */
void setUpPorts();

#endif	/* INITIALIZEHARDWARE_H */

