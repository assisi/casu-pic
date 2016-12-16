/****************************************************************************/
/* 2014 LARICS - Laboratory for Robotics and Intelligent Control Systems
; Project:  ASSISI CASU v1.3
; Processor:    dsPIC33Exxxx
; Compiler:     -
;============================================================================;
; FILE DESCRIPTION:
; Peltier firmware - peltier driver is controlled with DAC connected with SPI
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
float ymk1 = 0;
float ka1 = 1;

/* PI controller
 * inputs:  ref - reference value
 *          y - feedback value
 *          ka - adaptation gain 
 * returns: uk
 */
int PeltierPID(float ref, float y, float ka){
    float ek, uk, ui, up, Kp1;

    ek = ref - y;

    //uk = Kp*ek + Ki*(uk1 + ek);
    Kp1 = Kp*ka;
    up = Kp1 * ek;
    if (Ki <= 0.00001)
        ui = 0;
    else
        ui = uk1 + Ki * ek;
    
    uk = up + ui;
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

//Peltier temperature model
// Gm(s) = Ym(s)/Uref(s) = 1/(1+75s) - temperature model 
float TempModel(float uref){
    float Km = 0.97561; //40/41; 0.9736; //37/38; Td = 2s
    float ymk; 
    int i;
    //static float ymk1 = 25;
    
    ymk = (1-Km)*uref + Km*ymk1;
    ymk1 = ymk;
    
    return ymk; 
}

//Adaptive gain algorithm
float AdaptiveController(float uref, float y, float ym){
    float ka, em, sign;
    
    em = y - ym; //Process and model error
    
    if((uref-ym)>= 0)
        sign = 1;
    else
        sign = -1;
    
    ka = ka1 - 0.005*em*sign; //gama = 0.005
    ka1 = ka;
    
    
    return ka;
}
