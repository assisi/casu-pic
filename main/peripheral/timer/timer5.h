/*! \file timer5.h
 * \brief Declarations of functions and variables used for working with timer5.
 */

#ifndef TIMER5_H
#define	TIMER5_H
#endif

#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#elif defined(__dsPIC33E__)
#include <p33Exxxx.h>
#endif


/* List of SFRs for Timer 4 */
/* This list contains the SFRs with default (POR) values to be used for configuring Timer 4 */
/* The user can modify this based on the requirement */

/*! Address of timer5 period value register.
*/
#define PR5_VALUE           0xFFFF

/*! Address of timer5 configuration register.
 */
#define T5CON_VALUE         0x0000

 /* T5CON: TIMER5 CONTROL REGISTER */

/*! Timer5 on mask.
 */
#define T5_ON               0x8000
/*! Timer5 off mask.
 */
#define T5_OFF              0x0000
/*! Timer5 on/off mask.
 */
#define T5_OFF_ON_MASK      (~T5_ON)

/*! Timer5 mask for operation during sleep option.
 */
#define T5_IDLE_STOP        0x2000
/*! Timer5 mask for stopping operation during sleep option.
 */
#define T5_IDLE_CON         0x0000
/*! Timer5 mask for putting on/off operation during sleep option.
 */
#define T5_IDLE_MASK        (~T5_IDLE_STOP)

/*! Timer5 mask for enabling timer gate time accumulation.
 */
#define T5_GATE_ON          0x0040
/*! Timer5 mask for disabling timer gate time accumulation.
 */
#define T5_GATE_OFF         0x0000
/*! Timer5 mask for turning on/off timer gate time accumulation.
 */
#define T5_GATE_MASK        (~T5_GATE_ON)

/*! Timer5 prescaler value 1.
 */
#define T5_PS_1_1           0x0000
/*! Timer5 prescaler value 8.
 */
#define T5_PS_1_8           0x0010
/*! Timer5 prescaler value 64.
 */
#define T5_PS_1_64          0x0020
/*! Timer5 prescaler value 256.
 */
#define T5_PS_1_256         0x0030

#define T5_32BIT_MODE_ON    0x0008      /* Timer 4 and Timer 3 form a 32 bit Timer */
#define T5_32BIT_MODE_OFF   0x0000
#define T5_32BIT_MODE_MASK   (~T5_32BIT_MODE_ON)

/*! Timer5 mask for turning on synchronization with external clock input.
 */
#define T5_SOURCE_EXT       0x0002
/*! Timer5 mask for turning on synchronization with internal clock input.
 */
#define T5_SOURCE_INT       0x0000
/*! Timer5 mask for turning on/off external clock source.
 */
#define T5_SOURCE_MASK      (~T5_SOURCE_EXT)

/* defines for Timer Interrupts */
/*! Timer5 interrupt priority 0.
 */
#define T5_INT_PRIOR_0      0x0000
/*! Timer5 interrupt priority 1.
 */
#define T5_INT_PRIOR_1      0x0001
/*! Timer5 interrupt priority 4.
 */
#define T5_INT_PRIOR_2      0x0002
/*! Timer5 interrupt priority 3.
 */
#define T5_INT_PRIOR_3      0x0003
/*! Timer5 interrupt priority 4.
 */
#define T5_INT_PRIOR_4      0x0004
/*! Timer5 interrupt priority 5.
 */
#define T5_INT_PRIOR_5      0x0005
/*! Timer5 interrupt priority 6.
 */
#define T5_INT_PRIOR_6      0x0006
/*! Timer5 interrupt priority 7.
 */
#define T5_INT_PRIOR_7      0x0007

/*! Mask for turning on timer5 interrupt.
 */
#define T5_INT_ON           0x0008
/*! Mask for turning off timer5 interrupt.
 */
#define T5_INT_OFF          0x0000      /* Interrupt Disable */

/* Macros to  Enable/Disable interrupts and set Interrupt priority of Timers 4*/

/*! Macro for turning on timer5 interrupt.
 */
#define EnableIntT5                    asm("BSET IEC0,#3")
/*! Macro for turning off timer5 interrupt.
 */
#define DisableIntT5                   asm("BCLR IEC0,#3")
/*! Macro for setting timer5 interrupt priority.
 */
#define SetPriorityIntT5(priority)     (IPC1bits.T5IP = priority)

/* Timer5 Function Prototypes */

/*! \brief Function configures and turns on timer5.
 *
 * @param config Value of timer configuration register.
 * @param period Value of timer period value.
*/
void OpenTimer5(unsigned int config, unsigned int period) __attribute__ ((section(".libperi")));

/*! \brief Function turns off timer5.
 */
void CloseTimer5(void) __attribute__ ((section (".libperi")));

/*! \brief Function reads current timer5 value.
 *
 *@return Returns current timer5 value.
 */
unsigned int ReadTimer5(void) __attribute__ ((section (".libperi")));

/*! \brief Function writes value to timer5 counter register.
 *
 * @param timer Time value to be written in timer counter register.
 */
void WriteTimer5( unsigned int timer) __attribute__ ((section (".libperi")));

/*! \brief Function configures timer5 interrupt settings.
 *
 * @param config Timer5 interrupt settings value. Use predefined masks for turning timer interrupt on/off and setting interrupt priority.
 */
void ConfigIntTimer5(unsigned int ) __attribute__ ((section (".libperi")));

/*! \brief Function clears timer5 interrupt flag.
 */
void ClearIntTimer5() __attribute__ ((section (".libperi")));
