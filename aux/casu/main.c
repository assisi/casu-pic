



#include "p33FJ06GS101.h"
#include "../casu/initializeHardware.h"
#include "../actuators/pwm.h"

/* Configuration Bit Settings */
_FOSCSEL(FNOSC_FRC)
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON)
_FWDT(FWDTEN_OFF)
_FPOR(FPWRT_PWR128 )
_FICD(ICS_PGD1 & JTAGEN_OFF)

void init_PWM(void);

int main()
{
/* Configure Oscillator to operate the device at 40Mhz
	   Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
 	   Fosc= 7.37*(43)/(2*2)=80Mhz for Fosc, Fcy = 40Mhz */

	/* Configure PLL prescaler, PLL postscaler, PLL divisor */
	PLLFBD=41; 				/* M = PLLFBD + 2 */
	CLKDIVbits.PLLPOST=0;   /* N1 = 2 */
	CLKDIVbits.PLLPRE=0;    /* N2 = 2 */

    __builtin_write_OSCCONH(0x01);			/* New Oscillator FRC w/ PLL */
    __builtin_write_OSCCONL(0x01);  		/* Enable Switch */
      
	while(OSCCONbits.COSC != 0b001);		/* Wait for new Oscillator to become FRC w/ PLL */  
    while(OSCCONbits.LOCK != 1);			/* Wait for Pll to Lock */

	/* Now setup the ADC and PWM clock for 120MHz
	   ((FRC * 16) / APSTSCLR ) = (7.37 * 16) / 1 = ~ 120MHz*/

	ACLKCONbits.FRCSEL = 1;					/* FRC provides input for Auxiliary PLL (x16) */
	ACLKCONbits.SELACLK = 1;				/* Auxiliary Oscillator provides clock source for PWM & ADC */
	ACLKCONbits.APSTSCLR = 7;				/* Divide Auxiliary clock by 1 */
	ACLKCONbits.ENAPLL = 1;					/* Enable Auxiliary PLL */
	
	while(ACLKCONbits.APLLCK != 1);			/* Wait for Auxiliary PLL to Lock */

    init_PWM();
    
    while(1);                     			/* Infinite Loop */
}

void init_PWM()
{

    PTCONbits.PTEN = 0;     //PWM module disable
    unsigned long period = ((unsigned long) FOSC/((unsigned long)FPWM*PWMPRE));
    //Primary Master Time Base
    PTCON2bits.PCLKDIV = 0b000;     //PWM prescaler = 1
    PTPER = (int) period;

    //PWM1 Generator Initialization
    DTR1 = 50;
    ALTDTR1 = 50;
    PWMCON1 = 0;    //Edge-aligned mode, master time base, individual duty for both channels
    PHASE1 = PTPER;
    SPHASE1 = PTPER;
    PDC1 = PTPER/4;
    //SDC1 = PTPER/2;//PWM1H duty
    //PWM I/O control register
    IOCON1bits.PMOD = 0b00; //Complementary PWM mod
    PWMCON1bits.TRGSTAT = 0;
    PWMCON1bits.FLTSTAT = 0;
    PWMCON1bits.CLSTAT = 0;
    IOCON1bits.PENL = 1;
    IOCON1bits.PENH = 1;
    PTCONbits.SEIEN = 0; // disable special event primary master time base interrupt
    IFS5bits.PWM1IF = 0;
    IPC23bits.PWM1IP = 3; // priority 3
    TRIG1 = period;
    IEC5bits.PWM1IE = 0;
    PWMCON1bits.TRGIEN = 0; // a trigger event generates an interrupt request
    
    //Time base control register
    PTCONbits.PTSIDL = 1;   //PWM time base halts in CPU Idle mode
    PTCONbits.PTEN = 1;     //PWM module enable
    


}