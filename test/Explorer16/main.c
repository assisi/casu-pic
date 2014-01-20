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
#include <p33Fxxxx.h>
#include "../../sensors/adxl345/adxl345.h"
#include "../../sensors/adt7320/adt7320.h"
#include "../../peripheral/uart/uart2.h"
#include "initializeHardware.h"
#include "../../peripheral/gpio/digitalIO.h"
#include "../../peripheral/timer/timerFunctions.h"
#include "../../peripheral/i2c/i2c1.h"
#include "../../pic-to-beaglebone/pic2beaglebone.h"

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
    // Fin = 8 Mhz, 8  * (40/2/2) = 80
    PLLFBD=38; // M=40
    CLKDIVbits.PLLPOST=0; // N2=2
    CLKDIVbits.PLLPRE=0; // N1=2
    // Initiate Clock Switch to Primary Oscillator with PLL (NOSC=0b011)
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(OSCCON | 0x01);

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC!= 0b011);
    // Wait for PLL to lock
    while (OSCCONbits.LOCK!= 1);
    
    char sbuff[200] = {0};
    char *sptr;
    int ax = 0, ay = 0, az = 0;
    float temp = 0;
    UINT8 status;
    UINT16 data;
    UINT16 i = 0;

    setUpPorts();
    uart2Init(9600, FOSC);
    delay_t1(100);
    sprintf(sbuff, "(Re)starting device \n\r", temp);
    uart2WriteString(sbuff);
         uart2WriteString(sbuff);
    status = adxl345Init(aSlave0);
    if (status == 0) {
        sprintf(sbuff, "SPI comm failed ");
        uart2WriteString(sbuff);
    }
    else if (status == -1) {
        sprintf(sbuff, "Device id failed ");
        uart2WriteString(sbuff);
    }
    else if (status == -2) {
        sprintf(sbuff, "Device powering failed ");
        uart2WriteString(sbuff);
    }
    delay_t1(100);

    for(i = 0; i < 10; i ++) {
        if (adxlReadReg(aSlave0, REG_POWER_CTL, &data) < 0) {
            sprintf(sbuff, "SPI comm failed ");
            uart2WriteString(sbuff);
        }
        sprintf(sbuff, "Power ctl reg = %x \n\r", data);
        uart2WriteString(sbuff);
    }

    status = adxl345Init(aSlave1);
    if (status == 0) {
        sprintf(sbuff, "SPI comm failed ");
        uart2WriteString(sbuff);
    }
    else if (status == -1) {
        sprintf(sbuff, "Device id failed ");
        uart2WriteString(sbuff);
    }
    else if (status == -2) {
        sprintf(sbuff, "Device powering failed ");
        uart2WriteString(sbuff);
    }
    delay_t1(100);

    status = adt7320Init(tSlave0, ADT_CONT_MODE |ADT_16_BIT);
    if (status == 0) {
        sprintf(sbuff, "ADT7320 1, SPI comm failed ");
        uart2WriteString(sbuff);
    }
    else if (status == -1) {
        sprintf(sbuff, "ADT7320 1, Device id failed ");
        uart2WriteString(sbuff);
    }
    else if (status == -2) {
        sprintf(sbuff, "ADT7320 1 , Device settings failed ");
        uart2WriteString(sbuff);
    }
    delay_t1(100);

    status = adt7320Init(tSlave1, ADT_CONT_MODE |ADT_16_BIT);
    if (status == 0) {
        sprintf(sbuff, "ADT7320 1, SPI comm failed ");
        uart2WriteString(sbuff);
    }
    else if (status == -1) {
        sprintf(sbuff, "ADT7320 1, Device id failed ");
        uart2WriteString(sbuff);
    }
    else if (status == -2) {
        sprintf(sbuff, "ADT7320 1 , Device settings failed ");
        uart2WriteString(sbuff);
    }
    delay_t1(100);

    // init i2c1 bus in a slave mode
    I2C1SlaveInit(0x0b, 1);     // address = 0x0b, interrupt priority = 1
    //i2c1_init();

    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD5 = 0;
    PORTDbits.RD3 = 1;
    PORTDbits.RD5 = 1;

    TRISFbits.TRISF3 = 0;
    PORTFbits.RF3 = 1;
    TRISAbits.TRISA1 = 0;
    digitalPin slave1;
    slave1.pinNum = 1;
    slave1.portReg = &PORTAbits;
    i = 0;
    digitalLow(slave1);
    //PORTAbits.RA1 = 1;
    while (1) {
        if (pinValue(slave1))
            digitalLow(slave1);
        else
            digitalHigh(slave1);
       
        if (readAccX(aSlave0, &ax) <= 0) {
               ax = -999;
        }
        if (readAccY(aSlave0, &ay) <= 0) {
              ay = -999;
        }
        if (readAccZ(aSlave0, &az) <= 0) {
              az = -999;
        }

        // it is assume the front acc is slave0
        vAmp_f = sqrtl((double)ax * ax + (double)ay * ay + (double)az * az);

        if (DEBUG_UART) {
            sprintf(sbuff, "acc0 ax ay az = %d %d %d\n\r ", ax, ay, az);
            uart2WriteString(sbuff);
        }


        if (readAccX(aSlave1, &ax) <= 0) {
              ax = -999;
        }
        if (readAccY(aSlave1, &ay) <= 0) {
              ay = -999;
        }
        if (readAccZ(aSlave1, &az) <= 0) {
             az = -999;
        }

        // it is assume the right acc is slave1
        vAmp_r = sqrtl((double)ax * ax + (double)ay * ay + (double)az * az);

        if (DEBUG_UART) {
            sprintf(sbuff, "acc1 ax ay az = %d %d %d\n\r ", ax, ay, az);
            uart2WriteString(sbuff);
        }

        if (adt7320ReadTemp(tSlave0, &temp) <= 0) temp = -999;
        temp_f = temp;

        if (DEBUG_UART) {
            sprintf(sbuff, "temp0 = %.3f\n\r", temp);
            uart2WriteString(sbuff);
        }

        if (adt7320ReadTemp(tSlave1, &temp) <= 0) temp = -999;
        temp_r = temp;
        
        if (DEBUG_UART) {
            sprintf(sbuff, "temp1 = %.3f\n\r", temp);
            uart2WriteString(sbuff);
        }

        updateReferences();
        sprintf(sbuff, "temp_r, vibe_r, r_r, g_r, b_r  = %.1f, %d, %d , %d, %d \n\r", temp_ref, vibeFreq_ref, pwmR_ref, pwmG_ref, pwmB_ref);
        uart2WriteString(sbuff);
        updateMeasurements();
        delay_t1(500);
    
    }

    return (EXIT_SUCCESS);
}