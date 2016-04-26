#include "i2c2.h"

UINT8 i2c2_rx_buff[BUFF_SIZE] = {0};     // buffer for incoming data
UINT8 i2c2_rx_head = 0;                  // pointer to buffer element where new byte is to be stored
UINT8 i2c2_tx_buff[BUFF_SIZE] = {0};     // buffer for outgoing data
UINT8 i2c2_tx_head = 0;                  // pointer to a buffer element which will be send in next outgoing transmission
UINT8 msg_id = 0;
UINT16 msg_rec_bytes = 0;
UINT8 msg_status = 0;
UINT8 i2cStarted = 0;

/*
 * Function initializes i2c2 module as a slave device
 * inputs: address - i2c address to be assigned to device
 *         int_priority - interrupt priority to be assigned to slave I2C2 interrupt
 * returns: 1 - device initialized and enabled
 */
UINT8 I2C2SlaveInit(UINT8 address, UINT8 int_priority) {

    I2C2CONbits.SEN = 0;        // disable module before setting

    I2C2MSK = 0;                // disable address masking
    I2C2ADD = address;          // set up the address

    /*check*/
    I2C2CONbits.STREN = 1;      // Enable software or receive clock stretching;
    I2C2CONbits.GCEN = 0;       // General call address disabled
    I2C2CONbits.SMEN = 0;       // Disable SMBus input thresholds
    I2C2CONbits.DISSLW = 0;     // Slew rate control enabled
    I2C2CONbits.A10M = 0;       // 7-bit slave address
    I2C2CONbits.IPMIEN = 0;     // IPMI support disabled
    /*check*/
    I2C2CONbits.SCLREL = 1;     // release SCLx clock
    /*check*/
    I2C2CONbits.I2CSIDL = 0;    // Continue module operation in IDLE mode
    UINT8 dummy;
    dummy = I2C2RCV;      // read value to clear I2C2RBF flag
    I2C2STATbits.I2COV = 0;     // clear overflow flag


    _SI2C2IE = 0;               // Disable slave interrupt
    _SI2C2IF = 0;               // clear interrupt flag
    _SI2C2IP = int_priority;    // set interrupt priority
    _SI2C2IE = 1;               // enable slave interrupt


    I2C2CONbits.I2CEN = 1;      // enables module and configures SDA1 and SCL1 as serial port inputs

    return 1;                   // I2C2 initialized and enabled
}

/*
 * Slave I2C2 interrupt service routine
 * When interrupt occurs immediately store incoming data to rx_buff or send outgoing data from tx_buff
 */
void __attribute__((__interrupt__, auto_psv)) _SI2C2Interrupt(void) {

    if (I2C2STATbits.D_A == 0) {    // device address detected
        i2c2_rx_head = 0;
        i2c2_tx_head = 0;
        UINT8 dummy;
        msg_rec_bytes = 0;
        if (I2C2STATbits.R_W == 0) {
            // master request writing
            dummy = I2C2RCV;  // dummy read
        }
        else {
            // master request reading
            dummy = I2C2RCV;  // dummy read
            I2C2TRN = i2c2_tx_buff[i2c2_tx_head++];
            i2cStarted = 1;
            int i = 0;                  // watchdog variable
            while(I2C2STATbits.TBF) {
                //Wait till all
                i++;
                if (i == WAIT_TIME) break;
            }
        }
    }
    else {
        // data byte incoming or outgoing
        if (I2C2STATbits.R_W == 0) {
            // master requests writing
                // start reading data as there are no unread messages
                msg_rec_bytes++;
                if (msg_rec_bytes == 1)
                    msg_id = I2C2RCV;
                else
                    i2c2_rx_buff[i2c2_rx_head++] = I2C2RCV;

                if (msg_id == MSG_REF_VIBE_ID && msg_rec_bytes == IN_VIBE_REF_DATA_NUM) {
                    msg_status = MSG_REF_VIBE_ID;
                    updateReferences(msg_status);  //TODO update this function, use msg_status variable as as parameter.
                    msg_status = 0;
                }
                else if (msg_id == MSG_REF_LED_ID && msg_rec_bytes == IN_LED_REF_DATA_NUM) {
                    msg_status = MSG_REF_LED_ID;
                    updateReferences(msg_status);
                    msg_status = 0;
                }
                else if (msg_id == MSG_REF_TEMP_ID && msg_rec_bytes == IN_TEMP_REF_DATA_NUM) {
                    msg_status = MSG_REF_TEMP_ID;
                    updateReferences(msg_status);
                    msg_status = 0;
                }
                else if (msg_id == MSG_CAL_ID && msg_rec_bytes == IN_CAL_DATA_NUM) {
                    updateCalibrationData();
                    msg_status = 0;
                }
        }
        else {
            // master request reading
            UINT8 dummy;
            dummy = I2C2RCV;  // dummy read

            if (I2C2STATbits.ACKSTAT == 0) {
                // master expects more bytes
                I2C2TRN = i2c2_tx_buff[i2c2_tx_head++];
                int i = 0;
                while(I2C2STATbits.TBF) {
                    //Wait till all
                    i++;
                    if (i == WAIT_TIME) break;
                }
            }
        }
    }

    if (I2C2CONbits.SCLREL == 0) {
        I2C2CONbits.SCLREL = 1;	// Release SCL1 line
    }
    _SI2C2IF = 0;
}
