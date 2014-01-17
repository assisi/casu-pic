/* 
 * File:   uart2.h
 * Author: thaus
 *
 * Created on 2013. travanj 16, 10:32
 */

#ifndef UART2_H
#define	UART2_H
#endif	/* UART2_H */

#define BUFFER_SIZE 256

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#endif

struct buffer2 {
    int buff[BUFFER_SIZE];
    int head;
    int tail;
};

extern struct buffer2 uart2RxBuffer;
extern struct buffer2 uart2TxBuffer;

void uart2Init(unsigned long, unsigned long);

int uart2BytesAvailable();

char uart2ReadByte();

int uart2TxBuffFull();

int uart2TxBuffEmpty();

void uart2WriteByte(char ch);

void uart2WriteBuffer(char *data, int length);

void uart2WriteString(char *data);

// void __attribute__((__interrupt__)) _U2RXInterrupt(void);
// void __attribute__((__interrupt__)) _U2TXInterrupt(void);
