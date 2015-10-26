/*! \file digitalIO.h
 * \brief Declaration of functions used for controlling digital pins.
 */

#ifndef DIGITALIO_H
#define	DIGITALIO_H

#include <Generic.h>

typedef struct Pin {
    int *trisReg;
    int *portReg;
    UINT8 pinNum;
}digitalPin;

/*! \brief Function configures digital pin as input pin.
 *
 * @param pin Digital pin to be set.
 */
void digitalInput(digitalPin pin);

/*! \brief Function configures digital pin as output pin.
 *
 * @param pin Digital pin to be set.
 */
void digitalOutput(digitalPin pin);

/*! \brief Function sets digital pin in output high state.
 *
 * @param pin Digital pin to be set.
 */
void digitalHigh(digitalPin pin);

/*! \brief Function sets digital pin in output low state.
 *
 * @param pin Digital pin to be set.
 */
void digitalLow(digitalPin pin);

/*! \brief Function returns digital pin state.
 *
 * @param pin Digital pin to be read.
 * @return State of the digital pin. \n
 *         0 - digital low \n
 *         1 - digital high
 */
UINT8 pinValue(digitalPin pin);
#endif	/* DIIGTALIO_H */

