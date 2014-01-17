
#if defined(__PIC24F__)
#include <p24Fxxxx.h>
#elif defined(__dsPIC33F__)
#include <p33Fxxxx.h>
#endif
#include "timer1.h"

/********************************************************************
*    Function Name:  OpenTimer1                                     *
*    Description:    This routine configures the timer control      *
*                    register and timer period register.            *
*    Parameters:     config: bit definitions to configure Timer1    *
*                    period: Value to be loaded to PR reg           *
*    Return Value:   None                                           *
********************************************************************/

void OpenTimer1(unsigned int config,unsigned int period)
{
    TMR1  = 0;          /* Reset Timer1 to 0x0000 */
    PR1   = period;     /* assigning Period to Timer period register */
    T1CON = config;     /* Configure timer control reg */
}

/*******************************************************************
*    Function Name: ConfigIntTimer1                               *
*    Description:   This function configures interrupt and sets    *
*                   Interrupt Priority                             *
*    Parameters:    unsigned int config                            *
*    Return Value:  None                                           *
*******************************************************************/

void ConfigIntTimer1(unsigned int config)
{
    IFS0bits.T1IF = 0;                   /* clear IF bit */
    IPC0bits.T1IP = (config &0x0007);    /* assigning Interrupt Priority */
    IEC0bits.T1IE = (config &0x0008)>>3; /* Interrupt Enable /Disable */
}


/********************************************************************
*    Function Name:  CloseTimer1                                    *
*    Description:    This routine disables the Timer1 and its       *
*                    interrupt and flag bits.                       *
*    Parameters:     None                                           *
*    Return Value:   None                                           *
********************************************************************/

void CloseTimer1(void)
{
    IEC0bits.T1IE = 0;      /* Disable the Timer1 interrupt */
    T1CONbits.TON = 0;      /* Disable timer1 */
    IFS0bits.T1IF = 0;      /* Clear Timer interrupt flag */
}

/********************************************************************
*    Function Name:  WriteTimer1                                    *
*    Description:    This routine writes a 16-bit value to Timer1   *
*    Parameters:     unsigned int: value to write to Timer          *
*    Return Value:   None                                           *
********************************************************************/

void WriteTimer1(unsigned int timer)
{
    TMR1 = timer;     /* assign timer value to Timer1 Register */
}

/********************************************************************
*    Function Name:  WriteTimer1                                    *
*    Description:    This routine writes a 16-bit value to Timer1   *
*    Parameters:     unsigned int: value to write to Timer          *
*    Return Value:   None                                           *
********************************************************************/

unsigned int ReadTimer1(void)
{
    return (TMR1);      /* Return the Timer1 register */
}

/********************************************************************
*    Function Name:  ClearIntTimer                                  *
*    Description:    This routine clears Timer1 interrupt           *
*    Parameters:     none                                           *
*    Return Value:   None                                           *
********************************************************************/

void ClearIntTimer1(void)
{
    IFS0bits.T1IF = 0;
}
