/*!
 * File:   main.c
 * Author: thaus
 *
 * Created on 2014. sije?anj 06, 13:03
 */
/* REVISION HISTORY:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
 * Date, Author     |   Comments on this revision
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
 * 06.02.2014       |   Comment
 * 24.10.2016.      |   fft added
 */

#define ACC_RATE 2000   // Hz
#define PI 3.141593
#define MAIN_LOOP_DUR 80 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dsp.h>
#include <p33Exxxx.h>
#include "initializeHardware.h"
#include "../peripheral/gpio/digitalIO.h"
#include "../peripheral/timer/timerFunctions.h"
#include "../pic-to-beaglebone/pic2beaglebone.h"
#include "../sensors/adt7320/adt7320.h"
#include "../sensors/adt7420/adt7420.h"
#include "../sensors/adxl345/adxl345.h"
#include "../peripheral/i2c/i2c1.h"
#include "../peripheral/spi/spi1.h" 
#include "../sensors/vcnl4000/proxiVCNL4000.h"
#include "../peripheral/timer/timer2.h"
#include "../peripheral/timer/timer3.h" 
#include "../peripheral/timer/timer4.h" 
#include "../actuators/pwm.h"
#include "../actuators/peltier.h"


// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF);
// Enable Clock Switching and Configure POSC in XT mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
//I2C alt pins
_FPOR(ALTI2C2_OFF & ALTI2C1_ON);
//Watchdog timer -> Twtd = PR*POST/32000 [s]
_FWDT(FWDTEN_ON & WDTPRE_PR128 & WDTPOST_PS1024);   //Twdt ~ 4s
//_FWDT(FWDTEN_OFF);
/*
 * 
 */

int ax = 0, ay = 0, az = 0;
int mainLoopCount = 0;
float ax_b_l = 0, ay_b_l = 0, az_b_l = 0;
float ax_b_r = 0, ay_b_r = 0, az_b_r = 0;

float vibe_period = 100; // 100 ms


/****FFT****/
int k=0, str_count = 0, while_count = 0, enable_i2c2 = 0;
//static fractcomplex Twiddles_array[FFT_BUFF/2] __attribute__ ((space(xmemory), far, aligned (FFT_BUFF * 2    )));
static fractcomplex Twiddles_array[FFT_BUFF/2] __attribute__ ((space(xmemory)));
static fractcomplex      src_array[FFT_BUFF]   __attribute__ ((space(ymemory), far, aligned (FFT_BUFF * 2 * 2)));
fractcomplex destination_array[FFT_BUFF] = {{0}};

digitalPin accPin;
UINT16 accPeriod;

// Unused???
UINT16 countLoop;

UINT8 timer4_flag = 0;


