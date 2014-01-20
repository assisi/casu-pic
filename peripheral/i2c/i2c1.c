#include "i2c1.h"

UINT8 rx_buff[BUFF_SIZE] = {0};
UINT8 rx_head = 0;
UINT8 rx_bytes = 0;
UINT8 tx_buff[BUFF_SIZE] = {0};
UINT8 tx_head = 0;
UINT8 tx_bytes = 0;
UINT16 i2cIntNum = 0;

UINT8 I2C1SlaveInit(UINT8 address, UINT8 int_priority) {

    I2C1CONbits.SEN = 0;        // disable module before setting

    I2C1MSK = 0;                // disable address masking
    I2C1ADD = address;          // set up the address

    /*check*/
    I2C1CONbits.STREN = 1;      // Enable software or receive clock streching;
    I2C1CONbits.GCEN = 0;       // General call address disabled
    I2C1CONbits.SMEN = 0;       // Disable SMBus input thresholds
    I2C1CONbits.DISSLW = 0;     // Slew rate control enabled
    I2C1CONbits.A10M = 0;       // 7-bit slave address
    I2C1CONbits.IPMIEN = 0;     // IPMI support disabled
    /*check*/
    I2C1CONbits.SCLREL = 1;     // release SCLx clock
    /*check*/
    I2C1CONbits.I2CSIDL = 0;    // Continue module operation in IDLE mode
    UINT8 dummy = I2C1RCV;      // read value to clear i2c1RBF flag
    I2C1STATbits.I2COV = 0;     // clear overflow flag


    _SI2C1IE = 0;               // Disable slave interrupt
    _SI2C1IF = 0;               // clear interrupt flag
    _SI2C1IP = int_priority;               // set interrupt priority
    _SI2C1IE = 1;               // enable slave interrupt


    I2C1CONbits.I2CEN = 1;      // enables module and configures SDA1 and SCL1 as serial port inputs

    return 1;                   // i2c1 initialized and enabled
}

UINT8 I2C1SlaveWrite() {
    
    int i = 0;                                      // "watchdog timer" variable
    while(1) {
        I2C1TRN = tx_buff[tx_head++];               //send byte
        while (!_SI2C1IF) {                         // wait for master to respond
            i++;
            if (i == WAIT_TIME) {                   // check if the waiting lasts too long
                tx_head = 0;
                return 0;
            }
        }
        _SI2C1IF = 0;
        if (I2C1STATbits.ACKSTAT == 1) break;       // master expects no more data
        if (I2C1STATbits.TBF == 1) {
            // sending data failed
            I2C1STATbits.TBF = 0;
        }
        if (tx_head == BUFF_SIZE) tx_head = 0;      // reset tx_head

        i = 0;
    }
    
    return 1;
}

UINT8 I2C1SlaveRead() {

    int i = 0;  // watchog timer
    if ((I2C1CONbits.STREN) && (!I2C1CONbits.SCLREL))
            I2C1CONbits.SCLREL = 1;                 // Clock is released after ACK
    while(!I2C1STATbits.P) {                        // wait for the stop bit
        while(!I2C1STATbits.RBF) {                  // wait for the received byte
            if (I2C1STATbits.P) return 1;           // check the stop bit
            i++;
            if (i == WAIT_TIME) {
                rx_head = 0;
                return 0;
            }

        }
        rx_buff[rx_head++] = I2C1RCV;
        I2C1STATbits.I2COV = 0;                     // reset overflow flag
        if ((I2C1CONbits.STREN) && (!I2C1CONbits.SCLREL))
            I2C1CONbits.SCLREL = 1;                 // Clock is released after ACK
        if (rx_head == BUFF_SIZE) rx_head = 0;
        i = 0;
    }
    return 1;
}

/*
 * Slave i2c1 interrupt service routine
 */
void __attribute__((__interrupt__, auto_psv)) _SI2C1Interrupt(void) {
    
    i2cIntNum++;

    if (I2C1STATbits.D_A == 0) {
        // device address detected
        rx_head = 0;
        tx_head = 0;
        UINT8 dummy;
        if (I2C1STATbits.R_W == 0) {
            // master request writing
            dummy = I2C1RCV;  // dummy read
        }
        else {
            // master request reading
            dummy = I2C1RCV;  // dummy read
            I2C1TRN = tx_buff[tx_head++];
            int i = 0;                  // watchdog variable
            while(I2C1STATbits.TBF) {
                //Wait till all
                i++;
                if (i == WAIT_TIME) break;
            }
        }
    }
    else {
        // data byte incoming or outgoing
        if (I2C1STATbits.R_W == 0) {
            // master requests writing
            rx_buff[rx_head++] = I2C1RCV;
        }
        else {
            // master request reading
            UINT8 dummy;
            dummy = I2C1RCV;  // dummy read

            if (I2C1STATbits.ACKSTAT == 0) {
                // master expects more bytes
                I2C1TRN = tx_buff[tx_head++];
                int i = 0;
                while(I2C1STATbits.TBF) {
                    //Wait till all
                    i++;
                    if (i == WAIT_TIME) break;
                }
            }
        }

    }
    if (I2C1CONbits.SCLREL == 0) {
        I2C1CONbits.SCLREL = 1;	// Release SCL1 line
    }
    _SI2C1IF = 0;
}
