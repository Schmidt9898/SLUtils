#ifndef SLU_SAFETY
#define SLU_SAFETY

#include <avr/wdt.h>  // Include Watchdog Timer library

//TODO

void functions() {
    wdt_enable(WDTO_2S); // Enable the Watchdog Timer with a timeout of 2 seconds
    wdt_reset(); // Reset the watchdog timer to prevent a reset
};

#endif
