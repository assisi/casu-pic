#include "pic2beaglebone.h"

/*Init variables for measured data*/
float temp_f = 20.0, temp_b = 12.0, temp_r = -10.0, temp_l = 22.0, temp_t = 25.2;
float vAmp_f = 1.0 , vAmp_b = 2.0, vAmp_r = 3.0, vAmp_l = 4.0;
UINT16 fAmp_f = 200, fAmp_b = 300, fAmp_r = 400, fAmp_l = 500;
UINT16 proxy_f = 2500, proxy_fr = 2470, proxy_br = 2380, proxy_b = 3000, proxy_bl = 3100, proxy_fl = 3200, proxy_t = 1000;
UINT8  pwmMotor = 0, pwmR_ctl = 24, pwmG_ctl = 23, pwmB_ctl = 42, pwmR_diag = 43, pwmG_diag = 76, pwmB_diag = 80;
int ctlPeltier = 0;

/* Init variables for storing references and control inputs*/
float temp_ref = 0.0;
float temp_ref_old = 0.0;
float temp_ref_cur = 0.0;
UINT16 vibeFreq_ref = 0;
UINT8 ctlLED_r[3] = {0};
UINT8 diagLED_r[3] = {0};

/*
 * Function updates references (temperature, motor, LED1, LED2) transfered from beaglebone.
 * It takes data from rx_buff used in i2c communication with beaglebone
 */
void updateReferences() {
    UINT16 dummy;
    dummy = i2c2_rx_buff[0] | (i2c2_rx_buff[1] << 8);
    if (dummy > 32767)
        temp_ref = (dummy - 65536) / 10.0;
    else
        temp_ref = dummy / 10.0;

    if (temp_ref > 20 && temp_ref < 26)
        temp_ref = 26;
    if (temp_ref > 50)
        temp_ref = 50;

    if ((temp_ref - temp_ref_old > 0.1) || (temp_ref - temp_ref_old <  -0.1)) {
        // temperature reference changed
//        if (temp_ref > temp_l)      // left temperature sensor is used as feedback
//            PELTIER_PWM_MAX_N = 85;         // heating
//        else if (temp_l - temp_ref > 4.5)   // cooling with high temperature difference > 4.5, reduce maximum current to 30%
//            PELTIER_PWM_MAX_N = 30;
//        else
//            PELTIER_PWM_MAX_N = 60;         // cooling with low temperature difference < 4.5, increase allowed maximum current to 60%
        temp_ref_cur = temp_l;
    }
    temp_ref_old = temp_ref;

//    if (temp_ref < 20)
//        temp_ref = 20;
//    else if (temp_ref > 40)
//        temp_ref = 40;

    vibeFreq_ref = i2c2_rx_buff[2] | (i2c2_rx_buff[3] << 8);
    /* scale 0 - 500*/

    if (vibeFreq_ref > 100)
        vibeFreq_ref = 100;
    if (vibeFreq_ref < 0)
        vibeFreq_ref = 0;
    pwmMotor = vibeFreq_ref;
    ctlLED_r[0] = i2c2_rx_buff[4];
    ctlLED_r[1] = i2c2_rx_buff[5];
    ctlLED_r[2] = i2c2_rx_buff[6];
    diagLED_r[0] = i2c2_rx_buff[7];
    diagLED_r[1] = i2c2_rx_buff[8];
    diagLED_r[2] = i2c2_rx_buff[9];
}

/*
 * Function updates tx_buff used in i2c communication with Beaglebone with
 *  new measured/calculated data
 */
