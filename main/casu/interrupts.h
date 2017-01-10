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

#define ACC_TIMER_PRIORITY  6
#define DMA0_PRIORITY    6
#define BB_I2C_INT_PRIORITY 5
#define TIMER4_PRIORITY 4

#endif	/* INTERRUPTS_H */

