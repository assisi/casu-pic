
#include "uart2.h"
#include "math.h"

struct buffer2 uart2RxBuffer;
struct buffer2 uart2TxBuffer;

void uart2Init(unsigned long baudRate, unsigned long Fosc) {

    // select channels for uart2 tx/rx operations, not avalailable for dsPIC33Fxxxx
    // RPINR19bits.U2RXR = 10;     // U2rx = RP10/RF4
    // RPOR8bits.RP17R = 5;       // U2tx = RP17/RF5 (5 == function number for U2tx)
     // Set directions of UART IOs
    TRISFbits.TRISF4 = 1;
    TRISFbits.TRISF5 = 0;

    U2MODEbits.UARTEN = 1;      // Enable module
    U2MODEbits.USIDL = 0;       // continue module operation in Idle
    U2MODEbits.IREN = 0;        // IrDA disables
    U2MODEbits.RTSMD =  0;      // U2RTS in Flow control mode
    U2MODEbits.UEN = 0x0;       // U2RTS i U2CTS are not used
    U2MODEbits.WAKE = 0;        // No wake-up enabled
    U2MODEbits.LPBACK = 0;      // 0 - No loopback, 1 - Loopback mode
    U2MODEbits.ABAUD = 0;       // Baud rate measurement disabled
    U2MODEbits.URXINV = 0;       // U2RX Idle state is 1
    U2MODEbits.BRGH = 1;        // Normal speed mode (Fcy / 16)
    U2MODEbits.PDSEL = 0;       // 8-bit data, no parity
    U2MODEbits.STSEL = 0;       // One stop bit

    U2STAbits.UTXISEL0 = 0;     // Interrupt when transmit buffer empty
    U2STAbits.UTXISEL1 = 0;

    U2STAbits.UTXINV = 0;       // U2TX Idle 1
    U2STAbits.UTXBRK = 0;       // Sync Break transmisssion disabled
    U2STAbits.UTXEN = 1;        // Transmit enabled, U2TX pin controlled by uart2
    U2STAbits.URXISEL = 0;      // Interrupt is set on RSR transfer, making the rec buffer 3/4 ful
    U2STAbits.ADDEN = 0;        // Address Detect mode disabled
    
    unsigned long dummy, baudCalc1, baudCalc2;
    if (U2MODEbits.BRGH) {
        dummy = Fosc / 8 / baudRate - 1;      // Set baud rate
        baudCalc1 = (dummy + 1) * 8 / Fosc;
        baudCalc2 = (dummy + 2) * 8 / Fosc;
        if (fabs( baudCalc2 - baudRate) < fabs( baudCalc1 - baudRate))
            dummy++;
    }
    else {
        dummy = Fosc / 32 / baudRate;      // Set baud rate
        baudCalc1 = (dummy + 1) * 32 / Fosc;
        baudCalc2 = (dummy + 2) * 32 / Fosc;
        if (fabs( baudCalc2 - baudRate) < fabs( baudCalc1 - baudRate))
            dummy++;
    }
    U2BRG = dummy;   
    uart2RxBuffer.head = 0;
    uart2RxBuffer.tail = 0;
    uart2TxBuffer.head = 0;
    uart2TxBuffer.tail = 0;
    // Enable interrupts
    IEC1bits.U2TXIE = 1;        // Enable interrupt
    IFS1bits.U2TXIF = 0;        // Clear interrupt flag
    IEC1bits.U2RXIE = 1;        // Enable rx interrupt
    IFS1bits.U2RXIF = 0;        // Clear Interrupt Flag
}

int uart2BytesAvailable() {
    IEC1bits.U2RXIE = 0;        // disable rx interrupt
    int res = uart2RxBuffer.head - uart2RxBuffer.tail;
    IEC1bits.U2RXIE = 1;        // Enable rx interrupt
    if (res < 0){
        res = res + BUFFER_SIZE + 1;
    }
    return res;
}

char uart2ReadByte() {
    IEC1bits.U2RXIE = 0;        // Disable U2RX interrupt
    char res = uart2RxBuffer.buff[uart2RxBuffer.tail];
    uart2RxBuffer.tail += 1;
    if (uart2RxBuffer.tail >= BUFFER_SIZE) {
        uart2RxBuffer.tail -= BUFFER_SIZE;
    }
    IEC1bits.U2RXIE = 1;        // Enable U2RX interrupt
    return res;
}

/*
 * Buffer is full when there is still one emtpy slot remaining
 */
int uart2TxBuffFull() {
    IEC1bits.U2TXIE = 0;    // Disable tx interrupt
    int aux1 = uart2TxBuffer.head + 1;
    int aux2 = uart2TxBuffer.tail;
    IEC1bits.U2TXIE = 1;    // Enable tx interrupt
    if (aux1 >= BUFFER_SIZE) aux1 -= BUFFER_SIZE;
    if (aux1 == aux2)
        return 1;
    else
        return 0;
}

int uart2TxBuffEmpty() {
    IEC1bits.U2TXIE = 0;        // Disable Tx interrupt
    int res = uart2TxBuffer.head == uart2TxBuffer.tail;
    IEC1bits.U2TXIE = 1;        // Enable Tx interrupt
    return (res);
}

void uart2WriteByte(char ch) {

    while (uart2TxBuffFull()) {
        if (IEC1bits.U2TXIE == 0) {
            IEC1bits.U2TXIE = 1;   
        }
        if ((U2STAbits.UTXBF == 0) & (IFS1bits.U2TXIF == 0))
            IFS1bits.U2TXIF = 1;
    }
    
    IEC1bits.U2TXIE = 0;        // Disable Tx interrupt
    uart2TxBuffer.buff[uart2TxBuffer.head++] = ch;
    if (uart2TxBuffer.head >= BUFFER_SIZE) uart2TxBuffer.head -= BUFFER_SIZE;
    IEC1bits.U2TXIE = 1;        // Enable Tx interrupt
    if ((U2STAbits.UTXBF == 0) && (IFS1bits.U2TXIF == 0)) {
         IFS1bits.U2TXIF = 1;
    }

}

void uart2WriteBuffer(char *data, int length){

    int i;
    for (i = 0; i < length; i++) {
        uart2WriteByte(data[i]);
    }
}

void uart2WriteString(char *data) {
    int i = 0;
    while (data[i] != '\0')
    {
        uart2WriteByte(data[i]);
        i++;

    }
}


void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt(void) {

    while (U2STAbits.URXDA) {
        uart2RxBuffer.buff[uart2RxBuffer.head++] = U2RXREG;
        if (uart2RxBuffer.head >= BUFFER_SIZE)  uart2RxBuffer.head -= BUFFER_SIZE;
        if (uart2RxBuffer.head == uart2RxBuffer.tail) {
            uart2RxBuffer.tail += 1;
            if (uart2RxBuffer.tail >= BUFFER_SIZE)  uart2RxBuffer.tail -= BUFFER_SIZE;
        }
    }

    IFS1bits.U2RXIF = 0;
}

void __attribute__((__interrupt__, auto_psv)) _U2TXInterrupt(void) {

    if (uart2TxBuffEmpty()) {
        IEC1bits.U2TXIE = 0;    // disable Tx interrupts if tx buffer is empty
    }
    else
    {
        U2TXREG = uart2TxBuffer.buff[uart2TxBuffer.tail++];
        if (uart2TxBuffer.tail >= BUFFER_SIZE) uart2TxBuffer.tail -= BUFFER_SIZE;
    }
   
    IFS1bits.U2TXIF = 0;                            // clear interrupt flag
}
