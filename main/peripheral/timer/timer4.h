/*! \file timer4.h
 * \brief Declarations of functions and variables used for working with timer4.
 */

#ifndef TIMER4_H
#define	TIMER4_H
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

/*! Address of timer4 period value register.
*/
#define PR4_VALUE           0xFFFF

/*! Address of timer4 configuration register.
 */
#define T4CON_VALUE         0x0000

 /* T4CON: TIMER4 CONTROL REGISTER */

/*! Timer4 on mask.
 */
#define T4_ON               0x8000     
/*! Timer4 off mask.
 */
#define T4_OFF              0x0000
/*! Timer4 on/off mask.
 */
#define T4_OFF_ON_MASK      (~T4_ON)

/*! Timer4 mask for operation during sleep option.
 */
#define T4_IDLE_STOP        0x2000 
/*! Timer4 mask for stopping operation during sleep option.
 */
#define T4_IDLE_CON         0x0000
/*! Timer4 mask for putting on/off operation during sleep option.
 */
#define T4_IDLE_MASK        (~T4_IDLE_STOP)

/*! Timer4 mask for enabling timer gate time accumulation.
 */
#define T4_GATE_ON          0x0040 
/*! Timer4 mask for disabling timer gate time accumulation.
 */
#define T4_GATE_OFF         0x0000
/*! Timer4 mask for turning on/off timer gate time accumulation.
 */
#define T4_GATE_MASK        (~T4_GATE_ON)

/*! Timer4 prescaler value 1.
 */
#define T4_PS_1_1           0x0000
/*! Timer4 prescaler value 8.
 */
#define T4_PS_1_8           0x0010
/*! Timer4 prescaler value 64.
 */
#define T4_PS_1_64          0x0020
/*! Timer4 prescaler value 256.
 */
#define T4_PS_1_256         0x0030     

#define T4_32BIT_MODE_ON    0x0008      /* Timer 4 and Timer 3 form a 32 bit Timer */
#define T4_32BIT_MODE_OFF   0x0000
#define T4_32BIT_MODE_MASK   (~T4_32BIT_MODE_ON)

/*! Timer4 mask for turning on synchronization with external clock input.
 */
#define T4_SOURCE_EXT       0x0002
/*! Timer4 mask for turning on synchronization with internal clock input.
 */
#define T4_SOURCE_INT       0x0000
/*! Timer4 mask for turning on/off external clock source.
 */
#define T4_SOURCE_MASK      (~T4_SOURCE_EXT)

/* defines for Timer Interrupts */
/*! Timer4 interrupt priority 0.
 */
#define T4_INT_PRIOR_0      0x0000
/*! Timer4 interrupt priority 1.
 */
#define T4_INT_PRIOR_1      0x0001      
/*! Timer4 interrupt priority 4.
 */
#define T4_INT_PRIOR_2      0x0002      
/*! Timer4 interrupt priority 3.
 */
#define T4_INT_PRIOR_3      0x0003     
/*! Timer4 interrupt priority 4.
 */
#define T4_INT_PRIOR_4      0x0004     
/*! Timer4 interrupt priority 5.
 */
#define T4_INT_PRIOR_5      0x0005    
/*! Timer4 interrupt priority 6.
 */
#define T4_INT_PRIOR_6      0x0006      
/*! Timer4 interrupt priority 7.
 */
#define T4_INT_PRIOR_7      0x0007     

/*! Mask for turning on timer4 interrupt.
 */
#define T4_INT_ON           0x0008
/*! Mask for turning off timer4 interrupt.
 */
#define T4_INT_OFF          0x0000      /* Interrupt Disable */

/* Macros to  Enable/Disable interrupts and set Interrupt priority of Timers 4*/

/*! Macro for turning on timer4 interrupt.
 */
#define EnableIntT4                    asm("BSET IEC0,#6")
/*! Macro for turning off timer4 interrupt.
 */
#define DisableIntT4                   asm("BCLR IEC0,#6")
/*! Macro for setting timer4 interrupt priority.
 */
#define SetPriorityIntT4(priority)     (IPC1bits.T4IP = priority)

/* Timer4 Function Prototypes */

/*! \brief Function configures and turns on timer4.
 *
 * @param config Value of timer configuration register.
 * @param period Value of timer period value.
*/
void OpenTimer4(unsigned int config, unsigned int period) __attribute__ ((section(".libperi")));

/*! \brief Function turns off timer4.
 */
void CloseTimer4(void) __attribute__ ((section (".libperi")));

/*! \brief Function reads current timer4 value.
 *
 *@return Returns current timer4 value.
 */
unsigned int ReadTimer4(void) __attribute__ ((section (".libperi")));

/*! \brief Function writes value to timer4 counter register.
 *
 * @param timer Time value to be written in timer counter register.
 */
void WriteTimer4( unsigned int timer) __attribute__ ((section (".libperi")));

/*! \brief Function configures timer4 interrupt settings.
 *
 * @param config Timer4 interrupt settings value. Use predefined masks for turning timer interrupt on/off and setting interrupt priority.
 */
void ConfigIntTimer4(unsigned int ) __attribute__ ((section (".libperi")));

/*! \brief Function clears timer4 interrupt flag.
 */
void ClearIntTimer4() __attribute__ ((section (".libperi")));