void updateMeasurements() {
    UINT16 dummy;

    if (temp_f >= 0)
        dummy = (int)(temp_f * 10);
    else
        dummy = (int)(temp_f * 10) + 65536;
    i2c2_tx_buff[0] = dummy & 0x00FF;
    i2c2_tx_buff[1] = (dummy & 0xFF00) >> 8;

    if (temp_r >= 0)
        dummy = (temp_r * 10);
    else
        dummy = (int)(temp_r * 10) + 65536;
    i2c2_tx_buff[2] = dummy & 0x00FF;
    i2c2_tx_buff[3] = (dummy & 0xFF00) >> 8;

    if (temp_b >= 0)
        dummy = (temp_b * 10);
    else
        dummy = (int)(temp_b * 10) + 65536;
    i2c2_tx_buff[4] = dummy & 0x00FF;
    i2c2_tx_buff[5] = (dummy & 0xFF00) >> 8;
    
    if (temp_l >= 0)
        dummy = (temp_l * 10);
    else
        dummy = (int)(temp_l * 10) + 65536;
    i2c2_tx_buff[6] = dummy & 0x00FF;
    i2c2_tx_buff[7] = (dummy & 0xFF00) >> 8;

    if (temp_t >= 0)
        dummy = (temp_t * 10);
    else
        dummy = (int)(temp_t * 10) + 65536;
    i2c2_tx_buff[8] = dummy & 0x00FF;
    i2c2_tx_buff[9] = (dummy & 0xFF00) >> 8;

    dummy = vAmp_f * 10;
    i2c2_tx_buff[10] = dummy & 0x00FF;
    i2c2_tx_buff[11] = (dummy & 0xFF00) >> 8;

    dummy = vAmp_r * 10;
    i2c2_tx_buff[12] = dummy & 0x00FF;
    i2c2_tx_buff[13] = (dummy & 0xFF00) >> 8;

    dummy = vAmp_b * 10;
    i2c2_tx_buff[14] = dummy & 0x00FF;
    i2c2_tx_buff[15] = (dummy & 0xFF00) >> 8;

    dummy = vAmp_l * 10;
    i2c2_tx_buff[16] = dummy & 0x00FF;
    i2c2_tx_buff[17] = (dummy & 0xFF00) >> 8;

    i2c2_tx_buff[18] = fAmp_f & 0x00FF;
    i2c2_tx_buff[19] = (fAmp_f & 0xFF00) >> 8;

    i2c2_tx_buff[20] = fAmp_r & 0x00FF;
    i2c2_tx_buff[21] = (fAmp_r & 0xFF00) >> 8;

    i2c2_tx_buff[22] = fAmp_b & 0x00FF;
    i2c2_tx_buff[23] = (fAmp_b& 0xFF00) >> 8;

    i2c2_tx_buff[24] = fAmp_l & 0x00FF;
    i2c2_tx_buff[25] = (fAmp_l & 0xFF00) >> 8;

    i2c2_tx_buff[26] = proxy_f & 0x00FF;
    i2c2_tx_buff[27] = (proxy_f & 0xFF00) >> 8;

    i2c2_tx_buff[28] = proxy_fr & 0x00FF;
    i2c2_tx_buff[29] = (proxy_fr & 0xFF00) >> 8;

    i2c2_tx_buff[30] = proxy_br & 0x00FF;
    i2c2_tx_buff[31] = (proxy_br & 0xFF00) >> 8;

    i2c2_tx_buff[32] = proxy_b & 0x00FF;
    i2c2_tx_buff[33] = (proxy_b & 0xFF00) >> 8;

    i2c2_tx_buff[34] = proxy_bl & 0x00FF;
    i2c2_tx_buff[35] = (proxy_bl & 0xFF00) >> 8;

    i2c2_tx_buff[36] = proxy_fl & 0x00FF;
    i2c2_tx_buff[37] = (proxy_fl & 0xFF00) >> 8;

    i2c2_tx_buff[38] = proxy_t & 0x00FF;
    i2c2_tx_buff[39] = (proxy_t & 0xFF00) >> 8;

    if(ctlPeltier < 0)
        ctlPeltier += 200;
    
    i2c2_tx_buff[40] = ctlPeltier;
    i2c2_tx_buff[41] = pwmMotor;

    // send back what you received
    i2c2_tx_buff[42] = ctlLED_r[0];
    i2c2_tx_buff[43] = ctlLED_r[1];
    i2c2_tx_buff[44] = ctlLED_r[2];

    i2c2_tx_buff[45] = diagLED_r[0];
    i2c2_tx_buff[46] = diagLED_r[1];
    i2c2_tx_buff[47] = diagLED_r[2];
}