/* 
 * File:   peltier.h
 * Author: Karlo
 *
 * Created on 2014. January 20, 11:06
 * Peltier firmware
 */

#ifndef PELTIER_H
#define	PELTIER_H

#include <Generic.h>
#include "../test/CASU-karlo/initializeHardware.h"

#define IMAX     3          //[A] - maximum output current
#define DAC_G    0          //DAC output gain 0 - 2x, 1 - 1x
#define DMAX     IMAX*250+1500            //DAC maximum voltage for maximum current, Itec=(Vout-1.5)*4
#define DOFFSET 1500                      //Peltier zero current
#define DCOEF   (DMAX-DOFFSET)/100        //Output multiplification factor

void PeltierInit(digitalPin csPin);

void PeltierSet(digitalPin csPin, UINT8 lShdn, int set);

#endif	/* PELTIER_H */

