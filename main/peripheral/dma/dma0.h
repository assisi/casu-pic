/*! \file dma0.h
 * \brief Declaration of functions and variables used for controlling SPI1 bus.
 */

#ifndef DMA0_H
#define	DMA0_H

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
#include "../../sensors/adxl345/adxl345.h"
#include "../../actuators/pwm.h"
#define FFT_BUFF 256

extern int source_array[FFT_BUFF];
extern int mainLoopCount;

extern UINT16 dma0BufferA[512] __attribute__((space(dma)));
//extern unsigned int dma0BufferB[8] __attribute__((space(dma)));


/*! \brief Function initializes DMA0 channel.
 *
 *  @param ctrl_register control register DMA0CON; values explained in dma0.c
 *  @param req_register peripheral interrupt request (refer to dspic33e DMA manual)
 *	@param peripheral_address  static address of the peripheral data register
 *  @return Function returns 1 if initialization succeeded, else 0.
 */
//UINT8 dma0Init(UINT16 ctrl_register, UINT16 req_register, volatile unsigned int *peripheral_address);

UINT8 dma0Init(void);

void dma0Start();

void dma0Stop();

#endif	/* DMA0_H */
