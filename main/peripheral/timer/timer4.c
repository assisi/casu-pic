#include "timer4.h"

/********************************************************************
*    Function Name:  OpenTimer4                                     *
*    Description:    This routine configures the timer control      *
*                    register and timer period register.            *
*    Parameters:     config: bit definitions to configure Timer4    *
*                    period: value to be loaded to PR reg           *
*    Return Value:   None                                           *
********************************************************************/
void OpenTimer4(unsigned int config,unsigned int period)
{
    TMR4  = 0;          /* Reset Timer4 to 0x0000 */
    PR4   = period;     /* assigning Period to Timer period register */
    T4CON = config;     /* configure control reg */
    T4CONbits.T32 = 0;
}

/*******************************************************************
*    Function Name: ConfigIntTimer4                                *
*    Description:   This function configures interrupt and sets    *
*                   Interrupt Priority                             *
*    Parameters:    unsigned int config                            *
*    Return Value:  None                                           *
*******************************************************************/

void ConfigIntTimer4(unsigned int config)
{
    IFS1bits.T4IF = 0;                   /* clear IF bit */
    IPC6bits.T4IP = (config &0x0007);    /* assigning Interrupt Priority */
    IEC1bits.T4IE = (config &0x0008)>>3; /* Interrupt Enable /Disable */
}

/********************************************************************
*    Function Name:  CloseTimer4                                    *
*    Description:    This routine disables the Timer4 and its       *
*                    interrupt and flag bits.                       *
*    Parameters:     None                                           *
*    Return Value:   None                                           *
********************************************************************/

void CloseTimer4(void)
{
    IEC1bits.T4IE = 0;      /* Disable the Timer4 interrupt */
    T4CONbits.TON = 0;      /* Disable timer4 */
    IFS1bits.T4IF = 0;      /* Clear Timer interrupt flag */
}

/********************************************************************
*    Function Name:  ReadTimer4                                     *
*    Description:    This routine reads the 16-bit value from       *
*                    Timer4 Register.                               *
*    Parameters:     None                                           *
*    Return Value:   unsigned int: Timer  16-bit value              *
********************************************************************/

unsigned int ReadTimer4(void)
{
    return (TMR4);      /* Return the Timer4 register */
}

/********************************************************************
*    Function Name:  WriteTimer4                                    *
*    Description:    This routine writes a 16-bit value to Timer4   *
*    Parameters:     unsigned int: value to write to Timer          *
*    Return Value:   None                                           *
********************************************************************/

void WriteTimer4(unsigned int timer)
{
    TMR4 = timer;     /* assign timer value to Timer4 Register */
}

/********************************************************************
*    Function Name:  ClearIntTimer                                  *
*    Description:    This routine clears Timer1 interrupt           *
*    Parameters:     none                                           *
*    Return Value:   None                                           *
********************************************************************/
void ClearIntTimer4(void)
{
    IFS1bits.T4IF = 0;
}
