/*! \file peltier.h
 * Declarations of functions and variables used for controlling Peltier device.
 */

#ifndef PELTIER_H
#define	PELTIER_H

#include <Generic.h>
#include "../test/CASU-karlo/initializeHardware.h"

/*! Maximum output current in A
 */
#define IMAX     3

/*! Digital to analog converter (DAC) output gain, values: 0 - 2x, 1 - 1x
 */
#define DAC_G    0

/*! DAC maximum voltage for maximum current.
 */
#define DMAX     IMAX*250+1500

/*! Zero referent value of Peltier device.
 */
#define REF_OFFSET 1500

/*! Output multiplification factor.
 */
#define DCOEF   (DMAX-DOFFSET)/100

/*! Maximum pwm for positive current (heating CASU).
 */
extern int PELTIER_PWM_MAX_P; 

/*! Maximum pwm value for negative current (cooling CASU).
 */
extern int PELTIER_PWM_MAX_N;

/*! Temperature PID controller output value from the previous time step.
 */
extern float uk1;

/*! Temperature PID controller proportional gain.
 */
extern float Kp;

/*! Temperature PID controller integral gain.
 */
extern float Ki;

/*! \brief Function initializes Peltier control pins - digital pin for enabling/disabling Peltier device and digital pin used as SPI chip select of DAC.
 *
 * @param csPin SPI chip select pin for DAC used for generating Peltier current referent value.
 */
void PeltierInit(digitalPin csPin);

/*! \brief Function sets PWM referent value of the Peltier device.
 *
 * @param csPin SPI chip select pin for DAC used for generating Peltier current referent value.
 * @param lShdn Enable/disable (1/0) DAC.
 * @param set PWM referent value for the Peltier device, range [-100, 100].
 */
void PeltierSet(digitalPin csPin, UINT8 lShdn, int set);

/*! \brief Function implements temperature PID controller.
 *
 * @param ref Referent temperature value.
 * @param y Measured CASU temperature value.
 */
int PeltierPID(float ref, float meas);

#endif	/* PELTIER_H */
