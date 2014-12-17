/* \file adt7320.h
 * Declaration of functions and variables used for SPI communication with ADT7320 digital temperature sensor.
 */

#ifndef ADT7320_H
#define	ADT7320_H

#include "../../peripheral/spi/spi2.h"
#include "../../peripheral/gpio/digitalIO.h"

/*! ADT7320 control registers.
 */
enum ADT_REGISTERS {
    ADT_REG_STATUS = 0, /*!< Status register. */
    ADT_REG_CONFIG,     /*!< Configuration register. */
    ADT_REG_TEMP_VAL,   /*!< Current temperature value. */
    ADT_REG_ID,         /*!< Sensor ID value*/
    ADT_REG_TCRIT,      /*!< Critical overtemperature limit value  */
    ADT_REG_THYST,      /*!< The temperature hysteresis value for the THIGH, TLOW, and TCRIT temperature limits.*/
    ADT_REG_THIGH,      /*!< The overtemperature limit value. */
    ADT_REG_TLOW        /*!< The undertemperature limit value. */
};

/*! Default device ID value.
 */
#define ADT_DEV_ID 0xc3
/*! Default device status register value.
 */
#define ADT_INIT_STATUS 0x80
/*! Read flag.
 */
#define ADT_READ 0x40
/*! Write flag.
 */
#define ADT_WRITE 0x00              

/*Flags for configuration register*/

/*! 16 bit mode flag.
 */
#define ADT_16_BIT 0b10000000
/*! 13 bit mode flag.
 */
#define ADT_13_BIT 0
/*! Continuous mode flag.
 */
#define ADT_CONT_MODE 0
/*! One shot mode flag.
 */
#define ADT_OS_MODE 0b00100000
/*! One measurement per second mode flag.
 */
#define ADT_SPS_MODE 0b01000000
/*! Shutdown mode flag.
 */
#define ADT_SD_MODE 0b01100000
/*! Interrupt mode flag.
 */
#define ADT_INT_MODE 0
/*! Critical temperature mode flag.
 */
#define ADT_CT_MODE 0b00010000
/*! Interrupt pin polarity low flag.
 */
#define ADT_INT_LOW 0                   
/*! Interrupt pin polarity high flag.
 */
#define ADT_INT_HIGH 0b00001000
/*! Critical temperature pin polarity low flag.
 */
#define ADT_CT_LOW 0
/*! Critical temperature pin polarity high flag.
 */
#define ADT_CT_HIGH 0b00000100
/*! One fault for raising interrupt and/or critical overtemperature interrupt.
 */
#define ADT_FQ_1 0
/*! Two faults for raising interrupt and/or critical overtemperature interrupt.
 */
#define ADT_FQ_2 0b00000001
/*! Three faults for raising interrupt and/or critical overtemperature interrupt.
 */
#define ADT_FQ_3 0b00000010
/*! Four faults for raising interrupt and/or critical overtemperature interrupt.
 */
#define ADT_FQ_4 0b00000011

/*Flags for status registers*/
/*! Mask for the new measurement bit in the status register.
 */
#define ADT_TEMP_READY 0b10000000

/*Defined chip select pins for adt7320 sensors used*/

/*! Digital pin used as a SPI chip select pin for the front temperature sensor (front = front CASU side).
 */
extern digitalPin tSlaveF;
/*! Digital pin used as a SPI chip select pin for the right temperature sensor.
 */
extern digitalPin tSlaveR;
/*! Digital pin used as a SPI chip select pin for the back temperature sensor.
 */
extern digitalPin tSlaveB;
/*! Digital pin used as a SPI chip select pin for the left temperature sensor.
 */
extern digitalPin tSlaveL;

/*! Macro for ADT7320 command format.
 */
#define ADT_COMMAND(reg, readWrite)  ((reg << 3) | readWrite)

/*! Global variable used for storing how many bits are used for conversion, values: \n
 * 1 - 16 bits
 * 0 - 13 bits
*/
extern UINT8 bits16Used;

/*! \brief Function initializes ADT7320, digital temperature sensor with SPI communication.
 * 
 * @param csPin Digital pin used as a SPI chip select pin of the temperature sensor.
 * @param config Configuration register data. Use defined macros for configuring this register.
 * @return 0 - SPI communication failed. \n
 *         -1 - Reading device id failed. \n
 *         -2 - Reading device config register failed. \n
 *          1 - Everything set up and ready for use.
 *
 * Example of usage: status = adt7320Init(pin, ADT_16_BIT | ADT_CONT_MODE);
 */
UINT8 adt7320Init(digitalPin csPin, UINT8 config);

/*! Function for reading adt7320 temperature value.
 * @param csPin Digital pin used as a SPI chip select pin of the temperature sensor.
 * @param temp Memory location used for storing temperature value.
 * @return 0 - SPI communication failed. \n
 *         1 - Reading successfully completed.
 * 
 * Example of usage: status = adt7320ReadTemp(&temp)
 */
UINT8 adt7320ReadTemp(digitalPin csPin, float *temp);

#endif	/* ADT7320_H */


