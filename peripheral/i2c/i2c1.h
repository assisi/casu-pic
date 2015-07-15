/* 
 * File:   i2c1.h
 * Author: thaus
 *
 * Created on January 17, 2014, 3:22 PM
 */

#ifndef I2C1_H
#define	I2C1_H

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif

#include <Generic.h>

#define BUFF_SIZE 64
#define WAIT_TIME 100
#define I2C1_DEBUG 1

#define PCAMUX_ADD  0x70    //PCA9547 mux address

#define I2C1_TIMEOUT    500    //I2C1 timeout Tout = Tcyc*Ninst*I2C1_TIMEOUT
                                //I2C1_TIMEOUT = 500us/(0.025us*40) = 500


extern UINT8 rx_buff[BUFF_SIZE], tx_buff[BUFF_SIZE];

void I2C1Start(void);

char I2C1Idle(void);

void I2C1NotAck(void);

void I2C1Ack(void);

void I2C1Stop(void);

void I2C1Restart(void);

unsigned char I2C1MasterRead(void);

UINT8 I2C1MasterInit(void);

UINT8 I2C1SlaveInit(UINT8 address, UINT8 int_priority);

char I2C1ChSelect(UINT8 enable, UINT8 channel);

void muxReset();

int MUXTest();

UINT8 I2C1WriteByte(UINT8 slaveAdd, UINT8 registerAdd, UINT8 data);

unsigned char I2C1ReadByte(UINT8 slaveAdd, UINT8 registerAdd);

void __attribute__((__interrupt__, auto_psv)) _SI2C1Interrupt(void);

#endif	/* I2C1_H */

