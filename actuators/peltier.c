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
; Date, Author      |	Comments on this revision
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; 24.01.2014        |	Implementation
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
*/

#include "peltier.h"
#include "../peripheral/spi/spi1.h"

void PeltierInit(digitalPin csPin){

    spi1Init(2, 0);  // mode 2, spi interrupt disabled

    digitalHigh(PELTIER_EN);    //Disable peltier driver
    digitalOutput(csPin);   // set chip select pin as an output pin
    chipDeselect(csPin);    // deselect device
}

/* Set peltier output current
 * inputs:  lShdn (active low) - shut down DAC -> 0 - DAC off, 1 - DAC on
 *          set - peltier current in range [-100 100]% -> Ipeltier = [-Imax Imax]
 * returns: none
 */
void PeltierSet(digitalPin csPin, UINT8 lShdn, int set){

    unsigned int msg, dout;

    lShdn &= 0x01;

    if(set > 100)
        set = 100;
    if(set < -100)
        set = -100;

    set *= DCOEF;
    //dout = DCOEF*set + 1500;
    dout = 1500;
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