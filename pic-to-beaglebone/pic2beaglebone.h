/*! \file pic2beaglebone.h
 * Declarations of functions and variables used for I2C communication with single-board computer (SBC), such as BeagleBone.
 */

#ifndef PIC2BEAGLEBONE_H
#define	PIC2BEAGLEBONE_H

#include "../peripheral/i2c/i2c2.h"
#include <Generic.h>
#include "../actuators/peltier.h"

/*! Number of incoming bytes in PIC-SBC I2C communication.
 */
#define IN_DATA_NUM 10

/*! Number of outgoing bytes in PIC-SBC I2C communication.
 */
#define OUT_DATA_NUM 48

/*Variables for measured data*/

/*! Front temperature sensor value.
 */
extern float temp_f;
/*! Back temperature sensor value.
 */
extern float temp_b;
/*! Right temperature sensor value.
 */
extern float temp_r;
/*! Left temperature sensor value.
 */
extern float temp_l;
/*! Top temperature sensor value.
 */
extern float temp_t;

/*! Amplitude of vibration in g (g = 9.81 m/s2) measured by front accelerometer.
 */
extern float vAmp_f;
/*! Amplitude of vibration in g measured by back accelerometer.
 */
extern float vAmp_b;
/*! Amplitude of vibration in g measured by right accelerometer.
 */
extern float vAmp_r;
/*! Amplitude of vibration in g measured by left accelerometer.
 */
extern float vAmp_l;

/*! Frequency of vibration in Hz measured by front accelerometer.
 */
extern UINT16 fAmp_f;
/*! Frequency of vibration in Hz measured by back accelerometer.
 */
extern UINT16 fAmp_b;
/*! Frequency of vibration in Hz measured by right accelerometer.
 */
extern UINT16 fAmp_r;
/*! Frequency of vibration in Hz measured by left accelerometer.
 */
extern UINT16 fAmp_l;

/*! Front proximity sensor value in bits, range [0-65535].
 */
extern UINT16 proxy_f;
/*! Front-right proximity sensor value in bits, range [0-65535].
 */
extern UINT16 proxy_fr;
/*! Back-right proximity sensor value in bits, range [0-65535].
 */
extern UINT16 proxy_br;
/*! Back proximity sensor value in bits, range [0-65535].
 */
extern UINT16 proxy_b;
/*! Back-left proximity sensor value in bits, range [0-65535].
 */
extern UINT16 proxy_bl;
/*! Front-left proximity sensor value in bits, range [0-65535].
 */
extern UINT16 proxy_fl;
/*! Top proximity sensor value in bits, range [0-65535].
 */
extern UINT16 proxy_t;

/* Variables for storing references and control inputs*/

/*! PWM control value of the vibration motor, range [0,100].
 */
extern UINT8 pwmMotor;
/*! PWM control value of red LED used as bee stimulus, range [0,100].
 */
extern UINT8 pwmR_ctl;
/*! PWM control value of green LED used as bee stimulus, range [0,100].
 */
extern UINT8 pwmG_ctl;
/*! PWM control value of blue LED used as bee stimulus, range [0,100].
 */
extern UINT8 pwmB_ctl;
/*! PWM control value of red LED used as diagnostic light, range [0,100].
 */
extern UINT8 pwmR_diag;
/*! PWM control value of green LED used as diagnostic light, range [0,100].
 */
extern UINT8 pwmG_diag;
/*! PWM control value of blue LED used as diagnostic light, range [0,100].
 */
extern UINT8 pwmB_diag;
/*! PWM control value of the Peltier device, range [-100,100].
 */
extern int ctlPeltier;

/*! Referent temperature value of the CASU body.
 */
extern float temp_ref;
/*! The most recent referent temperature value of the CASU body. Used when prefilter of the tempereture reference is included.
 */
extern float temp_ref_cur;
/*! Referent temperature value of the CASU body in the previous time step. Used when prefilter of the tempereture reference is included.
 */
extern float temp_ref_old;
/*! Referent frequency of vibration of the CASU body.
 */
extern UINT16 vibeFreq_ref;
/*! Referent PWM values of red,green and blue LEDs used as bee stimulus.
 */
extern UINT8 ctlLED_r[3];
/*! Referent PWM values of red,green and blue LEDs used as diagnostic light.
 */
extern UINT8 diagLED_r[3];

/*! \brief Function updates referent values of each stimuli. Referent values are received through I2C communication with SBC.
 */
void updateReferences();
/*! \brief Function measured values of each sensor. Measured values are sent through I2C communication to SBC.
 */
void updateMeasurements();


#endif	/* PIC2BEAGLEBONE_H */

