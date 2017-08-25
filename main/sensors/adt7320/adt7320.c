#include "adt7320.h"

/* Global variable used for storing how many bits are used for conversion
 * It is assumed that all connected sensors use same precision
 * 1 - 16 bits
 * 0 - 13 bits
 */
UINT8 bits16Used;

/* Function for initializing ADT7320, digital temperature sensor with SPI comm
 * inputs:  csPin - chip select pin for temperature sensor
 *          config - configuration register content
 * returns: 0 - if SPI comm failed
 *          -1 - if reading device id failed
 *          -2 - if reading device config register, after setting it, failed
 *          1 - everything set up and ready for use
 * Example of usage: status = adt7320Init(pin, ADT_16_BIT | ADT_CONT_MODE);
 */
int adt7320Init(digitalPin csPin, UINT8 config) {

    digitalOutput(csPin);   // set csPin digital output
    chipDeselect(csPin);    // set csPin high, i.e. deselect slave
    // check if spi2 is enabled
    if (!SPI2STATbits.SPIEN) {
        spi2Init(2, 0);     // mode 2, interrupt disabled
    }

    UINT16 buff[2], i;
    UINT8 status;
    // reset the device by sending 0xFFFFFFFF
    buff[0] = 0xFFFF;
    buff[1] = 0xFFFF;

    chipSelect(csPin);
    status = spi2TransferBuff(buff, 2);
    chipDeselect(csPin);
    if (status <= 0) return 0;

    // wait for some time; should be 500us
    for(i = 0; i < 5000; i++);

    // read device id, expected value 0xc3
    for(i = 0; i <  10; i++) {
        buff[0] = ADT_COMMAND(ADT_REG_ID, ADT_READ) << 8;
        chipSelect(csPin);
        status = spi2TransferBuff(buff, 1);
        chipDeselect(csPin);
        if (status <= 0) return 0;
        if ((buff[0] & 0x00FF) != ADT_DEV_ID) return -1;
    }

    // read status register
    for(i = 0; i <  10; i++) {
        buff[0] = ADT_COMMAND(ADT_REG_STATUS, ADT_READ) << 8;
        chipSelect(csPin);
        status = spi2TransferBuff(buff, 1);
        chipDeselect(csPin);
        if (status <= 0) return 0;
        if ((buff[0] & 0x00FF) != ADT_INIT_STATUS) return -1;
    }


     // the device can now be enabled and configured
     buff[0] = (ADT_COMMAND(ADT_REG_CONFIG, ADT_WRITE) << 8) | config;
     chipSelect(csPin);
     status = spi2TransferBuff(buff, 1);
     chipDeselect(csPin);
     if (status <= 0) return 0;

     // read config register to check changes
     buff[0] = (ADT_COMMAND(ADT_REG_CONFIG, ADT_READ) << 8);
     chipSelect(csPin);
     status = spi2TransferBuff(buff, 1);
     chipDeselect(csPin);
     if (status <= 0) return 0;
     if ((buff[0] & 0x00FF) != config) return -2;
     bits16Used = config & ADT_16_BIT;
     return 1;
}

/* Function for reading adt7320 temperature value
 * inputs : csPin - chip select pin for temperature sensor
 *          temp - pointer to a float variable where the value will be stored
 * returns: 0 - is the SPI comm fails
 *          1 - else
 * Exaple of usage status = adt7320ReadTemp(&temp)
 */
int adt7320ReadTemp(digitalPin csPin, float *temp) {

    UINT16 buff[2] = {0};
    UINT8 status;

    buff[0] = ADT_COMMAND(ADT_REG_TEMP_VAL, ADT_READ) << 8;
    chipSelect(csPin);
    status = spi2TransferBuff(buff, 2);
    chipDeselect(csPin);
    if (status <= 0) return 0;
    buff[1] = (buff[1] >> 8) | ((buff[0] & 0x00FF) << 8);
    if (bits16Used) {
        // 16 bit mode
        if (buff[1] & 0x8000)  {
            // negative temp
            buff[1] = (buff[1] & 0x7FFF) - 0x8000;
        }
        *temp = buff[1] / 128.0;
    }
    else {
        if (buff[1] & 0x1000) {
            // neg temp
            buff[1] = (buff[1] & 0xeFFF) - 0x1000;
        }
        *temp = buff[1] / 16.0;
    }

    return 1;
}
