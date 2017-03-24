
#include "interrupts.h"

// all interrupt routines should be implemented here

// Timer 3 interrupt service for reading accelerometer measurements at exactly 3.2 KHz
void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void)
{
    dma1Start();
    IFS0bits.T3IF = 0;
}

// Timer 4 interrupt service for temperature loop
void __attribute__((__interrupt__, __auto_psv__)) _T4Interrupt(void)
{
    timer4_flag = 1;
    IFS1bits.T4IF = 0;
}

// Timer 5 interrupt service for FFT on accelerometer reading
void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void)
{
    timer5_flag = 1;
    IFS1bits.T5IF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA1Interrupt(void)
{
    dma_spi_tx_count++;
    IFS0bits.DMA1IF = 0;
}


void __attribute__((__interrupt__, no_auto_psv)) _DMA0Interrupt(void)
{
    digitalPin accPin = aSlaveR;
    dma_spi_rx_count++;
    CloseTimer3();
    chipDeselect(accPin);
    dma_spi2_started = 0;
    dma_spi2_done = 1;
    IFS0bits.DMA0IF = 0;    // Clear the DMA0 Interrupt Flag
}
