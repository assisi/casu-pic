/*
 * File:   adxl345.h
 * Author: thaus
 *
 * Created on 2014. sijecanj 06, 14:08
 */

#include "adxl345.h"

/* Define and initialize digital pins used as chip select pins*/
digitalPin aSlaveF = {&TRISEbits, &PORTEbits, 8};
digitalPin aSlaveR = {&TRISGbits, &PORTGbits, 12};
digitalPin aSlaveB = {&TRISGbits, &PORTGbits, 2};
digitalPin aSlaveL = {&TRISBbits, &PORTBbits, 9};

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
    int countID = 0;
    for (i = 0; i < 10; i++) {
        // add select bit function
        status = adxl345readID(csPin, &data);
        if (status <= 0) return 0;
        data = data & 0x00FF;
        if (data == ADXL_DEVID) {
            countID++;
        }
    }
    if (countID < 5) {
        return -1;
    }

    /* Enable device*/
    chipSelect(csPin);
    delay_t1(2);
    status = spi2Write(ADXL_COMMAND(REG_POWER_CTL, ADXL_WRITE) | 0x0008);
    chipDeselect(csPin);
    if (status <= 0)    return 0;

    /* Check if writing was succesfull*/
    chipSelect(csPin);
    delay_t1(2);
    status = spi2TransferWord(ADXL_COMMAND(REG_POWER_CTL, ADXL_READ), &data);
    chipDeselect(csPin);
    if (status <= 0)    return 0;
    data = data & 0x00FF;
    if (data != 0x08) return -2;
    
    return 1;
}

/* Function reads device id
 * inputs: csPin - digital pin used for chip select
 *         id - pointer to variable where id is to be store
 * returns: 1 - communication succeeded
 *          0 - communication failed
 */
int adxl345readID(digitalPin csPin, UINT16 *id) {

    UINT8 status;
    chipSelect(csPin);
    delay_t1(2);
    UINT16 data = ADXL_COMMAND(REG_DEVID, ADXL_READ);
    status = spi2TransferWord(data, id);
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
int readAccX(digitalPin csPin, int *ax) {

   UINT16 data;
   UINT8 status;

   chipSelect(csPin);
   delay_t1(2);
   status = spi2TransferWord(ADXL_COMMAND(REG_DATAX0, ADXL_READ), &data);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;
   *ax = data & 0x00FF;
  
   chipSelect(csPin);
   delay_t1(2);
   status = spi2TransferWord(ADXL_COMMAND(REG_DATAX1, ADXL_READ), &data);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;
   *ax = *ax + 256 * (data & 0x00FF);

   if (*ax < 32767) *ax = *ax - 65536;
   
   return 1;
}

/* Function reads accelerometer Y axis value
 * inputs: csPin - digital pin used for chip select
 *         ay - pointer to a variable where data is to be stored
 * returns: 1 - communication succeeded
 *          0 - communication failed
 */
int readAccY(digitalPin csPin, int *ay) {

   UINT16 data;
   UINT8 status;

   chipSelect(csPin);
   delay_t1(2);
   status = spi2TransferWord(ADXL_COMMAND(REG_DATAY0, ADXL_READ), &data);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;
   *ay = data & 0x00FF;

   chipSelect(csPin);
   delay_t1(2);
   status = spi2TransferWord(ADXL_COMMAND(REG_DATAY1, ADXL_READ), &data);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;
   *ay = *ay + 256 * (data & 0x00FF);

   if (*ay < 32767) *ay = *ay - 65536;

   return 1;
}

/* Function reads accelerometer Z axis value
 * inputs: csPin - digital pin used for chip select
 *         az - pointer to a variable where data is to be stored
 * returns: 1 - communication succeeded
 *          0 - communication failed
 */
int readAccZ(digitalPin csPin, int *az) {

   UINT16 data;
   UINT8 status;

   chipSelect(csPin);
   delay_t1(2);
   status = spi2TransferWord(ADXL_COMMAND(REG_DATAZ0, ADXL_READ), &data);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;
   *az = data & 0x00FF;

   chipSelect(csPin);
   delay_t1(2);
   status = spi2TransferWord(ADXL_COMMAND(REG_DATAZ1, ADXL_READ), &data);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;
   *az = *az + 256 * (data & 0x00FF);

   if (*az <= 32767) *az = *az - 65536;

   return 1;
}

/* Function reads accelerometer X, Y and Z axis values in a row, using multiple
 * byte read feature
 * inputs: csPin - digital pin used for chip select
 *         acc - pointer to a variable where data is to be stored
 * returns: 1 - communication succeeded
 *          0 - communication failed
 */
int readAccXYZ(digitalPin csPin, int *acc) {
   
   UINT16 data[7] = {0};
   UINT8 status;

   data[0] = ADXL_COMMAND(REG_DATAX0, ADXL_READ | ADXL_MB);
   chipSelect(csPin);
   delay_t1(2);
   status = spi2TransferBuff(data, 7);
   chipDeselect(csPin);
   if (status <= 0 ) return 0;

   acc[0] = data[1] + 256 * data[2];
   if (acc[0] <= 32767) acc[0] = acc[0] - 65536;

   acc[1] = data[3] + 256 * data[4];
   if (acc[1] <= 32767) acc[1] = acc[1] - 65536;

   acc[2] = data[5] + 256 * data[6];
   if (acc[2] <= 32767) acc[2] = acc[2] - 65536;
   
   return 1;
}

/* Function reads a device register
 * inputs: csPin - digital pin used for chip select
 *         reg - register to be read
 *         data - pointer of the variable where the data is to be stored
 * returns: 1 - communication succeeded
 *          0 - communication failed
 */
int adxlReadReg(digitalPin csPin ,UINT8 reg, UINT16 *data) {
    UINT8 status;
    chipSelect(csPin);
    delay_t1(2);
    status = spi2TransferWord(ADXL_COMMAND(reg, ADXL_READ), data);
    chipDeselect(csPin);
    if (status <= 0) return 0;

    *data = *data & 0x00FF;
    return 1;
}
