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

#define IMAX     3   //[A] - maximum output current
#define I2C1ADD  0   //Peltier DAC I2C address


void PeltierSet(UINT8 set);

#endif	/* PELTIER_H */

