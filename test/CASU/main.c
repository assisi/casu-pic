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
#include "../../pic-to-beaglebone/pic2beaglebone.h"
#include "../../sensors/adt7320/adt7320.h"
#include "../../sensors/adxl345/adxl345.h"

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
    //__builtin_write_OSCCONH(0x03);
    
    // tune FRC
    OSCTUN = 23;  // 23 * 0.375 = 8.625 % -> 7.37 Mhz * 1.08625 = 8.005Mhz
    // Initiate Clock Switch to external oscillator NOSC=0b011 (alternative use FRC with PLL (NOSC=0b01)
    __builtin_write_OSCCONH(0b011);
    __builtin_write_OSCCONL(OSCCON | 0x01);

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC!= 0b011);
    // Wait for PLL to lock
    while (OSCCONbits.LOCK!= 1);

    // local variables in main function
    int status = 0;
    UINT8 error = 0;
    int ax = 0, ay = 0, az = 0;
    float temp = 0;

    setUpPorts();

    delay_t1(500);
    digitalHigh(LED2R);
    while(1) {
        
        updateReferences();

//        if (diagLED_r[0] > 0)
//            digitalHigh(LED2R);
//        else
//            digitalLow(LED2R);

        if (diagLED_r[1] > 0)
            digitalHigh(LED2G);
        else
            digitalLow(LED2G);

        if (diagLED_r[2] > 0)
            digitalHigh(LED2B);
        else
            digitalLow(LED2B);

        updateMeasurements();
        delay_t1(20);

    }

