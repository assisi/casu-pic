/*! \file pic2beaglebone.h
 * \brief Declaration of functions and variables used for I2C communication with single-board computer (SBC), such as BeagleBone.
 *

 * Sensors are labeled with respect to the CASU side they are mounted to. The front temperature sensor refers to the sensor mounted on the front CASU side and the front-right proximity sensor refers to the sensor mounted on the front-right side of the hexagonal CASU upper body part.
 */

#ifndef PIC2BEAGLEBONE_H
#define	PIC2BEAGLEBONE_H

#define FAN_COOLER_ON 0
#define FAN_COOLER_OFF 100

#define MAX_FFT_NUM 4  // number of max fft component sent to the beaglebone

#include "../peripheral/i2c/i2c2.h"
#include <Generic.h>
#include "../actuators/peltier.h"
#include "../fft/fft.h"
#include "../peripheral/spi/spi2.h"
#include "../peripheral/spi/spi1.h"

/*Variables for measured data*/

/*!< Array containing latest temperature values 6 sensors + two estimated temperatures. */
extern float temp_old[8];
extern int index_filter[8];

/*! The front temperature sensor value.
 */
extern float temp_f;
/*! The back temperature sensor value.
 */
extern float temp_b;
/*! The right temperature sensor value.
 */
extern float temp_r;
/*! The left temperature sensor value.
 */
extern float temp_l;
/*! The top temperature sensor value.
 */
extern float temp_pcb, temp_flexPCB;

extern float temp_casu, temp_casu1, temp_wax, temp_wax1;    // temperature

/*! Flag if temp_wax was filtered.
*/
extern UINT8 filtered_glitch;

/*! Amplitude of vibration in g (g = 9.81 m/s2) measured by the front accelerometer.
 */
extern float vAmp_m[4];
/*! Amplitude of vibration in g measured by the accelerometer for 4 dominant frequencies.
 */

/*! Frequency of vibration in Hz measured by the acceleromer. This array contains the first 4 dominant frequencies.
 */
extern UINT16 fAmp_m[4];
/*! Frequency of vibration in Hz measured by the back accelerometer.
 */

/*! The front proximity sensor value in bits, range [0-65535].
 */
extern UINT16 proxy_f;
/*! The front-right proximity sensor value in bits, range [0-65535].
 */
extern UINT16 proxy_fr;
/*! The back-right proximity sensor value in bits, range [0-65535].

 */
extern UINT16 proxy_br;
/*! The back proximity sensor value in bits, range [0-65535].
 */
extern UINT16 proxy_b;
/*! The back-left proximity sensor value in bits, range [0-65535].
 */
extern UINT16 proxy_bl;
/*! The front-left proximity sensor value in bits, range [0-65535].

 */
extern UINT16 proxy_fl;
/*! The top proximity sensor value in bits, range [0-65535].
 */
extern UINT16 proxy_t;

/* Variables for storing references and control inputs*/

extern UINT16 speakerAmp_ref;

extern UINT16 speakerAmp_ref_old;

extern UINT16 speakerFreq_ref;

extern UINT16 speakerFreq_ref_old;


extern UINT8 pwmR_ctl;
/*! Control PWM value of green LED used as bee stimulus, range [0,100].
 */
extern UINT8 pwmG_ctl;
/*! Control PWM value of blue LED used as bee stimulus, range [0,100].

 */
extern UINT8 pwmB_ctl;
/*! Control PWM value of red LED used as diagnostic light, range [0,100].
 */
extern UINT8 pwmR_diag;
/*! Control PWM value of green LED used as diagnostic light, range [0,100].
 */
extern UINT8 pwmG_diag;
/*! Control PWM value of blue LED used as diagnostic light, range [0,100].

 */
extern UINT8 pwmB_diag;

/*! Control PWM value of the Peltier device, range [-100,100].
 */
extern float ctlPeltier;

/*! Referent temperature value of the CASU body.
 */
extern float temp_ref;
/*! The most recent referent temperature value of the CASU body. Used with the prefilter of the temperature reference.
 */

extern float ramp_slope;

extern float temp_ref_cur;
/*! Referent temperature value of the CASU body in the previous time step. Used with the prefilter of the temperature reference.

 */
extern float temp_ref_old;

extern float temp_ref_ramp;
/*! Referent frequency of vibration of the CASU body.
 */
extern UINT16 vibeFreq_ref;
/*! Referent PWM values of red, green and blue LEDs used as bee stimulus.
 */
extern UINT8 ctlLED_r[3];
/*! Referent PWM values of red, green and blue LEDs used as diagnostic light.

 */
extern UINT8 diagLED_r[3];

extern UINT8 new_vibration_reference;
/*! \brief Function updates referent values of each stimuli. Referent values are received through I2C communication with SBC.

 */
void updateReferences(UINT8 msg_id);
/*! \brief Function measured values of each sensor. Measured values are sent through I2C communication to SBC.
 */
void updateMeasurements();

extern int fanCooler;
extern UINT8 calRec;
extern UINT8 tempCtlOn, fanCtlOn;
extern float Kf1, Kf2, Kf3;
extern float temp_ref_l, temp_ref_h, temp_ref_shutdown;
extern float Kp, Ki;

extern float alphak1, C_sigma, C_sigma_m, K1_alpha, K2_beta, epsilon;
extern UINT8 controller_type;


/* Variables for storing references and control inputs*/
extern UINT16 motPwm_ref;
extern UINT8 fanBlower_r;

extern int source_array[FFT_BUFF];
extern int amplitudes[(int)(FFT_BUFF/2)];
extern float accPeriod;
extern UINT16 maxAmp;

void updateCalibrationData();

void updateAccLog();

#endif	/* PIC2BEAGLEBONE_H */
