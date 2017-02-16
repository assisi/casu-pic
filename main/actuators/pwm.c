/****************************************************************************/
/* 2015 LARICS - Laboratory for Robotics and Intelligent Control Systems
; Project:  ASSISI CASU v1.0
; Processor:    dsPIC33Exxxx
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
void PWMInit(void) {

    PTCONbits.PTEN = 0;     //PWM module disable
    unsigned long period = ((unsigned long) FOSC/((unsigned long)FPWM*PWMPRE));
    //Primary Master Time Base
    PTCON2bits.PCLKDIV = 0;     //PWM presceler = 4
    PTPER = (int) period/10;

    //PWM1 Generator Initialization
    DTR1 = 15;
    ALTDTR1 = 15;
    PWMCON1 = 0;    //Edge-aligned mode, master time base, individual duty for both channels
    PHASE1 = PTPER;
    SPHASE1 = PTPER;
    PDC1 = PTPER/2;   //PWM1H duty
    //SDC1 = PTPER/2;   //PWM1L duty
    //PWM I/O control register
    IOCON1bits.PENL = 1;
    IOCON1bits.PENH = 1;
    IOCON1bits.PMOD = 0b00; //Complementary PWM mod

    //PWM2 Generator Initialization - Vibration motor
    PWMCON2 = 0;    //Edge-aligned mode, master time base, individual duty for both channels
    PHASE2 = PTPER;
    SPHASE2 = PTPER;
    SDC2 = 0;   //duty
    //PWM I/O control register
    IOCON2bits.PENL = 0;
    IOCON2bits.PENH = 0;    //Not used
    IOCON2bits.PMOD = 0b11; //Independent PWM mode

    //PWM3 Generator Initialization - Fans
    PWMCON3 = 0;    //Edge-aligned mode, master time base, individual duty for both channels
    PHASE3 = PTPER;
    SPHASE3 = PTPER;
    PDC3 = 0;   //PWM3H duty
    SDC3 = 0;   //PWM3L duty
    //PWM I/O control register
    IOCON3bits.PENL = 1;
    IOCON3bits.PENH = 1;
    IOCON3bits.PMOD = 0b11; //Independent PWM mode

    //PWM4 Generator Initialization
    PWMCON4 = 0;    //Edge-aligned mode, master time base, individual duty for both channels
    PHASE4 = PTPER;
    SPHASE4 = PTPER;
    DTR4 = 0;
    ALTDTR4 =0;
    PDC4 = 0;   //PWM4H duty
    SDC4 = PTPER;   //PWM4L duty
    //PWM I/O control register
    IOCON4bits.PENL = 0;
    IOCON4bits.PENH = 0;
    IOCON4bits.PMOD = 0b11; //Independent PWM mode

    //PWM5 Generator Initialization
    PWMCON5 = 0;    //Edge-aligned mode, master time base, individual duty for both channels
    PHASE5 = PTPER;
    SPHASE5 = PTPER;
    DTR5 = 0;
    ALTDTR5 =0;
    PDC5 = PTPER;   //PWM1H duty
    SDC5 = 0;   //PWM1L duty
    //PWM I/O control register
    IOCON5bits.PENL = 0;
    IOCON5bits.PENH = 0;
    IOCON5bits.PMOD = 0b11; //Independent PWM mode

    //PWM6 Generator Initialization
    PWMCON6 = 0;    //Edge-aligned mode, master time base, individual duty for both channels
    PHASE6 = PTPER;
    SPHASE6 = PTPER;
    DTR6 = 0;
    ALTDTR6 =0;
    PDC6 = 0;   //PWM1H duty
    SDC6 = PTPER;   //PWM1L duty
    //PWM I/O control register
    IOCON6bits.PENL = 0;
    IOCON6bits.PENH = 0;
    IOCON6bits.PMOD = 0b11; //Independent PWM mode

    //Time base control register
    PTCONbits.PTSIDL = 1;   //PWM time base halts in CPU Idle mode
    PTCONbits.PTEN = 1;     //PWM module enable
}

/* Set RGB LED value
 * inputs: red - [0 100]%
 *         blue - [0 100]%
 *         green - [0 100]%
 * returns: none
 */
