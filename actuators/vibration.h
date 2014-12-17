/*! \file vibration.h
 * Declarations of functions used for controlling vibration motor.
 */


#ifndef VIBRATION_H
#define	VIBRATION_H

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif

#include <Generic.h>

/*! \brief Function inititalizes PWM module used for controlling vibration motor
 */
void VibrationInit(void);

/*! \brief Function sets PWM referent value for vibration motor.
 *
 * @param set PWM referent value, range [0-100].
 */
void VibrationSet(UINT8 set);

#endif	/* VIBRATION_H */

