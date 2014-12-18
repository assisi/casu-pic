/*! \file spi1.h
 * \brief Declaration of functions and variables used for controlling SPI1 bus.
 */

#ifndef SPI1_H
#define	SPI1_H

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif

#include <Generic.h>
#include "../gpio/digitalIO.h"

/*! \brief Macro used for selecting a SPI device by putting corresponding chip select pin to digital low.
 */
#define chipSelect(slave) digitalLow(slave)

/*! \brief Macro used for deselecting a SPI device by putting corresponding chip select pin to digital high.
 */
#define chipDeselect(slave) digitalHigh(slave)

/*! \brief Function initializes SPI1 bus.
 *
 *  @param mode SPI mode, possible modes are 0, 1, 2, 3
 *  @param int_en Interrupt enable flag, 1 - enable spi interrupts, 0 - disable
 *  @return Function returns 1 if initialization succeeded, else 0.
 */
UINT8 spi1Init(UINT8 mode, UINT8 int_en);

/*! \brief Function sends 16 bit data over SPI1 bus.
 *
 * @param  data 16 bit data.
 * @return Returns 1 if sending succeeded, else 0.
 */
UINT8 spi1Write(UINT16 data);

/*! \brief Function sends and receives 16 bit data over SPI1 bus.
 * @param out 16 bit data to be send.
 * @param in Pointer to a memory location where incoming 16 bit data is saved.
 * @return Return 1 if transfer succeeded, else 0.
 */
UINT8 spi1TransferWord(UINT16 out, UINT16 *in);

/*! \brief Function sends data stored in buffer over SPI1 bus and stores in the same buffer received data.
 *
 * @param buff Pointer to the buffer.
 * @param len Buffer size.
 * @return Returns 1 if transfer succeeded, else 0.
 */
UINT8 spi1TransferBuff(UINT16 *buff, UINT16 len);

/*! \brief Function checks if SPI1 transmit (tx) buffer is full.
 *
 * @return Returns 1 if tx buff full, else 0.
 */
UINT8 spi1TxBuffFull();

#endif	/* SPI1_H */

