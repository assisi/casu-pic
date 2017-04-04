#include "dma1.h"

/* Function initializes DMA1 for communication with peripheral
 * ctrl_register - 16bit control register for DMA setup
 *                        bit 15 (CHEN) enable/disable
 *                        bit 14 (SIZE) byte/word
 *                        bit 13 (DIR)  DMA write/DMA read
 *                        bit 12 (HALF) interrupt when half/all data is moved
 *                        bit 11 (NULLW) null write/normal operation
 *                        bit 10-6 unimplemented (0)
 *                        bit 5-4 (AMODE) reserved/peripheral indirect/register indirect without increment/with increment
 *                        bit 3-2 unimplemented (0)
 *                        bit 1-0 (MODE) bit 1 ping-pong/no; bit 0 one-shot/continuous
 *         req_register -
 *         peripheral_address e.g. &SPI2BUF
 * return: 1 - initialization succeeded
 *         0 - initialization failed
 */
UINT16 dma1BufferA[2] __attribute__((space(dma)));

/*UINT8 dma0Init(UINT16 ctrl_register, UINT16 req_register, volatile unsigned int *peripheral_address) {

    LedUser(0,100,0);
    DMA0CON = ctrl_register; // set register while disabled

    DMA0STAH = __builtin_dmaoffset(&dma0BufferA);
    DMA0STBH = __builtin_dmaoffset(&dma0BufferB);
    dma0BufferA = ADXL_COMMAND(REG_DATAX0, ADXL_READ);
    dma0BufferB = ADXL_COMMAND(REG_DATAX1, ADXL_READ);

    //DMA0STAH = __builtin_dmaoffset(&BufferA);
    DMA0REQ = req_register;
    DMA0PAD = (volatile unsigned int *) peripheral_address;
    DMA0CNT = 0; // transmit DMA0CNT + 1 blocks

    IFS0bits.DMA0IF = 0;
    IEC0bits.DMA0IE = 1;          // Enable DMA interrupt
    DMA0CONbits.CHEN = 1; //enable

  return 1;
}
*/

UINT8 dma1Init(void) {

    //LedUser(0, 100, 0);

    DMA1CONbits.CHEN = 0; // disable
    DMA1CONbits.SIZE = 0;  // word
    DMA1CONbits.DIR = 1; // RAM to peripheral
    DMA1CONbits.HALF = 0; // interrupt at full transfer
    DMA1CONbits.NULLW = 0; // null data write operation
    DMA1CONbits.AMODE = 0; // Register Indirect with Post-Increment mode
    DMA1CONbits.MODE = 0b01; // one shot, ping-pong disabled

    DMA1STAL = __builtin_dmaoffset(&dma1BufferA[0]);
    DMA1STAH = 0x0000;

    // not using ping pong
    //DMA1STBL = __builtin_dmaoffset(&dma1BufferB);
    //DMA1STBH = 0x0000;

    dma1BufferA[0] = ADXL_COMMAND(REG_DATAZ0, ADXL_READ);
    dma1BufferA[1] = ADXL_COMMAND(REG_DATAZ1, ADXL_READ);;

    DMA1REQbits.IRQSEL =  0b00100001; // SPI2
    DMA1PAD = (volatile unsigned int) &SPI2BUF;
    DMA1CNT = 1; // transmit DMA1CNT + 1 bytes
    IFS0bits.DMA1IF = 0;          // clear interrupt
    IEC0bits.DMA1IE = 0;          // Disable DMA interrupt
    DMA1CONbits.CHEN = 1;         // enable

  return 1;
}

void dma1Start(void)
{
    //DMA1CONbits.CHEN = 0;         // disable
    DMA1CONbits.CHEN = 1;         // enable
    DMA1REQbits.FORCE = 1;        // force transfer
}

void dma1Stop(void)
{
    DMA1CONbits.CHEN = 0;
}
