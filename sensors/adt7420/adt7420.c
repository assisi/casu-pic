/****************************************************************************/
/* 2014 LARICS - Laboratory for Robotics and Intelligent Control Systems
; Project:	ASSISI CASU v1.3
; Processor:	dsPIC33Exxxx
; Compiler:     -
;============================================================================;
; FILE DESCRIPTION:
; ADT7420 I2C temperature device firmware
;
; REVISION HISTORY:
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; Date, Author      |	Comments on this revision
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; 24.04.2014        |	Implementation
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
*/

#include "adt7420.h"
#include "../../peripheral/i2c/i2c1.h"

/* Function for initializing ADT7420, digital temperature sensor with I2C comm
 * inputs:  config - configuration register content
 *          address - i2c address
 * returns: 0 - if I2C comm failed
 *          -1 - if reading device id failed
 *          -2 - if reading device config register, after setting it, failed
 *          1 - everything set up and ready for use
 * Example of usage: status = adt7420Init(ADT74_16_BIT | ADT74_CONT_MODE);
 */
int adt7420Init(UINT8 config, UINT8 address) {
    
    unsigned char rec = 0;
    int i = 0;
    
    while((ADT74_ID != rec) && (i<5)){    //Address failed
         rec = I2C1ReadByte(address, ADT74_ID_REG);
         i++;
    }

    //Set configuration register - uncomment if you want to change configuration register
   I2C1WriteByte(address, ADT74_CONFIG, config);
   rec = I2C1ReadByte(address, ADT74_CONFIG);
   if(rec != config)   //Reading from congfiguration register failed
        return -2;
        
   return 1;
}

/* Function for reading adt7320 temperature value
 * inputs : csPin - chip select pin for temperature sensor
 *          temp - pointer to a float variable where the value will be stored
 * returns: 0 - 
 *          1 - else
 * Exaple of usage status = adt7320ReadTemp(&temp)
 */
int adt7420ReadTemp(float *temp, UINT8 address) {

    UINT8 buffer[2];
    int res;

    buffer[0] = I2C1ReadByte(address, ADT74_TLOW);
    buffer[1] = I2C1ReadByte(address, ADT74_THIGH);
  
    res = (buffer[0] & 0xFF) + (buffer[1] << 8);
    res >>= 3;  //Results is stored on [15:3] position

    if(res & 0x1000)  //Negative
        res -= 8192;

    *temp = (float)res/16;

    return 1;
}