int main(int argc, char** argv) {

    /*Configuring POSC with PLL, with goal FOSC = 80 MHZ */
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    // Fin = 8 Mhz, 8  * (40/2/2) = 80
    PLLFBD = 18; // M=40          // change to 38 for POSC 80 Mhz - this worked only on a single MCU for uknown reason
    CLKDIVbits.PLLPOST = 0; // N2=2
    CLKDIVbits.PLLPRE = 0; // N1=2

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
    int ax = 0, ay = 0, az = 0;
    float kAdapt = 1;           //Adaptive gain coeff.
    float temp = 0;
    float tempBridge[4];        //Bridge temperature copy
    float uref_m[4];
    int statusProxi[8];
    int statusTemp[4];          //Temperature sensors initialization status
    int tempLoopControl = 0;
    int tempNum = 0;            //Sensors number
    int tempSensors = 0;
    UINT16 timerVal = 0;
    float timeElapsed = 0.0;
    char muxCh;
    //extern UINT8 pwmMotor;
    extern UINT16 speakerAmp_ref;
    extern UINT16 speakerFreq_ref;
    extern UINT8 proxyStandby;
   
    setUpPorts();

    digitalHigh(LED2R);
    delay_t1(500);

    PWMInit();
    delay_t1(100);
    //PeltierSetPwm(50);
    VibrationSet(0);

    accPin = aSlaveR;
    accPeriod = 500;  // 500 us

    status = adxl345Init(accPin);
    ax = status;
    delay_t1(5);

    /* Init FFT coefficients */
    //TwidFactorInit(LOG2_FFT_BUFF, &Twiddles_array[0],0);

    // read 100 values to calculate bias
    int m;
    int n = 0;
    for (m = 0;m < 100; m++) {

        status = readAccXYZ(accPin, &ax, &ay, &az);
        if (status <= 0) {
            //
        }
        else {
            ax_b_l += ax;
            ay_b_l += ay;
            az_b_l += az;
            n++;
        }
        delay_t1(1);
    }

    ax_b_l /= n;
    ay_b_l /= n;
    az_b_l /= n;

    _SI2C2IE = 0;
    _SI2C2IF = 0;

    LedBee(0, 0, 0);
    //LedUser(0, 0, 0);
    VibrationSet(0);
    FanCooler(0);
    //PeltierSetPwm(50);
    ax = 0;

    for (i = 0; i < 13; i++) {
        delay_t1(200);
        ClrWdt();
    }
        
    // Accelerometer initialization
/*    status = adxl345Init(aSlaveF);
    delay_t1(5);
    status = adxl345Init(aSlaveR);
    delay_t1(5);
    status = adxl345Init(aSlaveL);
    delay_t1(5);
    status = adxl345Init(aSlaveB);
    delay_t1(5);
*/    
    // Proximity sensors initalization    
    I2C1MasterInit();
    status = VCNL4000Init();  
    
    // PWM intialization
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

        chipSelect(slaveVib);
        status = spi1TransferWord(inBuff[0], &speakerAmp_ref_old);
        chipDeselect(slaveVib);

        count++;
    }
        
    count = 0;
    
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

    // Cooler temperature sensors initalization
    status = adt7420Init(0, ADT74_I2C_ADD_mainBoard);
    delay_t1(1);
    muxCh = I2C1ChSelect(1,6);
    status = adt7420Init(0, ADT74_I2C_ADD_flexPCB);

    // Temperature sensors initialization
    statusTemp[0] = adt7320Init(tSlaveF, ADT_CONT_MODE | ADT_16_BIT);
    delay_t1(5);
    statusTemp[1] = adt7320Init(tSlaveR, ADT_CONT_MODE | ADT_16_BIT);
    delay_t1(5);
    statusTemp[2] = adt7320Init(tSlaveB, ADT_CONT_MODE | ADT_16_BIT);
    delay_t1(5);
    statusTemp[3] = adt7320Init(tSlaveL, ADT_CONT_MODE | ADT_16_BIT);
    delay_t1(5);
     
    // Temperature estimation initialization
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
    
    // CASU ring average temperature
    temp_casu = 0;
    tempNum = 0;
    tempSensors = 0;

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
    temp_wax = temp_casu; 
    temp_wax1 = temp_casu;
    ka1 = 1;
    
    //Temperature PT1 model initial value
    ymk1 = temp_casu;
    
    diagLED_r[0] = 0;
    diagLED_r[1] = 0;
    diagLED_r[2] = 0;
    
    // Configure i2c2 as a slave device and interrupt priority 1
    I2C2SlaveInit(I2C2_CASU_ADD, 1);
    
