/* 
 * File:   timer2.h
 * Author: thaus
 *
 * Created on 2013. travanj 14, 16:54
 */

#ifndef TIMER2_H
#define	TIMER2_H
#endif

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif


/* List of SFRs for Timer 2 */
/* This list contains the SFRs with default (POR) values to be used for configuring Timer 2 */
/* The user can modify this based on the requirement */
#define TMR2_VALUE          0x0000
#define PR2_VALUE           0xFFFF
#define T2CON_VALUE         0x0000

//#ifdef USE_AND_OR /* Format for AND_OR based bit setting */

   /* T2CON: TIMER2 CONTROL REGISTER */
#define T2_ON               0x8000      /* Timer2 ON */
#define T2_OFF              0x0000      /* Timer2 OFF */
#define T2_OFF_ON_MASK      (~T2_ON)

#define T2_IDLE_STOP        0x2000 /* operate during sleep */
#define T2_IDLE_CON         0x0000 /* stop operation during sleep */
#define T2_IDLE_MASK        (~T2_IDLE_STOP)

#define T2_GATE_ON          0x0040 /* Timer Gate time accumulation enabled */
#define T2_GATE_OFF         0x0000 /* Timer Gate time accumulation disabled */
#define T2_GATE_MASK        (~T2_GATE_ON)

#define T2_PS_1_1           0x0000      /* Prescaler 1:1 */
#define T2_PS_1_8           0x0010      /*           1:8 */
#define T2_PS_1_64          0x0020      /*          1:64 */
#define T2_PS_1_256         0x0030      /*         1:256 */
#define T2_PS_MASK          (~T2_PS_1_256)

#define T2_32BIT_MODE_ON    0x0008      /* Timer 2 and Timer 3 form a 32 bit Timer */
#define T2_32BIT_MODE_OFF   0x0000
#define T2_32BIT_MODE_MASK   (~T2_32BIT_MODE_ON)

#define T2_SOURCE_EXT       0x0002 /* External clock source */
#define T2_SOURCE_INT       0x0000 /* Internal clock source */
#define T2_SOURCE_MASK      (~T2_SOURCE_EXT)

/* defines for Timer Interrupts */

#define T2_INT_PRIOR_0      0x0000      /* 000 = Interrupt is priority 0 */
#define T2_INT_PRIOR_1      0x0001      /* 001 = Interrupt is priority 1 */
#define T2_INT_PRIOR_2      0x0002      /* 010 = Interrupt is priority 2 */
#define T2_INT_PRIOR_3      0x0003      /* 011 = Interrupt is priority 3 */
#define T2_INT_PRIOR_4      0x0004      /* 100 = Interrupt is priority 4 */
#define T2_INT_PRIOR_5      0x0005      /* 101 = Interrupt is priority 5 */
#define T2_INT_PRIOR_6      0x0006      /* 110 = Interrupt is priority 6 */
#define T2_INT_PRIOR_7      0x0007      /* 111 = Interrupt is priority 7 */

#define T2_INT_ON           0x0008      /* Interrupt Enable */
#define T2_INT_OFF          0x0000      /* Interrupt Disable */


//#else /* Format for backward compatibility (AND based bit setting). */
//
///* Timer2 Control Register (T2CON) Bit Defines */
//
//#define T2_ON               0xffff      /* Timer2 ON */
//#define T2_OFF              0x7fff      /* Timer2 OFF */
//
//#define T2_IDLE_CON         0xdfff      /* operate during sleep */
//#define T2_IDLE_STOP        0xffff      /* stop operation during sleep */
//
//#define T2_GATE_ON          0xffff      /* Timer2 Gate time accumulation enabled  */
//#define T2_GATE_OFF         0xffbf      /* Timer2 Gate time accumulation disabled */
//
//#define T2_PS_1_1           0xffcf      /* Prescaler 1:1   */
//#define T2_PS_1_8           0xffdf      /*           1:8   */
//#define T2_PS_1_64          0xffef      /*           1:64  */
//#define T2_PS_1_256         0xffff      /*           1:256 */
//
//#define T2_32BIT_MODE_ON    0xffff      /* Timer 2 and Timer 3 form a 32 bit Timer */
//#define T2_32BIT_MODE_OFF   0xfff7
//
//#define T2_SOURCE_EXT       0xffff      /* External clock source */
//#define T2_SOURCE_INT       0xfffd      /* Internal clock source */
//
///* defines for Timer Interrupts */
//#define T2_INT_PRIOR_7      0xffff      /* 111 = Interrupt is priority 7 */
//#define T2_INT_PRIOR_6      0xfffe      /* 110 = Interrupt is priority 6 */
//#define T2_INT_PRIOR_5      0xfffd      /* 101 = Interrupt is priority 5 */
//#define T2_INT_PRIOR_4      0xfffc      /* 100 = Interrupt is priority 4 */
//#define T2_INT_PRIOR_3      0xfffb      /* 011 = Interrupt is priority 3 */
//#define T2_INT_PRIOR_2      0xfffa      /* 010 = Interrupt is priority 2 */
//#define T2_INT_PRIOR_1      0xfff9      /* 001 = Interrupt is priority 1 */
//#define T2_INT_PRIOR_0      0xfff8      /* 000 = Interrupt is priority 0 */
//
//#define T2_INT_ON           0xffff      /* Interrupt Enable */
//#define T2_INT_OFF          0xfff7      /* Interrupt Disable */
//
//#endif /* USE_AND_OR */

/* Macros to  Enable/Disable interrupts and set Interrupt priority of Timers 2*/
#define EnableIntT2                    asm("BSET IEC0,#6")
#define DisableIntT2                   asm("BCLR IEC0,#6")
#define SetPriorityIntT2(priority)     (IPC1bits.T2IP = priority)

/* Timer2 Function Prototypes */

/* OpenTimer2 */
void OpenTimer2(unsigned int config, unsigned int period) __attribute__ ((section
(".libperi")));

/* CloseTimer2 */
void CloseTimer2(void) __attribute__ ((section (".libperi")));

/* ReadTimer2 */
unsigned int ReadTimer2(void) __attribute__ ((section (".libperi")));

/* WriteTimer2 */
void WriteTimer2( unsigned int timer) __attribute__ ((section (".libperi")));

/* ConfigIntTimer2 */
void ConfigIntTimer2(unsigned int ) __attribute__ ((section (".libperi")));

/* ClerIntTimer2 */
void ClearIntTimer2() __attribute__ ((section (".libperi")));

