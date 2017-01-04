/*! \file dma1.h
 * \brief Declaration of functions and variables used for controlling SPI1 bus.
 */

#ifndef DMA1_H
#define	DMA1_H

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif
 
#include <dsp.h>
#include <math.h>

#include <Generic.h>
#include "../gpio/digitalIO.h"
#include "../../actuators/pwm.h"
#include "../../sensors/adxl345/adxl345.h"
#include "../../pic-to-beaglebone/pic2beaglebone.h" 

extern int mainLoopCount;
extern UINT16 dma1BufferA[2] __attribute__((space(dma)));

/*! \brief Function initializes DMA1 channel.
 *
 *  @param ctrl_register control register DMA0CON; values explained in dma1.c
 *  @param req_register peripheral interrupt request (refer to dspic33e DMA manual)
 *	@param peripheral_address  static address of the peripheral data register
 *  @return Function returns 1 if initialization succeeded, else 0.
 */
//UINT8 dma1Init(UINT16 ctrl_register, UINT16 req_register, volatile unsigned int *peripheral_address);

UINT8 dma1Init(void);

UINT8 dma1Start(void);

#endif	/* DMA0_H */

