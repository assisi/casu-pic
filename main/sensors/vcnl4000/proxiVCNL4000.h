/*! \file  proxiVCNL4000.h
 * \brief Declaration of functions and variables used for I2C communication with VCNL4000 digital proximity sensor.
 */

#ifndef PROXIVCNL4000_H
#define	PROXIVCNL4000_H

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif

/*! VCNL4000 I2C address.
 */
#define VCNL4000_Address    0x13
/*! VCNL4000 address byte in writing transmision.
 */
#define VCNL4000_Address_WR    0x26
/*! VCNL4000 address byte in reading transmision.
 */
#define VCNL4000_Address_RD    0x27

/*Function registers address*/
/*! Address of command register.
 */
#define COMM0   0x80
/*! Address of product id revision register.
 */
#define PRODID  0x81
/*! Address of IR LED current register.
 */
#define IRLED3  0x83
/*! Address of ambient light parameter register.
 */
#define ALIGH4  0x84
/*! Address of the ambient light value the most significant byte register.
 */
#define ALRES5  0x85
/*! Address of the ambient light value the least significant byte register.
 */
#define ALRES6  0x86
/*! Address of the proximity value the most significant byte register.
 */
#define PRRES7  0x87
/*! Address of the proximity value the least significant byte register.
 */
#define PRRES8  0x88
/*! Address of the proximity measurement signal frequency register.
 */
#define PRMEA9  0x89
/*! Address of the proximity modulator timing adjustment register.
 */
#define PRMOD10 0x8A

/*! @struct VCNL4000Data Represents proximity sensor measurements.
 * @var VCNL4000Data::Distance Distance value.
 * @var VCNL4000Data::Light Ambient light value.
 */
struct VCNL4000Data{
    unsigned int Distance;
    unsigned int Light;
};

/*! \brief Function initializes digital proximity sensor.
*/
char VCNL4000Init(void);


/*! \brief Function reads ambient light value.
 *
 * @return Returns -1 if I2C communication error occured, else returns ambient light value.
 */
int VCNL4000Light(void);

/*! \brief Function reads proximity value.
 *
 * @return Returns -1 if I2C communication error occured, else returns proximity value.
 */
int VCNL4000ReadProxi();

#endif
