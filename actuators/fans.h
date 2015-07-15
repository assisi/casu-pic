/* 
 * File:   fans.h
 * Author: Karlo
 *
 * Created on 2015. sije?anj 22, 15:33
 */

#ifndef FANS_H
#define	FANS_H

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif

#include <Generic.h>
#include "../test/CASU-karlo/initializeHardware.h"



#endif	/* FANS_H */

