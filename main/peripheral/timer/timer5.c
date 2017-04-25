#include "timer5.h"

/********************************************************************
*    Function Name:  OpenTimer5                                     *
*    Description:    This routine configures the timer control      *
*                    register and timer period register.            *
*    Parameters:     config: bit definitions to configure Timer5    *
*                    period: value to be loaded to PR reg           *
*    Return Value:   None                                           *
********************************************************************/
void OpenTimer5(unsigned int config,unsigned int period)
{
    TMR5  = 0;          /* Reset Timer5 to 0x0000 */
    PR5   = period;     /* assigning Period to Timer period register */
    T5CON = config;     /* configure control reg */
}

/*******************************************************************
*    Function Name: ConfigIntTimer5                                *
*    Description:   This function configures interrupt and sets    *
*                   Interrupt Priority                             *
*    Parameters:    unsigned int config                            *
*    Return Value:  None                                           *
*******************************************************************/

void ConfigIntTimer5(unsigned int config)
{
    IFS1bits.T5IF = 0;                   /* clear IF bit */
    IPC7bits.T5IP = (config &0x0007);    /* assigning Interrupt Priority */
    IEC1bits.T5IE = (config &0x0008)>>3; /* Interrupt Enable /Disable */
}

/********************************************************************
*    Function Name:  CloseTimer5                                    *
*    Description:    This routine disables the Timer5 and its       *
*                    interrupt and flag bits.                       *
*    Parameters:     None                                           *
*    Return Value:   None                                           *
********************************************************************/

void CloseTimer5(void)
{
    IEC1bits.T5IE = 0;      /* Disable the Timer5 interrupt */
    T5CONbits.TON = 0;      /* Disable timer5 */
    IFS1bits.T5IF = 0;      /* Clear Timer interrupt flag */
}

/********************************************************************
*    Function Name:  ReadTimer5                                     *
*    Description:    This routine reads the 16-bit value from       *
*                    Timer5 Register.                               *
*    Parameters:     None                                           *
*    Return Value:   unsigned int: Timer  16-bit value              *
********************************************************************/

unsigned int ReadTimer5(void)
{
    return (TMR5);      /* Return the Timer5 register */
}

/********************************************************************
*    Function Name:  WriteTimer5                                    *
*    Description:    This routine writes a 16-bit value to Timer5   *
*    Parameters:     unsigned int: value to write to Timer          *
*    Return Value:   None                                           *
********************************************************************/

void WriteTimer5(unsigned int timer)
{
    TMR5 = timer;     /* assign timer value to Timer5 Register */
}

/********************************************************************
*    Function Name:  ClearIntTimer                                  *
*    Description:    This routine clears Timer1 interrupt           *
*    Parameters:     none                                           *
*    Return Value:   None                                           *
********************************************************************/
void ClearIntTimer5(void)
{
    IFS1bits.T5IF = 0;
}
