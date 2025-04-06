#define __USE_SERIAL__

#include "SLUtils.h"

SluLed led_green(A1);
SluLed led_red(A0);


//STORED_DATA(int, wdt_value);


#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

void setup()
{
    InitSerial();
    println("Start timer example");
    SluSetClockSpeed(SluClockSpeed::_125kHz); // Set clock speed to 16MHz
    Serial.flush();

}

void loop() {
    led_red.toggle();
    SluDelay(100);
}

