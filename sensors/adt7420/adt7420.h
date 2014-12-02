/* 
 * File:   adt7420.h
 * Author: 
 *
 * Created on 2014. April 24
 * ADT7420 digital temperature sensor firmware
 */

#ifndef ADT7420_H
#define	ADT7420_H

#include "../../peripheral/gpio/digitalIO.h"

#define ADT74_I2C_ADD   0x48    //AD7420 device address
#define ADT74_ID        0xCB    //AD7420 device ID number

//Registers address
#define ADT74_THIGH     0x00    //Temp most significant byte
#define ADT74_TLOW      0x01    //Temp least significant byte
#define ADT74_STATUS    0x02    //Status register
#define ADT74_CONFIG    0x03    //Configuration
#define ADT74_TH_HIGH   0x04    //Thigh most significant byte
#define ADT74_TH_LOW    0x05    //Thigh least significant byte
#define ADT74_TL_HIGH   0x06    //Tlow most significant byte
#define ADT74_TL_LOW    0x07    //Tlow least significant byte
#define ADT74_TCR_HIGH  0x08    //Tcrit
#define ADT74_TCR_LOW   0x09    //Tcrit
#define ADT74_THYST     0x0A    //Thyst setpoint
#define ADT74_ID_REG    0x0B    //Device ID
#define ADT74_SOF_RES    0x2F    //Software reset

/*Flags for configuration register*/
#define ADT74_16_BIT 0b10000000           // 16 bit mode
#define ADT74_13_BIT 0                    // 13 bit mode
#define ADT74_CONT_MODE 0                 // continuous mode
#define ADT74_OS_MODE 0b00100000          // one shot mode
#define ADT74_SPS_MODE 0b01000000         // one measure per sec mode
#define ADT74_SD_MODE 0b01100000          // shutdown
#define ADT74_INT_MODE 0                  // interrupt mode
#define ADT74_CT_MODE 0b00010000          // comparator mode
#define ADT74_INT_LOW 0                   // interrupt pin polarity LOW
#define ADT74_INT_HIGH 0b00001000         // interrupt pin high
#define ADT74_CT_LOW 0                    // comparator pin polarity LOW
#define ADT74_CT_HIGH 0b00000100          // comparator pin high
#define ADT74_FQ_1 0                      // 1 fault for raising int and/or ct
#define ADT74_FQ_2 0b00000001             // 2 faults for raising int and/or ct
#define ADT74_FQ_3 0b00000010             // 3 faults for raising int and/or ct
#define ADT74_FQ_4 0b00000011             // 4 faults for raising int and/or ct

/*Flags for status registers*/
#define ADT74_TEMP_READY 0b10000000       // this bit in status register is set when new measurement is available


UINT8 adt7420Init(UINT8 config);

UINT8 adt7420ReadTemp(float *temp);

#endif	/* ADT7420_H */