//    while (i2cStarted == 0) {
//        delay_t1(200);
//        ClrWdt();
//    }
    
    /* // Timers not initialized - no FFT
    OpenTimer3(T3_ON| T3_PS_1_1, ticks_from_us(accPeriod, 1));
    ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_1);

    OpenTimer4(T4_ON| T4_PS_1_1, ticks_from_ms(1000, 1));
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_3);
    */

    LedUser(diagLED_r[0], diagLED_r[1], diagLED_r[2]);

    while(1) {
        
        ConfigIntTimer2(T1_INT_OFF);    //Disable timer interrupt
        IFS0bits.T2IF = 0;              //Clear interrupt flag
        OpenTimer2(T1_ON | T1_PS_1_256, 30000); //Configure timer
        
        // Not run - timers not started
//        if (timer4_flag == 1) {
//            timer4_flag = 0;
//
//            /*
//             * FOSC = 40 Mhz
//             *      FFT_BUFF = 256: t(fft) = 20 ms
//             *      FFT_BUFF = 512: t(fft) = 40 ms
//             * FOSC = 40 Mhz
//             *      FFT_BUFF = 256: t(fft) = 10 ms
//             *      FFT_BUFF = 512: t(fft) = 23 ms
//             */
//
//            /*FastFourierTransform(&source_array[0], &amplitudes[0], &destination_array[0], &src_array[0], &Twiddles_array[0]);
//            updateAccLog();
//            */
//            // Only works when no other process (FFT only): 
//            /*
//            _SI2C2IE = 1;
//            while (i2c2_tx_ready == 1) {
//                delay_t1(10);
//                ax++;
//                if (ax == 500) {
//                    break;
//                    i2c2_tx_ready = 0;
//                }
//                ClrWdt();   //Clear watchdog timer
//            }
//            _SI2C2IE = 0;
//            */
//
//            mainLoopCount = 0;
//            OpenTimer3(T3_ON | T3_PS_1_1, ticks_from_us(accPeriod, 1));
//            ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_1);
//            
//            OpenTimer4(T4_ON| T4_PS_1_1, ticks_from_ms(1000, 1));
//            ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_3);
//        }
//        else {
//            delay_t1(2);
//            ClrWdt();   //Clear watchdog timer
//        }
        

        if (!proxyStandby)  {    
            // Front
            statusProxi[0] = I2C1ChSelect(1, 2);
            proxy_f = VCNL4000ReadProxi();
            delay_t1(1);
            // Back right
            statusProxi[1] = I2C1ChSelect(1, 4);
            proxy_br = VCNL4000ReadProxi();
            delay_t1(1);
            // Front right
            statusProxi[2] = I2C1ChSelect(1, 3);
            proxy_fr = VCNL4000ReadProxi();
            delay_t1(1);
            // Back
            statusProxi[3] = I2C1ChSelect(1, 5);
            proxy_b = VCNL4000ReadProxi();
            delay_t1(1);
            // Back left
            statusProxi[4] = I2C1ChSelect(1, 0);
            proxy_bl = VCNL4000ReadProxi();
            delay_t1(1);
            // Front left
            statusProxi[5] = I2C1ChSelect(1, 1);
            proxy_fl = VCNL4000ReadProxi();
            delay_t1(1);    
        }   
        else {
            proxy_f = 0;            // Front
            proxy_br = 0;            // Back right
            proxy_fr = 0;            // Front right
            proxy_b = 0;            // Back
            proxy_bl = 0;            // Back left
            proxy_fl = 0;            // Front left
        }
        
        // Temperature readings and control
        // readings every 2 seconds
        // PID control every 2 seconds
        if (tempLoopControl >= 25) {
            // Cooler temperature
            adt7420ReadTemp(&temp_pcb, ADT74_I2C_ADD_mainBoard);
            muxCh = I2C1ChSelect(1, 6);
            adt7420ReadTemp(&temp_flexPCB, ADT74_I2C_ADD_flexPCB);

            if (tempSensors > 0) {
                // We have at least one temperature sensor working
                // Peltier controlled
                if (statusTemp[0] == 1)
                    adt7320ReadTemp(tSlaveF, &temp_f);
                if (statusTemp[1] == 1)
                    adt7320ReadTemp(tSlaveR, &temp_r);
                if (statusTemp[2] == 1)
                    adt7320ReadTemp(tSlaveB, &temp_b);
                if (statusTemp[3] == 1)
                    adt7320ReadTemp(tSlaveL, &temp_l);

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

            if (tempNum > 0)
                temp_casu /= tempNum;
            else
                temp_casu = temp_casu1;

            // Wax temperature estimation - PT1, Tustin discretization:
            // y(k) = Kf1 * y(k-1) + Kf2 * u(k) + Kf3 * u(k-1)
            temp_wax = Kf1 * temp_casu + Kf2 * temp_casu1 + Kf3 * temp_wax1;
            temp_wax1 = temp_wax;
            temp_casu1 = temp_casu;
            
            //Temperature reference model
            temp_model = TempModel(uref_m[3]);
            //Delay of the model reference for 4 steps
            for(i=1; i<4; i++)
                uref_m[4-i] = uref_m[3-i];
            uref_m[0] = temp_ref;
            
            if(fabs(temp_ref - temp_wax) > 0.5)     //When is close to reference temperature do not use adaptation
                kAdapt = AdaptiveController(temp_ref, temp_wax, temp_model);
                        
            // Peltier feedback loop
            if (tempSensors > 0) {
                if (tempCtlOn == 1) {
                    if (temp_ref <= temp_ref_shutdown) {
                        ctlPeltier = 0;
                        PeltierVoltageSet(ctlPeltier);
                        //Reset adaptive controller parameters
                        ymk1 = temp_wax;
                        for(i=0; i<4; i++)
                            uref_m[i] = temp_wax;
                        ka1 = 1;
                    }
                    else {
                        if (temp_ref > temp_ref_h)
                            temp_ref = temp_ref_h;
                        if (temp_ref < temp_ref_l)
                            temp_ref = temp_ref_l;

                        ctlPeltier = PeltierPID(temp_ref, temp_wax, kAdapt);
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
        }
        else {   
            tempLoopControl++;
        }  

        // Cooler fan control
        if (fanCtlOn == 1) {
            if (temp_pcb >= 30 && fanCooler == FAN_COOLER_OFF)
                fanCooler = FAN_COOLER_ON;
            else if (temp_pcb <= 29 && fanCooler == FAN_COOLER_ON)
                fanCooler = FAN_COOLER_OFF;
            // In case of I2C1 fail turn on the fan
            if ((proxy_f == 0xFFFF) && (proxy_fr == 0xFFFF) && (proxy_br == 0xFFFF) && (proxy_b == 0xFFFF) && (proxy_bl == 0xFFFF) && (proxy_fl == 0xFFFF)) {
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
        
        //TEST
//        temp_casu = temp_model;
//        //temp_flexPCB = kAdapt;
//        proxy_fr = kAdapt*100;
//        proxy_fl = uref_m[0];
//        proxy_bl = uref_m[1];
//        proxy_b = uref_m[2];
//        proxy_br = uref_m[3];
        
        updateMeasurements();

        timerVal = ReadTimer2();
        CloseTimer2();
        timeElapsed = ms_from_ticks(timerVal, 256);
        if (timeElapsed < MAIN_LOOP_DUR)
            delay_t1(MAIN_LOOP_DUR - timeElapsed);
        
        ClrWdt(); //Clear watchdog timer

    } // end while(1)
    return (EXIT_SUCCESS);
}


// Timer 3 interrupt service for reading accelerometer measurements at exactly 2 KHz
void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void)
{
    if (mainLoopCount < FFT_BUFF) {
        readAccZ(accPin, &az);
        az = az - az_b_l;
        source_array[mainLoopCount++] = az;
        if (mainLoopCount < 100)
            LedUser(0, 100, 0);
        else if (mainLoopCount < 200)
            LedUser(100, 0, 100);
        else
            LedUser(0, 100, 100);
   }
    else if (mainLoopCount == FFT_BUFF) {
        LedUser(100, 100, 100);

        CloseTimer3();
        
        ConfigIntTimer3(T3_INT_OFF);
        OpenTimer3(T3_OFF | T3_PS_1_1, ticks_from_us(accPeriod, 1));
    }
    // Clear Timer 3 interrupt flag
    //ClearIntTimer3();
}


// Timer 4 interrupt service for FFT on accelerometer reading 
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(void)
{
    ConfigIntTimer4(T4_INT_OFF);
    CloseTimer4();
    OpenTimer4(T4_OFF | T4_PS_1_1, ticks_from_ms(1000, 1));
    //ClearIntTimer4();
    timer4_flag = 1;
}
