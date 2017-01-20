/****************************************************************************/
/* 2014 LARICS - Laboratory for Robotics and Intelligent Control Systems
; Project:  ASSISI CASU v1.3
; Processor:    dsPIC33Exxxx
; Compiler:     -
;============================================================================;
; FILE DESCRIPTION:
; Peltier firware - peltier driver is controlled with DAC connected with SPI
; Schematic documentation:
 * "ASSISI_controller_board_v1.3"
 * "ASSISI_CASU05_AmpBoard"
;
; REVISION HISTORY:
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; Date, Author      | Comments on this revision
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
; 24.01.2014        | Implementation
; 27.04.2014        | PI controller added and tested on CASU V0.5
; 17.02.2015        | New board design
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
*/
#include <math.h>

#include "peltier.h"
#include "../peripheral/spi/spi1.h"
#include "pwm.h"
#include "../pic-to-beaglebone/pic2beaglebone.h"

//PI controller global variables and parameters
float uk1 = 0;

//SMC controller global variables
float yk1 = 25;
float vk1 = 0;
float emk1 = 0;
float alphak1 = 7;

float ymk1 = 25;

//SMC controller parameters
float k_sat = 10;
float C_sigma = 1;
float C_sigma_m = 1;
float epsilon = 0.4;
float K1_alpha = 0.01;
float K2_beta = 0.01;

// test only
//float sigma_m;
//float sigma;

/* PI controller
 * inputs:  ref - reference value
 *          y - feedback value
 * returns: uk
 */
float PeltierPID(float ref, float y){
    float ek, uk, ui, up;

    ek = ref - y;

    //uk = Kp*ek + Ki*(uk1 + ek);
    up = Kp * ek;
    if (Ki <= 0.00001)
        ui = 0;
    else
        ui = uk1 + Ki * ek;

    uk = up + ui;
    uk1 = ui;

    if(uk > 100) {
        uk = 100;
        uk1 = uk - up;
        if (uk1 < 0)
            uk1 = 0;
    }
    else if(uk < -100) {
        uk = -100;
        uk1 = uk - up;
        if (uk1 > 0)
            uk1 = 0;
    }
    //uk1 = uk;// - Kp*ek;

    return uk;
}

float PeltierSMC(float ref, float y, float alpha, float beta) {
    float e, d_y;
    float sigma;
    float sat_sigma;
    float v, u;

    e = ref - y;
    d_y = y - yk1;
    yk1 = y;
    sigma = e + C_sigma * e * d_y;
    sat_sigma = saturation(sigma, k_sat);

    v = beta * sat_sigma + vk1;
    vk1 = v;
    u = alpha * sqrt(fabs(sigma)) * sat_sigma + v;

    return u;
}

int PeltierCooler(float tc){

    if(tc > 35)
        FanCooler(100);
    else
        FanCooler(0);
    return 1;

}

void PeltierResetPID(){

    uk1 = 0;
    temp_ref_cur = temp_wax;
}

void SmcParamAdapt(float *params, float ym, float y, float ref) {
    float em, e, sat_e, d_em;
    float sigma_m;
    float sat_sigma_m;
    float alpha, beta;

    em = ym - y;
    e = ref - y;
    sat_e = saturation(e, k_sat);
    em = em * sat_e;
    d_em = em - emk1;
    emk1 = em;
    sigma_m = em + C_sigma_m * d_em;
    sat_sigma_m = saturation(sigma_m, k_sat);

    if (fabs(sigma_m) > epsilon) {
        alpha = alphak1 + K1_alpha * sat_sigma_m;
    }
    else {
        alpha = alphak1;
    }

    beta = K2_beta * alpha;
    *(params) = alpha;
    *(params + 1) = beta;

    alphak1 = alpha;
}

// Logistic sigmoid saturation (-1,1) of K*x
float saturation(float x, float K) {
    float result;
    result = 2.0 / (1 + exp(-2 * K * x)) - 1;
    return result;
}

float TempModel(float uref) {
    float Km = 0.98684; //75/76; //0.97561; //40/41; 0.9736; //37/38; Td = 2s
    float ymk;

    ymk = (1-Km)*uref + Km*ymk1;
    ymk1 = ymk;

    return ymk;
}
