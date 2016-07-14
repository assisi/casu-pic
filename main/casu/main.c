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
 * 06.02.2014       |	Comment
 */

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

#define MAIN_LOOP_DUR 80 

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
    UINT8 tempstatus=33;
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
    int watchCounter = 0;
    int tempSensors = 0;
    UINT16 timerVal = 0;
    UINT16 dummy;
    float timeElapsed = 0.0;
    char muxCh;
    extern UINT8 pwmMotor;
    extern UINT16 speakerAmp_ref;
    extern UINT16 speakerFreq_ref;
    extern UINT8 proxyStandby;
    
    setUpPorts();
     // configure i2c2 as a slave device and interrupt priority 1
    // wait for everything to initialize
    // it seems that it takes few seconds for voltage to settle down after power on
    for (i = 0; i < 13; i++) {
        delay_t1(200);
        ClrWdt();
    }
        
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
    LedUser(100, 0, 0);
    
    // Peltier initialization - set to 0
    ctlPeltier = 0;
    PeltierVoltageSet(ctlPeltier);    
    
    // Speaker initialization - set to 0,1
    spi1Init(2, 0);
    
    speakerAmp_ref = 0;
    speakerAmp_ref_old = 10;
    speakerFreq_ref = 1;
    speakerFreq_ref_old = 10;
    int count = 0;
    
    UINT16 inBuff[2] = {0};
    UINT16 outBuff[2] = {0};
    
    inBuff[0] = (speakerAmp_ref & 0x0FFF) | 0x1000;
    chipSelect(slaveVib);
    status = spi1TransferWord(inBuff[0], outBuff);
    chipDeselect(slaveVib);
    
    chipSelect(slaveVib);
    status = spi1TransferWord(inBuff[0], &speakerAmp_ref_old);
    chipDeselect(slaveVib);
    
    while (speakerAmp_ref != speakerAmp_ref_old) {
        if (count > 5 ) {
            // Error !
            //LedUser(100,0,0);
            break;
        }
        
        inBuff[0] = (speakerAmp_ref & 0x0FFF) | 0x1000;
        chipSelect(slaveVib);
        status = spi1TransferWord(inBuff[0], outBuff);
        chipDeselect(slaveVib);

        //delay_t1(1);
        chipSelect(slaveVib);
        status = spi1TransferWord(inBuff[0], &speakerAmp_ref_old);
        chipDeselect(slaveVib);
        count++;
    }
        
    count = 0;
    inBuff[0] = (speakerFreq_ref & 0x0FFF) | 0x2000;
    chipSelect(slaveVib);
    status = spi1TransferWord(inBuff[0], outBuff);
    chipDeselect(slaveVib);

    chipSelect(slaveVib);
    status = spi1TransferWord(inBuff[0], &speakerFreq_ref_old);
    chipDeselect(slaveVib);
    
    while (speakerFreq_ref != speakerFreq_ref_old) {
        
        if (count > 5 ) {
            // Error !
            //LedUser(0,100,0);
            break;
        }

        inBuff[0] = (speakerFreq_ref & 0x0FFF) | 0x2000;
        chipSelect(slaveVib);
        status = spi1TransferWord(inBuff[0], outBuff);
        chipDeselect(slaveVib);

        chipSelect(slaveVib);
        status = spi1TransferWord(inBuff[0], &speakerFreq_ref_old);
        chipDeselect(slaveVib);
        count++;
    }

    //ADT7420 sensors initalization
    status = adt7420Init(0, ADT74_I2C_ADD_mainBoard);
    delay_t1(1);
    muxCh = I2C1ChSelect(1,6);
    status = adt7420Init(0, ADT74_I2C_ADD_flexPCB);
    UINT16 dummyData[2] = {0};
     
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
    
    //CASU ring average temperature
    temp_casu = 0;
    tempNum = 0;

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
    
    diagLED_r[0] = 0;
    diagLED_r[1] = 0;
    diagLED_r[2] = 0;
    
    I2C2SlaveInit(I2C2_CASU_ADD, 1);
    
    while (i2cStarted == 0) {
        delay_t1(200);
        ClrWdt();
    }
    
    LedUser(diagLED_r[0], diagLED_r[1],diagLED_r[2]);
   
    while(1) {
        
        ConfigIntTimer2(T1_INT_OFF);    //Disable timer interrupt
        IFS0bits.T2IF = 0; //Clear interrupt flag
        OpenTimer2(T1_ON | T1_PS_1_256, 30000); //Configure timer
        
 
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
        
        if (!proxyStandby)  {
            
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
            
        }   else {
        
                proxy_f = 0;
                //Front right
                proxy_fr = 0;
                //Back right
                proxy_br = 0;
                //Back
                proxy_b = 0;
                //Back left
                proxy_bl = 0;
                //Front left
                proxy_fl = 0;       
        
        }
        
        //Temperature readings and control
        // readings every 2 second
        // PID control every 2 seconds
        if (tempLoopControl >= 25) {
            
            //Cooler temperature
            adt7420ReadTemp(&temp_pcb, ADT74_I2C_ADD_mainBoard);
            muxCh = I2C1ChSelect(1, 6);
            adt7420ReadTemp(&temp_flexPCB, ADT74_I2C_ADD_flexPCB);

            if (tempSensors > 0) {
                // we have at least on temp sensor working

                // peltier controlled
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

                tempBridge[0] = temp_f;
                tempBridge[1] = temp_r;
                tempBridge[2] = temp_b;
                tempBridge[3] = temp_l;

                //CASU ring average temperature
                temp_casu = 0;
                tempNum = 0;
                for (i = 0; i < 4; i++) {
                    if (statusTemp[i] == 1 && tempBridge[i] > 20 && tempBridge[i] < 60) {
                        tempNum++;
                        temp_casu += tempBridge[i];
                    }
                }
            }
            else {
                tempNum = 0;
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

            //# PELTIER FEEDBACK LOOP
            if (tempSensors > 0) {
                if (tempCtlOn == 1) {
             
                    if (temp_ref <= temp_ref_shutdown) {
                        ctlPeltier = 0;
                        PeltierVoltageSet(ctlPeltier);
                    }
                    else {
                        if (temp_ref > temp_ref_h)
                            temp_ref = temp_ref_h;
                        if (temp_ref < temp_ref_l)
                            temp_ref = temp_ref_l;
                        
                        ctlPeltier = PeltierPID(temp_ref, temp_wax);
                    }
                    
                    if ((temp_casu > 45) || (temp_pcb > 45)) { //Check limits
                        ctlPeltier = 0;
                    }
                    PeltierVoltageSet(-ctlPeltier);
                    
                }
                else {
                    ctlPeltier = (temp_ref * 5) - 150;
                    PeltierVoltageSet(-ctlPeltier);
                }
            }
            else {
                ctlPeltier = 0;
                PeltierVoltageSet(ctlPeltier);
            }

            tempLoopControl = 0;

        } else {
            
            tempLoopControl++;
        }  

//# COOLER FAN CONTROL
        if (fanCtlOn == 1) {
            if (temp_pcb >= 30 && fanCooler == FAN_COOLER_OFF)
                fanCooler = FAN_COOLER_ON;
            else if (temp_pcb <= 29 && fanCooler == FAN_COOLER_ON)
                fanCooler = FAN_COOLER_OFF;
            //In case of I2C1 fail turn on the fan
            if((proxy_f == 0xFFFF) && (proxy_fr == 0xFFFF) && (proxy_br == 0xFFFF) && (proxy_b == 0xFFFF) && (proxy_bl == 0xFFFF) && (proxy_fl == 0xFFFF)){
                fanCooler = FAN_COOLER_ON;
            }
        }
        else if (fanCtlOn == 2) {
            fanCooler = 0;
        }
        else {
            fanCooler = 100;
        }
        FanCooler(fanCooler);

        updateMeasurements();
        timerVal = ReadTimer2();
        CloseTimer2();
        timeElapsed = ms_from_ticks(timerVal, 256);
        if (timeElapsed < MAIN_LOOP_DUR)
            delay_t1(MAIN_LOOP_DUR - timeElapsed);
        
        ClrWdt(); //Clear watchdog timer
    }   

    return (EXIT_SUCCESS);
}
