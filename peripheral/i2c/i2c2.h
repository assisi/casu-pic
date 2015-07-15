/* 
 * File:   i2c1.h
 * Author: thaus
 *
 * Created on January 17, 2014, 3:22 PM
 */

#ifndef I2C2_H
#define	I2C2_H

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif

#include <Generic.h>

#define I2C2_CASU_ADD 0x12
#define BUFF_SIZE 64
#define WAIT_TIME 100
#define I2C1_DEBUG 1
#define IN_REF_DATA_NUM 12
#define IN_CAL_DATA_NUM 13
#define OUT_MEAS_DATA_NUM 55

// msg ids
#define MSG_RESET_ID 1
#define MSG_CAL_ID 2
#define MSG_REF_ID 3

extern UINT8 i2c2_rx_buff[BUFF_SIZE], i2c2_tx_buff[BUFF_SIZE];
extern UINT8 msg_id;
extern UINT16 msg_rec_bytes;
extern UINT8 msg_status;

UINT8 I2C2SlaveInit(UINT8 address, UINT8 int_priority);

#endif	/* I2C2_H */