void LedUser(UINT8 red, UINT8 green, UINT8 blue)
{
    unsigned int Kled = PTPER/100;  //Multiplication factor

    //Red
    if(red > 100) red = 100;
    if(red == 0)
        IOCON4bits.PENL = 0;
    else    {
        IOCON4bits.PENL = 1;
        SDC4 = Kled*(unsigned int)(100-red);
    }


    //Green
    if(green > 100) green = 100;

    if(green == 0)
        IOCON5bits.PENH = 0;
    else    {
        IOCON5bits.PENH = 1;
        PDC5 = Kled*(unsigned int)(100-green);
    }
    //Blue

    if(blue > 100) blue = 100;

    if(blue == 0)
        IOCON6bits.PENL = 0;
    else    {
        IOCON6bits.PENL = 1;
        SDC6 = Kled*(unsigned int)(100-blue);
    }
    return;
}

/* Set RGB LED bee value
 * inputs: red - [0 100]%
 *         blue - [0 100]%
 *         green - [0 100]%
 * returns: none
 */
void LedBee(UINT8 red, UINT8 green, UINT8 blue)
{
    unsigned int Kled = PTPER/100;  //Multiplication factor
    //Red
    if(red > 100)
        red = 100;
    PDC4 = Kled*(unsigned int)red;

    //Green
    if(green > 100)
        green = 100;
    PDC6 = Kled*(unsigned int)green;

    //Blue
    if(blue > 100)
        blue = 100;
    SDC5 = Kled*(unsigned int)blue;
    return;
}

/* Set cooler fan output voltage
 * inputs: set - [0 100]%
 * returns: none
 */
void FanCooler(UINT8 set)
{
    unsigned int K = PTPER/100;  //Multiplication factor

    if(set < 0)
        set = 0;
    if(set > 100)
        set = 100;

    SDC3 = K*(unsigned int)set;

    return;
}

/* Set blower fan output voltage
 * inputs: set - [0 100]%
 * returns: none
 */
void FanBlower(UINT8 set)
{
    unsigned int K = PTPER/100;  //Multiplication factor

    if(set < 0)
        set = 0;
    if(set > 100)
        set = 100;

    PDC3 = K*(unsigned int)set;

    return;
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


/* Set peltier pwm output current
 * inputs:  setVoltage - output voltage in range [-100 100]%
 * returns: none
 */
void PeltierVoltageSet(float setVoltage){

    float VabsMax = 80;
    if (setVoltage > VabsMax)
        setVoltage = VabsMax;
    else if (setVoltage < -VabsMax)
        setVoltage = - VabsMax;

    float dummy = (float) PTPER;
    PDC1 = (int)(dummy * (setVoltage + 100.0) / 200.0);
    return;
}

void PeltierSetPwm(int set){

    unsigned int K = PTPER/100;
    static int set_s;

    if(set > PELTIER_PWM_MAX_P)
        set = PELTIER_PWM_MAX_P;
    if(set < -PELTIER_PWM_MAX_N)
        set = -PELTIER_PWM_MAX_N;

    //Sign check
    if((set_s > 0) && (set < 0))
        delay_t1(5);
    if((set_s < 0) && (set > 0))
        delay_t1(5);

    set_s = set;    //Save old value

    if(set > 0){
        PDC1 = K*set;
        SDC1 = 0;
    }
    else if(set < 0){
        PDC1 = 0;
        set *= -1;
        SDC1 = K*set;
    }
    else{
        PDC1 = 0;
        SDC1 = 0;
    }

    return;
}

//Peltier bridge off
void PeltierOff(){
    digitalLow(PEL_L);
    digitalLow(PEL_H);
}

//Peltier bridge on
void PeltierOn(int direction){

    if(direction > 0){
        digitalLow(PEL_L);
        digitalHigh(PEL_H);
    }
    else{
        digitalLow(PEL_H);
        digitalHigh(PEL_L);
    }
}
