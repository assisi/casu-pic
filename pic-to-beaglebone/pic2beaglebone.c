#include "pic2beaglebone.h"

/*Init variables for measured data*/
float temp_f = -1, temp_b = -1, temp_r = -1, temp_l = -1, temp_t = -1, temp_casu = 25, temp_casu1 = 25, temp_wax = 25, temp_wax1 = 25;
float vAmp_f = -1 , vAmp_b = -1, vAmp_r = -1, vAmp_l = -1;
float fAmp_f = 0, fAmp_b = 0, fAmp_r = 0, fAmp_l = 0;
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
UINT16 motPwm_ref = 0;
UINT8 ctlLED_r[3] = {0};
UINT8 diagLED_r[3] = {0};
UINT8 fanBlower_r = 0;
UINT8 calRec = 0;
int raw_acc[4][FFT_BUFF] = {0};
int raw_acc_single[FFT_BUFF] = {0};
int source_array[FFT_BUFF] = {0};
int amplitudes[FFT_BUFF] = {0};

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
    
    if (temp_ref > temp_ref_h)
        temp_ref = temp_ref_h;

    if ((temp_ref - temp_ref_old > 0.1) || (temp_ref - temp_ref_old <  -0.1)) {
        // temperature reference changed
//        if (temp_ref > temp_l)      // left temperature sensor is used as feedback
//            PELTIER_PWM_MAX_N = 85;         // heating
//        else if (temp_l - temp_ref > 4.5)   // cooling with high temperature difference > 4.5, reduce maximum current to 30%
//            PELTIER_PWM_MAX_N = 30;
//        else
//            PELTIER_PWM_MAX_N = 60;         // cooling with low temperature difference < 4.5, increase allowed maximum current to 60%
        if ((temp_ref - temp_ref_old) * ctlPeltier < -0.1) {  // changing heating/cooling direction
            PeltierResetPID();
        }

        temp_ref_cur = temp_wax;
    }
    temp_ref_old = temp_ref;

//    if (temp_ref < 20)
//        temp_ref = 20;
//    else if (temp_ref > 40)
//        temp_ref = 40;
    motPwm_ref = i2c2_rx_buff[2] | (i2c2_rx_buff[3] << 8);
    /* scale 0 - 500*/

    if (motPwm_ref > 100)
        motPwm_ref = 100;
    if (motPwm_ref < 0)
        motPwm_ref = 0;
    pwmMotor = motPwm_ref;
    ctlLED_r[0] = i2c2_rx_buff[4];
    ctlLED_r[1] = i2c2_rx_buff[5];
    ctlLED_r[2] = i2c2_rx_buff[6];
    diagLED_r[0] = i2c2_rx_buff[7];
    diagLED_r[1] = i2c2_rx_buff[8];
    diagLED_r[2] = i2c2_rx_buff[9];
    fanBlower_r = i2c2_rx_buff[10];

    LedUser(diagLED_r[0], diagLED_r[1],diagLED_r[2]);
    VibrationSet(pwmMotor);     // this is actually motor pwm
    FanBlower(fanBlower_r);
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
    UINT16 dummy;
    int dummy1;

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

    dummy = fAmp_f * 10;
    i2c2_tx_buff[18] = dummy & 0x00FF;
    i2c2_tx_buff[19] = (dummy & 0xFF00) >> 8;

    dummy = fAmp_r * 10;
    i2c2_tx_buff[20] = dummy & 0x00FF;
    i2c2_tx_buff[21] = (dummy & 0xFF00) >> 8;

    dummy = fAmp_b * 10;
    i2c2_tx_buff[22] = dummy & 0x00FF;
    i2c2_tx_buff[23] = (dummy & 0xFF00) >> 8;

    dummy = fAmp_l * 10;
    i2c2_tx_buff[24] = dummy & 0x00FF;
    i2c2_tx_buff[25] = (dummy & 0xFF00) >> 8;

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
        dummy1 = ctlPeltier + 201;
    else
        dummy1 = ctlPeltier;
    
    i2c2_tx_buff[40] = dummy1;
    i2c2_tx_buff[41] = pwmMotor;

    // send back what you received
    i2c2_tx_buff[42] = ctlLED_r[0];
    i2c2_tx_buff[43] = ctlLED_r[1];
    i2c2_tx_buff[44] = ctlLED_r[2];

    i2c2_tx_buff[45] = diagLED_r[0];
    i2c2_tx_buff[46] = diagLED_r[1];
    i2c2_tx_buff[47] = diagLED_r[2];

    i2c2_tx_buff[48] = fanBlower_r;
    i2c2_tx_buff[49] = fanCooler;

    //CASU ring temperature
    if (temp_casu >= 0)
        dummy = (temp_casu * 10);
    else
        dummy = (int)(temp_casu * 10) + 65536;
//    if (temp_ref_cur >= 0)
//        dummy = (temp_ref_cur * 10);
//    else
//        dummy = (int)(temp_ref_cur * 10) + 65536;
    i2c2_tx_buff[50] = dummy & 0x00FF;
    i2c2_tx_buff[51] = (dummy & 0xFF00) >> 8;

    //CASU wax temperature estimated value
    if (temp_wax >= 0)
        dummy = (temp_wax * 10);
    else
        dummy = (int)(temp_wax * 10) + 65536;
    i2c2_tx_buff[52] = dummy & 0x00FF;
    i2c2_tx_buff[53] = (dummy & 0xFF00) >> 8;
    if (temp_ref >= 0)
        dummy = (temp_ref * 10);
    else
        dummy = (int)(temp_ref * 10) + 65536;
    i2c2_tx_buff[54] = dummy & 0x00FF;
    i2c2_tx_buff[55] = (dummy & 0xFF00) >> 8;
    if (temp_ref_cur >= 0)
        dummy = (temp_ref_cur * 10);
    else
        dummy = (int)(temp_ref_cur * 10) + 65536;
    i2c2_tx_buff[56] = dummy & 0x00FF;
    i2c2_tx_buff[57] = (dummy & 0xFF00) >> 8;
    i2c2_tx_buff[58] = calRec;
}

void updateAccLog() {
    UINT16 i, dummy ;

    for (i = 0; i < FFT_BUFF; i=i+1) {

        /*
        if (source_array[i] >= 0)
            dummy = source_array[i];
        else {
            dummy = source_array[i] + 65536;
        }
        */

        if (amplitudes[i] >= 0)
            dummy = amplitudes[i];
        else {
            dummy = amplitudes[i] + 65536;
        }

        i2c2_tx_buff[2*i] = dummy & 0x00FF;
        i2c2_tx_buff[2*i+1] = (dummy & 0xFF00) >> 8;
    }
    i2c2_tx_ready = 1;
}