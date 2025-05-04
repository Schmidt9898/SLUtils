



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

ISR(INT0_vect) {
    led_red.turn(true); 
}


void setup() {

    InitSerial();
    println("Start test");
    Serial.flush();

    led_red.turn(true);

    cli();  // Disable global interrupts
    EICRA |= (1 << ISC01);  // Trigger interrupt on falling edge (INT0)
    EICRA &= ~(1 << ISC00); // Falling edge
    // Enable external interrupt 0 (INT0)
    EIMSK |= (1 << INT0);   // Enable interrupt on INT0
    sei();  // Enable global interrupts
    SluSetClockSpeed(SluClockSpeed::_500kHz);
    // Set sleep mode to power down
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
}

void loop() {

    // Code execution continues here after wake-up
    println("continues after wake-up");
    //led_red.turn(true);
    Serial.flush();
    long i =  millis();
    volatile int val = 0;
    while(millis() < i + 3000) {
        val = val + 1;
    }
    led_red.turn(false);
    sleep_mode();    // Enter sleep mode

}