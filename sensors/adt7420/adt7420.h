/*! \file adt7420.h
 * \brief Declaration of functions and variables used for I2C communication with ADT7420 digital temperature sensor.
 */
 
#ifndef ADT7420_H
#define	ADT7420_H

#include "../../peripheral/gpio/digitalIO.h"

/*! AD7420 device I2C address.
 */
#define ADT74_I2C_ADD   0x48
/*! AD7420 device ID number.
 */
#define ADT74_ID        0xCB    

/*Registers address */
/*! Address of the register containing the most significant byte of the current temperature value.
 */
#define ADT74_THIGH     0x00
/*! Address of the register containing the least significant byte of the current temperature value.
 */
#define ADT74_TLOW      0x01
/*! Address of the status register.

 */
#define ADT74_STATUS    0x02    //Status register
/*! Address of the configuration register.
 */
#define ADT74_CONFIG    0x03
/*! Address of the register containing the most significant byte of the overtemperature value.
 */
#define ADT74_TH_HIGH   0x04    
/*! Address of the register containing the least significant byte of the overtemperature value.

 */
#define ADT74_TH_LOW    0x05
/*! Address of the register containing the most significant byte of the undertemperature value.
 */
#define ADT74_TL_HIGH   0x06   
/*! Address of the register containing the least significant byte of the undertemperature value.
 */
#define ADT74_TL_LOW    0x07
/*! Address of the register containing the most significant byte of the critical temperature value.

 */
#define ADT74_TCR_HIGH  0x08 
/*! Address of the register containing the least significant byte of the critical temperature value.
 */
#define ADT74_TCR_LOW   0x09
/*! Address of the register containing the hysteris value of temperature (over,under,critical).
 */
#define ADT74_THYST     0x0A
/*! Address of the register containing device ID.
 */
#define ADT74_ID_REG    0x0B  
/*! Address of the register used for software reset.
 */
#define ADT74_SOF_RES    0x2F 

/*Flags for configuration register*/

/*! 16 bit mode flag.
 */
#define ADT74_16_BIT 0b10000000
/*! 13 bit mode flag.
 */
#define ADT74_13_BIT 0
/*! Continuous mode flag.
 */
#define ADT74_CONT_MODE 0              
/*! One shot mode flag.
 */
#define ADT74_OS_MODE 0b00100000         
/*! One measurement per second mode flag.
 */
#define ADT74_SPS_MODE 0b01000000         
/*! Shutdown mode flag.
 */
#define ADT74_SD_MODE 0b01100000         
/*! Interrupt mode flag.
 */
#define ADT74_INT_MODE 0                 
/*! The critical temperature mode flag.
 */
#define ADT74_CT_MODE 0b00010000        
/*! Interrupt pin polarity low flag.
 */
#define ADT74_INT_LOW 0                  
/*! Interrupt pin polarity high flag.
 */
#define ADT74_INT_HIGH 0b00001000       
/*! Critical temperature pin polarity low flag.
 */
#define ADT74_CT_LOW 0                   
/*! Critical temperature pin polarity high flag.
 */
#define ADT74_CT_HIGH 0b00000100          
/*! One fault for raising interrupt and/or the critical overtemperature interrupt.
 */
#define ADT74_FQ_1 0                    
/*! Two faults for raising interrupt and/or the critical overtemperature interrupt.
 */
#define ADT74_FQ_2 0b00000001             
/*! Three faults for raising interrupt and/or the critical overtemperature interrupt.
 */
#define ADT74_FQ_3 0b00000010             
/*! Four faults for raising interrupt and/or the critical overtemperature interrupt.
 */
#define ADT74_FQ_4 0b00000011             

/*Flags for status registers*/
/*! Mask for the new measurement bit in the status register.
 */
#define ADT74_TEMP_READY 0b10000000


/*! \brief Function initializes ADT7420, digital temperature sensor with I2C interface.
 *
 * @param config Configuration register data. Use defined macros for configuring this register.
 * @return 0 - I2C communication failed. \n
 *         -1 - Reading device id failed. \n
 *         -2 - Reading device config register failed. \n
 *          1 - Everything set up and ready for use.
 *

 * Example of usage: status = adt7420Init(ADT74_16_BIT | ADT74_CONT_MODE);
 */
int adt7420Init(UINT8 config);

/*! \brief Function for reading ADT7420 temperature value.
 *
 * @param temp Memory location used for storing temperature value.
 * @return 0 - I2C communication failed. \n
 *         1 - Reading successfully completed.
 *
 * Example of usage: status = adt7420ReadTemp(&temp)
 */
int adt7420ReadTemp(float *temp);

#endif	/* ADT7420_H */
