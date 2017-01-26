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
 * 04.01.2017.      |   dma+spi+fft implemented
 */

#define ACC_RATE 3200   // Hz
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
#include "../peripheral/dma/dma0.h"
#include "../peripheral/dma/dma1.h"
#include "../sensors/vcnl4000/proxiVCNL4000.h"
#include "../fft/fft.h"
#include "../peripheral/timer/timer2.h"
#include "../peripheral/timer/timer3.h"
#include "../peripheral/timer/timer4.h"
#include "../actuators/pwm.h"
#include "../actuators/peltier.h"
#include "interrupts.h"

// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF);
// Enable Clock Switching and Configure POSC in XT mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
//I2C alt pins
_FPOR(ALTI2C2_OFF & ALTI2C1_ON);
//Watchdog timer -> Twtd = PR*POST/32000 [s]
_FWDT(FWDTEN_ON & WDTPRE_PR128 & WDTPOST_PS1024);   //Twdt ~ 4s
//_FWDT(FWDTEN_OFF);

int ax = 0, ay = 0, az = 0;
float ax_b_l = 0, ay_b_l = 0, az_b_l = 0;
float ax_b_r = 0, ay_b_r = 0, az_b_r = 0;

float vibe_period = 100; // 100 ms

/****FFT****/
int k = 0, str_count = 0, while_count = 0, enable_i2c2 = 0;
//static fractcomplex Twiddles_array[FFT_BUFF/2] __attribute__ ((space(xmemory), far, aligned (FFT_BUFF * 2    )));
static fractcomplex Twiddles_array[FFT_BUFF/2] __attribute__ ((space(xmemory)));
static fractcomplex      src_array[FFT_BUFF]   __attribute__ ((space(ymemory), far, aligned (FFT_BUFF * 2 * 2)));
fractcomplex destination_array[FFT_BUFF] = {{0}};

digitalPin accPin;
UINT16 dma_spi_rx_count = 0;
UINT16 dma_spi_tx_count = 0;
int statusTemp[4];          //Temperature sensors initialization status
float tempBridge[4];        //Bridge temperature copy
int tempNum = 0;            //Sensors number
int tempSensors = 0;
UINT8 dma_spi2_started = 0;
char muxCh;
float delta_freq;

UINT8 timer4_flag = 0;

float uref_m[4] = {25};
float temp_model = 25;
float smc_parameters[2] = {0};
float alpha = 7.0, beta = 0;

