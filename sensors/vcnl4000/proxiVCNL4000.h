/* 
 * File:   ProxiVCNL4000.h
 * Author: Karlo
 *
 * Created on 2013. srpanj 10, 21:49
 */

#ifndef PROXIVCNL4000_H
#define	PROXIVCNL4000_H

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif

#define VCNL4000_Address    0x13
#define VCNL4000_Address_WR    0x26
#define VCNL4000_Address_RD    0x27
//Function registers address
#define COMM0   0x80
#define PRODID  0x81
#define IRLED3  0x83
#define ALIGH4  0x84
#define ALRES5  0x85
#define ALRES6  0x86
#define PRRES7  0x87
#define PRRES8  0x88
#define PRMEA9  0x89
#define PRMOD10 0x8A

struct VCNL4000Data{
    unsigned int Distance;
    unsigned int Light;
};

int VCNL4000Light(void);

int VCNL4000ReadProxi();

int VCNL4000Proxi(void);

#endif