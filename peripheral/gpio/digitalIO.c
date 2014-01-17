#include "digitalIO.h"


void digitalInput(digitalPin pin) {
    UINT16 mask = 0x0001;
    mask = mask << pin.pinNum;
    *pin.trisReg = (*pin.trisReg) | mask;
}

void digitalOutput(digitalPin pin) {
    UINT16 mask = 0x0001;
    mask = mask << pin.pinNum;
    *pin.trisReg = (*pin.trisReg) & (~mask);
}

/* Function sets digital pin to 0
 * inputs: pin - digital pin
 * return: None
 * Example of usage:
 * digitalPin pin;
 * pin.port = &PORTAbits;
 * pin.pinNum = 0;
 * digitalLow(pin);
 */
void digitalLow(digitalPin pin) {
    UINT16 mask = 0x0001;
    mask = mask << pin.pinNum;
    *pin.portReg = (*pin.portReg) & (~mask);
}

/* Function sets digital pin to 1
 * inputs: pin - digital pin
 * return: None
 * Example of usage:
 * digitalPin pin;
 * pin.port = &PORTAbits;
 * pin.pinNum = 0;
 * digitalHigh(pin);
 */
void digitalHigh(digitalPin pin) {
    UINT16 mask = 0x0001;
    mask = mask << pin.pinNum;
    *pin.portReg = (*pin.portReg) | (mask);
}

/* Function returns digital pin value
 * inputs: pin - digital pin
 * return: 0  if digital pin is low
 *         1  if digital pin is high
 * Example of usage:
 * digitalPin pin;
 * pin.port = &PORTAbits;
 * value = pinValue(pin);
 */
UINT8 pinValue(digitalPin pin) {
    UINT16 mask = 0x0001;
    mask = mask << pin.pinNum;
    return (*pin.portReg & mask) >> pin.pinNum;
}