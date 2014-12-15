/*! \file initializeHardware.h
 *  \brief Declarations of functions used for initializing devices (MCU ports, MCU peripheral devices)
 */

#ifndef INITIALIZEHARDWARE_H
#define	INITIALIZEHARDWARE_H

// 
/*! Desired frequency Fosc.
 * Note that CPU frequency Fp = Fcy = Fosc /2
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

/*! Digital pin red LED is connected to.
 */
extern digitalPin LED2R;

/*! Digital pin green LED is connected to.
 */
extern digitalPin LED2G;

/*! Digital pin blue LED is connected to.
 */
extern digitalPin LED2B;

/*! Digital pin used as SPI select pin (active low) for digital-to-analog converter (DAC) used for generating Peltier referent value.
 */
extern digitalPin PELTIER;   //SPI select

/*! Digital pin used for disabling(low) / enabling (high) Peltier driver.
 */
extern digitalPin PELTIER_EN;

/*! Digital pin used for reseting I2C multiplexer (active digital low)
 */
extern digitalPin MUXRST;   //I2C mux

/*! Function sets MCU ports as digital or analog. Initializes digital pins and peripheral devices (I2c, timer).
 */
void setUpPorts();

#endif	/* INITIALIZEHARDWARE_H */

