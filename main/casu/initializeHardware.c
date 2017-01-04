#include "initializeHardware.h"
#include "../peripheral/timer/timerFunctions.h"
#include "../peripheral/i2c/i2c2.h"
#include "../sensors/adxl345/adxl345.h"
#include "../sensors/adt7320/adt7320.h"
#include "../peripheral/timer/timer2.h"

/*
digitalPin LED2R = {&TRISEbits, &PORTEbits, 6};
digitalPin LED2G = {&TRISCbits, &PORTCbits, 2};
digitalPin LED2B = {&TRISCbits, &PORTCbits, 3};
*/ 

/* Define and initialize digital pins used as chip select pins*/
digitalPin aSlaveR = {&TRISEbits, &PORTEbits, 8};
digitalPin aSlaveF = {&TRISGbits, &PORTGbits, 12};
digitalPin aSlaveB = {&TRISDbits, &PORTDbits, 5};
digitalPin aSlaveL = {&TRISBbits, &PORTBbits, 9};

/*define slaves' chip select pins*/
digitalPin tSlaveF = {&TRISBbits, &PORTBbits, 5};
digitalPin tSlaveL = {&TRISAbits, &PORTAbits, 6};
digitalPin tSlaveB = {&TRISAbits, &PORTAbits, 4};
digitalPin tSlaveR = {&TRISBbits, &PORTBbits, 10};
digitalPin slaveVib = {&TRISDbits, &PORTDbits, 4};


//I2C1 mux reset pin
digitalPin MUXRST = {&TRISAbits, &PORTAbits, 15};

//Peltier bridge inputs
digitalPin PEL_L = {&TRISEbits, &PORTEbits, 0};
digitalPin PEL_H = {&TRISEbits, &PORTEbits, 1};

void setUpPorts() {
    /* Set port configuration */
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;; // all ports digitaL
    delay_t1(20);
    /*
    // configure LED2 ports as outputs
    digitalOutput(LED2R);
    digitalOutput(LED2G);
    digitalOutput(LED2B);
    digitalHigh(LED2R);
    digitalHigh(LED2G);
    digitalHigh(LED2B);
    */
    
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
    digitalOutput(slaveVib);
    digitalHigh(slaveVib);

    
    //I2C1 mux reset pin
    digitalOutput(MUXRST);

    digitalOutput(PEL_L);
    digitalOutput(PEL_H);
    digitalLow(PEL_L);
    digitalLow(PEL_H);

    //OpenTimer2(T2_ON | T2_PS_1_256, ticks_from_ms(200, 256));
    //ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_2);
}

/*
 * Function implements PI controller for Peltier device.
 * We use global variables for reference and measured temperature.
 * int i - row index
 * int j - column index
 */
/*
int PIDcontroller_t(float temp) {

    float ek, uk, ui, up;

    //uk = Kp*ek + Ki*(uk1 + ek);
    up = Kp * ek;
    ui = uk1 + Ki * ek;
//    if (ek < 5  && ek > -5)
//        ui = uk1 + Ki * ek;
//    else {
//        ui = 0;
//    }

    uk = up + ui;

    //uk = Kp * ek + uk1 + Ki * ek; // up + ui
    uk1 = ui;

    if(uk > 100) {
        uk = 100;
        uk1 = uk - up;
        if (uk1 < 0)
            uk1 = 0;
    }
    else if(uk < -100) {
        uk = -100;
        uk1 = uk - up;
        if (uk1 > 0)
            uk1 = 0;
    }
    //uk1 = uk;// - Kp*ek;

    return (int)uk;

    float e_t = temp_ref - temp;
    float de_t = e_t - eOld_t;
    float u, up,ui;
    if (e_t > deadZone_t)
        e_t = e_t - deadZone_t;
    else if (e_t < -deadZone_t)
        e_t = e_t + deadZone_t;
    else
        e_t = 0;

    up = Kp_t * e_t;
    ui = ui_old + Ki_t * e_t;
    u  = Kp_t * e_t + Ki_t * e_t;

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
*/
