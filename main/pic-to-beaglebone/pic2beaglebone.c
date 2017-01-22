#include "pic2beaglebone.h"

/*Init variables for measured data*/
float temp_f = -1, temp_b = -1, temp_r = -1, temp_l = -1, temp_pcb = -1,
        temp_casu = 25, temp_casu1 = 25, temp_wax = 25, temp_wax1 = 25, temp_flexPCB = -1;
float vAmp_m[4] = {-1.0};
UINT16 fAmp_m[4] = {0};
UINT16 proxy_f = 0, proxy_fr = 0, proxy_br = 0, proxy_b = 0, proxy_bl = 0, proxy_fl = 0, proxy_t = 0;
float ctlPeltier = 0;
int fanCooler = 0;
UINT8 tempCtlOn = 0, fanCtlOn = 0;
float Kf1 = 0.1428, Kf2 = 0.1428, Kf3 = 0.7101;
float Kp = 20, Ki = 0.9;
float temp_ref_shutdown = 24.9;
float temp_ref_l = 26, temp_ref_h = 45;
//UINT8 pwmMotor = 0;

/* Init variables for storing references and control inputs*/
float temp_ref = 25.0;
float temp_ref_old = 25.0;
float temp_ref_cur = 25.0;
UINT16 motPwm_ref = 0;
UINT8 ctlLED_r[3] = {0};
UINT8 diagLED_r[3] = {0};
UINT8 fanBlower_r = 0;
UINT8 calRec = 0;
UINT16 speakerAmp_ref = 0, speakerFreq_ref = 0;
UINT16 speakerAmp_ref_old = 0;
UINT16 speakerFreq_ref_old = 0;
UINT8 proxyStandby = 0;
int source_array[FFT_BUFF] = {0};
int amplitudes[FFT_BUFF/2] = {0};
float accPeriod;
UINT16 maxAmp;

/*
 * Function updates references (temperature, motor, LED1, LED2) transfered from beaglebone.
 * It takes data from rx_buff used in i2c communication with beaglebone
 */
