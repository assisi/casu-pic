/* 
 * File:   adt7320.h
 * Author: thaus
 *
 * Created on 2014. sijecanj 08, 21:12
 * ADT7320 digital temperature sensor firmware
 */

#ifndef ADT7320_H
#define	ADT7320_H

#include "../../peripheral/spi/spi2.h"
#include "../../peripheral/gpio/digitalIO.h"

/*ADT7220 registers*/
enum ADT_REGISTERS {
    ADT_REG_STATUS = 0,
    ADT_REG_CONFIG,
    ADT_REG_TEMP_VAL,
    ADT_REG_ID,
    ADT_REG_TCRIT,
    ADT_REG_THYST,
    ADT_REG_THIGH,
    ADT_REG_TLOW
};

#define ADT_DEV_ID 0xc3                 // default device id
#define ADT_INIT_STATUS 0x80            // default device status register
#define ADT_READ 0x40               // read flag
#define ADT_WRITE 0x00              // write flag

/*Flags for configuration register*/
#define ADT_16_BIT 0b10000000           // 16 bit mode
#define ADT_13_BIT 0                    // 13 bit mode
#define ADT_CONT_MODE 0                 // continuous mode
#define ADT_OS_MODE 0b00100000          // one shot mode
#define ADT_SPS_MODE 0b01000000         // one measure per sec mode
#define ADT_SD_MODE 0b01100000          // shutdown
#define ADT_INT_MODE 0                  // interrupt mode
#define ADT_CT_MODE 0b00010000          // comparator mode
#define ADT_INT_LOW 0                   // interrupt pin polarity LOW
#define ADT_INT_HIGH 0b00001000         // interrupt pin high
#define ADT_CT_LOW 0                    // comparator pin polarity LOW
#define ADT_CT_HIGH 0b00000100          // comparator pin high
#define ADT_FQ_1 0                      // 1 fault for raising int and/or ct
#define ADT_FQ_2 0b00000001             // 2 faults for raising int and/or ct
#define ADT_FQ_3 0b00000010             // 3 faults for raising int and/or ct
#define ADT_FQ_4 0b00000011             // 4 faults for raising int and/or ct

/*Flags for status registers*/
#define ADT_TEMP_READY 0b10000000       // this bit in status register is set when new measurement is available

/*Define commands for selecting ADT7320 slaves*/
#define ADT_SLAVE0_ON PORTDbits.RD0 = 0
#define ADT_SLAVE0_OFF PORTDbits.RD0 = 1

#define ADT_SLAVE1_ON PORTDbits.RD5 = 0
#define ADT_SLAVE1_OFF PORTDbits.RD5 = 1

/*ADT7320 command format*/
#define ADT_COMMAND(reg, readWrite)  ((reg << 3) | readWrite)

extern UINT8 bits16Used;
int adt7320Init(digitalPin csPin, UINT8 config);
UINT8 adt7320ReadTemp(digitalPin csPin, float *temp);

#endif	/* ADT7320_H */


