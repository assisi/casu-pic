/* 
 * File:   led.h
 * Author: Karlo
 *
 * Created on 2014. sije?anj 23, 15:58
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


#define PWMPRE 4   //PWM input clock prescaler
#define FPWM   20000 //PWM frequecy

void LEDInit(void);

void LedUser(UINT8 red, UINT8 green, UINT8 blue);

void LedBee(UINT8 red, UINT8 green, UINT8 blue);
#endif	/* LED_H */

