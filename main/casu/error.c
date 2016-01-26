#include "error.h"
#include "../actuators/pwm.h"
#include "initializeHardware.h"

extern digitalPin LED2R;
extern digitalPin LED2G;
extern digitalPin LED2B;

int ErrorInitCheck(int status){
    if (status == 0) {
        /*
         * Send error message "Temp_F, SPI comm failed"
         */
        return 1;
        digitalHigh(LED2R);
    }
    else if (status == -1) {
        /*
         * Send error message "Temp_F, Device ID failed"
         */
        return 1;
        digitalHigh(LED2G);
    }
    else if (status == -2) {
        /*
         * Send error message "Temp_F, Device settings failed"
         */
        return 1;
        digitalHigh(LED2B);
    }
}