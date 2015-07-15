/****************************************************************************/
/* 2014 LARICS - Laboratory for Robotics and Intelligent Control Systems
; Project:	ASSISI CASU v1.3
; Processor:	dsPIC33Exxxx
; Compiler:     -
;============================================================================;
; FILE DESCRIPTION:
; Peltier firware - peltier driver is controlled with DAC connected with SPI
; Schematic documentation:
 * "ASSISI_controller_board_v1.3"
 * "ASSISI_CASU05_AmpBoard"
;
; REVISION HISTORY:
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; Date, Author      | Comments on this revision
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; 24.01.2014        | Implementation
; 27.04.2014        | PI controller added and tested on CASU V0.5
; 17.02.2015        | New board design
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
*/

#include "peltier.h"
#include "../peripheral/spi/spi1.h"
#include "pwm.h"
#include "../pic-to-beaglebone/pic2beaglebone.h"

//PI controller global variables and parameters
float uk1 = 0;


/* PI controller
 * inputs:  ref - reference value
 *          y - feedback value
 * returns: uk
 */
int PeltierPID(float ref, float y){
    float ek, uk, ui, up;

    ek = ref - y;

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
}

int PeltierCooler(float tc){

    if(tc > 35)
        FanCooler(100);
    else
        FanCooler(0);
    return 1;

}

void PeltierResetPID(){

    uk1 = 0;
    temp_ref_cur = temp_wax;
}