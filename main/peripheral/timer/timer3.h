/*! \file timer3.h
 * \brief Declarations of functions and variables used for working with timer3.
 */

#ifndef TIMER3_H
#define	TIMER3_H
#endif

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif


#include "Generic.h"

/*! Address of timer3 period value register.
*/
#define PR1_VALUE           0xFFFF

/*! Address of timer3 configuration register.
 */
#define T3CON_VALUE         0x0000

   /* T3CON: TIMER3 CONTROL REGISTER */

/*! Timer3 on mask.
 */
#define T3_ON               0x8000 /* Timer3 ON */
/*! Timer3 off mask.
 */
#define T3_OFF              0x0000 /* Timer3 OFF */
/*! Timer3 on/off mask.
 */
#define T3_OFF_ON_MASK      (~T3_ON)

/*! Timer3 mask for operation during sleep option.
 */
#define T3_IDLE_STOP        0x2000 /* operate during sleep */
/*! Timer3 mask for stopping operation during sleep option.
 */
#define T3_IDLE_CON         0x0000 /* stop operation during sleep */
/*! Timer3 mask for turning on/off operation during sleep option.
 */
#define T3_IDLE_MASK        (~T3_IDLE_STOP)

/*! Timer3 mask for enabling timer gate time accumulation.
 */
#define T3_GATE_ON          0x0040 /* Timer Gate time accumulation enabled */
/*! Timer3 mask for disabling timer gate time accumulation.
 */
#define T3_GATE_OFF         0x0000 /* Timer Gate time accumulation disabled */
/*! Timer3 mask for turning on/off timer gate time accumulation.
 */
#define T3_GATE_MASK        (~T3_GATE_ON)

/*! Timer3 prescaler value 1.
 */
#define T3_PS_1_1           0x0000 /* Prescaler 1:1 */
/*! Timer3 prescaler value 8.
 */
#define T3_PS_1_8           0x0010 /*           1:8 */
/*! Timer3 prescaler value 64.
 */
#define T3_PS_1_64          0x0020 /*          1:64 */
/*! Timer3 prescaler value 256.
 */
#define T3_PS_1_256         0x0030 /*         1:256 */

/*! Timer3 mask for turning on synchronization with external clock input.
 */
#define T3_SYNC_EXT_ON      0x0004
/*! Timer3 mask for turning off synchronization with external clock input.
 */
#define T3_SYNC_EXT_OFF     0x0000
/*! Timer3 mask for turning on/off synchronization with external clock input.
 */
#define T3_SYNC_EXT_MASK    (~T3_SYNC_EXT_ON)

/*! Timer3 mask for turning on external clock source.
 */
#define T3_SOURCE_EXT       0x0002 /* External clock source */
/*! Timer3 mask for turning on internal clock source.
 */
#define T3_SOURCE_INT       0x0000 /* Internal clock source */
/*! Timer3 mask for turning on/off external clock source.
 */
#define T3_SOURCE_MASK      (~T3_SOURCE_EXT)

/* defines for Timer Interrupts */
/*! Timer3 interrupt priority 0.
 */
#define T3_INT_PRIOR_0      0x0000 /* 000 = Interrupt is priority 0 */
/*! Timer3 interrupt priority 1.
 */
#define T3_INT_PRIOR_1      0x0001 /* 001 = Interrupt is priority 1 */
/*! Timer3 interrupt priority 2.
 */
#define T3_INT_PRIOR_2      0x0002 /* 010 = Interrupt is priority 2 */
/*! Timer3 interrupt priority 3.
 */
#define T3_INT_PRIOR_3      0x0003 /* 011 = Interrupt is priority 3 */
/*! Timer3 interrupt priority 4.
 */
#define T3_INT_PRIOR_4      0x0004 /* 100 = Interrupt is priority 4 */
/*! Timer3 interrupt priority 5.
 */
#define T3_INT_PRIOR_5      0x0005 /* 101 = Interrupt is priority 5 */
/*! Timer3 interrupt priority 6.
 */
#define T3_INT_PRIOR_6      0x0006 /* 110 = Interrupt is priority 6 */
/*! Timer3 interrupt priority 7.
 */
#define T3_INT_PRIOR_7      0x0007 /* 111 = Interrupt is priority 7 */

/*! Mask for turning on timer3 interrupt.
 */
#define T3_INT_ON           0x0008 /* Interrupt Enable */
/*! Mask for turning off timer3 interrupt.
 */
#define T3_INT_OFF          0x0000 /* Interrupt Disable */

/*! Macro for turning on timer3 interrupt.
 */
#define EnableIntT3                    asm("BSET IEC0,#3")
/*! Macro for turning off timer3 interrupt.
 */
#define DisableIntT3                   asm("BCLR IEC0,#3")
/*! Macro for setting timer3 interrupt priority.
 */
#define SetPriorityIntT3(priority)     (IPC0bits.T3IP = priority)

/* Timer3 Function Prototypes */

/*! \brief Function configures and turns on timer3.
 *
 * @param config Value of timer configuration register.
 * @param period Value of timer period value.
*/
void OpenTimer3( unsigned int config, unsigned int period);

/*! \brief Function turns off timer3.
 */
void CloseTimer3(void);

/*! \brief Function reads current timer3 value.
 *
 *@return Returns current timer3 value.
 */
unsigned int ReadTimer3(void);

/*! \brief Function writes value to timer3 counter register.
 *
 * @param timer Timer value to be written in timer counter register.
 */
void WriteTimer3( unsigned int timer);

/*! \brief Function configures timer3 interrupt settings.
 *
 * @param config Timer3 interrupt settings value. Use predefined masks for turning timer interrupt on/off and setting interrupt priority.
 */
void ConfigIntTimer3(unsigned int config);

/*! \brief Function clears timer3 interrupt flag.
 */
void ClearIntTimer3(void);
