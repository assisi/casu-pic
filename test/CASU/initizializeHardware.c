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