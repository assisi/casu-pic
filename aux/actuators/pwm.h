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

#include "p33FJ06GS101.h"

#include <Generic.h>
#define PWMPRE 1   //PWM input clock prescaler
#define FPWM   30000 //PWM frequecy

#define PELTIER_PWM_MAX_P 60 //60    //Peltier max pwm output
#define PELTIER_PWM_MAX_N 60 //30    //Peltier min pwm output

#define PELTIER_TD_MIN  1   //in seconds
#define PELTIER_T       30 //in seconds
#define PELTIER_K       0.24

void PWMInit(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

