/*! \file i2c2.h
 * \brief Declarations of functions and variables used for working with i2c2.
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

/*! Size of the buffers used for transmitting (tx) and receiving data (rx).
 */
#define BUFF_SIZE 3000

#define ACC_BUFF 256 //should be the same as fft buf

/*! Wait time in number of simple while loop cycles. Used for i2c fault diagnostic.
 */
#define WAIT_TIME 100

/*! Buffer for incoming data (rx buffer).
 */
extern UINT8 i2c2_rx_buff[BUFF_SIZE];

/*! Buffer for outgoing data (tx buffer).
 */
extern UINT8 i2c2_tx_buff[BUFF_SIZE];

extern UINT16 i2c2_rx_head;

extern UINT16 i2c2_tx_head;

extern UINT8 i2c2_tx_ready;

/*! \brief Function initializes i2c2 module as a slave device.
 *
 * @param address Address of the i2c2 module used as a slave device.
 * @param int_priority Priority of i2c2 interrupt routine for exchanging data with master.
 * @return Device successfully configured and initialized.
 */
UINT8 I2C2SlaveInit(UINT8 address, UINT8 int_priority);

#endif	/* I2C2_H */

