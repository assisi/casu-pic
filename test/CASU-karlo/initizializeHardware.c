#include "initializeHardware.h"
#include "../../peripheral/timer/timerFunctions.h"


//Peltier DAC
digitalPin PELTIER = {&TRISFbits, &PORTFbits, 3};   //SPI select
digitalPin PELTIER_EN = {&TRISAbits, &PORTAbits, 14};


void setUpPorts() {
    /* Set port configuration */
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0x0000;; // all ports digitaL
    delay_t1(20);


    digitalOutput(PELTIER_EN);

}

