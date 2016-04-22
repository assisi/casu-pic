/*! \file   adxl345.h
 * \brief Declaration of functions and variables used for SPI communication with ADXL345 digital accelerometer sensor.
 */

#ifndef ADXL345_H
#define	ADXL345_H

#include <Generic.h>
#include <stdio.h>
#include "../../peripheral/gpio/digitalIO.h"
/* Sensors are connected to spi2 */
#include "../../peripheral/spi/spi2.h"

/*! Mask for reading a register.
 */
#define ADXL_READ 0x80
/*! Mask for writing to a register.
 */
#define ADXL_WRITE 0
/*! Mask for reading multiple bytes in s single transmission.
 */
#define ADXL_MB 0x60
/*! ADXL345 default device id.
 */
#define ADXL_DEVID 0xe5

/*! \brief Macro for ADXL345 command format.
 *
 * Example of usage: \n
 *      reading single byte from reg 0:                ADXL_COMMAND(0, ADXL_READ)\n

 *      reading multiple bytes/reg starting from reg 0:  ADXL_COMMAND(0, ADXL_READ | ADXL_MB)\n
 *      writing a byte to register 0:                   ADXL_COMMAND(0, ADXL_WRITE)
 */
#define ADXL_COMMAND(reg, options)  ((reg | options) << 8)

/*! ADXL345 control registers.
 */
enum ADXL_REGISTERS {
    REG_DEVID = 0x00,       /*!< Device id register. */
    REG_THRESH_TAP = 0x1D,  /*!< Tap threshold value register. */
    REG_OFSX,               /*!< X offset value register. */
    REG_OFSY,               /*!< Y offset value register. */
    REG_OFSZ,               /*!< Z offset value register. */
    REG_DUR,                /*!< Tap duration value register. */
    REG_LATENT,             /*!< Tap latency value register. */
    REG_WINDOW,             /*!< Tap window value register. */
    REG_THRESH_ACT,         /*!< Activity threshold value register. */
    REG_THRESH_INACT,       /*!< Inactivity threshold value register. */
    REG_TIME_INACT,         /*!< Inactivity time value register. */
    REG_ACT_INACT_CTL,      /*!< Axis enable control for activity and inactivity detection register. */
    REG_THRESH_FF,          /*!< Free-fall threshold value register. */
    REG_TIME_FF,            /*!< Free-fall time value register. */
    REG_TAP_AXES,           /*!< Axis control for single tap/double tap register. */
    REG_ACT_TAP_STATUS,     /*!< Source of single tap/double tap register. */
    REG_BW_RATE,            /*!< Data rate and power mode control register. */
    REG_POWER_CTL,          /*!< Power saving feature control register. */
    REG_INT_ENABLE,         /*!< Interrupt enable control register. */
    REG_INT_MAP,            /*!< Interrupt mapping control register. */
    REG_INT_SOURCE,         /*!< Source of interrupts register. */
    REG_DATA_FORMAT,        /*!< Data format control register. */
    REG_DATAX0,             /*!< X axis data the least significant byte register. */
    REG_DATAX1,             /*!< X axis data the most significant byte register. */
    REG_DATAY0,             /*!< Y axis data the least significant byte register. */
    REG_DATAY1,             /*!< Y axis data the most significant byte register. */
    REG_DATAZ0,             /*!< Z axis data the least significant byte register. */
    REG_DATAZ1,             /*!< Z axis data the most significant byte register. */
    REG_FIFO_CTL,           /*!< FIFO control register. */
    REG_FIFO_STATUS         /*!< FIFO status register. */
};

/*! \brief Function initializes and enables ADXL345 device.
 * @param  csPin Digital pin used as a SPI chip select pin.
 * @return  0 - Error in SPI communication. \n
 *         -1 - Error in reading device ID. \n
 *         -2 - Error in enabling device. \n
 *           1 - Initialization successfully completed.
 */
int adxl345Init(digitalPin csPin);

/*! \brief Function reads accelerometer X axis value.
 * @param  csPin Digital pin used as a SPI chip select pin.
 * @param  ax Memory location for storing the data.
 * @return 1 - Communication succeeded \n
 *         0 - Communication failed
 */
UINT8 readAccX(digitalPin csPin, int *ax);

/*! \brief Function reads accelerometer Y axis value.
 * @param  csPin Digital pin used as a SPI chip select pin.
 * @param  ay Memory location for storing the data.
 * @return 1 - Communication succeeded \n
 *         0 - Communication failed
 */
UINT8 readAccY(digitalPin csPin, int *ay);

/*! \brief Function reads accelerometer Z axis value.
 * @param  csPin Digital pin used as a SPI chip select pin.
 * @param  ax Memory location for storing the data.
 * @return 1 - Communication succeeded \n
 *         0 - Communication failed
 */
UINT8 readAccZ(digitalPin csPin, int *az);

/*! \brief Function reads accelerometer X,Y,Z axis values in a single SPI transmission.
 * @param  csPin Digital pin used as a SPI chip select pin.
 * @param  acc Memory location for storing the data.
 * @return 1 - Communication succeeded \n
 *         0 - Communication failed
 */
UINT8 readAccXYZ(digitalPin csPin, int *acc);

/*! \brief Function reads device id.
 * @param  csPin Digital pin used as a SPI chip select pin.
 * @param  id  Memory location for storing the data.
 * @return  1 - Communication succeeded. \n
 *          0 - Communication failed.
 */
UINT8 adxl345readID(digitalPin csPin, UINT16 *id);

/*! \brief Function reads a single device register.
 * @param  csPin Digital pin used as a SPI chip select pin.
 * @param  reg Address of the register.
 * @param data Memory location for storing the data.
 * @return 1 - Communication succeeded.
 *         0 - Communication failed.
 */
UINT8 adxlReadReg(digitalPin csPin, UINT8 reg, UINT16 *data);

#endif	/* ADXL345_H */

