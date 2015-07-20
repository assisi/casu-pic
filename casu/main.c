/*! \file main.c
 * \brief Main function of the CASU firmware.
 */

#define ACC_RATE 2000   // Hz
#define PI 3.141593

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
#include "../sensors/vcnl4000/proxiVCNL4000.h"
#include "../fft/fft.h"
#include "../peripheral/timer/timer2.h"

int ax = 0, ay = 0, az = 0;
int mainLoopCount = 0;
float ax_b_l = 0, ay_b_l = 0, az_b_l = 0;
float ax_b_r = 0, ay_b_r = 0, az_b_r = 0;

float vibe_period = 100; // 100 ms
float time = 0;
float motor_pwm = 0;
int time_int = 0;
UINT16 data = 0;

/****FFT****/
int k=0, str_count = 0, while_count = 0, enable_i2c2 = 0;
static fractcomplex Twiddles_array[FFT_BUFF/2] __attribute__ ((space(xmemory), far, aligned (FFT_BUFF * 2    )));
static fractcomplex      src_array[FFT_BUFF]   __attribute__ ((space(ymemory), far, aligned (FFT_BUFF * 2 * 2)));
//int source_array[FFT_BUFF] = {0};
fractcomplex destination_array[FFT_BUFF] = {0};
//int amplitudes[FFT_BUFF] = {0};
int flag123 = 0;
//ax_b_l = 0, ay_b_l = 0, az_b_l = 0;
//float ax_b_r = 0, ay_b_r = 0, az_b_r = 0;

digitalPin accPin;
UINT16 accPeriod;
UINT16 countLoop;

/*! \brief Selects internal FRC at POR.
*/
_FOSCSEL(FNOSC_FRC & IESO_OFF);
/*! \brief Enables clock switching and configures POSC in XT mode.
*/
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT);
/*! \brief Selects I2C1 alternate pins.
*/
_FPOR(ALTI2C2_OFF & ALTI2C1_ON);
/*! \brief Configures watchdog timer -> Twtd = PR*POST/32000 [s].
*/
_FWDT(FWDTEN_ON & WDTPRE_PR128 & WDTPOST_PS1024);   //Twdt ~ 4s

/*! \brief Main function of the CASU firmware.
 *
 * Initializes oscillators, peripheral devices, sensors and actuators.
 * There are three loops that run at different rates.
 * The fastest loop runs at 1 kHz and is responsible for taking accelerometer measurements (inside timer2 interrupt routine).
 * The second loop executes after 256 accelerometer samples are collected (approximatelly every 295 ms), executes FFT, I2C communication with Single-Board Computer (SBC), proximity readings.
 * The third loop runs at 1 Hz, reads temperature values and executes temperature controller.
 */

/*
 * Measured timings:
 * Total loop timing = 295 ms
 * Code execution timing = 40ms
 * FFT (5) + sorting (5) 10 ms
 * everything else 30 ms
 * proxi read 28 ms!!!
 */

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
    UINT8 error = 0;
    int status = 0;
    countLoop = 0;

    setUpPorts();
    I2C2SlaveInit(0x11, 1);

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
    TwidFactorInit(LOG2_FFT_BUFF, &Twiddles_array[0],0);

    // read 100 values to calculate bias
    int m;
    int n = 0;
    for (m = 0;m < 100; m++) {

        status = readAccXYZ(accPin, &ax, &ay, &az);
        if (status <= 0) {
            //
        }
        ax_b_l += ax;
        ay_b_l += ay;
        az_b_l += az;
        n++;
        delay_t1(1);
    }

    ax_b_l /= n;
    ay_b_l /= n;
    az_b_l /= n;

    _SI2C2IE = 0;
    _SI2C2IF = 0;
    OpenTimer2(T2_ON| T2_PS_1_1, ticks_from_us(accPeriod, 1));
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);

    LedBee(0, 0, 0);
    LedUser(0, 0, 0);
    VibrationSet(0);
    FanCooler(0);
    //PeltierSetPwm(50);
    ax = 0;

    while(1) {

        if (mainLoopCount == FFT_BUFF) {
            FastFourierTransform(&source_array[0], &amplitudes[0], &destination_array[0], &src_array[0], &Twiddles_array);
            /*
             * FOSC = 40 Mhz
             *      FFT_BUFF = 256: t(fft) = 20 ms
             *      FFT_BUFF = 512: t(fft) = 40 ms
             * FOSC = 40 Mhz
             *      FFT_BUFF = 256: t(fft) = 10 ms
             *      FFT_BUFF = 512: t(fft) = 23 ms
             */
            updateAccLog();
            _SI2C2IE = 1;

            while (i2c2_tx_ready == 1) {
                delay_t1(10);
                ax++;
                if (ax == 500) {
                    break;
                    i2c2_tx_ready = 0;
                }
                ClrWdt();   //Clear watchdog timer
            }
            _SI2C2IE = 0;
            mainLoopCount = 0;

            //countLoop = 0;
            OpenTimer2(T2_ON | T2_PS_1_1, ticks_from_us(accPeriod, 1));
            ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
            LedUser(0, 0, 10);

        }
        else {
            delay_t1(2);
            ClrWdt();   //Clear watchdog timer
        }


    }
    return (EXIT_SUCCESS);
}


// Timer 2 interrupt service for reading acc measurements at exactly 1 KHz
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{
    // Clear Timer 2 interrupt flag
    //ClearIntTimer2();
    IFS0bits.T2IF = 0;
    //if (countLoop < 100)
        //countLoop++;

    //else {
       if (mainLoopCount < FFT_BUFF) {
           readAccZ(accPin, &az);
           az = az - az_b_l;
           source_array[mainLoopCount++] = az;
       }

       else if (mainLoopCount == FFT_BUFF) {

            ConfigIntTimer2(T2_INT_OFF);
            OpenTimer2(T2_OFF | T2_PS_1_1, ticks_from_us(accPeriod, 1));
            LedUser(0, 10, 0);
        }
}


//    motor_pwm = sin(2.0 * PI * time / vibe_period);
//    time = time + 0.5 ; // 0.5 ms
//    if (time >= vibe_period) time = 0;
//    motor_pwm = motor_pwm * 45.0 + 50; // range , 100
//    if (motor_pwm > 95)
//        motor_pwm = 95;
//    else if (motor_pwm < 5)
//        motor_pwm = 5;
//    PeltierSetPwm(motor_pwm);
//    if (time_int <= 4) {
//        motor_pwm = 0;
//        time_int++;
//    }
//    else if (time_int <= 8) {
//        motor_pwm = 100;
//        time_int++;
//    }
//    else {
//        motor_pwm = 100;
//        time_int = 0;
//    }
//}