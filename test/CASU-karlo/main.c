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

#include "../../actuators/led.h"
#include "../../actuators/peltier.h"
#include "../../actuators/vibration.h"
#include "../../sensors/vcnl4000/proxiVCNL4000.h"
#include "../../peripheral/i2c/i2c1.h"

// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF);
// Enable Clock Switching and Configure POSC in XT mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
//I2C alt pins
_FPOR(ALTI2C2_OFF & ALTI2C1_ON);
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
    //Initalization
    LEDInit();

    PeltierInit(PELTIER);

    VibrationInit();
    
    UINT8 ledUserR, ledUserB, ledUserG;
    ledUserR = 0; ledUserG = 0; ledUserB = 0;
    //LedBee(10, 50, 100);  //Test OK

    UINT8 vibSet = 0;
    int tmp = 0;

    setUpPorts();

    PeltierSet(PELTIER, 0, 1);

    I2C1MasterInit();
    int ProxiLight = 1;
    int proxi = 10;

    VibrationSet(100);

    //tmp = VCNL4000Init();
    proxi = 1;
    //tmp = MUXTest();

    while (1) {

        if (status == 0) {
            LedBee(0, 0, 0);
            LedUser(10, 0, 0);
            status++;
        }
        else if (status == 1) {
            LedUser(0, 10, 0);
            status++;
        }
        else if (status == 2) {
            LedUser(0, 0, 10);
            status++;
        }
        else if (status == 3) {
            LedUser(0, 0, 0);
            LedBee(50, 0, 0);
            status++;
        }
        else if (status == 4) {
            LedBee(0, 50, 0);
            status++;
        }
        else if (status == 5) {
            LedBee(0, 0, 50);
            status = 0;
        }
        //digitalHigh(LED2R);
        //PWM testing
        if (status == 6) {
            LedUser(ledUserR, ledUserG, ledUserB);
            ledUserB += 10;
            if(ledUserB > 100)
                ledUserB = 0;
        }

        if (status == 7) {
            if(ledUserG > 0) {
                ledUserG = 0;
                LedUser(ledUserR, ledUserG, ledUserB);
            }
            else {
                ledUserG = 1;
                LedUser(ledUserR, ledUserG, ledUserB);
            }

            VibrationSet(vibSet);
            vibSet += 10;
            if(vibSet > 100)
                vibSet = 0;

            //PeltierSet(PELTIER, 1, 0);
        }

        if(status == 8){
            proxi = 10;
            proxi = MUXTest();
            //proxi = VCNL4000Proxi();
            //I2C1WriteByte(VCNL4000_Address_WR, 1);
            //ProxiLight = VCNL4000Light();

        }

        //I2C MUX TESTING
        //tMux = 0;
        //tmp = (int) MUXTest();
        tmp = 1;

        tmp = MuxRead();

        proxi = 1;

        tmp = (int) VCNL4000Init();

        proxi = 1;
        //proxi = VCNL4000ReadProxi();
//        proxi = I2C1ReadByte(VCNL4000_Address, COMM0);
//        I2C1WriteByte(VCNL4000_Address, COMM0, proxi | 0x08);
//        proxi = 1;
//        proxi = I2C1ReadByte(VCNL4000_Address, COMM0);
        //proxi = I2C1ReadByte(VCNL4000_Address, PRODID);
        tmp  = 0;


        delay_t1(1000);

    }

    return (EXIT_SUCCESS);
}