#include "timerFunctions.h"
#include "timer45.h"

/*
 * Function suspends program using timer1
 * int msec - desired CPU "sleep" time in milliseconds
 */
void delay_t1(int msec) {
    int count = msec / 400;
    int i;
    for(i = 0; i <= count; i++) {
        OpenTimer1(T1_ON | T1_PS_1_64, ticks_from_ms(msec, 64));
        ConfigIntTimer1(T1_INT_OFF | T1_INT_PRIOR_1);
        while (1) {
            int dummy = IFS0bits.T1IF;
            if (dummy == 1)
                break;
        }

        CloseTimer1();
    }
}
/*
 * Function suspends program using timer1
 * inputs:  usec - delay time in useconds
 * returns: -1 - error
 * Tpr = Tcyc*PRE*PR
 * PR = Tpr/(Tcyc*PRE) = Tpr/(0.025*Pre)
 */
int delay_t1_us(unsigned int usec) {
    unsigned int ticks = 5 * usec; //ticks = Fcyc/Pre*usec, PRE = 8
    if (usec > DELAY_T1_US_MAX)
        return -1;

    ConfigIntTimer1(T1_INT_OFF);    //Disable timer interrupt
    IFS0bits.T1IF = 0; //Clear interrupt flag
    OpenTimer1(T1_ON | T1_PS_1_8, ticks); //Configure timer

    while (!IFS0bits.T1IF); //Wait for usec

    CloseTimer1();
    return 1;
}

/*
 * Function calculates number of CPU ticks corresponding to time in milliseconds
 * This function is used together with the timer functions for setting values in timer registers
 * int msec - time in milliseconds
 * int prescaler - prescaler used for corresponding timer
 */
unsigned long ticks_from_ms(float msec, int prescaler) {
    unsigned long ticks;
    ticks = FOSC / 2 / prescaler * msec / 1000.0;
    return ticks;
}

unsigned long ticks_from_us(float usec, int prescaler) {
    unsigned long ticks;
    ticks = FOSC / 2.0 / prescaler * usec / 1000000.0;
    return ticks;
}

/*
 * Function calculates number of CPU ticks corresponding to time in seconds
 * This function is used together with the timer functions for setting values in timer registers
 * int sec - time in seconds
 * int prescaler - prescaler used for corresponding timer
 */
unsigned long ticks_from_s(float sec, int prescaler) {
    unsigned long ticks;
    ticks = FOSC / 2 / prescaler * sec;
    return ticks;
}