 /*! \file peltier.h
 * \brief Declarations of functions and variables used for controlling Peltier device.
 */

#ifndef PELTIER_H
#define	PELTIER_H

#include <Generic.h>
#include "../casu/initializeHardware.h"

#define IMAX     3          //[A] - maximum output current
#define DAC_G    0          //DAC output gain 0 - 2x, 1 - 1x
#define DMAX     IMAX*250+1500            //DAC maximum voltage for maximum current, Itec=(Vout-1.5)*4
#define REF_OFFSET 1500                      //Peltier zero current
#define DCOEF   (DMAX-DOFFSET)/100        //Output multiplification factor

extern float uk1;

void PeltierInit(digitalPin csPin);

void PeltierSet(digitalPin csPin, UINT8 lShdn, int set);

float PeltierPID(float ref, float y);

float PeltierSMC(float ref, float y, float alpha, float beta);

void PeltierResetPID(void);

void SmcParamAdapt(float *params, float ym, float y, float ref);

float saturation(float x, float K);

float TempModel(float uref);

#endif	/* PELTIER_H */