//test only
extern float sigma_m;
extern float sigma;


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
    float temp = 0;
    int statusProxi[8];
    int tempLoopControl = 0;
    int slowLoopControl = 0;
    UINT16 timerVal = 0;
    float timeElapsed = 0.0;
    //extern UINT8 pwmMotor;
    extern UINT16 speakerAmp_ref;
    extern UINT16 speakerFreq_ref;
    extern UINT8 proxyStandby;
    UINT16 dummy = 0x0000;

    setUpPorts();
    delay_t1(50);

    PWMInit();
    delay_t1(50);

    ctlPeltier = 0;
    PeltierVoltageSet(ctlPeltier);
    FanCooler(0);
    diagLED_r[0] = 100;
    diagLED_r[1] = 0;
    diagLED_r[2] = 0;
    LedUser(diagLED_r[0], diagLED_r[1],diagLED_r[2]);

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
            //LedUser(100, 0, 0);
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
            //LedUser(0, 100, 0);
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

    accPin = aSlaveR;
    accPeriod = 1.0 / ACC_RATE * 1000000.0;  // in us; for ACC_RATE = 3200 Hz it should equal 312.5 us
    status = adxl345Init(accPin);
    ax = status;
    delay_t1(5);

    /* Init FFT coefficients */
    TwidFactorInit(LOG2_FFT_BUFF, &Twiddles_array[0],0);
    delta_freq = (float)ACC_RATE / FFT_BUFF;

    // read 100 values to calculate bias
    int m;
    int n = 0;
    for (m = 0; m < 100; m++) {

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

    // Proximity sensors initalization
    I2C1MasterInit();
    status = VCNL4000Init();

    // Cooler temperature sensors initalization
    status = adt7420Init(0, ADT74_I2C_ADD_mainBoard);
    delay_t1(1);
    muxCh = I2C1ChSelect(1, 6);
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
        adt7320ReadTemp(tSlaveF, &temp_f);
        delay_t1(1);
        adt7320ReadTemp(tSlaveL, &temp_l);
        delay_t1(1);
        adt7320ReadTemp(tSlaveB, &temp_b);
        delay_t1(1);
        adt7320ReadTemp(tSlaveR, &temp_r);
        delay_t1(1);
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
    temp_model = temp_wax;

    for (i = 0; i < 4; i++) {
        uref_m[i] = temp_wax;
    }

    // Configure i2c2 as a slave device and interrupt priority 5
    I2C2SlaveInit(I2C2_CASU_ADD, BB_I2C_INT_PRIORITY);

    // delay for 2 sec
    for(i = 0; i < 4; i ++) {
        delay_t1(500);
        ClrWdt();
    }

    while (i2cStarted == 0) {
        delay_t1(200);
        ClrWdt();
    }

    dma0Init();
    dma1Init();

    CloseTimer4();
    ConfigIntTimer4(T4_INT_ON | TIMER4_PRIORITY);
    OpenTimer4(T4_ON | T4_PS_1_256, ticks_from_ms(500, 256));

    diagLED_r[0] = 0;
    diagLED_r[1] = 0;
    diagLED_r[2] = 0;
    LedUser(diagLED_r[0], diagLED_r[1],diagLED_r[2]);

    start_acc_acquisition();

    while(1) {

        ConfigIntTimer2(T2_INT_OFF);    // Disable timer interrupt
        IFS0bits.T2IF = 0;              // Clear interrupt flag
        OpenTimer2(T2_ON | T2_PS_1_256, 65535); // Configure timer

        if (!proxyStandby) {
            statusProxi[0] = I2C1ChSelect(1, 2);            // Front
            proxy_f = VCNL4000ReadProxi();
            delay_t1(1);
            statusProxi[1] = I2C1ChSelect(1, 4);            // Back right
            proxy_br = VCNL4000ReadProxi();
            delay_t1(1);
            statusProxi[2] = I2C1ChSelect(1, 3);            // Front right
            proxy_fr = VCNL4000ReadProxi();
            delay_t1(1);
            statusProxi[3] = I2C1ChSelect(1, 5);            // Back
            proxy_b = VCNL4000ReadProxi();
            delay_t1(1);
            statusProxi[4] = I2C1ChSelect(1, 0);            // Back left
            proxy_bl = VCNL4000ReadProxi();
            delay_t1(1);
            statusProxi[5] = I2C1ChSelect(1, 1);            // Front left
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


        if (timer4_flag == 1) {
            // every 1 second
            timer4_flag = 0;

            if (slowLoopControl == 0 || slowLoopControl == 2 ) {
                // fft loop every 1 sec
                fftLoop();
                dummy = 1;
                start_acc_acquisition();
                slowLoopControl++;
            }
            else if (slowLoopControl == 3) {
                // temp loop every 2 sec
                tempLoop();
                slowLoopControl = 0;
            }
            else {
                slowLoopControl++;
            }

        }

        // Cooler fan control
        if (fanCtlOn == 1) {
            if (temp_pcb >= 25 && fanCooler == FAN_COOLER_OFF)
                fanCooler = FAN_COOLER_ON;
            else if (temp_pcb <= 24 && fanCooler == FAN_COOLER_ON)
                fanCooler = FAN_COOLER_OFF;
            // In case of I2C1 fail turn on the fan
            if ((proxy_f == 0xFFFF) && (proxy_fr == 0xFFFF) && (proxy_br == 0xFFFF) && (proxy_b == 0xFFFF) && (proxy_bl == 0xFFFF) && (proxy_fl == 0xFFFF))
                fanCooler = FAN_COOLER_ON;
        }
        else if (fanCtlOn == 2)
            fanCooler = FAN_COOLER_ON;
        else
            fanCooler = FAN_COOLER_OFF;

        //TEST
        temp_f = temp_model;
//        if (temp_ref < 30) {
//            temp_r = smc_parameters[0] * 10;
//        }
//        else {
//            temp_r = smc_parameters[0] / 2.0 * 10.0;
//        }
        temp_r = alpha*10;
        temp_b = sigma_m * 10;
        temp_l = sigma * 10;

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

void tempLoop() {
    int i = 0;
    // Cooler temperature
    adt7420ReadTemp(&temp_pcb, ADT74_I2C_ADD_mainBoard);
    muxCh = I2C1ChSelect(1, 6);
    adt7420ReadTemp(&temp_flexPCB, ADT74_I2C_ADD_flexPCB);

    if (tempSensors > 0) {
        // We have at least one temperature sensor working
        // Peltier controlled

        if (dma_spi2_started == 1) {
            // error - something wrong with dma, spi or timers!?
            // this should never happen
            // if this happens, use old temp values
        }
        else {
            if (statusTemp[0] == 1)
                adt7320ReadTemp(tSlaveF, &temp_f);
            if (statusTemp[1] == 1)
                adt7320ReadTemp(tSlaveR, &temp_r);
            if (statusTemp[2] == 1)
                adt7320ReadTemp(tSlaveB, &temp_b);
            if (statusTemp[3] == 1)
                adt7320ReadTemp(tSlaveL, &temp_l);
        }
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
    else
        tempNum = 0;

    if (tempNum > 0)
        temp_casu /= tempNum;
    else
        temp_casu = temp_casu1;

    // Wax temperature estimation - PT1, Tustin discretization:
    // y(k) = Kf1 * y(k-1) + Kf2 * u(k) + Kf3 * u(k-1)
    temp_wax = Kf1 * temp_casu + Kf2 * temp_casu1 + Kf3 * temp_wax1;
    temp_wax1 = temp_wax;
    temp_casu1 = temp_casu;

    // local vars

    // Delay of the model reference for 4 steps
    for (i = 1; i < 4; i++) {
            uref_m[4 - i] = uref_m[3 - i];
    }
    uref_m[0] = temp_ref;

    // Temperature reference model
    if (uref_m[3] > temp_ref_shutdown) {
        temp_model = TempModel(uref_m[3]);
    }

    // Peltier feedback loop
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
                if (controller_type == 0) {
                    ctlPeltier = PeltierPID(temp_ref, temp_wax);
                }
                else {
                    LedUser(100,0,0);
                    // SMC parameter adaptation alpha & beta
                    if ((alpha > 5.0) && (alpha < 15.0)) {
                        SmcParamAdapt(&smc_parameters[0], temp_model, temp_wax, temp_ref);
                        alpha = smc_parameters[0];
                        beta = smc_parameters[1];
                    }
                    /*if (temp_ref >= 30) {
                        // rough linearization
                        alpha = alpha / 2.0;
                    }*/
                    //Gain scheduling
                    float polyvalue, polycoef[6], alpha_lin;
                    polycoef[0] = 0.0006;
                    polycoef[1] = -0.0131;
                    polycoef[2] = 0.1041;
                    polycoef[3] = -0.2716;
                    polycoef[4] = 0.0028;
                    polycoef[5] = 1.0316;
                    polyvalue = 0;
                    for(i = 0; i < 6; i++) {
                        polyvalue += polycoef[i] * powf(temp_wax - 26, 5 - i);
                    }
                    alpha_lin = alpha / polyvalue;
                    //alpha = alpha/(1+(powf(temp_wax - 26,4)/7000));

                    ctlPeltier = PeltierSMC(temp_ref, temp_wax, alpha_lin, beta);
                }
            }
            if ((temp_casu > 45) || (temp_pcb > 45)) //Check limits
                ctlPeltier = 0;
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
}

void start_acc_acquisition() {

    dma0Start();
    dma_spi2_started = 1;
    chipSelect(aSlaveR);
    CloseTimer3();
    ConfigIntTimer3(T3_INT_ON | ACC_TIMER_PRIORITY);
    OpenTimer3(T3_ON | T3_PS_1_1, ticks_from_us(accPeriod, 1));

}

void fftLoop() {
    int i = 0;
    UINT16 acc_data;
    for(i = 0; i < FFT_BUFF; i++) {
        acc_data = (dma0BufferA[2*i] & 0xFF) + (dma0BufferA[2*i+1] & 0xFF) * 256;
        if (acc_data > 32767)
            source_array[i] = acc_data - 65536;
        else
            source_array[i] = acc_data;
        source_array[i] -= az_b_l;
    }

    FastFourierTransform(&source_array[0], &amplitudes[0], &destination_array[0], &src_array[0], &Twiddles_array[0]);
    find_max_amplitudes();
}

UINT16 find_min_value_index(int array[], UINT16 len) {

    int min_value;
    UINT16 i, min_value_index = 0;

    min_value = array[min_value_index];

    for(i = 1; i < len; i++) {
        if (array[i] < min_value) {
            min_value = array[i];
            min_value_index = i;
        }
    }

    return min_value_index;

}

void switch_int_vars(int *var1, int *var2) {
    int dummy;
    dummy = *var1;
    *var1 = *var2;
    *var2 = dummy;
}

void switch_uint_vars(UINT16 *var1, UINT16 *var2) {
    UINT16 dummy;
    dummy = *var1;
    *var1 = *var2;
    *var2 = dummy;
}


void find_max_amplitudes() {

    int max_amplitudes[MAX_FFT_NUM];
    UINT16 max_frequencies[MAX_FFT_NUM];
    UINT16 i, j , min_value_index;

    for(i = 0; i < MAX_FFT_NUM; i++) {
        max_amplitudes[i] = amplitudes[i];
        max_frequencies[i] = i;
    }

    min_value_index = find_min_value_index(max_amplitudes, MAX_FFT_NUM);

    for (i = MAX_FFT_NUM; i < FFT_BUFF / 2; i ++) {
        if (amplitudes[i] > max_amplitudes[min_value_index]) {
            max_amplitudes[min_value_index] = amplitudes[i];
            max_frequencies[min_value_index] = i;
            min_value_index = find_min_value_index(max_amplitudes, MAX_FFT_NUM);
        }
    }

    for (i = 0; i < MAX_FFT_NUM-1; i++) {
        for (j = i + 1; j < MAX_FFT_NUM; j++) {
            if (max_amplitudes[j] > max_amplitudes[i]) {
                switch_int_vars(&max_amplitudes[i], &max_amplitudes[j]);
                switch_uint_vars(&max_frequencies[i], &max_frequencies[j]);
            }
        }

    }

    for(i = 0; i < MAX_FFT_NUM; i++) {
        vAmp_m[i] = max_amplitudes[i];
        fAmp_m[i] = max_frequencies[i] * delta_freq;
    }
}

unsigned int flagDmaLed=0;
int iMLC = 0;
// Timer 3 interrupt service for reading accelerometer measurements at exactly 2 KHz
void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void)
{
    dma1Start();
    IFS0bits.T3IF = 0;

}

// Timer 4 interrupt service for FFT on accelerometer reading
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(void)
{

    timer4_flag = 1;
    IFS1bits.T4IF = 0;

}


void __attribute__((__interrupt__, no_auto_psv)) _DMA1Interrupt(void) {

    dma_spi_tx_count++;
    IFS0bits.DMA1IF = 0;
}


void __attribute__((__interrupt__, no_auto_psv)) _DMA0Interrupt(void) {

  dma_spi_rx_count++;
  CloseTimer3();
  chipDeselect(accPin);
  dma_spi2_started = 0;
  IFS0bits.DMA0IF = 0;    // Clear the DMA0 Interrupt Flag
}
