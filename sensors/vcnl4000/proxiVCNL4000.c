/****************************************************************************/
/* 2014 LARICS - Laboratory for Robotics and Intelligent Control Systems
; Project:	ASSISI CASU v1.3
; Processor:	dsPIC33Exxxx
; Compiler:     -
;============================================================================;
; FILE DESCRIPTION:
; Proximity sensors are connected on I2C bus through I2C mux
; Schematic documentation:
 * "ASSISI_controller_board_v1.3"
 * "ASSISI_Connector_Board12"
 * "ASSISI_CASU_03_top"
;
; REVISION HISTORY:
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; Date, Author      |	Comments on this revision
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; 27.01.2014        |	Implementation
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
*/

#include <i2c.h>
#include "../../peripheral/i2c/i2c1.h"
#include "../vcnl4000/proxiVCNL4000.h"
#include "../../peripheral/timer/timerFunctions.h"


char VCNL4000Init(void){

    unsigned char deviceID;
    char res, muxCh;
    int i,j;

    res = 0;
    for (i = 0; i <= 6; i++) {

        muxCh = I2C1ChSelect(1,i);
        //res = MuxRead();
        deviceID = 0; j = 0;
        while ((deviceID != 0x11)&(j < 5)) {
            deviceID = I2C1ReadByte(VCNL4000_Address, PRODID);
            j++;
        }
        
        if (deviceID == 0x11) {
            I2C1WriteByte(VCNL4000_Address, IRLED3, 20);
            I2C1WriteByte(VCNL4000_Address, ALIGH4, 0x0F);
            I2C1WriteByte(VCNL4000_Address, PRMEA9, 0);
            I2C1WriteByte(VCNL4000_Address, PRMOD10, 0x81);
            res++;
        }
    }

    return res;
}



int VCNL4000Light(void)
{
    int resAmbLight = 0;
    int temp = 0;
    int i = 0;

    StartI2C1();
    MasterWriteI2C1(VCNL4000_Address_WR);
    i = 0;
    while(I2C1STATbits.ACKSTAT)
    {
        i++;
        if(i>1000)
        {
            I2C1CONbits.PEN = 1;
            return -1;
        }
    }

    //Send register register location
    MasterWriteI2C1(ALRES5);
    i = 0;
    while(I2C1STATbits.ACKSTAT)
    {
        i++;
        if(i>1000)
        {
            StopI2C1();
            return -1;
        }
    }

    RestartI2C1(); //Send start condition again
    MasterWriteI2C1(VCNL4000_Address_RD);
    i = 0;
    while(I2C1STATbits.ACKSTAT)
    {
        i++;
        if(i>1000)
        {
            StopI2C1();
            return -1;
        }
    }
    temp = MasterReadI2C1();
    NotAckI2C1();
    StopI2C1();
    resAmbLight = temp<<8;  //Write high byte

    StartI2C1();
    MasterWriteI2C1(VCNL4000_Address_WR);
    i = 0;
    while(I2C1STATbits.ACKSTAT)
    {
        i++;
        if(i>1000)
        {
            StopI2C1();
            return -1;
        }
    }

    StartI2C1();
    MasterWriteI2C1(VCNL4000_Address_WR);
    i = 0;
    while(I2C1STATbits.ACKSTAT)
    {
        i++;
        if(i>1000)
        {
            StopI2C1();
            return -1;
        }
    }
    
    //Send register register location
    MasterWriteI2C1(ALRES6);
    i = 0;
    while(I2C1STATbits.ACKSTAT)
    {
        i++;
        if(i>1000)
        {
            StopI2C1();
            return -1;
        }
    }

    RestartI2C1(); //Send start condition again
    MasterWriteI2C1(VCNL4000_Address_RD);
    i = 0;
    while(I2C1STATbits.ACKSTAT)
    {
        i++;
        if(i>1000)
        {
            StopI2C1();
            return -1;
            
        }
    }

    temp = MasterReadI2C1();
    NotAckI2C1();
    StopI2C1();
    resAmbLight += temp;  //Write high byte

    return resAmbLight;
}

//INT16 VCNL4000Light(void)
//{
//    INT16 resAmbLight = 0;
//    INT16 temp = 0;
//    temp = i2cReadByte(VCNL4000_Address, ALRES5);
//    resAmbLight = temp<<8;  //Write high byte
//    temp = i2cReadByte(VCNL4000_Address, ALRES6);
//    resAmbLight += temp;  //Write low byte
//
//    return resAmbLight;
//}

int VCNL4000Proxi(void)
{
    int resProxi = 0;
    int temp = 0;
    temp = I2C1ReadByte(VCNL4000_Address, PRRES7);
    resProxi = temp<<8;  //Write high byte
    temp = I2C1ReadByte(VCNL4000_Address, PRRES8);
    resProxi += temp;  //Write low byte

    return resProxi;
}

int VCNL4000ReadProxi(){
    char data[2];
    char temp, tout;
    int proxValue;
    unsigned char deviceID = 0;
    int i;

    i = 0;
    while((deviceID != 0x11)&(i<5)){
        deviceID = I2C1ReadByte(VCNL4000_Address, PRODID);
        i++;
    }
    if(deviceID != 0x11)
        return -1;

    temp = I2C1ReadByte(VCNL4000_Address, COMM0);
    I2C1WriteByte(VCNL4000_Address, COMM0, temp | 0x08);  // command the sensor to perform a proximity measure
    
    temp = 0; tout = 0;
    while(temp != 0x20) {
        delay_t1(1);
        temp = I2C1ReadByte(VCNL4000_Address,COMM0);
        temp &= 0x20;
        tout++;
        if(tout > 10)
            return -2;
    };

    data[1] = I2C1ReadByte(VCNL4000_Address, PRRES7);
    data[0] = I2C1ReadByte(VCNL4000_Address, PRRES8);
    proxValue = (int)(data[1] << 8) + (int)data[0];

    return proxValue;
}
