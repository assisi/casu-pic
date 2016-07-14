/****************************************************************************/
/* 2015 LARICS - Laboratory for Robotics and Intelligent Control Systems
; Project:	ASSISI CASU v1.0
; Processor:	dsPIC33Exxxx
; Compiler:     -
;============================================================================;
; FILE DESCRIPTION:
; PWM module control firware
; Schematic documentation:
 * "ASSISI_CASU_PIC"
;
; REVISION HISTORY:
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; Date, Author      | Comments on this revision
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; 17.02.2015        | LED function copied here
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
*/

#include "pwm.h"
#include "../casu/initializeHardware.h"
#include "../peripheral/timer/timerFunctions.h"
#include "../peripheral/timer/timer45.h"
#include "../peripheral/timer/timer2.h"

/* Initialize PWM module
 * inputs: none
 * returns: none
 */


void SpeakerSetPwm(int set){
    //PDC1 = PTPER*0.8; // / 200 * (set+100);
}

