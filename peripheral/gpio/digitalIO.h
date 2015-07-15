/* 
 * File:   digitalIO.h
 * Author: thaus
 *
 * Created on 2014. sije?anj 16, 17:24
 */

#ifndef DIGITALIO_H
#define	DIGITALIO_H

#include <Generic.h>

typedef struct Pin {
    int *trisReg;
    int *portReg;
    UINT8 pinNum;
}digitalPin;

void digitalInput(digitalPin pin);
void digitalOutput(digitalPin pin);
void digitalHigh(digitalPin pin);
void digitalLow(digitalPin pin);
UINT8 pinValue(digitalPin pin);
#endif	/* DIIGTALIO_H */