//    status = adxl345Init(aSlaveF);
//    if (status == 0) {
//        /*
//         * Send error message "ACC_F, SPI comm failes"
//         */
//        error = 1;
//        digitalHigh(LED2R);
//    }
//    else if (status == -1) {
//        /*
//         * Send error message "ACC_F, Device id failed "
//         */
//        error = 1;
//        digitalHigh(LED2G);
//    }
//    else if (status == -2) {
//        /*
//         * Send error message "ACC_F, Device powering failed"
//         */
//        error = 1;
//        digitalHigh(LED2B);
//    }
//    delay_t1(100);
//
//    status = adxl345Init(aSlaveR);
//    if (status == 0) {
//        /*
//         * Send error message "ACC_F, SPI comm failes"
//         */
//        error = 1;
//        digitalHigh(LED2R);
//    }
//    else if (status == -1) {
//        /*
//         * Send error message "ACC_F, Device id failed "
//         */
//        error = 1;
//        digitalHigh(LED2G);
//    }
//    else if (status == -2) {
//        /*
//         * Send error message "ACC_F, Device powering failed"
//         */
//        error = 1;
//        digitalHigh(LED2B);
//    }
//    delay_t1(100);
//
//    status = adxl345Init(aSlaveB);
//    if (status == 0) {
//        /*
//         * Send error message "ACC_F, SPI comm failes"
//         */
//        error = 1;
//        digitalHigh(LED2R);
//    }
//    else if (status == -1) {
//        /*
//         * Send error message "ACC_F, Device id failed "
//         */
//        error = 1;
//        digitalHigh(LED2G);
//    }
//    else if (status == -2) {
//        /*
//         * Send error message "ACC_F, Device powering failed"
//         */
//        error = 1;
//        digitalHigh(LED2B);
//    }
//    delay_t1(100);
//
//    status = adxl345Init(aSlaveL);
//    if (status == 0) {
//        /*
//         * Send error message "ACC_F, SPI comm failes"
//         */
//        error = 1;
//        digitalHigh(LED2R);
//    }
//    else if (status == -1) {
//        /*
//         * Send error message "ACC_F, Device id failed "
//         */
//        error = 1;
//        digitalHigh(LED2G);
//    }
//    else if (status == -2) {
//        /*
//         * Send error message "ACC_F, Device powering failed"
//         */
//        error = 1;
//        digitalHigh(LED2B);
//    }
//    delay_t1(100);
//
//    status = adt7320Init(tSlaveF, ADT_CONT_MODE |ADT_16_BIT);
//    if (status == 0) {
//        /*
//         * Send error message "Temp_F, SPI comm failed"
//         */
//        error = 1;
//        digitalHigh(LED2R);
//    }
//    else if (status == -1) {
//        /*
//         * Send error message "Temp_F, Device ID failed"
//         */
//        error = 1;
//        digitalHigh(LED2G);
//    }
//    else if (status == -2) {
//        /*
//         * Send error message "Temp_F, Device settings failed"
//         */
//        error = 1;
//        digitalHigh(LED2B);
//    }
//    delay_t1(100);
//
//    status = adt7320Init(tSlaveR, ADT_CONT_MODE |ADT_16_BIT);
//    if (status == 0) {
//        /*
//         * Send error message "Temp_F, SPI comm failed"
//         */
//        error = 1;
//        digitalHigh(LED2R);
//    }
//    else if (status == -1) {
//        /*
//         * Send error message "Temp_F, Device ID failed"
//         */
//        error = 1;
//        digitalHigh(LED2G);
//    }
//    else if (status == -2) {
//        /*
//         * Send error message "Temp_F, Device settings failed"
//         */
//        error = 1;
//        digitalHigh(LED2B);
//    }
//    delay_t1(100);
//
//    status = adt7320Init(tSlaveB, ADT_CONT_MODE |ADT_16_BIT);
//    if (status == 0) {
//        /*
//         * Send error message "Temp_F, SPI comm failed"
//         */
//        error = 1;
//        digitalHigh(LED2R);
//    }
//    else if (status == -1) {
//        /*
//         * Send error message "Temp_F, Device ID failed"
//         */
//        error = 1;
//        digitalHigh(LED2G);
//    }
//    else if (status == -2) {
//        /*
//         * Send error message "Temp_F, Device settings failed"
//         */
//        error = 1;
//        digitalHigh(LED2B);
//    }
//    delay_t1(100);
//
//    status = adt7320Init(tSlaveL, ADT_CONT_MODE |ADT_16_BIT);
//    if (status == 0) {
//        /*
//         * Send error message "Temp_F, SPI comm failed"
//         */
//        error = 1;
//        digitalHigh(LED2R);
//    }
//    else if (status == -1) {
//        /*
//         * Send error message "Temp_F, Device ID failed"
//         */
//        error = 1;
//        digitalHigh(LED2G);
//    }
//    else if (status == -2) {
//        /*
//         * Send error message "Temp_F, Device settings failed"
//         */
//        error = 1;
//        digitalHigh(LED2B);
//    }
//    delay_t1(100);
//
//    status = 0;
//
//    while (1) {
//
//        updateReferences();
//        if (error == 0) {
//            if (status == 0) {
//                digitalHigh(LED2R);
//                digitalLow(LED2G);
//                digitalLow(LED2B);
//                status++;
//            }
//            else if (status == 1) {
//                digitalLow(LED2R);
//                digitalHigh(LED2G);
//                digitalLow(LED2B);
//                status++;
//            }
//            else if (status == 2) {
//                digitalLow(LED2R);
//                digitalLow(LED2G);
//                digitalHigh(LED2B);
//                status = 0;
//            }
//        }
//
//        if (readAccX(aSlaveF, &ax) <= 0) {
//               ax = 1;
//               error = 1;
//               digitalHigh(LED2R);
//        }
//        if (readAccY(aSlaveF, &ay) <= 0) {
//              ay = 0;
//              error = 1;
//              digitalHigh(LED2R);
//        }
//        if (readAccZ(aSlaveF, &az) <= 0) {
//              az = 0;
//              error = 1;
//              digitalHigh(LED2R);
//        }
//
//        vAmp_f = sqrtl((double)ax * ax + (double)ay * ay + (double)az * az);
//
//        if (readAccX(aSlaveR, &ax) <= 0) {
//               ax = 1;
//               error = 1;
//               digitalHigh(LED2R);
//        }
//        if (readAccY(aSlaveR, &ay) <= 0) {
//              ay = 0;
//              error = 1;
//              digitalHigh(LED2R);
//        }
//        if (readAccZ(aSlaveR, &az) <= 0) {
//              az = 0;
//              error = 1;
//              digitalHigh(LED2R);
//        }
//
//        vAmp_r = sqrtl((double)ax * ax + (double)ay * ay + (double)az * az);
//
//        if (readAccX(aSlaveB, &ax) <= 0) {
//               ax = 1;
//               error = 1;
//               digitalHigh(LED2R);
//        }
//        if (readAccY(aSlaveB, &ay) <= 0) {
//              ay = 0;
//              error = 1;
//              digitalHigh(LED2R);
//        }
//        if (readAccZ(aSlaveB, &az) <= 0) {
//              az = 0;
//              error = 1;
//              digitalHigh(LED2R);
//        }
//
//        vAmp_b = sqrtl((double)ax * ax + (double)ay * ay + (double)az * az);
//
//         if (readAccX(aSlaveL, &ax) <= 0) {
//               ax = 1;
//               error = 1;
//               digitalHigh(LED2R);
//        }
//        if (readAccY(aSlaveL, &ay) <= 0) {
//              ay = 0;
//              error = 1;
//              digitalHigh(LED2R);
//        }
//        if (readAccZ(aSlaveL, &az) <= 0) {
//              az = 0;
//              error = 1;
//              digitalHigh(LED2R);
//        }
//
//        vAmp_l = sqrtl((double)ax * ax + (double)ay * ay + (double)az * az);
//
//        if (adt7320ReadTemp(tSlaveF, &temp) <= 0) {
//            temp = 1;
//            error = 1;
//            digitalHigh(LED2G);
//        }
//        temp_f = temp;
//
//        if (adt7320ReadTemp(tSlaveR, &temp) <= 0) {
//            temp = 1;
//            error = 1;
//            digitalHigh(LED2G);
//        }
//        temp_r = temp;
//
//        if (adt7320ReadTemp(tSlaveB, &temp) <= 0) {
//            temp = 1;
//            error = 1;
//            digitalHigh(LED2G);
//        }
//        temp_b = temp;
//
//        if (adt7320ReadTemp(tSlaveL, &temp) <= 0) {
//            temp = 1;
//            error = 1;
//            digitalHigh(LED2G);
//        }
//        temp_l = temp;
//
//        delay_t1(1000);
//        updateMeasurements();
//    }

    return (EXIT_SUCCESS);
}