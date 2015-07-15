/* 
 * File:   pic2beaglebone.h
 * Author: thaus
 *
 * Created on January 19, 2014, 7:31 PM
 */

#ifndef PIC2BEAGLEBONE_H
#define	PIC2BEAGLEBONE_H

#include "../peripheral/i2c/i2c2.h"
#include <Generic.h>
#include "../actuators/peltier.h"

/*Variables for measured data*/
extern float temp_f, temp_b, temp_r, temp_l, temp_t, temp_casu, temp_casu1, temp_wax, temp_wax1;    // temperature
extern float vAmp_f, vAmp_b, vAmp_r, vAmp_l;           // vibration amplitude
extern UINT16 fAmp_f, fAmp_b, fAmp_r, fAmp_l;          // vibraition frequency
extern UINT16 proxy_f, proxy_fr, proxy_br, proxy_b, proxy_bl, proxy_fl, proxy_t;      // values from proximty sensors
extern UINT8 pwmMotor; // control inputs to Peltier, vibe motor, RGB led for controlling bees and RGB led for diagnostics
extern int ctlPeltier, fanCooler;
extern UINT8 calRec;
extern UINT8 tempCtlOn, fanCtlOn;
extern float Kf1, Kf2, Kf3;
extern float temp_ref_l, temp_ref_h;
extern float Kp, Ki;

/* Variables for storing references and control inputs*/
extern float temp_ref, temp_ref_old, temp_ref_cur;
extern UINT16 motPwm_ref;
extern UINT8 ctlLED_r[3];
extern UINT8 diagLED_r[3];
extern UINT8 fanBlower_r;

void updateReferences();
void updateCalibrationData();
void updateMeasurements();


#endif	/* PIC2BEAGLEBONE_H */

