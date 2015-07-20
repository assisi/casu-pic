
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
 * 17.02.2015, KG   | CASU V1.0
 */

//#define TEST_AMP_BOARD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <p33Exxxx.h>
#include "initializeHardware.h"
#include "../peripheral/gpio/digitalIO.h"
#include "../peripheral/timer/timerFunctions.h"
#include "../pic-to-beaglebone/pic2beaglebone.h"
#include "../sensors/adt7320/adt7320.h"
#include "../sensors/adt7420/adt7420.h"
#include "../sensors/adxl345/adxl345.h"
#include "../peripheral/i2c/i2c1.h"
#include "../sensors/vcnl4000/proxiVCNL4000.h"
#include "../actuators/pwm.h"
#include "../actuators/peltier.h"
#include "../peripheral/timer/timer2.h"

// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF);
// Enable Clock Switching and Configure POSC in XT mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
//I2C alt pins
_FPOR(ALTI2C2_OFF & ALTI2C1_ON);
//Watchdog timer -> Twtd = PR*POST/32000 [s]
_FWDT(FWDTEN_ON & WDTPRE_PR128 & WDTPOST_PS1024);   //Twdt ~ 4s

#define PI 3.14159265359
int dt = 0;
float vibe_period = 5000.0; // in usec
float dt_f = 0;
float t_f = 0;
int N = 100;
int pwm_i[100];


/*
 *
 */
