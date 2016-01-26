/*! \file timer2.h
 * \brief Declarations of functions and variables used for working with timer2.
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

/*! Address of timer2 period value register.
*/
#define PR2_VALUE           0xFFFF

/*! Address of timer2 configuration register.
 */
#define T2CON_VALUE         0x0000

 /* T2CON: TIMER2 CONTROL REGISTER */

/*! Timer2 on mask.
 */
#define T2_ON               0x8000     
/*! Timer2 off mask.
 */
#define T2_OFF              0x0000
/*! Timer2 on/off mask.
 */
#define T2_OFF_ON_MASK      (~T2_ON)

/*! Timer2 mask for operation during sleep option.
 */
#define T2_IDLE_STOP        0x2000 
/*! Timer2 mask for stopping operation during sleep option.
 */
#define T2_IDLE_CON         0x0000
/*! Timer2 mask for putting on/off operation during sleep option.
 */
#define T2_IDLE_MASK        (~T2_IDLE_STOP)

/*! Timer2 mask for enabling timer gate time accumulation.
 */
#define T2_GATE_ON          0x0040 
/*! Timer2 mask for disabling timer gate time accumulation.
 */
#define T2_GATE_OFF         0x0000
/*! Timer2 mask for turning on/off timer gate time accumulation.
 */
#define T2_GATE_MASK        (~T2_GATE_ON)

/*! Timer2 prescaler value 1.
 */
#define T2_PS_1_1           0x0000
/*! Timer2 prescaler value 8.
 */
#define T2_PS_1_8           0x0010
/*! Timer2 prescaler value 64.
 */
#define T2_PS_1_64          0x0020
/*! Timer2 prescaler value 256.
 */
#define T2_PS_1_256         0x0030     

#define T2_32BIT_MODE_ON    0x0008      /* Timer 2 and Timer 3 form a 32 bit Timer */
#define T2_32BIT_MODE_OFF   0x0000
#define T2_32BIT_MODE_MASK   (~T2_32BIT_MODE_ON)

/*! Timer2 mask for turning on synchronization with external clock input.
 */
#define T2_SOURCE_EXT       0x0002
/*! Timer2 mask for turning on synchronization with internal clock input.
 */
#define T2_SOURCE_INT       0x0000
/*! Timer2 mask for turning on/off external clock source.
 */
#define T2_SOURCE_MASK      (~T2_SOURCE_EXT)

/* defines for Timer Interrupts */
/*! Timer2 interrupt priority 0.
 */
#define T2_INT_PRIOR_0      0x0000
/*! Timer2 interrupt priority 1.
 */
#define T2_INT_PRIOR_1      0x0001      
/*! Timer2 interrupt priority 2.
 */
#define T2_INT_PRIOR_2      0x0002      
/*! Timer2 interrupt priority 3.
 */
#define T2_INT_PRIOR_3      0x0003     
/*! Timer2 interrupt priority 4.
 */
#define T2_INT_PRIOR_4      0x0004     
/*! Timer2 interrupt priority 5.
 */
#define T2_INT_PRIOR_5      0x0005    
/*! Timer2 interrupt priority 6.
 */
#define T2_INT_PRIOR_6      0x0006      
/*! Timer2 interrupt priority 7.
 */
#define T2_INT_PRIOR_7      0x0007     

/*! Mask for turning on timer2 interrupt.
 */
#define T2_INT_ON           0x0008
/*! Mask for turning off timer2 interrupt.
 */
#define T2_INT_OFF          0x0000      /* Interrupt Disable */

/* Macros to  Enable/Disable interrupts and set Interrupt priority of Timers 2*/

/*! Macro for turning on timer2 interrupt.
 */
#define EnableIntT2                    asm("BSET IEC0,#6")
/*! Macro for turning off timer2 interrupt.
 */
#define DisableIntT2                   asm("BCLR IEC0,#6")
/*! Macro for setting timer2 interrupt priority.
 */
#define SetPriorityIntT2(priority)     (IPC1bits.T2IP = priority)

/* Timer2 Function Prototypes */

/*! \brief Function configures and turns on timer2.
 *
 * @param config Value of timer configuration register.
 * @param period Value of timer period value.
*/
void OpenTimer2(unsigned int config, unsigned int period) __attribute__ ((section
(".libperi")));

/*! \brief Function turns off timer2.
 */
void CloseTimer2(void) __attribute__ ((section (".libperi")));

/*! \brief Function reads current timer2 value.
 *
 *@return Returns current timer2 value.
 */
unsigned int ReadTimer2(void) __attribute__ ((section (".libperi")));

/*! \brief Function writes value to timer2 counter register.
 *
 * @param timer Time value to be written in timer counter register.
 */
void WriteTimer2( unsigned int timer) __attribute__ ((section (".libperi")));

/*! \brief Function configures timer2 interrupt settings.
 *
 * @param config Timer2 interrupt settings value. Use predefined masks for turning timer interrupt on/off and setting interrupt priority.
 */
void ConfigIntTimer2(unsigned int ) __attribute__ ((section (".libperi")));

/*! \brief Function clears timer2 interrupt flag.
 */
void ClearIntTimer2() __attribute__ ((section (".libperi")));
