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
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
*/

#include "peltier.h"
#include "../peripheral/spi/spi1.h"

//PI controller global variables and parameters
float uk1 = 0;
float Kp = 20; //8
float Ki = 0.6; //0.9;
int PELTIER_PWM_MAX_P = 60;
int PELTIER_PWM_MAX_N = 30;
/* Initalization of Peltier control pins
 * inputs:  csPin - Multiplexor SPI chip select pin
 * returns:
 */
void PeltierInit(digitalPin csPin){

    spi1Init(2, 0);  // mode 2, spi interrupt disabled

    digitalOutput(PELTIER_EN);
    digitalLow(PELTIER_EN);    //Disable peltier driver
    digitalOutput(csPin);   // set chip select pin as an output pin
    chipDeselect(csPin);    // deselect device

    uk1 = 0; //Controller variable
}

/* Set peltier output current
 * inputs:  lShdn (active low) - shut down DAC -> 0 - DAC off, 1 - DAC on
 *          set - peltier current in range [-100 100]% -> Ipeltier = [-Imax Imax]
 * returns: none
 * test: Resistor 2 Ohm
 * dout = 1150 -> Iout = -1.4A
 * dout = 1350 -> Iout = -0.56A
 * dout = 1400 -> Iout = -0.36A
 * dout = 1500 -> Iout = 0A
 * dout = 1550 -> Iout = 0.15A
 * dout = 1700 -> Iout = 0.57A
 * dout = 1800 -> Iout = 1A
 * dout = 2000 -> Iout = 1.5A
 */
void PeltierSet(digitalPin csPin, UINT8 lShdn, int set){

    unsigned int msg, dout;

    lShdn &= 0x01;

    if(set > PELTIER_PWM_MAX_P)
        set = PELTIER_PWM_MAX_P;
    if(set < -PELTIER_PWM_MAX_N)
        set = -PELTIER_PWM_MAX_N;

    //set *= DCOEF;
    dout = set*5 + REF_OFFSET;    //dout = DCOEF*set + 1500;
    msg = (lShdn << 12) | (DAC_G << 13) | (dout & 0xFFF);
    /* Enable device*/
    chipSelect(csPin);
    spi1Write(msg);
    chipDeselect(csPin);

    if(set == 0)
        digitalLow(PELTIER_EN);    //Turn off driver
    else
        digitalHigh(PELTIER_EN);     //Turn on driver
    return;
}

/* PI controller
 * inputs:  ref - reference value
 *          y - feedback value
 * returns: uk
 */
int PeltierPID(float ref, float meas){
    float ek, uk;

    ek = ref - meas;
    //uk = Kp*ek + Ki*(uk1 + ek);
    uk = Kp * ek + uk1 + Ki * ek; // up + ui

    if(uk > 100) {
        uk = 100;
    }
    else if(uk < -100) {
        uk = -100;
    }

    uk1 = uk - Kp*ek;

    return (int)uk;
}