int main(int argc, char** argv) {

    /*Configuring POSC with PLL, with goal FOSC = 80 MHZ */
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    // Fin = 8 Mhz, 8  * (20/2/2) = 40
    PLLFBD=18; // M=20
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
    float temp = 0, temp_ls;
    float tempBridge[4];    //Bridge temperature copy
    int statusProxi[8];
    int statusTemp[4];      //Temperature sensors initialization status
    int tempLoopControl = 0;
    int tempNum;            //Sensors number
    int tempLoop = 0;
    int mainLoopCounter = 0;
    int tempSensors = 0;

    setUpPorts();
    digitalHigh(LED2R);
     // configure i2c2 as a slave device with address 0x0b and interrupt priority 1
    I2C2SlaveInit(I2C2_CASU_ADD, 1);
    delay_t1(500);
    digitalLow(LED2R);
    /*
    status = adxl345Init(aSlaveF);
    //error = ErrorInitCheck(status);
    delay_t1(5);
    status = adxl345Init(aSlaveR);
    //error = ErrorInitCheck(status);
    delay_t1(5);
    status = adxl345Init(aSlaveL);
    //error = ErrorInitCheck(status);
    delay_t1(5);
    status = adxl345Init(aSlaveB);
    //error = ErrorInitCheck(status);

    delay_t1(5);
    */
    PWMInit();
    LedUser(0, 0, 0);
    PeltierSetPwm2(50); // Complementary mode

    /*
    statusTemp[0] = adt7320Init(tSlaveF, ADT_CONT_MODE |ADT_16_BIT);
    delay_t1(5);
    statusTemp[1] = adt7320Init(tSlaveR, ADT_CONT_MODE |ADT_16_BIT);
    //error = ErrorInitCheck(status);
    delay_t1(5);
    statusTemp[2] = adt7320Init(tSlaveB, ADT_CONT_MODE |ADT_16_BIT);
    delay_t1(5);
    statusTemp[3] = adt7320Init(tSlaveL, ADT_CONT_MODE |ADT_16_BIT);
    //error = ErrorInitCheck(status);
    ax = 0;

    //error = ErrorInitCheck(status);

    //error = ErrorInitCheck(status);
    delay_t1(5);

    //Proximirty sensors initalization
    I2C1MasterInit();
    status = VCNL4000Init();
    ax =0 ;
    //PWM intialization
    ax = 0;
    //ADT7420 sensors initalization
    //status = adt7420Init(0);
    */

    //Estimation initializaction

    /*
    for (i = 0; i < 50; i++) {
        adt7320ReadTemp(tSlaveF, &temp_f); delay_t1(5);
        adt7320ReadTemp(tSlaveL, &temp_l); delay_t1(5);
        adt7320ReadTemp(tSlaveB, &temp_b); delay_t1(5);
        adt7320ReadTemp(tSlaveR, &temp_r); delay_t1(5);
    }

    tempBridge[0] = temp_f;
    tempBridge[1] = temp_r;
    tempBridge[2] = temp_b;
    tempBridge[3] = temp_l;

    if (statusTemp[0] != 1)
        temp_f = -1;
    if (statusTemp[1] != 1)
        temp_r = -1;
    if (statusTemp[2] != 1)
        temp_b = -1;
    if (statusTemp[3] != 1)
        temp_l = -1;

    vAmp_f = 0;
    vAmp_r = 0;
    vAmp_b = 0;
    vAmp_l = 0;
    fAmp_f = 0;
    fAmp_r = 0;
    fAmp_b = 0;
    fAmp_l = 0;

    //CASU ring average temperature
    temp_casu = 0;
    tempNum = 0;
//    while (tempNum == 0) {
//        for (i = 0; i < 4; i++) {
//            if (statusTemp[i] == 1 && tempBridge[i] > 20 && tempBridge[i] < 60) {
//                tempNum++;
//                temp_casu += tempBridge[i];
//            }
//        }
//
//        if (tempNum > 0)
//            break;
//        watchCounter++;
//        // if after 100 round we still don't have temp measurements - break
//        if (watchCounter == 100) {
//            for (i = 0; i < 4; i++) {
//                statusTemp[i] = -1;
//            }
//            break;
//        }
//        delay_t1(5);
//    }
/*
    for (i = 0; i < 4; i++) {
        if (statusTemp[i] == 1 && tempBridge[i] > 20 && tempBridge[i] < 60) {
            tempNum++;
            temp_casu += tempBridge[i];
            tempSensors++;
        }
    }

    if (tempNum > 0)
        temp_casu /= tempNum;
    else
        temp_casu = -1;
    temp_casu1 = temp_casu;
    temp_wax = temp_casu; temp_wax1 = temp_casu;
*/
    float pwm_f;
    i = 0;
    for (i = 0; i < N; i++) {
        pwm_f = sin(2.0 * PI * i / N);
        pwm_i[i] = (pwm_f * 50 + 100) / 2.0 ;
    }
    dt_f = vibe_period / N;
    OpenTimer2(T2_OFF | T2_PS_1_1, ticks_from_us(dt_f, 1));
    ConfigIntTimer2(T2_INT_OFF | T2_INT_PRIOR_2);

    while(1) {


        if (msg_status == MSG_REF_ID) {
            updateReferences();
            msg_status = 0;
            if (vibeFreq_ref == 0 || vibeAmp_ref == 0) {
                //LedUser(0, 10, 0);
                PeltierSetPwm2(50);
                OpenTimer2(T2_OFF | T2_PS_1_1, ticks_from_us(0.1, 1));
                ConfigIntTimer2(T2_INT_OFF | T2_INT_PRIOR_2);
                vibe_period = 0 ;
                SpeakerOff();
                vibeFreq_ref = 0;
                vibeAmp_ref = 0;
                vibeFreq_old = vibeFreq_ref;
            }
            else if (vibeFreq_old != vibeFreq_ref)
            {
                SpeakerOn();
                //LedUser(0, 0 , 10);
                vibeFreq_old = vibeFreq_ref;
                vibe_period = 1000000.0 / vibeFreq_ref; // in usec
                dt_f = vibe_period / N; // 2 ms
                OpenTimer2(T2_ON | T2_PS_1_1, ticks_from_us(dt_f, 1));
                ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
            }
        }

        updateMeasurements();
        delay_t1(10);

        ClrWdt(); //Clear watchdog timer
    }

    return (EXIT_SUCCESS);
}

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{
    int pwm_int;
    float pwm_fl;
    IFS0bits.T2IF = 0;
    /*
    pwm_fl = sinf(2 * PI * dt / vibe_period);

    // scale pwm_int 25->75
    pwm_int = (pwm_fl * 50 + 100) / 2 ;
    if (pwm_int > 75) {
        pwm_int  = 75;
    }
    else if (pwm_int < 25) {
        pwm_int = 25;
    }
    dt = dt + 100;
    if (dt >= vibe_period) {
        dt = 0;
    }

    // scale pwm int according to given amplitude
    //pwm_int = pwm_int * vibeAmp_ref / 100.0;
    */
    PeltierSetPwm2(vibeAmp_ref * pwm_i[dt] / 100);

    dt = dt + 1;
    /*
    t_f = t_f + dt_f;
    if (t_f >= vibe_period) {
        dt = 0;
        t_f = 0;
    }
    */
    if (dt == N) {
        dt = 0;
        /*
        LedUser(10, 0, 0);
         * */
    }
}
