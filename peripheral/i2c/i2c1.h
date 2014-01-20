/* 
 * File:   i2c1.h
 * Author: thaus
 *
 * Created on January 17, 2014, 3:22 PM
 */

#ifndef I2C1_H
#define	I2C1_H

#include <p33Fxxxx.h>
#include <Generic.h>

#define BUFF_SIZE 64
#define WAIT_TIME 100
#define I2C1_DEBUG 1

extern UINT8 rx_buff[BUFF_SIZE], tx_buff[BUFF_SIZE];

UINT8 I2C1SlaveInit(UINT8 address, UINT8 int_priority);

#endif	/* I2C1_H */

