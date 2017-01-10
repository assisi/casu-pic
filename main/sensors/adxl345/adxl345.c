/*
 * File:   adxl345.h
 * Author: thaus
 *
 * Created on 2014. sijecanj 06, 14:08
 */

#include "adxl345.h"

/* Function initializes and enables adxl345 device
 * inputs: csPin - digital pin used for chip select
 * return: 1 - if everything succeeded
 *         0 - if there was an error with SPI communication
 *         -1 - if device id read is different thed default (factory) id
 *         -2 - if device enabling failed
 */
int adxl345Init(digitalPin csPin) {

     // check if spi2 is enabled
    if (!SPI2STATbits.SPIEN) {
        spi2Init(2, 0);  // mode 2, spi interrupt disabled
    }

    digitalOutput(csPin);   // set chip select pin as an output pin
    chipDeselect(csPin);    // deselect device

    /* Read DEVID register to check device functionality*/
    UINT16 data;
    int i;
    int status;
    for (i = 0; i < 10; i++) {
        // add select bit function
        status = adxl345readID(csPin, &data);
        if (status <= 0) return 0;
        data = data & 0x00FF;
        if (data != ADXL_DEVID) {
            return -1;
        }
    }

    /* Enable device*/
    chipSelect(csPin);
    status = spi2Write(ADXL_COMMAND(REG_POWER_CTL, ADXL_WRITE) | 0x0008);
    chipDeselect(csPin);
    if (status <= 0)    return 0;

    /* Check if writing was succesfull*/
    chipSelect(csPin);
    status = spi2TransferWord(ADXL_COMMAND(REG_POWER_CTL, ADXL_READ), &data);
    chipDeselect(csPin);
    if (status <= 0)    return 0;
    /*data = data & 0x00FF;
    if (data != 0x08) return -2;
    */
    /* Set bandwidth*/
    chipSelect(csPin);
    status = spi2Write(ADXL_COMMAND(REG_BW_RATE, ADXL_WRITE) | 0x0F);  // set 3.2 khz output rate
    chipDeselect(csPin);
    if (status <= 0)    return 0;

    /* Check if writing was succesfull*/
    chipSelect(csPin);
    status = spi2TransferWord(ADXL_COMMAND(REG_BW_RATE, ADXL_READ), &data);
    chipDeselect(csPin);
    if (status <= 0)    return 0;
    /*
    data = data & 0x00FF;
    if (data != 0x0C) return -2;
    */
    /* Set range and full resolution*/
    chipSelect(csPin);
    status = spi2Write(ADXL_COMMAND(REG_DATA_FORMAT, ADXL_WRITE) | 0b00001000);  // set 2g and full resolution
    chipDeselect(csPin);
    if (status <= 0)    return 0;

    /* Check if the writing was succesfull*/
    chipSelect(csPin);
    status = spi2TransferWord(ADXL_COMMAND(REG_DATA_FORMAT, ADXL_READ), &data);
    chipDeselect(csPin);
    if (status <= 0)    return 0;
    /*
    data = data & 0x00FF;
    if (data != 0b00001000) return -2;
    */
    return 1;
}

/* Function reads device id
 * inputs: csPin - digital pin used for chip select
 *         id - pointer to variable where id is to be store
 * returns: 1 - communication succeeded
 *          0 - communication failed
 */
UINT8 adxl345readID(digitalPin csPin, UINT16 *id) {

    UINT8 status;
    chipSelect(csPin);
    status = spi2TransferWord(ADXL_COMMAND(REG_DEVID, ADXL_READ), id);
    chipDeselect(csPin);
    if (status <= 0) return 0;
    return 1;
}

/* Function reads accelerometer X axis value
 * inputs: csPin - digital pin used for chip select
 *         ax - pointer to a variable where data is to be stored
 * returns: 1 - communication succeeded
 *          0 - communication failed
 */
UINT8 readAccX(digitalPin csPin, int *ax) {

   UINT16 data;
   UINT8 status;

   chipSelect(csPin);
   status = spi2TransferWord(ADXL_COMMAND(REG_DATAX0, ADXL_READ), &data);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;
   *ax = data & 0x00FF;

   chipSelect(csPin);
   status = spi2TransferWord(ADXL_COMMAND(REG_DATAX1, ADXL_READ), &data);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;
   *ax = *ax + 256 * (data & 0x00FF);

   if (*ax > 32767) *ax = *ax - 65536;

   return 1;
}

/* Function reads accelerometer Y axis value
 * inputs: csPin - digital pin used for chip select
 *         ay - pointer to a variable where data is to be stored
 * returns: 1 - communication succeeded
 *          0 - communication failed
 */
UINT8 readAccY(digitalPin csPin, int *ay) {

   UINT16 data;
   UINT8 status;

   chipSelect(csPin);
   status = spi2TransferWord(ADXL_COMMAND(REG_DATAY0, ADXL_READ), &data);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;
   *ay = data & 0x00FF;

   chipSelect(csPin);
   status = spi2TransferWord(ADXL_COMMAND(REG_DATAY1, ADXL_READ), &data);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;
   *ay = *ay + 256 * (data & 0x00FF);

   if (*ay > 32767) *ay = *ay - 65536;

   return 1;
}

/* Function reads accelerometer Z axis value
 * inputs: csPin - digital pin used for chip select
 *         az - pointer to a variable where data is to be stored
 * returns: 1 - communication succeeded
 *          0 - communication failed
 */
UINT8 readAccZ(digitalPin csPin, int *az) {

   UINT16 data;
   UINT8 status;

   chipSelect(csPin);
   status = spi2TransferWord(ADXL_COMMAND(REG_DATAZ0, ADXL_READ), &data);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;
   *az = data & 0x00FF;

   chipSelect(csPin);
   status = spi2TransferWord(ADXL_COMMAND(REG_DATAZ1, ADXL_READ), &data);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;
   *az = *az + 256 * (data & 0x00FF);

   if (*az > 32767) *az = *az - 65536;

   return 1;
}

/* Function reads accelerometer X, Y and Z axis values in a row, using multiple
 * byte read feature
 * inputs: csPin - digital pin used for chip select
 *         acc - pointer to a variable where data is to be stored
 * returns: 1 - communication succeeded
 *          0 - communication failed
 */
UINT8 readAccXYZ(digitalPin csPin, int *ax, int *ay, int *az) {

   UINT16 data[7] = {0};
   UINT8 status;

   data[0] = ADXL_COMMAND(REG_DATAX0, ADXL_READ | ADXL_MB);
   chipSelect(csPin);
   status = spi2TransferBuff(data, 4);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;

   *ax = (data[0] & 0x00FF) | (data[1] & 0xFF00);
   if (*ax > 32767) *ax = *ax - 65536;

   *ay = (data[1] & 0x00FF) | (data[2] & 0xFF00);
   if (*ay > 32767) *ay = *ay - 65536;

   *az = (data[2] & 0x00FF) | (data[3] & 0xFF00);
   if (*az > 32767) *az = *az - 65536;

   status = 1;

   return 1;
}

/* Function reads a device register
 * inputs: csPin - digital pin used for chip select
 *         reg - register to be read
 *         data - pointer of the variable where the data is to be stored
 * returns: 1 - communication succeeded
 *          0 - communication failed
 */
UINT8 adxlReadReg(digitalPin csPin ,UINT8 reg, UINT16 *data) {
    UINT8 status;
    chipSelect(csPin);
    status = spi2TransferWord(ADXL_COMMAND(reg, ADXL_READ), data);
    chipDeselect(csPin);
    if (status <= 0) return 0;

    *data = *data & 0x00FF;
    return 1;
}