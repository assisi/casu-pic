/*! \file i2c1.h
 * \brief Declarations of functions and variables used for working with i2c1.
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

/*! Size of the buffers used for transmitting (tx) and receiving data (rx).
 */
#define BUFF_SIZE 64
/*! Wait time in number of simple while loop cycles. Used for i2c fault diagnostic.
 */
#define WAIT_TIME 100

/* TODO Move this address somewhere else.*/
/*! I2C multiplexer (PCA9547) address.
 */
#define PCAMUX_ADD  0x70    //PCA9547 mux address

/*! Wait time in microseconds used for i2c fault diagnostic.
 */
#define I2C1_TIMEOUT    500    //I2C1 timeout Tout = Tcyc*Ninst*I2C1_TIMEOUT
                                //I2C1_TIMEOUT = 500us/(0.025us*40) = 500

/*! Buffer for incoming data (rx buffer).
 */
extern UINT8 rx_buff[BUFF_SIZE];
/*! Buffer for outgoing data (tx buffer).
 */
extern UINT8 tx_buff[BUFF_SIZE];

/*! \brief Function initiates i2c1 bus (master mode).
 */
void I2C1Start(void);

/*! \brief Function puts i2c1 bus in idle mode (master mode).
 *
 * @return -1 Failed to put i2c in idle mode \n
 *          1 Succesfully put i2c in idle mode 
 */
int I2C1Idle(void);

/*! \brief Function sends not-acknowledge signal on i2c1 bus (master mode).
 */
void I2C1NotAck(void);

/*! \brief Function sends acknowledge signal on i2c1 bus (master mode).
 */
void I2C1Ack(void);

/*! \brief Function stops i2c1 bus (master mode).
 */
void I2C1Stop(void);

/*! \brief Function restarts i2c1 bus (master mode).
 */
void I2C1Restart(void);

/*! \brief Function reads byte on i2c1 bus (master mode).
 *
 * @return Byte read on i2c1 bus.
 */
unsigned char I2C1MasterRead(void);

/*! \brief Function initializes i2c1 as a master device.
 *
 * @return 1 - device successfully configured and initialized.
 */
UINT8 I2C1MasterInit(void);

/*! \brief Function initializes i2c1 module as a slave device.
 *
 * @param address Address of the i2c1 module used as a slave device.
 * @param int_priority Priority of i2c1 interrupt routine for exchanging data with master.
 * @return 1 - device successfully configured and initialized.
 */
UINT8 I2C1SlaveInit(UINT8 address, UINT8 int_priority);

/*! \brief Function selects channel on external i2c multiplexer (PCA9547).
 *
 * @param enable Enable/disable (1/0) i2c mux.
 * @param channel Number of channel to be selected.
 * @return -1 - Channel selection failed. \n
 *         [0-7] - Number of the selected channel. 
 */
int I2C1ChSelect(UINT8 enable, UINT8 channel);

/*! \brief  Function resets i2c multiplexer (PCA9547).
 */
void muxReset();

/*! \brief Function tests basic i2c multiplexer functionality (selecting and reading channel).
 *
 * @return -1 - Multiplexer test failed. \n
 *         [0-7] - Number of the selected channel.
 */
int MUXTest();

/*! \brief Function writes byte to i2c slave device register.
 *
 * @param slaveAdd i2c address of the slave device.
 * @param registerAdd Slave device register address.
 * @param data Byte data to be written.
 * @return 0 - Error occured during transmission. \n
 *         1 - Data successfully transmitted.
 */
int I2C1WriteByte(UINT8 slaveAdd, UINT8 registerAdd, UINT8 data);

/*! \brief Function reads byte from i2c slave device register.
 *
 * @param slaveAdd Address of i2c slave device.
 * @param registerAdd Slave device register address.
 * @return Returns 255 if error occured during transmission, else returns read byte.
 */
unsigned char I2C1ReadByte(UINT8 slaveAdd, UINT8 registerAdd);

/*! \brief Interrupt routine for i2c1 device. 
 */
void __attribute__((__interrupt__, auto_psv)) _SI2C1Interrupt(void);

#endif	/* I2C1_H */

