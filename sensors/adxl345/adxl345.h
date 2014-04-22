/* 
 * File:   adxl345.h
 * Author: thaus
 *
 * Created on 2014. sije?anj 06, 14:08
 */

#ifndef ADXL345_H
#define	ADXL345_H

#include <Generic.h>
#include <stdio.h>
#include "../../peripheral/gpio/digitalIO.h"
/* Sensors are connected to spi2 */
#include "../../peripheral/spi/spi2.h"

#define ADXL_READ 0x80           // reading register
#define ADXL_WRITE 0             // writing register
#define ADXL_MB 0x60             // reading multiple bytes
#define ADXL_DEVID 0xe5              // device id

/*Read-Write command
 * Example of usage:
 *  reading single byte from reg 0 :                ADXL_COMMAND(0, ADXL_READ)
 *  reading multiple bytes/reg starting from reg 0  ADXL_COMMAND(0, ADXL_READ | ADXL_MB)
 *  writing a byte to register  0                   ADXL_COMMAND(0, ADXL_WRITE)
 */
#define ADXL_COMMAND(reg, options)  ((reg | options) << 8)

enum ADXL_REGISTERS {
    REG_DEVID = 0x00,
    REG_THRESH_TAP = 0x1D,
    REG_OFSX,
    REG_OFSY,
    REG_OFSZ,
    REG_DUR,
    REG_LATENT,
    REG_WINDOW,
    REG_THRESH_ACT,
    REG_THRESH_INACT,
    REG_TIME_INACT,
    REG_ACT_INACT_CTL,
    REG_THRESH_FF,
    REG_TIME_FF,
    REG_TAP_AXES,
    REG_ACT_TAP_STATUS,
    REG_BW_RATE,
    REG_POWER_CTL,
    REG_INT_ENABLE,
    REG_INT_MAP,
    REG_INT_SOURCE,
    REG_DATA_FORMAT,
    REG_DATAX0,
    REG_DATAX1,
    REG_DATAY0,
    REG_DATAY1,
    REG_DATAZ0,
    REG_DATAZ1,
    REG_FIFO_CTL,
    REG_FIFO_STATUS
};

/* Digital pins used as chip select pins for SPI comm*/
extern digitalPin aSlaveF;
extern digitalPin aSlaveR;
extern digitalPin aSlaveB;
extern digitalPin aSlaveL;

int adxl345Init(digitalPin csPin);
int readAccX(digitalPin csPin, int *ax);
int readAccY(digitalPin csPin, int *ay);
int readAccZ(digitalPin csPin, int *az);
int readAccXYZ(digitalPin csPin, int *acc);
int adxl345readID(digitalPin csPin, UINT16 *id);
int adxlReadReg(digitalPin csPin, UINT8 reg, UINT16 *data);

#endif	/* ADXL345_H */

