#include "initializeHardware.h"
#include "../../peripheral/timer/timerFunctions.h"

void setUpPorts() {
    AD1PCFGH = 0xFFFF; // all ports digital
    AD1PCFGL = 0xFFFF;  // all ports digital
    delay_t1(20);
    TRISAbits.TRISA0 = 0;    // diagnostic LED on Explorer16
    PORTAbits.RA0 = 1;      // Turn LED on
    TRISDbits.TRISD3 = 0;
    PORTDbits.RD3 = 1;
}

