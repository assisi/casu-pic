#include "dma0.h"

/* Function initializes DMA0 for communication with peripheral
 * inputs: ctrl_register  - 16bit control register for DMA setup
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


UINT16 dma0BufferA[512] __attribute__((space(dma)));
//unsigned int dma0BufferB[8] __attribute__((space(dma)));

UINT8 dma0Init(void) {

  DMA0CONbits.CHEN = 0; // enable
  DMA0CONbits.SIZE = 0; // word
  DMA0CONbits.DIR = 0; // peripheral to RAM
  DMA0CONbits.HALF = 0; // interrupt when all data is moved
  DMA0CONbits.NULLW = 0; // no null write
  DMA0CONbits.AMODE = 0; //  Register Indirect with Post-Increment mode
  DMA0CONbits.MODE = 0b01; // one shot, Ping-pong disabled

  DMA0STAL = __builtin_dmaoffset(&dma0BufferA[0]);
  DMA0STAH = 0x0000;
  //DMA0STBL = __builtin_dmaoffset(&dma0BufferB[0]);
  
  DMA0REQbits.IRQSEL = 0b00100001; //SPI2
  DMA0PAD = (volatile unsigned int ) &SPI2BUF;
  DMA0CNT = 511; // transmit DMA0CNT+1 bytes

  IFS0bits.DMA0IF = 0;          // clear interrupt flag
  IEC0bits.DMA0IE = 1;          // Enable DMA interrupt
  DMA0CONbits.CHEN = 1;         // enable channel

  return 1;

}

void dma0Start() {
    DMA0CONbits.CHEN = 1;         // enable channel
}
 