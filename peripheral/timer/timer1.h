/*! \file timer1.h
 * \brief Declarations of functions and variables used for working with timer1.
 */

#ifndef TIMER1_H
#define	TIMER1_H
#endif

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif


#include "Generic.h"

/* List of SFRs for Timer 1 */
/* This list contains the SFRs with default (POR) values to be used for configuring Timer 1 */
/* The user can modify this based on the requirement */

/*! Address of timer1 period value register.
*/
#define PR1_VALUE           0xFFFF

/*! Address of timer1 configuration register.
 */
#define T1CON_VALUE         0x0000

   /* T1CON: TIMER1 CONTROL REGISTER */

/*! Timer1 on mask.
 */
#define T1_ON               0x8000 /* Timer1 ON */
/*! Timer1 off mask.
 */
#define T1_OFF              0x0000 /* Timer1 OFF */
/*! Timer1 on/off mask.
 */
#define T1_OFF_ON_MASK      (~T1_ON)

/*! Timer1 mask for operation during sleep option.
 */
#define T1_IDLE_STOP        0x2000 /* operate during sleep */
/*! Timer1 mask for stopping operation during sleep option.
 */
#define T1_IDLE_CON         0x0000 /* stop operation during sleep */
/*! Timer1 mask for turning on/off operation during sleep option.
 */
#define T1_IDLE_MASK        (~T1_IDLE_STOP)

/*! Timer1 mask for enabling timer gate time accumulation.
 */
#define T1_GATE_ON          0x0040 /* Timer Gate time accumulation enabled */
/*! Timer1 mask for disabling timer gate time accumulation.
 */
#define T1_GATE_OFF         0x0000 /* Timer Gate time accumulation disabled */
/*! Timer1 mask for turning on/off timer gate time accumulation.
 */
#define T1_GATE_MASK        (~T1_GATE_ON)

/*! Timer1 prescaler value 1.
 */
#define T1_PS_1_1           0x0000 /* Prescaler 1:1 */
/*! Timer1 prescaler value 8.
 */
#define T1_PS_1_8           0x0010 /*           1:8 */
/*! Timer1 prescaler value 64.
 */
#define T1_PS_1_64          0x0020 /*          1:64 */
/*! Timer1 prescaler value 256.
 */
#define T1_PS_1_256         0x0030 /*         1:256 */

/*! Timer1 mask for turning on synchronization with external clock input.
 */
#define T1_SYNC_EXT_ON      0x0004
/*! Timer1 mask for turning off synchronization with external clock input.
 */
#define T1_SYNC_EXT_OFF     0x0000
/*! Timer1 mask for turning on/off synchronization with external clock input.
 */
#define T1_SYNC_EXT_MASK    (~T1_SYNC_EXT_ON)

/*! Timer1 mask for turning on external clock source.
 */
#define T1_SOURCE_EXT       0x0002 /* External clock source */
/*! Timer1 mask for turning on internal clock source.
 */
#define T1_SOURCE_INT       0x0000 /* Internal clock source */
/*! Timer1 mask for turning on/off external clock source.
 */
#define T1_SOURCE_MASK      (~T1_SOURCE_EXT)

/* defines for Timer Interrupts */
/*! Timer1 interrupt priority 0.
 */
#define T1_INT_PRIOR_0      0x0000 /* 000 = Interrupt is priority 0 */
/*! Timer1 interrupt priority 1.
 */
#define T1_INT_PRIOR_1      0x0001 /* 001 = Interrupt is priority 1 */
/*! Timer1 interrupt priority 2.
 */
#define T1_INT_PRIOR_2      0x0002 /* 010 = Interrupt is priority 2 */
/*! Timer1 interrupt priority 3.
 */
#define T1_INT_PRIOR_3      0x0003 /* 011 = Interrupt is priority 3 */
/*! Timer1 interrupt priority 4.
 */
#define T1_INT_PRIOR_4      0x0004 /* 100 = Interrupt is priority 4 */
/*! Timer1 interrupt priority 5.
 */
#define T1_INT_PRIOR_5      0x0005 /* 101 = Interrupt is priority 5 */
/*! Timer1 interrupt priority 6.
 */
#define T1_INT_PRIOR_6      0x0006 /* 110 = Interrupt is priority 6 */
/*! Timer1 interrupt priority 7.
 */
#define T1_INT_PRIOR_7      0x0007 /* 111 = Interrupt is priority 7 */

/*! Mask for turning on timer1 interrupt.
 */
#define T1_INT_ON           0x0008 /* Interrupt Enable */
/*! Mask for turning off timer1 interrupt.
 */
#define T1_INT_OFF          0x0000 /* Interrupt Disable */



/* Macros to  Enable/Disable interrupts and set Interrupt priority of Timers 1*/

/*! Macro for turning on timer1 interrupt.
 */
#define EnableIntT1                    asm("BSET IEC0,#3")
/*! Macro for turning off timer1 interrupt.
 */
#define DisableIntT1                   asm("BCLR IEC0,#3")
/*! Macro for setting timer1 interrupt priority.
 */
#define SetPriorityIntT1(priority)     (IPC0bits.T1IP = priority)

/* Timer1 Function Prototypes */

/*! \brief Function configures and turns on timer1.
 *
 * @param config Value of timer configuration register.
 * @param period Value of timer period value.
*/
void OpenTimer1( unsigned int config, unsigned int period);

/*! \brief Function turns off timer1.
 */
void CloseTimer1(void);

/*! \brief Function reads current timer1 value.
 *
 *@return Returns current timer1 value.
 */
unsigned int ReadTimer1(void);

/*! \brief Function writes value to timer1 counter register.
 *
 * @param timer Timer value to be written in timer counter register.
 */
void WriteTimer1( unsigned int timer);

/*! \brief Function configures timer1 interrupt settings.
 *
 * @param config Timer1 interrupt settings value. Use predefined masks for turning timer interrupt on/off and setting interrupt priority.
 */
void ConfigIntTimer1(unsigned int config);

/*! \brief Function clears timer1 interrupt flag.
 */
void ClearIntTimer1(void);
