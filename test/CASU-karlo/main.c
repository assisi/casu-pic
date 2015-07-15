/* 
 * File:   main.c
 * Author: thaus
 *
 * Created on 2014. sije?anj 06, 13:03
 */
//05.02.14. - proximity sensors testing with I2C1 multiplexor -> test OK

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

    int proxiInit = 0;
    int status = 9;

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
    int proxi = 0;

    int proxiRes[7];
    int i;

    //VibrationSet(100);

    proxiInit = VCNL4000Init();
    proxi = 1;

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

//            VibrationSet(vibSet);
//            vibSet += 10;
//            if(vibSet > 100)
//                vibSet = 0;

            PeltierSet(PELTIER, 1, 1);
        }

        if(status == 8){
            //1. samo 1. proxi senzor je priklju?en - uspje?no ?itanje svih kanala
            for(i=0; i<7; i++){
                tmp = I2C1ChSelect(1, i);
                proxi = 0;
            }
        }

        if (status == 9) {
            //I2C MUX TESTING
            for(i=0; i<7; i++)
                proxiRes[i] = 0;
            
            //Front
            tmp = I2C1ChSelect(1, 1);
            proxiRes[0] = VCNL4000ReadProxi();
            proxi = 0;
            delay_t1(1);
            //Front right
            tmp = I2C1ChSelect(1, 0);
            proxiRes[1] = VCNL4000ReadProxi();
            proxi = 0;
            delay_t1(1);
            //Back right
            tmp = I2C1ChSelect(1, 7);
            proxiRes[2] = VCNL4000ReadProxi();
            proxi = 0;
            delay_t1(1);
            //Back
            tmp = I2C1ChSelect(1, 6);
            proxiRes[3] = VCNL4000ReadProxi();
            proxi = 0;
            delay_t1(1);
            //Back left
            proxi = I2C1ChSelect(1, 5);
            proxiRes[4] = VCNL4000ReadProxi();
            proxi = 0;
            delay_t1(1);
            //Front left
            tmp = I2C1ChSelect(1, 2);
            proxiRes[5] = VCNL4000ReadProxi();
            proxi = 0;
            delay_t1(1);
            //Top
            tmp = I2C1ChSelect(1, 4);
            proxiRes[6] = VCNL4000ReadProxi();
            proxi = 0;

            //Sensori - T -> ne radi
            //FL ->
        }
        if(status == 10){
            proxi = VCNL4000ReadProxi();
        }



        delay_t1(1000);
    }

    return (EXIT_SUCCESS);
}