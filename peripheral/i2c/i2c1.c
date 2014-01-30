#include "i2c1.h"
#include <i2c.h>

UINT8 rx_buff[BUFF_SIZE] = {0};     // buffer for incoming data
UINT8 rx_head = 0;                  // pointer to buffer element where new byte is to be stored
UINT8 tx_buff[BUFF_SIZE] = {0};     // buffer for outgoing data
UINT8 tx_head = 0;                  // pointer to a buffer element which will be send in next outgoing transmission

/*
 * Function initializes i2c1 module as a master device
 * inputs: none
 * returns: 1 - device initialized and enabled
 */
UINT8 I2C1MasterInit(void) {

    TRISDbits.TRISD9 = 0;
    TRISDbits.TRISD10 = 0;
    TRISAbits.TRISA15 = 0;
    PORTAbits.RA15 = 1;

    I2C1CON = 0;                //Reset I2C1
    //Control register
    I2C1CONbits.IPMIEN = 0;     // IPMI support disabled
    I2C1CONbits.A10M = 0;       // 7-bit slave address
    I2C1CONbits.DISSLW = 0;     // Slew rate control disabled
    I2C1CONbits.SMEN = 0;       // Disable SMBus input thresholds
    I2C1CONbits.ACKDT = 0;      // ACK data bit
    I2C1CONbits.ACKEN = 0;      // ACK enabled
    //Baud rate
    I2C1BRG = 0x18B;            //Fscl = 100kHz in case of Fcy = 40MHz
    //Disable interrupts
    IEC1bits.MI2C1IE = 0;   //DIsavble Master int
    IEC1bits.SI2C1IE = 0;   //Disable Slave int
    IFS1bits.MI2C1IF = 0;   //Clear Master int
    IFS1bits.SI2C1IF = 0;   //Clear Slave int

    UINT8 dummy = I2C1RCV;      // read value to clear i2c1RBF flag
    I2C1STATbits.I2COV = 0;     // clear overflow flag

    I2C1CONbits.I2CEN = 1;      // enables module and configures SDA1 and SCL1 as serial port inputs

    return 1;                   // i2c1 initialized and enabled
}

/*
 * Function initializes i2c1 module as a slave device
 * inputs: address - i2c address to be assigned to device
 *         int_priority - interrupt priority to be assigned to slave i2c1 interrupt
 * returns: 1 - device initialized and enabled
 */
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
    I2C1CONbits.SCLREL = 0;     // CLK 
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



/*
 * Function select I2C "PCA9547" multiplexor channel
 * inputs:  enable - mux enable
 *          channel - mux channel - [0, 7]
 * returns: -1 - error
 *          1 - channel selected
 */
int I2C1ChSelect(UINT8 enable, UINT8 channel) {

    int ret;
    if(channel > 7)     //Check channel range
        return -1;

    UINT8 data;
    data = (enable << 3) | channel;
    //ret = I2C1WriteByte(PCAMUX_ADD, data);
    return ret;
}

/*
 * PCA9547 test function
 * inputs:  none
 * returns: -1 - error
 *          1 - test ok
 */
int MUXTest() {

    UINT8 address;
    UINT8 check = 0;
    UINT8 trByte, recByte;
    recByte = 0;

    //Read byte from mux
    StartI2C1();
    IdleI2C1(); //Wait to complete
    address = (PCAMUX_ADD<<1) & 0xFE;    //write
    MasterWriteI2C1(address);
    IdleI2C1();
    if(I2C1STATbits.ACKSTAT)
    {
        StopI2C1();
        IdleI2C1();
        return -1;
    }
    MasterWriteI2C1(0x08);
    IdleI2C1();
    if(I2C1STATbits.ACKSTAT)
    {
        StopI2C1();
        IdleI2C1();
        return -1;
    }


    //Read byte from mux
    StartI2C1();
    IdleI2C1(); //Wait to complete
    address = (PCAMUX_ADD<<1) | 0x01;    //read
    MasterWriteI2C1(address);
    IdleI2C1();
    if(I2C1STATbits.ACKSTAT)
    {
        StopI2C1();
        IdleI2C1();
        return -1;
    }
    recByte = MasterReadI2C1();
    NotAckI2C1();
    IdleI2C1();
    StopI2C1();
    IdleI2C1();

    return recByte;
}

int MuxRead(void){

    UINT8 address;
    UINT8 recByte;

    //Read byte from mux
    StartI2C1();
    IdleI2C1(); //Wait to complete
    address = (PCAMUX_ADD<<1) | 0x01;    //read
    MasterWriteI2C1(address);
    IdleI2C1();
    if(I2C1STATbits.ACKSTAT)
    {
        StopI2C1();
        IdleI2C1();
        return -1;
    }
    recByte = MasterReadI2C1();
    NotAckI2C1();
    IdleI2C1();
    StopI2C1();
    IdleI2C1();

    return recByte;
}

/*
 * function reads byte from I2C data
 * inputs:  slaveAdd
 *          data
 * returns: 0 -> error is occurred
 *          1 -> return read data
 */
UINT8 I2C1WriteByte(UINT8 slaveAdd, UINT8 registerAdd, UINT8 data)
{
    UINT8 address;

    StartI2C1();
    IdleI2C1(); //Wait to complete
    address = (slaveAdd<<1) & 0xFE;    //write
    MasterWriteI2C1(address);
    IdleI2C1();
    if(I2C1STATbits.ACKSTAT)
    {
        StopI2C1();
        IdleI2C1();
        return 0;
    }

    //Send register register location
    MasterWriteI2C1(registerAdd);
    IdleI2C1();
    if(I2C1STATbits.ACKSTAT)
    {
        StopI2C1();
        IdleI2C1();
        return 0;
    }

    //Send register register location
    MasterWriteI2C1(data);
    IdleI2C1();
    if(I2C1STATbits.ACKSTAT)
    {
        StopI2C1();
        IdleI2C1();
        return 0;
    }
    StopI2C1();

    return 1;   //Byte is successful sent
}


/*
 * Function reads I2C device from slaveAdd
 * inputs:  slaveAdd
 *          registerAdd
 * returns: int recByte -> received byte from slave device
 *          -1 -> reading error
 */
unsigned char I2C1ReadByte(UINT8 slaveAdd, UINT8 registerAdd)
{
    unsigned char recByte = 0;
    UINT8 address;

    StartI2C1();
    IdleI2C1(); //Wait to complete
    address = (slaveAdd<<1) & 0xFE;    //write
    MasterWriteI2C1(address);
    IdleI2C1();
    if(I2C1STATbits.ACKSTAT)
    {
        StopI2C1();
        IdleI2C1();
        return 0;
    }

    //Send register register location
    MasterWriteI2C1(registerAdd);
    IdleI2C1();
    if(I2C1STATbits.ACKSTAT)
    {
        StopI2C1();
        IdleI2C1();
        return 0;
    }

    RestartI2C1(); //Send start condition again
    IdleI2C1();
    address = (slaveAdd<<1) | 0x01;    //Read
    MasterWriteI2C1(address);
    IdleI2C1();
    if(I2C1STATbits.ACKSTAT)
    {
        StopI2C1();
        IdleI2C1();
        return 0;
    }
    recByte = MasterReadI2C1();
    //NotAckI2C1();
    IdleI2C1();
    StopI2C1();
    IdleI2C1();

    return recByte;
}

/*
 * Slave i2c1 interrupt service routine
 * When interrupt occurs immediately store incoming data to rx_buff or send outgoing data from tx_buff
 */
void __attribute__((__interrupt__, auto_psv)) _SI2C1Interrupt(void) {

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
