/* 
 * File:   main.c
 * Author: thaus
 *
 * Created on 2014. sije?anj 06, 13:03
 */
/* REVISION HISTORY:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
 * Date, Author     |	Comments on this revision
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
 * 06.02.2014       |	3 Proximity sensors reading - Laussane training
 * 17.04.2014, KG   |   AMP board testing sequance is added -> #define TEST_AMP_BOARD
 */

//#define TEST_AMP_BOARD

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
#include "../../sensors/adt7420/adt7420.h"
#include "../../sensors/adxl345/adxl345.h"
#include "../../peripheral/i2c/i2c1.h"
#include "../../sensors/vcnl4000/proxiVCNL4000.h"

// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF);
// Enable Clock Switching and Configure POSC in XT mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
//I2C alt pins
_FPOR(ALTI2C2_OFF & ALTI2C1_ON);
//Watchdog timer -> Twtd = PR*POST/32000 [s]
_FWDT(FWDTEN_ON & WDTPRE_PR128 & WDTPOST_PS1024);   //Twdt ~ 4s

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
    int i = 0;
    UINT8 error = 0;
    int ax = 0, ay = 0, az = 0;
    float temp = 0;
    float temp_ls;
    int ntemp_l = 0;
    int tmpPeltier, tmpVibra, tmpTsens;
    int statusProxi[8];
    int tempLoopControl = 0;
    UINT8 peltierlShdn = 0;     //Peltier shut down pin - active low

    setUpPorts();

    delay_t1(500);
    digitalHigh(LED2R);

    status = adxl345Init(aSlaveR);
    // The ports are already set so we can not turn LEDs with digital low /high 
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
    delay_t1(5);

    status = adxl345Init(aSlaveL);
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
    delay_t1(5);

    status = adxl345Init(aSlaveF);
    delay_t1(5);

    status = adt7320Init(tSlaveR, ADT_CONT_MODE |ADT_16_BIT);
    if (status == 0) {
        /*
         * Send error message "Temp_F, SPI comm failed"
         */
        error = 1;
        digitalHigh(LED2R);
    }
    else if (status == -1) {
        /*
         * Send error message "Temp_F, Device ID failed"
         */
        error = 1;
        digitalHigh(LED2G);
    }
    else if (status == -2) {
        /*
         * Send error message "Temp_F, Device settings failed"
         */
        error = 1;
        digitalHigh(LED2B);
    }

    delay_t1(5);

    status = adt7320Init(tSlaveL, ADT_CONT_MODE |ADT_16_BIT);
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
    delay_t1(5);

    status = adt7320Init(tSlaveF, ADT_CONT_MODE |ADT_16_BIT);

    //Proximirty sensors initalization
    I2C1MasterInit();
    status = VCNL4000Init();  

    LEDInit();
    delay_t1(10);

    VibrationInit();
    delay_t1(10);
    
    PeltierInit(PELTIER);
    //PeltierSet(PELTIER, 1, 100);
    tmpPeltier = 60; tmpVibra = 0;

    //ADT7420 sensors initalization
    I2C1ChSelect(1,6);  //Top board
    delay_t1(1);
    status = adt7420Init(0);

    temp_ls = temp_l;
    
    while(1) {

        updateReferences();
        
        LedBee(ctlLED_r[0], ctlLED_r[1], ctlLED_r[2]);
        LedUser(diagLED_r[0], diagLED_r[1],diagLED_r[2]);
        VibrationSet(pwmMotor);     // this is actually motor pwm

        if (readAccX(aSlaveR, &ax) <= 0) {
            ax = 0;
        }
        if (readAccY(aSlaveR, &ay) <= 0) {
              ay = 0;
        }
        if (readAccZ(aSlaveR, &az) <= 0) {
              az = 0;
        }

        vAmp_r = sqrtl((double)ax * ax + (double)ay * ay + (double)az * az);
        delay_t1(1);

        if (readAccX(aSlaveL, &ax) <= 0) {
            ax = 0;
        }
        if (readAccY(aSlaveL, &ay) <= 0) {
              ay = 0;
        }
        if (readAccZ(aSlaveL, &az) <= 0) {
              az = 0;
        }
        vAmp_l = sqrtl((double)ax * ax + (double)ay * ay + (double)az * az);

        if (1) { //CASU 4
            //Front
            status = I2C1ChSelect(1, 3);
            proxy_f = VCNL4000ReadProxi();
            //Front right
            status = I2C1ChSelect(1, 4);
            proxy_fr = VCNL4000ReadProxi();
            //delay_t1(1);
            //Back right
            status = I2C1ChSelect(1, 5);
            proxy_br = VCNL4000ReadProxi();
            //delay_t1(1);
            //Back
            status = I2C1ChSelect(1, 0);
            proxy_b = VCNL4000ReadProxi();
            //delay_t1(1);
            //Back left
            status = I2C1ChSelect(1, 1);
            proxy_bl = VCNL4000ReadProxi();
            //delay_t1(1);
            //Front left
            status = I2C1ChSelect(1, 2);
            proxy_fl = VCNL4000ReadProxi();
            //delay_t1(1);
        }
        if (0) { //CASU 3
            //Front
            status = I2C1ChSelect(1, 0);
            proxy_f = VCNL4000ReadProxi();
            //Front right
            status = I2C1ChSelect(1, 4);
            proxy_fr = VCNL4000ReadProxi();
            //Back right
            status = I2C1ChSelect(1, 2);
            proxy_br = VCNL4000ReadProxi();
            //Back
            status = I2C1ChSelect(1, 3);
            proxy_b = VCNL4000ReadProxi();
            //Back left
            status = I2C1ChSelect(1, 1);
            proxy_bl = VCNL4000ReadProxi();
            //Front left
            status = I2C1ChSelect(1, 5);
            proxy_fl = VCNL4000ReadProxi();
        }
        if (0) { //CASU 2
            //Front
            status = I2C1ChSelect(1, 0);
            proxy_f = VCNL4000ReadProxi();
            //Front right
            status = I2C1ChSelect(1, 1);
            proxy_fr = VCNL4000ReadProxi();
            //Back right
            status = I2C1ChSelect(1, 5);
            proxy_br = VCNL4000ReadProxi();
            //Back
            status = I2C1ChSelect(1, 3);
            proxy_b = VCNL4000ReadProxi();
            //Back left
            status = I2C1ChSelect(1, 4);
            proxy_bl = VCNL4000ReadProxi();
            //Front left
            status = I2C1ChSelect(1, 2);
            proxy_fl = VCNL4000ReadProxi();
        }
        if (0) { //CASU1
            //Front
            status = I2C1ChSelect(1, 0);
            proxy_f = VCNL4000ReadProxi();
            //Front right
            status = I2C1ChSelect(1, 1);
            proxy_fr = VCNL4000ReadProxi();
            //delay_t1(1);
            //Back right
            status = I2C1ChSelect(1, 2);
            proxy_br = VCNL4000ReadProxi();
            //delay_t1(1);
            //Back
            status = I2C1ChSelect(1, 3);
            proxy_b = VCNL4000ReadProxi();
            //delay_t1(1);
            //Back left
            status = I2C1ChSelect(1, 4);
            proxy_bl = VCNL4000ReadProxi();
            //delay_t1(1);
            //Front left
            status = I2C1ChSelect(1, 5);
            proxy_fl = VCNL4000ReadProxi();
            //delay_t1(1);
        }
        //Top
        status = I2C1ChSelect(1, 6);
        proxy_t = VCNL4000ReadProxi();

        if((proxy_f == -1) && (proxy_fl == -1) && (proxy_bl == -1) && (proxy_b == -1) && (proxy_br == -1) && (proxy_fr == -1))
            muxReset();

        //Temperature readings and control
        if (tempLoopControl >= 3) {

            //Bridge sensors position (Graz workshop)
            //Front
            if(0){
                adt7320ReadTemp(tSlaveR, &temp_f);
            }
            else{
                adt7320ReadTemp(tSlaveR, &temp_b);
            }
            adt7320ReadTemp(tSlaveL, &temp_l);
            adt7320ReadTemp(tSlaveF, &temp_r);


            //Ring temperature filtering
            if(abs(temp_l - temp_ls) >= 1){
                ntemp_l++;
                if(ntemp_l > 5){
                    ntemp_l = 0;
                }
                else{
                    temp_l = temp_ls;
                }
            }

            temp_ls = temp_l;

            status = I2C1ChSelect(1, 6);
            adt7420ReadTemp(&temp_t);   //Top temperature sensor

            if(temp_ref <= 20){     // temp_ref < 20 , turn off
                ctlPeltier = 0;
                peltierlShdn = 0;
            }
            else{
                if (temp_ref - temp_ref_cur > 0.2)
                    temp_ref_cur += 0.2;
                else if (temp_ref - temp_ref_cur < -0.2)
                    temp_ref_cur -= 0.2;
                else
                    temp_ref_cur = temp_ref;
                
                ctlPeltier = PeltierPID(temp_ref_cur, temp_l);
                peltierlShdn = 1;
            }
            
            if (temp_l>55){
                peltierlShdn = 0;
                ctlPeltier = 0;
            }

            if(ctlPeltier > 0)
                ctlPeltier *= 0.6;
            else
                ctlPeltier *= 0.2;
            PeltierSet(PELTIER, peltierlShdn, ctlPeltier);  //Set peltier pwm
            //PeltierSet(PELTIER, 1, 0);

            tempLoopControl = 0;
        }
        else {
            tempLoopControl++;
        }

        updateMeasurements();
        ay = 0;
        delay_t1(200);
        
        ClrWdt();   //Clear watchdog timer
    }

    return (EXIT_SUCCESS);
}
