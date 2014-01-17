/* 
 * File:   timer1.h
 * Author: thaus
 *
 * Created on 2013. travanj 11, 10:46
 */

#ifndef TIMER1_H
#define	TIMER1_H
#endif

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include "Generic.h"

/* List of SFRs for Timer 1 */
/* This list contains the SFRs with default (POR) values to be used for configuring Timer 1 */
/* The user can modify this based on the requirement */
#define TMR1_VALUE          0x0000
#define PR1_VALUE           0xFFFF
#define T1CON_VALUE         0x0000

   /* T1CON: TIMER1 CONTROL REGISTER */
#define T1_ON               0x8000 /* Timer1 ON */
#define T1_OFF              0x0000 /* Timer1 OFF */
#define T1_OFF_ON_MASK      (~T1_ON)

#define T1_IDLE_STOP        0x2000 /* operate during sleep */
#define T1_IDLE_CON         0x0000 /* stop operation during sleep */
#define T1_IDLE_MASK        (~T1_IDLE_STOP)

#define T1_GATE_ON          0x0040 /* Timer Gate time accumulation enabled */
#define T1_GATE_OFF         0x0000 /* Timer Gate time accumulation disabled */
#define T1_GATE_MASK        (~T1_GATE_ON)

#define T1_PS_1_1           0x0000 /* Prescaler 1:1 */
#define T1_PS_1_8           0x0010 /*           1:8 */
#define T1_PS_1_64          0x0020 /*          1:64 */
#define T1_PS_1_256         0x0030 /*         1:256 */
#define T1_PS_MASK          (~T1_PS_1_256)

#define T1_SYNC_EXT_ON      0x0004 /* Synch external clk input */
#define T1_SYNC_EXT_OFF     0x0000 /* Do not synch external clk input */
#define T1_SYNC_EXT_MASK    (~T1_SYNC_EXT_ON)

#define T1_SOURCE_EXT       0x0002 /* External clock source */
#define T1_SOURCE_INT       0x0000 /* Internal clock source */
#define T1_SOURCE_MASK      (~T1_SOURCE_EXT)

/* defines for Timer Interrupts */

#define T1_INT_PRIOR_0      0x0000 /* 000 = Interrupt is priority 0 */
#define T1_INT_PRIOR_1      0x0001 /* 001 = Interrupt is priority 1 */
#define T1_INT_PRIOR_2      0x0002 /* 010 = Interrupt is priority 2 */
#define T1_INT_PRIOR_3      0x0003 /* 011 = Interrupt is priority 3 */
#define T1_INT_PRIOR_4      0x0004 /* 100 = Interrupt is priority 4 */
#define T1_INT_PRIOR_5      0x0005 /* 101 = Interrupt is priority 5 */
#define T1_INT_PRIOR_6      0x0006 /* 110 = Interrupt is priority 6 */
#define T1_INT_PRIOR_7      0x0007 /* 111 = Interrupt is priority 7 */

#define T1_INT_ON           0x0008 /* Interrupt Enable */
#define T1_INT_OFF          0x0000 /* Interrupt Disable */



/* Macros to  Enable/Disable interrupts and set Interrupt priority of Timers 1*/
#define EnableIntT1                    asm("BSET IEC0,#3")
#define DisableIntT1                   asm("BCLR IEC0,#3")
#define SetPriorityIntT1(priority)     (IPC0bits.T1IP = priority)

/* Timer1 Function Prototypes */

/* OpenTimer1 */
void OpenTimer1( unsigned int config, unsigned int period);

/* CloseTimer1 */
void CloseTimer1(void);

/* ReadTimer1 */
unsigned int ReadTimer1(void);

/* WriteTimer1 */
void WriteTimer1( unsigned int timer);

/* Config Int Timer1 */
void ConfigIntTimer1(unsigned int config);

/* Clear Int Timer 1 */
void ClearIntTimer1(void);
