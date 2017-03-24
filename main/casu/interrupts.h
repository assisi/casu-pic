/*
 * File:   interrupts.h
 * Author: tomislav
 *
 * Created on January 9, 2017, 9:48 AM
 */

#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif

#include "../peripheral/spi/spi1.h"
#include "../peripheral/timer/timer3.h"
#include "../peripheral/timer/timer4.h"
#include "../peripheral/timer/timer5.h"
#include "../peripheral/dma/dma0.h"
#include "../peripheral/dma/dma1.h"

#define ACC_TIMER_PRIORITY  6
#define DMA0_PRIORITY    5
#define BB_I2C_INT_PRIORITY 5
#define TEMP_LOOP_PRIORITY 5
#define FFT_LOOP_PRIORITY 4

extern UINT8 timer4_flag;
extern UINT8 timer5_flag;
extern UINT16 dma_spi_tx_count;
extern UINT16 dma_spi_rx_count;
extern UINT8 dma_spi2_started;
extern UINT8 dma_spi2_done;

void __attribute__((__interrupt__)) _T3Interrupt(void);
void __attribute__((__interrupt__)) _T4Interrupt(void);
void __attribute__((__interrupt__)) _T5Interrupt(void);
void __attribute__((__interrupt__)) _DMA0Interrupt(void);
void __attribute__((__interrupt__)) _DMA1Interrupt(void);

#endif	/* INTERRUPTS_H */
