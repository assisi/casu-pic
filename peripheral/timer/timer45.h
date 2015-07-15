/*
 * File:   timer45.h
 * Author: Karlo
 *
 * Created on 2015. o?ujak 24, 18:49
 */

#ifndef TIMER45_H
#define	TIMER45_H

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif

#ifdef	__cplusplus
extern "C" {
#endif

  /* T2CON: TIMER2 CONTROL REGISTER */
#define T4_ON               0x8000      /* Timer2 ON */
#define T4_OFF              0x0000      /* Timer2 OFF */
#define T4_OFF_ON_MASK      (~T4_ON)

#define T4_IDLE_STOP        0x2000 /* operate during sleep */
#define T4_IDLE_CON         0x0000 /* stop operation during sleep */
#define T4_IDLE_MASK        (~T4_IDLE_STOP)

#define T4_GATE_ON          0x0040 /* Timer Gate time accumulation enabled */
#define T4_GATE_OFF         0x0000 /* Timer Gate time accumulation disabled */
#define T4_GATE_MASK        (~T4_GATE_ON)

#define T4_PS_1_1           0x0000      /* Prescaler 1:1 */
#define T4_PS_1_8           0x0010      /*           1:8 */
#define T4_PS_1_64          0x0020      /*          1:64 */
#define T4_PS_1_256         0x0030      /*         1:256 */
#define T4_PS_MASK          (~T4_PS_1_256)

#define T4_32BIT_MODE_ON    0x0008      /* Timer 2 and Timer 3 form a 32 bit Timer */
#define T4_32BIT_MODE_OFF   0x0000
#define T4_32BIT_MODE_MASK   (~T4_32BIT_MODE_ON)

#define T4_SOURCE_EXT       0x0002 /* External clock source */
#define T4_SOURCE_INT       0x0000 /* Internal clock source */
#define T4_SOURCE_MASK      (~T4_SOURCE_EXT)


void OpenTimer45(unsigned int config, unsigned long int period);
int ReadIntTimer45(void);
void ConfigIntTimer45(unsigned int config);
void CloseTimer45(void);
unsigned long int ReadTimer45(void);
void WriteTimer45(unsigned long int timer);
void ClearIntTimer45(void);
void OpenTimer4(unsigned int config, unsigned long int period);



#endif	/* TIMER45_H */