void updateReferences(UINT8 msg_id) {
    UINT16 dummy;
    int status = 0;

    if (msg_id == MSG_REF_VIBE_ID) {

        speakerAmp_ref = i2c2_rx_buff[0];
        if (speakerAmp_ref > 100)
            speakerAmp_ref = 100;
        else if (speakerAmp_ref < 0)
            speakerAmp_ref = 0;

        speakerFreq_ref = i2c2_rx_buff[1] | (i2c2_rx_buff[2] << 8);
        if (speakerFreq_ref > 2000)
            speakerFreq_ref = 2000;
        else if (speakerFreq_ref < 1)
            speakerFreq_ref = 1;

        int count = 0;
        //fAmp_m[0] = 0;
        UINT16 inBuff[2] = {0};
        UINT16 outBuff[2] = {0};
        inBuff[0] = (speakerAmp_ref & 0x0FFF) | 0x1000;
        if (SPI1STATbits.SPITBF)
            while(SPI1STATbits.SPITBF);
        while (speakerAmp_ref != speakerAmp_ref_old) {
            //fAmp_m[1] = count;
            if (count > 0) {
                delay_t1(5);
            }
            if (count > 10 ) {
                //fAmp_m[0] = 1;
                // Error !
                //LedUser(100,0,0);

                break;
            }
            chipSelect(slaveVib);
            status = spi1TransferWord(inBuff[0], outBuff);
            chipDeselect(slaveVib);
            //fAmp_m[2] = status;

            //delay_t1(1);
            chipSelect(slaveVib);
            status = spi1TransferWord(inBuff[0], &speakerAmp_ref_old);
            chipDeselect(slaveVib);

            count++;
        }

        count = 0;
        if (SPI1STATbits.SPITBF) {
            //LedUser(100,0,0);
            while(SPI1STATbits.SPITBF);

        }
        inBuff[0] = (speakerFreq_ref & 0x0FFF) | 0x2000;
        while (speakerFreq_ref != speakerFreq_ref_old) {
            if (count > 0) {
                delay_t1(5);
            }
            if (count > 10 ) {
                // Error !
                //LedUser(0,100,0);
                break;
            }
            chipSelect(slaveVib);
            status = spi1TransferWord(inBuff[0], outBuff);
            chipDeselect(slaveVib);

            chipSelect(slaveVib);
            status = spi1TransferWord(inBuff[0], &speakerFreq_ref_old);
            chipDeselect(slaveVib);
            count++;
        }

    }
    else if (msg_id == MSG_REF_LED_ID) {
        diagLED_r[0] = i2c2_rx_buff[0];
        diagLED_r[1] = i2c2_rx_buff[1];
        diagLED_r[2] = i2c2_rx_buff[2];
        LedUser(diagLED_r[0], diagLED_r[1], diagLED_r[2]);
    }
    else if (msg_id == MSG_REF_TEMP_ID) {
        dummy = i2c2_rx_buff[0] | (i2c2_rx_buff[1] << 8);
        if (dummy > 32767)
            temp_ref = (dummy - 65536) / 10.0;
        else
            temp_ref = dummy / 10.0;
        if (temp_ref < 26)
            temp_ref = 0.0;
    }
    else if (msg_id == MSG_REF_PROXY_ID) {
        proxyStandby = i2c2_rx_buff[0];
    }
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

     if (temp_flexPCB >= 0)
        dummy = (temp_flexPCB * 10);
    else
        dummy = (int)(temp_flexPCB * 10) + 65536;
    i2c2_tx_buff[8] = dummy & 0x00FF;
    i2c2_tx_buff[9] = (dummy & 0xFF00) >> 8;

    if (temp_pcb >= 0)
        dummy = (temp_pcb * 10);
    else
        dummy = (int)(temp_pcb * 10) + 65536;
    i2c2_tx_buff[10] = dummy & 0x00FF;
    i2c2_tx_buff[11] = (dummy & 0xFF00) >> 8;

      //CASU ring temperature
    if (temp_casu >= 0)
        dummy = (temp_casu * 10);
    else
        dummy = (int)(temp_casu * 10) + 65536;

    i2c2_tx_buff[12] = dummy & 0x00FF;
    i2c2_tx_buff[13] = (dummy & 0xFF00) >> 8;

    //CASU wax temperature estimated value
    if (temp_wax >= 0)
        dummy = (temp_wax * 10);
    else
        dummy = (int)(temp_wax * 10) + 65536;
    i2c2_tx_buff[14] = dummy & 0x00FF;
    i2c2_tx_buff[15] = (dummy & 0xFF00) >> 8;

    if (temp_ref >= 0)
        dummy = (temp_ref * 10);
    else
        dummy = (int)(temp_ref * 10) + 65536;
    i2c2_tx_buff[16] = dummy & 0x00FF;
    i2c2_tx_buff[17] = (dummy & 0xFF00) >> 8;

    dummy = vAmp_m[0] * 10;
    i2c2_tx_buff[18] = dummy & 0x00FF;
    i2c2_tx_buff[19] = (dummy & 0xFF00) >> 8;
    i2c2_tx_buff_fast[0] = i2c2_tx_buff[18];
    i2c2_tx_buff_fast[1] = i2c2_tx_buff[19];

    dummy = vAmp_m[1] * 10;
    i2c2_tx_buff[20] = dummy & 0x00FF;
    i2c2_tx_buff[21] = (dummy & 0xFF00) >> 8;
    i2c2_tx_buff_fast[2] = i2c2_tx_buff[20];
    i2c2_tx_buff_fast[3] = i2c2_tx_buff[21];

    dummy = vAmp_m[2] * 10;
    i2c2_tx_buff[22] = dummy & 0x00FF;
    i2c2_tx_buff[23] = (dummy & 0xFF00) >> 8;
    i2c2_tx_buff_fast[4] = i2c2_tx_buff[22];
    i2c2_tx_buff_fast[5] = i2c2_tx_buff[23];

    dummy = vAmp_m[3] * 10;
    i2c2_tx_buff[24] = dummy & 0x00FF;
    i2c2_tx_buff[25] = (dummy & 0xFF00) >> 8;
    i2c2_tx_buff_fast[6] = i2c2_tx_buff[24];
    i2c2_tx_buff_fast[7] = i2c2_tx_buff[25];

// using fAmp_m[0] & fAmp_m[1] as flag and count

    //fAmp_m[0] = fAmp;
    //fAmp_m[0] = (int) *(source_array + 25);
    i2c2_tx_buff[26] = fAmp_m[0] & 0x00FF;
    i2c2_tx_buff[27] = (fAmp_m[0] & 0xFF00) >> 8;
    i2c2_tx_buff_fast[8] = i2c2_tx_buff[26];
    i2c2_tx_buff_fast[9] = i2c2_tx_buff[27];

    //fAmp_m[1] = (int) *(source_array + 26);
    i2c2_tx_buff[28] = fAmp_m[1] & 0x00FF;
    i2c2_tx_buff[29] = (fAmp_m[1] & 0xFF00) >> 8;
    i2c2_tx_buff_fast[10] = i2c2_tx_buff[28];
    i2c2_tx_buff_fast[11] = i2c2_tx_buff[29];

    i2c2_tx_buff[30] = fAmp_m[2] & 0x00FF;
    i2c2_tx_buff[31] = (fAmp_m[2] & 0xFF00) >> 8;
    i2c2_tx_buff_fast[12] = i2c2_tx_buff[30];
    i2c2_tx_buff_fast[13] = i2c2_tx_buff[31];

    i2c2_tx_buff[32] = fAmp_m[3] & 0x00FF;
    i2c2_tx_buff[33] = (fAmp_m[3] & 0xFF00) >> 8;
    i2c2_tx_buff_fast[14] = i2c2_tx_buff[32];
    i2c2_tx_buff_fast[15] = i2c2_tx_buff[33];

     // send back what you received
    i2c2_tx_buff[34] = speakerAmp_ref_old;
    i2c2_tx_buff[35] = speakerFreq_ref_old & 0x00FF;
    i2c2_tx_buff[36] = (speakerFreq_ref_old & 0xFF00) >> 8;
    i2c2_tx_buff_fast[16] = i2c2_tx_buff[34];
    i2c2_tx_buff_fast[17] = i2c2_tx_buff[35];
    i2c2_tx_buff_fast[18] = i2c2_tx_buff[36];

    i2c2_tx_buff[37] = proxy_f & 0x00FF;
    i2c2_tx_buff[38] = (proxy_f & 0xFF00) >> 8;
    i2c2_tx_buff_fast[19] = i2c2_tx_buff[37];
    i2c2_tx_buff_fast[20] = i2c2_tx_buff[38];

    i2c2_tx_buff[39] = proxy_fr & 0x00FF;
    i2c2_tx_buff[40] = (proxy_fr & 0xFF00) >> 8;
    i2c2_tx_buff_fast[21] = i2c2_tx_buff[39];
    i2c2_tx_buff_fast[22] = i2c2_tx_buff[40];

    i2c2_tx_buff[41] = proxy_br & 0x00FF;
    i2c2_tx_buff[42] = (proxy_br & 0xFF00) >> 8;
    i2c2_tx_buff_fast[23] = i2c2_tx_buff[41];
    i2c2_tx_buff_fast[24] = i2c2_tx_buff[42];

    i2c2_tx_buff[43] = proxy_b & 0x00FF;
    i2c2_tx_buff[44] = (proxy_b & 0xFF00) >> 8;
    i2c2_tx_buff_fast[25] = i2c2_tx_buff[43];
    i2c2_tx_buff_fast[26] = i2c2_tx_buff[44];

    i2c2_tx_buff[45] = proxy_bl & 0x00FF;
    i2c2_tx_buff[46] = (proxy_bl & 0xFF00) >> 8;
    i2c2_tx_buff_fast[27] = i2c2_tx_buff[45];
    i2c2_tx_buff_fast[28] = i2c2_tx_buff[46];

    i2c2_tx_buff[47] = proxy_fl & 0x00FF;
    i2c2_tx_buff[48] = (proxy_fl & 0xFF00) >> 8;
    i2c2_tx_buff_fast[29] = i2c2_tx_buff[47];
    i2c2_tx_buff_fast[30] = i2c2_tx_buff[48];

    if(ctlPeltier < 0) {
        dummy1 = ctlPeltier + 201;
        dummy1 = (int)((ctlPeltier + 201.0) * 100.0);
    }
    else {
        dummy1 = (int)(ctlPeltier * 100.0);
    }

    i2c2_tx_buff[49] = dummy1 & 0x00FF;
    i2c2_tx_buff[50] = (dummy1 & 0xFF00) >> 8;

    i2c2_tx_buff[51] = diagLED_r[0];
    i2c2_tx_buff[52] = diagLED_r[1];
    i2c2_tx_buff[53] = diagLED_r[2];
/*    i2c2_tx_buff_fast[31] = diagLED_r[0];
    i2c2_tx_buff_fast[32] = diagLED_r[1];
    i2c2_tx_buff_fast[33] = diagLED_r[2];
*/  i2c2_tx_buff_fast[31] = i2c2_tx_buff[51];
    i2c2_tx_buff_fast[32] = i2c2_tx_buff[52];
    i2c2_tx_buff_fast[33] = i2c2_tx_buff[53];

    if (fanCooler == FAN_COOLER_ON) {
        i2c2_tx_buff[54] = 100;
        i2c2_tx_buff_fast[34] = i2c2_tx_buff[54];
    }
    else {
        i2c2_tx_buff[54] = 0;
        i2c2_tx_buff_fast[34] = i2c2_tx_buff[54];
    }

    i2c2_tx_buff[55] = calRec;
    i2c2_tx_buff_fast[35] = i2c2_tx_buff[55];

}

void updateAccLog() {
    UINT16 i, dummy;
    UINT16 iMax = 0;

    maxAmp = *(amplitudes + 0);

    for (i = 0; i < FFT_BUFF; i++) {

        if (amplitudes[i] >= 0)
            dummy = amplitudes[i];
        else {
            dummy = amplitudes[i] + 65536;
        }

//sta ne bi amplitude trebale bit pozitivne??

        if ((i < 127) & (dummy > maxAmp)){
            maxAmp = dummy;
            iMax = i;
        }

        //dummy = ((*(source_array + i)) << (16 - ACC_RES - 1));
        //if (i > 127) dummy = 10;
        i2c2_tx_buff_acc[2*i] = dummy & 0x00FF;
        i2c2_tx_buff_acc[2*i+1] = (dummy & 0xFF00) >> 8;
//        i2c2_tx_buff_acc[2*i+1] = 1;

    }


    UINT16 sampleFreq = 1000000 / accPeriod; // us to Hz
    fAmp_m[0] = (int) (iMax * sampleFreq / FFT_BUFF);
    fAmp_m[1] = iMax + 1;

    i2c2_tx_ready = 1;
}
