/*! \file led.h
 * \brief Declarations of functions and variables used for controlling Light Emitting Diodes (LEDs).
 */

#ifndef LED_H
#define	LED_H

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif

#include <Generic.h>
#include "../test/CASU-karlo/initializeHardware.h"

/*! PWM input clock prescaler.
 */
#define PWMPRE 4   
/*! PWM frequency in Hz.
 */
#define FPWM 20000

/*! \brief Function initializes PWM modules used for controlling LEDs.
 */
void LEDInit(void);

/*!\brief Function sets PWM values of red, green and blue LED used for diagnostic purposes (User LED).
 *
 * @param red PWM value of red LED in range 0-100 [%].
 * @param green PWM value of green LED in range 0-100 [%].
 * @param blue PWM value of blue LED in range 0-100 [%].
 */
void LedUser(UINT8 red, UINT8 green, UINT8 blue);

/*!\brief Function sets PWM values of red, green and blue LED used as bee stimulus (Bee LED).
 *
 * @param red PWM value of red LED in range 0-100 [%].
 * @param green PWM value of green LED in range 0-100 [%].
 * @param blue PWM value of blue LED in range 0-100 [%].
 */
void LedBee(UINT8 red, UINT8 green, UINT8 blue);
#endif	/* LED_H */

