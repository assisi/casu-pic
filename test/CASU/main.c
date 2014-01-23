/* 
 * File:   main.c
 * Author: thaus
 *
 * Created on 2014. sije?anj 06, 13:03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <p33Exxxx.h>
#include "initializeHardware.h"
#include "../../peripheral/gpio/digitalIO.h"
#include "../../peripheral/timer/timerFunctions.h"

// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF);
// Enable Clock Switching and Configure POSC in XT mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);

//UINT16 i2cIntNum;

/*
 * 
 */
int main(int argc, char** argv) {

    /*Configuring POSC with PLL, with goal FOSC = 80 MHZ */
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    // Fin = 16 Mhz, 16  * (20/2/2) = 80
    PLLFBD=18; // M=40
    CLKDIVbits.PLLPOST=0; // N2=2
    CLKDIVbits.PLLPRE=0; // N1=2
    // Initiate Clock Switch to Primary Oscillator with PLL (NOSC=0b011)
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(OSCCON | 0x01);

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC!= 0b011);
    // Wait for PLL to lock
    while (OSCCONbits.LOCK!= 1);

    int status = 0;
    setUpPorts();
    while (1) {

        if (status == 0) {
            digitalHigh(LED2R);
            digitalLow(LED2G);
            digitalLow(LED2B);
            status++;
        }
        else if (status == 1) {
            digitalLow(LED2R);
            digitalHigh(LED2G);
            digitalLow(LED2B);
            status++;
        }
        else if (status == 2) {
            digitalLow(LED2R);
            digitalLow(LED2G);
            digitalHigh(LED2B);
            status = 0;
        }

        delay_t1(1000);
    }

    return (EXIT_SUCCESS);
}