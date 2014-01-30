/****************************************************************************/
/* 2014 LARICS - Laboratory for Robotics and Intelligent Control Systems
; Project:	ASSISI CASU
; Processor:	dsPIC33E
; Compiler:     -
;============================================================================;
; FILE DESCRIPTION:
; CASU RGB LEDs
; Schematic documentation:
 * "ASSISI_controller_board_v1.3"
 * "ASSISI_CASU05_AmpBoard"
;
; REVISION HISTORY:
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; Date, Author              |	Comments on this revision
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; 23.01.2014                |	Implementation
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
*/


#include "led.h"

/* Initialize user and bee LEDs
 * inputs: none
 * returns: none
 */
void LEDInit(void) {

    unsigned long period = ((unsigned long) FOSC/((unsigned long)FPWM*PWMPRE));
    //Primary Master Time Base
    PTCON2bits.PCLKDIV = 2;     //PWM presceler = 4
    PTPER = (int) period;
    
    //Time base control register
    PTCONbits.PTSIDL = 1;   //PWM time base halts in CPU Idle mode
    PTCONbits.PTEN = 1;     //PWM module enable

    //PWM3 Generator Initialization
    PWMCON3 = 0;    //Edge-aligned mode, master time base, individual duty for both channels
    PHASE3 = PTPER;
    SPHASE3 = PTPER;
    PDC3 = 0;   //PWM1H duty
    SDC3 = 0;   //PWM1L duty
    //PWM I/O control register
    IOCON3bits.PENL = 1;
    IOCON3bits.PENH = 1;
    IOCON3bits.PMOD = 0b11; //Independent PWM mode

    //PWM5 Generator Initialization
    PWMCON5 = 0;    //Edge-aligned mode, master time base, individual duty for both channels
    PHASE5 = PTPER;
    SPHASE5 = PTPER;
    PDC5 = 0;   //PWM1H duty
    SDC5 = 0;   //PWM1L duty
    //PWM I/O control register
    IOCON5bits.PENL = 1;
    IOCON5bits.PENH = 1;
    IOCON5bits.PMOD = 0b11; //Independent PWM mode

    //PWM6 Generator Initialization
    PWMCON6 = 0;    //Edge-aligned mode, master time base, individual duty for both channels
    PHASE6 = PTPER;
    SPHASE6 = PTPER;
    PDC6 = 0;   //PWM1H duty
    SDC6 = 0;   //PWM1L duty
    //PWM I/O control register
    IOCON6bits.PENL = 1;
    IOCON6bits.PENH = 1;
    IOCON6bits.PMOD = 0b11; //Independent PWM mode
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
    SDC6 = Kled*(unsigned int)red;

    //Green
    if(green > 100) green = 100;
    PDC5 = Kled*(unsigned int)green;

    //Blue
    if(blue > 100) blue = 100;
    SDC3 = Kled*(unsigned int)blue;

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
    PDC3 = Kled*(unsigned int)red;

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
