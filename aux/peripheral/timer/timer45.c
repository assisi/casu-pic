
#include "timer45.h"



/********************************************************************
*    Function Name:  OpenTimer45                                    *
*    Description:    This routine configures the timer control      *
*                    register and 32bit timer period register       *
*    Parameters:     config: bit definitions to configure Timer45   *
*                    period                                         *
*    Return Value:   None                                           *
********************************************************************/
void OpenTimer45(unsigned int config, unsigned long int period)
{
    T5CONbits.TON = 0;
    T4CONbits.TON = 0;
    T4CONbits.T32 = 1;
    T4CONbits.TCS = 0;
    T4CONbits.TGATE = 0;
    T4CONbits.TCKPS = 0b11;
    TMR4 = 0x00;
    TMR5 = 0x00;
    PR4 = period;
    PR5 = (period >> 16);
    IPC7bits.T5IP = 0x01; // Set Timer3 Interrupt Priority Level
    IFS1bits.T5IF = 0; // Clear Timer3 Interrupt Flag
    IEC1bits.T5IE = 1; // Enable Timer3 interrupt
    T4CONbits.TON = 1; // Start 32-bit Timer
}

void OpenTimer4(unsigned int config, unsigned long int period)
{
    T4CONbits.TON = 0;
    T4CONbits.T32 = 0;
    T4CONbits.TCS = 0;
    T4CONbits.TGATE = 0;
    T4CONbits.TCKPS = 0b11;
    TMR4 = 0x00;
    PR4 = period;
    IPC6bits.T4IP = 0x01; // Set Timer4 Interrupt Priority Level
    IFS1bits.T4IF = 0; // Clear Timer4 Interrupt Flag
    IEC1bits.T4IE = 1; // Enable Timer4 interrupt
    T4CONbits.TON = 1; // Start Timer
}

//Read interrupt flag
int ReadIntTimer45(void){

    return (int)(IFS1bits.T5IF);
}

/*******************************************************************
*    Function Name: ConfigIntTimer45                              *
*    Description:   This function configures interrupt and sets    *
*                   Interrupt Priority                             *
*    Parameters:    unsigned int config                            *
*    Return Value:  None                                           *
*******************************************************************/

void ConfigIntTimer45(unsigned int config)
{
    IFS1bits.T5IF = 0;                   /* clear IF bit */
    IPC7bits.T5IP = 0x01;//(config &0x0007);    /* assigning Interrupt Priority */
    IEC1bits.T5IE = 1;//(config &0x0008)>>3; /* Interrupt Enable /Disable */
}

/********************************************************************
*    Function Name:  CloseTimer2                                    *
*    Description:    This routine disables the Timer2 and its       *
*                    interrupt and flag bits.                       *
*    Parameters:     None                                           *
*    Return Value:   None                                           *
********************************************************************/

void CloseTimer45(void)
{
    IEC1bits.T5IE = 0;      /* Disable the Timer2 interrupt */
    T4CONbits.TON = 0;      /* Disable timer2 */
    IFS1bits.T5IF = 0;      /* Clear Timer interrupt flag */
}

/********************************************************************
*    Function Name:  ReadTimer2                                     *
*    Description:    This routine reads the 16-bit value from       *
*                    Timer2 Register.                               *
*    Parameters:     None                                           *
*    Return Value:   unsigned int: Timer  16-bit value              *
********************************************************************/

unsigned long int ReadTimer45(void)
{

    return (1);      /* Return the Timer2 register */
}

/********************************************************************
*    Function Name:  WriteTimer45                                   *
*    Description:    This routine writes a 32-bit value to Timer45  *
*    Parameters:     unsigned long int: value to write to Timer     *
*    Return Value:   None                                           *
********************************************************************/
void WriteTimer45(unsigned long int timer)
{
    TMR5HLD = timer >> 16;
    TMR4 = timer;
}

/********************************************************************
*    Function Name:  ClearIntTimer                                  *
*    Description:    This routine clears Timer1 interrupt           *
*    Parameters:     none                                           *
*    Return Value:   None                                           *
********************************************************************/
void ClearIntTimer45(void)
{
    IFS1bits.T5IF = 0;
}