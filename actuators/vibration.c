/****************************************************************************/
/* 2014 LARICS - Laboratory for Robotics and Intelligent Control Systems
; Project:	ASSISI CASU v1.3
; Processor:	dsPIC33Exxxx
; Compiler:     -
;============================================================================;
; FILE DESCRIPTION:
; Vibration motor firware
; Schematic documentation:
 * "ASSISI_controller_board_v1.3"
 * "ASSISI_CASU05_AmpBoard"
;
; REVISION HISTORY:
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; Date, Author      |	Comments on this revision
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; 24.01.2014        |	Implementation
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
*/

#include "vibration.h"
#include "../test/CASU-karlo/initializeHardware.h"
#include "../actuators/led.h"

/* Initialize vibration motor peripheral
 * inputs: none
 * returns: none
 */
void VibrationInit(void) {

    //Use same period as LED PWM from "led.h"
    unsigned long period = ((unsigned long) FOSC/((unsigned long)FPWM*PWMPRE));

    if(PTPER != period){ //Check if master period is set
        //Primary Master Time Base
        PTCON2bits.PCLKDIV = 2;     //PWM presceler = 4
        PTPER = (int) period;
    }
    //Time base control register
    PTCONbits.PTSIDL = 1;   //PWM time base halts in CPU Idle mode
    PTCONbits.PTEN = 1;     //PWM module enable

    //PWM2 Generator Initialization
    PWMCON2 = 0;    //Edge-aligned mode, master time base, individual duty for both channels
    PHASE2 = PTPER;
    SPHASE2 = PTPER;
    SDC2 = 0;   //PWM1L duty
    //PWM I/O control register
    IOCON2bits.PENL = 1;
    IOCON2bits.PENH = 0;    //Not used
    IOCON2bits.PMOD = 0b11; //Independent PWM mode

}

/* Set vibration motor speed
 * inputs: UINT8 set - [0 100]% vibration motor speed
 * returns: none
 */
void VibrationSet(UINT8 set)
{
    unsigned int K = PTPER/100;  //Multiplication factor

    if(set > 100)
        set = 100;

    SDC2 = K*(unsigned int)set;

    return;
}
