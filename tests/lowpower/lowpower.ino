



#define __USE_SERIAL__

#include "SLUtils.h"
SluLed led_green(A1);
SluLed led_red(A0);


void abort()
{
    led_red.turn(true);
    while (1)
    {
        delay(100);
    }
};

STORED_DATA(int, wdt_value);


#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

ISR(WDT_vect) {
    // This function is called when the watchdog timer expires
    wdt_reset();
    wdt_value.data = 1;
    wdt_value.save();
}

void setup() {

    InitSerial();
    println("Start test");
    print("wdt_value:");
    println(wdt_value.data);
    Serial.flush();
    wdt_value.data = 0;
    wdt_value.save();
    led_red.turn(true);
    cli();  // Disable global interrupts
    wdt_reset();  // Reset watchdog timer

    MCUSR;

// Setup Watchdog for interrupt and not reset, and a approximately 500ms timeout P.45
//WDTCR = (1<<WDIE) | (1<<WDP2) | (1<<WDP0);

    // Enable watchdog timer for ~8 seconds
    //wdt_enable(WDTO_4S);

    sei();  // Enable global interrupts

    // Set sleep mode to power down
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_mode();    // Enter sleep mode
}

void loop() {

    // Code execution continues here after wake-up
    led_red.turn(false);
    println("continues after wake-up");
    Serial.flush();

    delay(1000);  // Do something after waking up
}