/*! \file timerFunctions.h
 * \brief Declaration of functions for working with timers.
 */

#ifndef TIMERFUNCTIONS_H
#define	TIMERFUNCTIONS_H

#include "timer1.h"
#include "../../casu/initializeHardware.h"


/*!
 * Maximum time in microseconds (us) that can be counted with timer1 (16 bit)
 */
#define DELAY_T1_US_MAX     10000   //Maximum

/*!\brief Function delays program execution for a time given in milliseconds.
 *
 * @param msec Time in milliseconds.
 */
void delay_t1(int msec);

/*!\brief Function calculates CPU ticks to count with timer which correspond to a given time in milliseconds and timer prescaler.
 *
 * @param msec Time in milliseconds.
 * @param prescaler Timer prescaler. Use predefined timer prescale values: T1_PS_1_x, x = 1,8,64,256
 * @return Number of CPU ticks to be counted with timer.
 */
unsigned long ticks_from_ms(int msec, int prescaler);

unsigned long ticks_from_us(float usec, int prescaler);

float ms_from_ticks(UINT16 ticks, int prescaler);

unsigned long ticks_from_s(float sec, int prescaler);

#endif	/* TIMERFUNCTIONS_H */

