/*! \file peltier.h
 * \brief Declarations of functions and variables used for controlling Peltier device.
 */

#ifndef PELTIER_H
#define	PELTIER_H

#include <Generic.h>
#include "../test/CASU-karlo/initializeHardware.h"

/*! Maximal output current in A.
 */
#define IMAX     3

/*! Digital to analog converter (DAC) output gain, values: \n
 0 - 2x \n
 1 - 1x
 */
#define DAC_G    0

/*! DAC maximal voltage at maximal current.
 */
#define DMAX     IMAX*250+1500

/*! Zero referent value of Peltier device.
 */
#define REF_OFFSET 1500

/*! Output multiplification factor.
 */
#define DCOEF   (DMAX-DOFFSET)/100  /*Check DOFFSET*/

/*! Maximal PWM value (heating CASU).
 */
extern int PELTIER_PWM_MAX_P; 

/*! Minimal PWM value (cooling CASU).
 */
extern int PELTIER_PWM_MAX_N;

/*! Temperature PID controller output value in the previous time step.
 */
extern float uk1;

/*! Temperature PID controller proportional gain.
 */
extern float Kp;

/*! Temperature PID controller integral gain.
 */
extern float Ki;

/*! \brief Function initializes Peltier control pins - a digital pin for enabling/disabling Peltier device and a digital pin used as SPI chip select for DAC. DAC is used for generating Peltier current referent value.
 *
 * @param csPin SPI chip select pin for DAC.
 */
void PeltierInit(digitalPin csPin);

/*! \brief Function sets PWM referent value for the Peltier device.
 *
 * @param csPin SPI chip select pin for DAC.
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
