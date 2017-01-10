/* 
 * File:   pwm.h
 * Author: Karlo
 *
 * Created on 2015. velja?a 17, 15:59
 */

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif

#include <Generic.h>
#define PWMPRE 1   //PWM input clock prescaler
#define FPWM   10000 //PWM frequecy

#define PELTIER_PWM_MAX_P 60 //60    //Peltier max pwm output
#define PELTIER_PWM_MAX_N 60 //30    //Peltier min pwm output

#define PELTIER_TD_MIN  1   //in seconds
#define PELTIER_T       30 //in seconds
#define PELTIER_K       0.24

void PWMInit(void);

void LedUser(UINT8 red, UINT8 green, UINT8 blue);

void LedBee(UINT8 red, UINT8 green, UINT8 blue);

void FanCooler(UINT8 set);

void FanBlower(UINT8 set);

void PeltierVoltageSet(int setVoltage);

void VibrationSet(UINT8 set);

void PeltierSetPwm(int set);

void PeltierOff();

void PeltierOn(int direction);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

