/* 
 * File:   timerFunctions.h
 * Author: thaus
 *
 * Created on 2014. sije?anj 16, 22:32
 */

#ifndef TIMERFUNCTIONS_H
#define	TIMERFUNCTIONS_H

#include "timer1.h"
#include "../../test/CASU/initializeHardware.h"

//Constants for delay_t1_us(usec) functions
#define DELAY_T1_US_MAX     10000   //Maximum

void delay_t1(int msec);
long ticks_from_ms(int msec, int prescaler);

#endif	/* TIMERFUNCTIONS_H */

