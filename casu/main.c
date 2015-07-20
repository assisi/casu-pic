
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
#include "../fft/fft.h"

// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF);
// Enable Clock Switching and Configure POSC in XT mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
//I2C alt pins
_FPOR(ALTI2C2_OFF & ALTI2C1_ON);
//Watchdog timer -> Twtd = PR*POST/32000 [s]
_FWDT(FWDTEN_ON & WDTPRE_PR128 & WDTPOST_PS1024);   //Twdt ~ 4s

int ax = 0, ay = 0, az = 0;
int accSamples = 0;
int proxiCounter = 0;
float az_b[4] = {0};
int statusAcc[4] = {0};
digitalPin accPin;
int accNum = 0;
UINT16 accPeriod;

/****FFT****/
int k=0, str_count = 0, while_count = 0, enable_i2c2 = 0;
static fractcomplex Twiddles_array[FFT_BUFF/2] __attribute__ ((space(xmemory), far, aligned (FFT_BUFF * 2    )));
static fractcomplex      src_array[FFT_BUFF]   __attribute__ ((space(ymemory), far, aligned (FFT_BUFF * 2 * 2)));
fractcomplex destination_array[FFT_BUFF] = {0};

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
    int vibeLoopControl = 0;
    int tempNum;            //Sensors number
    int tempLoop = 0;
    int watchCounter = 0;
    int tempSensors = 0;
    float mainLoopDelay = 99; // msec

    setUpPorts();
    I2C2SlaveInit(I2C2_CASU_ADD, 1);
    delay_t1(500);
    digitalHigh(LED2R);

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

    //PWM intialization
    PWMInit();

    //ADT7420 sensors initalization
    status = adt7420Init(0);

    PeltierSetOut2(0);

    //Estimation initializaction
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

    vAmp_f = -1;
    vAmp_r = -1;
    vAmp_b = -1;
    vAmp_l = -1;
    fAmp_f = -1;
    fAmp_r = -1;
    fAmp_b = -1;
    fAmp_l = -1;

    statusAcc[ACC_F] = adxl345Init(aSlaveF);
    delay_t1(1);
    statusAcc[ACC_R] = adxl345Init(aSlaveR);
    delay_t1(1);
    statusAcc[ACC_B] = adxl345Init(aSlaveB);
    delay_t1(1);
    statusAcc[ACC_L] = adxl345Init(aSlaveL);

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

    accPin = aSlaveL;
    status = adxl345Init(accPin);
    accPeriod = 500; // 2 khz
    /* Init FFT coefficients */
    TwidFactorInit(LOG2_FFT_BUFF, &Twiddles_array[0],0);
    // read 100 values to calculate bias
    delay_t1(10);
    int m;
    for (m = 0; m < 50; m++) {
        if (statusAcc[ACC_F] > 0) {
            readAccZ(aSlaveF, &az);
            az_b[ACC_F] += az;
            accPin = aSlaveF;
            accNum = ACC_F;
        }
        delay_t1(1);
        if (statusAcc[ACC_R] > 0) {
            readAccZ(aSlaveR, &az);
            az_b[ACC_R] += az;
            accPin = aSlaveR;
            accNum = ACC_R;
        }
        delay_t1(1);
        if (statusAcc[ACC_B] > 0) {
            readAccZ(aSlaveB, &az);
            az_b[ACC_B] += az;
            accPin = aSlaveB;
            accNum = ACC_B;
        }
        delay_t1(1);
        if (statusAcc[ACC_L] > 0) {
            readAccZ(aSlaveL, &az);
            az_b[ACC_L] += az;
            accPin = aSlaveL;
            accNum = ACC_L;
        }
        delay_t1(1);
    }
    az_b[ACC_F] /= m;
    az_b[ACC_R] /= m;
    az_b[ACC_B] /= m;
    az_b[ACC_L] /= m;

    OpenTimer2(T2_ON| T2_PS_1_1, ticks_from_us(accPeriod, 1));
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);

    while(1) {

        /*
         **** Do not read acc sensors - we have to implement fft
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
        */
         //Front
        /*
        statusProxi[0] = I2C1ChSelect(1, 2);
        proxy_f = VCNL4000ReadProxi();
        delay_t1(1);
        //Front right
        statusProxi[2] = I2C1ChSelect(1, 3);
        proxy_fr = VCNL4000ReadProxi();
        delay_t1(1);
        //Back right
        statusProxi[1] = I2C1ChSelect(1, 4);
        proxy_br = VCNL4000ReadProxi();
        delay_t1(1);
        //Back
        statusProxi[3] = I2C1ChSelect(1, 5);
        proxy_b = VCNL4000ReadProxi();
        delay_t1(1);
        //Back left
        statusProxi[4] = I2C1ChSelect(1, 0);
        proxy_bl = VCNL4000ReadProxi();
        delay_t1(1);
        //Front left
        statusProxi[5] = I2C1ChSelect(1, 1);
        proxy_fl = VCNL4000ReadProxi();
        delay_t1(1);
        */

        //if((proxy_f == -1) && (proxy_fl == -1) && (proxy_bl == -1) && (proxy_b == -1) && (proxy_br == -1) && (proxy_fr == -1))
           // muxReset();

        //Temperature readings and control
        // readings every 2.5 second
        // PID control every 10 seconds

        ax = 0;

        if (vibeLoopControl >= 10) {
            // compute fft every second
            if (accSamples == FFT_BUFF) {
                /*
                if (statusAcc[ACC_F] > 0) {
                    FastFourierTransform(&raw_acc[ACC_F][0], &amplitudes[0],
                            &destination_array[0], &src_array[0], &Twiddles_array,
                            &fAmp_f, &vAmp_f);
                    if (vAmp_f < 40) {
                        // if amplitude is less than 40 mg, ignore the data, since this is the noise level
                        vAmp_f = 0;
                        fAmp_f = 0;
                    }
                }
                if (statusAcc[ACC_R] > 0) {
                    FastFourierTransform(&raw_acc[ACC_R][0], &amplitudes[0],
                            &destination_array[0], &src_array[0], &Twiddles_array,
                            &fAmp_r, &vAmp_r);
                    if (vAmp_r < 40) {
                        // if amplitude is less than 40 mg, ignore the data, since this is the noise level
                        vAmp_r = 0;
                        fAmp_r = 0;
                    }
                }
                if (statusAcc[ACC_B] > 0) {
                    FastFourierTransform(&raw_acc[ACC_B][0], &amplitudes[0],
                            &destination_array[0], &src_array[0], &Twiddles_array,
                            &fAmp_b, &vAmp_b);
                    if (vAmp_b < 40) {
                        // if amplitude is less than 40 mg, ignore the data, since this is the noise level
                        vAmp_b = 0;
                        fAmp_b = 0;
                    }
                }
                if (statusAcc[ACC_L] > 0) {
                    FastFourierTransform(&raw_acc[ACC_L][0], &amplitudes[0],
                            &destination_array[0], &src_array[0], &Twiddles_array,
                            &fAmp_l, &vAmp_l);
                    if (vAmp_l < 40) {
                        // if amplitude is less than 40 mg, ignore the data, since this is the noise level
                        vAmp_l = 0;
                        fAmp_l = 0;
                    }
                }
                */
                // For now we only compute fft for a single sensor
               FastFourierTransform(&raw_acc_single[0], &amplitudes[0],
                            &destination_array[0], &src_array[0], &Twiddles_array,
                            &fAmp_f, &vAmp_f);
               if (vAmp_f < 40) {
                       // if amplitude is less than 40 mg, ignore the data, since this is the noise level
                   vAmp_f = 0;
                   fAmp_f = 0;
               }
               //LedUser(0, 0, 100);
               accSamples = 0;
               vibeLoopControl = 0;
               LedUser(0, 0, 0);
            }

        }
        else
            vibeLoopControl++;


        if (tempLoopControl >= 25) {
            /*
            OpenTimer2(T2_OFF | T2_PS_1_1, ticks_from_us(accPeriod, 1));
            ConfigIntTimer2(T2_INT_OFF | T2_INT_PRIOR_2);
            //Cooler temperature
            adt7420ReadTemp(&temp_t);
            OpenTimer2(T2_ON | T2_PS_1_1, ticks_from_us(accPeriod, 1));
            ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
            */

            if (tempSensors > 0) {
                // we have at least on temp sensor working

                // peltier controlled
                    //diagLED_r[0] = 50;
                if (statusTemp[0] == 1)
                    adt7320ReadTemp(tSlaveF, &temp_f);
                else
                    temp_f = -1;
                if (statusTemp[1] == 1)
                    adt7320ReadTemp(tSlaveR, &temp_r);
                else
                    temp_r = -1;
                if (statusTemp[2] == 1)
                    adt7320ReadTemp(tSlaveB, &temp_b);
                else
                    temp_b = -1;
                if (statusTemp[3] == 1)
                    adt7320ReadTemp(tSlaveL, &temp_l);
                else
                    temp_l = -1;

                tempBridge[0] = temp_f; tempBridge[1] = temp_r;
                tempBridge[2] = temp_b; tempBridge[3] = temp_l;

                //CASU ring average temperature
                temp_casu = 0;
                tempNum = 0;
                for(i=0;i<4;i++){
                    if (statusTemp[i] == 1 && tempBridge[i] > 20 && tempBridge[i] < 60){
                        tempNum++;
                        temp_casu += tempBridge[i];
                    }
                }
                //temp_casu = temp_casu - temp_f; // front sensor mounted on wax
                if (tempNum > 0)
                    temp_casu /= tempNum;
                else
                    temp_casu = temp_casu1;

                //Wax temperature estimation - PT1, Tustin discretizaion:
                // y(k) = Kf1 * y(k-1) + Kf2 * u(k) + Kf3 * u(k-1)
                temp_wax = Kf1 * temp_casu + Kf2 * temp_casu1 + Kf3 * temp_wax1;
                temp_wax1 = temp_wax;
                temp_casu1 = temp_casu;

                if (tempLoop == 3) {
                    // 2.5 s peltier off
                    //diagLED_r[0] = 0;
                    PeltierOff();
                    tempLoop = 0;
                    //LedUser(100, 0, 0);

                }
                else {

                    if (tempCtlOn == 1) {

                        if ((temp_casu > 45) || (temp_t > 45)){

                            // casu if overheating, turn off everything
                            ctlPeltier = 0;
                            PeltierSetOut2(ctlPeltier);
                        }
                        else {

                            if(temp_ref <= temp_ref_l){     // temp_ref < 26 , turn off
                                ctlPeltier = 0;
                                PeltierResetPID();
                                PeltierSetOut2(ctlPeltier);
                            }
                            else{
                                // temp control is on -> calculate PID output every 10 seconds
                                if (tempLoop == 0) {
                                    // increase reference by 0.5°C every 10 sec
                                    //LedUser(0, 100, 0);
                                    if (temp_ref - temp_ref_cur > 0.5)
                                        temp_ref_cur += 0.5;
                                    else if (temp_ref - temp_ref_cur < -0.5)
                                        temp_ref_cur -= 0.5;
                                    else
                                        temp_ref_cur = temp_ref;
                                    ctlPeltier = PeltierPID(temp_ref_cur, temp_wax);//PeltierPID(temp_ref_cur, temp_l);
                                    PeltierSetOut2(ctlPeltier);

                                }

                            }
                        }

                    }
                    else {
                        // temp control is off -> propagate reference to the output
                        // ref is in the range [25, 45] -> transform to [-100, 100]
                        if (tempLoop == 0) {
                            if (temp_ref < 35) {
                                // [25 - 35] - > [0, 100]
                                ctlPeltier = 10 * temp_ref - 250;
                            }
                            else {
                                //[35-45] -> [0--100]
                                ctlPeltier = -10.0 * temp_ref + 350;
                            }
                            PeltierSetOut2(ctlPeltier);
                        }
                    }

                    tempLoop++;

                }
            }
            else {
                temp_casu = -1;
                temp_wax = -1;
                PeltierSetOut2(0);
            }
            //countLoop = 0;
            tempLoopControl = 0;
            mainLoopDelay = 97;
        }
        else {
            tempLoopControl++;
            mainLoopDelay = 99;
        }

        if (fanCtlOn == 1) {
            if (temp_t >= 30 && fanCooler == 0)
                fanCooler = 100;
            else if (temp_t <= 29 && fanCooler == 100)
                fanCooler = 0;
        }
        else if (fanCtlOn == 2) {
            fanCooler = 100;
        }
        else {
            fanCooler = 0;
        }
        FanCooler(fanCooler);

        updateMeasurements();
        delay_t1(mainLoopDelay);

        ClrWdt(); //Clear watchdog timer
    }

    return (EXIT_SUCCESS);
}

