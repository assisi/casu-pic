#include "pic2beaglebone.h"

/*Init variables for measured data*/
float temp_f = -1, temp_b = -1, temp_r = -1, temp_l = -1, temp_t = -1, temp_casu = 25, temp_casu1 = 25, temp_wax = 25, temp_wax1 = 25;
float vAmp_f = -1 , vAmp_b = -1, vAmp_r = -1, vAmp_l = -1;
UINT16 fAmp_f = 0, fAmp_b = 0, fAmp_r = 0, fAmp_l = 0;
UINT16 proxy_f = 0, proxy_fr = 0, proxy_br = 0, proxy_b = 0, proxy_bl = 0, proxy_fl = 0, proxy_t = 0;
UINT8  pwmMotor = 0;
int ctlPeltier = 0, fanCooler = 0;
UINT8 tempCtlOn = 0, fanCtlOn = 0;
float Kf1 = 0.1428, Kf2 = 0.1428, Kf3 = 0.7101;
float Kp = 20, Ki = 0.9;
float temp_ref_l = 26, temp_ref_h = 45;

/* Init variables for storing references and control inputs*/
float temp_ref = 25.0;
float temp_ref_old = 25.0;
float temp_ref_cur = 25.0;
UINT16 vibeFreq_ref = 0;
UINT16 vibeFreq_old = 0;
UINT16 vibeAmp_ref = 0;
UINT8 ctlLED_r[3] = {0};
UINT8 diagLED_r[3] = {0};
UINT8 fanBlower_r = 0;
UINT8 calRec = 0;

/*
 * Function updates references (temperature, motor, LED1, LED2) transfered from beaglebone.
 * It takes data from rx_buff used in i2c communication with beaglebone
 */
void updateReferences() {
    vibeFreq_ref = i2c2_rx_buff[0] | (i2c2_rx_buff[1] << 8);
     /* scale 0 - 1000*/
    if (vibeFreq_ref > 1000)
        vibeFreq_ref = 1000;
    else if (vibeFreq_ref < 0)
        vibeFreq_ref = 0;

    vibeAmp_ref = i2c2_rx_buff[2] | (i2c2_rx_buff[3] << 8);
    /* scale 0 - 100*/
    if (vibeAmp_ref > 100)
        vibeAmp_ref = 100;
    else if (vibeAmp_ref < 0)
        vibeAmp_ref = 0;
}

/*
 * Function updates calibration data.
 * It takes data from rx_buff used in i2c communication with beaglebone
 */
void updateCalibrationData() {
    UINT16 dummy;
    tempCtlOn = i2c2_rx_buff[0];
    Kp = (i2c2_rx_buff[1] | (i2c2_rx_buff[2] << 8)) / 10.0;
    Ki = (i2c2_rx_buff[3] | (i2c2_rx_buff[4] << 8)) / 1000.0;
    dummy = (i2c2_rx_buff[5] | (i2c2_rx_buff[6] << 8));
    if (dummy > 32767)
        Kf1 = (dummy - 65536) / 10000.0;
    else
        Kf1 = dummy / 10000.0;
    dummy = (i2c2_rx_buff[7] | (i2c2_rx_buff[8] << 8));
    if (dummy > 32767)
        Kf2 = (dummy - 65536) / 10000.0;
    else
        Kf2 = dummy / 10000.0;
    dummy = (i2c2_rx_buff[9] | (i2c2_rx_buff[10] << 8));
    if (dummy > 32767)
        Kf3 = (dummy - 65536) / 10000.0;
    else
        Kf3 = dummy / 10000.0;
    fanCtlOn = i2c2_rx_buff[11];
    calRec = 1;
}

/*
 * Function updates tx_buff used in i2c communication with Beaglebone with
 *  new measured/calculated data
 */
void updateMeasurements() {
    i2c2_tx_buff[0] = vibeFreq_ref & 0x00FF;
    i2c2_tx_buff[1] = (vibeFreq_ref & 0xFF00) >> 8;
    i2c2_tx_buff[2] = vibeAmp_ref & 0x00FF;
    i2c2_tx_buff[3] = (vibeAmp_ref & 0xFF00) >> 8;
}