#ifndef SLU_SAFETY
#define SLU_SAFETY

#include <avr/wdt.h> // Include Watchdog Timer library

void Abort()
{
    while (1)
    {
        delay(100); // Infinite loop to wait for wtd reset
    }
};

typedef enum : byte
{
    WDT_P_16MS = 0b00000000,   // WDP3..0 = 0000
    WDT_P_32MS = 0b00000001,   //          0001
    WDT_P_64MS = 0b00000010,   //          0010
    WDT_P_125MS = 0b00000011,  //          0011
    WDT_P_250MS = 0b00000100,  //          0100
    WDT_P_500MS = 0b00000101,  //          0101
    WDT_P_1000MS = 0b00000110, //          0110
    WDT_P_2000MS = 0b00000111, //          0111
    WDT_P_4000MS = 0b00100000, // WDP3     = 1 << 5, WDP2..0 = 000
    WDT_P_8000MS = 0b00100001  // WDP3     = 1 << 5, WDP2..0 = 001
} wdt_period_t;

inline void EnableWdt(wdt_period_t period)
{
    MCUSR &= ~_BV(WDRF);
    WDTCSR = _BV(WDCE) | _BV(WDE);
    WDTCSR = _BV(WDIE) | period;
};

inline void EnableResetWdt(wdt_period_t period)
{
    period |= _BV(WDE); // Enable the watchdog reset
    EnableWdt(period);  // Enable the Watchdog Timer with the specified timeout period
    wdt_reset();        // Reset the watchdog timer to prevent a reset
};

inline void EnableInterruptWdt(wdt_period_t period)
{
    cli();               // Disable global interrupts
    period &= ~_BV(WDE); // Disable the watchdog reset
    EnableWdt(period);   // Enable the Watchdog Timer with the specified timeout period
    wdt_reset();         // Reset the watchdog timer to prevent a reset
    sei();               // Enable global interrupts
};
inline void DisableWdt()
{
    cli();         // Disable global interrupts
    wdt_disable(); // Disable the Watchdog Timer
    sei();         // Enable global interrupts
};
inline void ResetWdt()
{
    wdt_reset(); // Reset the watchdog timer to prevent a reset
};

#endif