// Timer 2 interrupt service for reading acc measurements at exactly 1 KHz
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{
    // Clear Timer 2 interrupt flag
    IFS0bits.T2IF = 0;

    if (accSamples < FFT_BUFF) {
        // for now we read only single sensor
        /*
        if (statusAcc[ACC_F] > 0) {
            readAccZ(aSlaveF, &az);
            az = az - az_b[ACC_F];
            raw_acc[ACC_F][mainLoopCount] = az;
        }
        if (statusAcc[ACC_R] > 0) {
            readAccZ(aSlaveR, &az);
            az = az - az_b[ACC_R];
            raw_acc[ACC_R][mainLoopCount] = az;
        }
        if (statusAcc[ACC_B] > 0) {
            readAccZ(aSlaveB, &az);
            az = az - az_b[ACC_B];
            raw_acc[ACC_B][mainLoopCount] = az;
        }
        if (statusAcc[ACC_L] > 0) {
            readAccZ(aSlaveL, &az);
            az = az - az_b[ACC_L];
            raw_acc[ACC_L][mainLoopCount] = az;
        }
         mainLoopCount++;
        */
        readAccZ(accPin, &az);
        az = az - az_b[accNum];
        raw_acc_single[accSamples++] = az;
    }
    else {
        LedUser(0, 20, 0);

        if (proxiCounter >= 240) {
            /* Read proximity sensors and i2c temp sensor*/
            I2C1ChSelect(1, 2);
            proxy_f = VCNL4000ReadProxi();
            //delay_t1(1);
            //Front right
            I2C1ChSelect(1, 3);
            proxy_fr = VCNL4000ReadProxi();
            //delay_t1(1);
            //Back right
            I2C1ChSelect(1, 4);
            proxy_br = VCNL4000ReadProxi();
            //delay_t1(1);
            //Back
            I2C1ChSelect(1, 5);
            proxy_b = VCNL4000ReadProxi();
            //delay_t1(1);
            //Back left
            I2C1ChSelect(1, 0);
            proxy_bl = VCNL4000ReadProxi();
            //delay_t1(1);
            //Front left
            I2C1ChSelect(1, 1);
            proxy_fl = VCNL4000ReadProxi();
            //delay_t1(1);
            // read pcb temperature
            adt7420ReadTemp(&temp_t);
            proxiCounter = 0;
        }
   }
    proxiCounter++;
}