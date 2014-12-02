#include "initializeHardware.h"
#include "../../peripheral/timer/timerFunctions.h"
#include "../../peripheral/i2c/i2c2.h"
#include "../../sensors/adxl345/adxl345.h"
#include "../../sensors/adt7320/adt7320.h"
#include "../../peripheral/timer/timer2.h"

digitalPin LED2R = {&TRISCbits, &PORTCbits, 3};
digitalPin LED2G = {&TRISCbits, &PORTCbits, 2};
digitalPin LED2B = {&TRISEbits, &PORTEbits, 4};

//Peltier DAC
digitalPin PELTIER = {&TRISFbits, &PORTFbits, 3};   //SPI select
digitalPin PELTIER_EN = {&TRISAbits, &PORTAbits, 14};

//I2C1 mux reset pin
digitalPin MUXRST = {&TRISAbits, &PORTAbits, 15};

float Kp_t = 0.5;
float Ki_t = 0.0025;
float uOld_t = 0;
float eOld_t = 0;
float deOld_t = 0;
float deadZone_t = 0.25;
int tempCounter = 0;

void setUpPorts() {
    /* Set port configuration */
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;; // all ports digitaL
    delay_t1(20);
    // configure LED2 ports as outputs
    digitalOutput(LED2R);
    digitalOutput(LED2G);
    digitalOutput(LED2B);
    // configure accelerometer SPI chip select ports as outputs
    digitalOutput(aSlaveF);
    digitalHigh(aSlaveF);
    digitalOutput(aSlaveR);
    digitalHigh(aSlaveR);
    digitalOutput(aSlaveB);
    digitalHigh(aSlaveB);
    digitalOutput(aSlaveL);
    digitalHigh(aSlaveL);

    // configure temperature sensor SPI chip select ports as outputs
    digitalOutput(tSlaveF);
    digitalHigh(tSlaveF);
    digitalOutput(tSlaveR);
    digitalHigh(tSlaveR);
    digitalOutput(tSlaveB);
    digitalHigh(tSlaveB);
    digitalOutput(tSlaveL);
    digitalHigh(tSlaveL);

    // configure i2c2 as a slave device with address 0x0b and interrupt priority 1
    I2C2SlaveInit(0x0b, 1);

    //I2C1 mux reset pin
    digitalOutput(MUXRST);

    //OpenTimer2(T2_ON | T2_PS_1_256, ticks_from_ms(200, 256));
    //ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);

}

/*
 * Function implements PI controller for Peltier device.
 * We use global variables for reference and measured temperature.
 * int i - row index
 * int j - column index
 */
int PIDcontroller_t(float temp) {

    float e_t = temp_ref - temp;      // sensor is connected to the right conector
    float de_t = e_t - eOld_t;

    if (e_t > deadZone_t)
        e_t = e_t - deadZone_t;
    else if (e_t < -deadZone_t)
        e_t = e_t + deadZone_t;
    else
        e_t = 0;


    //float u  = uOld[i][j] + Kp * de + Ki * e + Kd * (de - deOld[i][j]);
    float u  = uOld_t + Kp_t * de_t + Ki_t * e_t;

    // rate limiter
    if (u - uOld_t  > 5)
        u = uOld_t + 5;
    else if (u - uOld_t < - 5)
        u = uOld_t - 5;


    if (u > 100)
        u = 100;
    else if (u < -100)
        u = -100;

    uOld_t = u;
    eOld_t = e_t;
    deOld_t = de_t;

    return (int)u;
}

// Timer 2 interrupt service for calculation PID controller
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{
    // Clear Timer 1 interrupt flag
    ClearIntTimer2();
    tempCounter++;
    if (tempCounter == 5) {
        temp_l = 0;
        adt7320ReadTemp(tSlaveL, &temp_l);
        ctlPeltier = PIDcontroller_t(temp_l);
        //PeltierSet(PELTIER, 1, ctlPeltier);
        tempCounter = 0;
    }
}

