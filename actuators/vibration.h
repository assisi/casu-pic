/* 
 * File:   vibration.h
 * Author: Karlo
 *
 * Created on 2014. sije?anj 24, 18:07
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

void VibrationInit(void);

void VibrationSet(UINT8 set);

#endif	/* VIBRATION_H */

