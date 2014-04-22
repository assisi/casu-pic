#include "timerFunctions.h"

/*
 * Function suspends program using timer1
 * int msec - desired CPU "sleep" time in milliseconds
 */
void delay_t1(int msec) {
    int count = msec / 400;
    int i;
    for(i = 0; i <= count; i++) {
        OpenTimer1(T1_ON | T1_PS_1_256, ticks_from_ms(msec, 256));
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
 * Function calculates number of CPU ticks corresponding to time in milliseconds
 * This function is used together with the timer functions for setting values in timer registers
 * int msec - time in milliseconds
 * int prescaler - prescaler used for corresponding timer
 */
long ticks_from_ms(int msec, int prescaler) {
    unsigned long ticks;
    ticks = FOSC / 2 / prescaler * msec / 1000;
    return ticks;
}

long ms_from_ticks(long ticks, int prescaler) {
    unsigned long res;
    res = ticks * 1000 * 2 * prescaler / FOSC;
    return res;
}