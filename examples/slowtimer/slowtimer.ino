#define __USE_SERIAL__

#include "SLUtils.h"

SluLed led_red(13);

void setup()
{
    InitSerial();
    println("Start timer example");
    Serial.flush();
    SluSetClockSpeed(SluClockSpeed::_500kHz); // Set clock speed to 500kHz
}

void loop() {
    led_red.toggle();
    SluDelay(500);
}

