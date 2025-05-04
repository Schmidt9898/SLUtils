#define __USE_SERIAL__

#include "SLUtils.h"

SluLed led_red(A0);

void mypowerDown() {
    // This function is called before entering power down mode
    // You can add any code here that needs to run before going to sleep
    println("Entering power down mode...");
    Serial.flush();
    led_red.turn(false); // Turn off the LED before going to sleep
};

void mypowerUp() {
    // This function is called after leaving power down mode
    // You can add any code here that needs to run after waking up
    println("Waking up from power down mode...");
};

void myInteruptr(){

}

void setup()
{
    InitSerial();
    println("Start timer example");
    SluSetClockSpeed(SluClockSpeed::_16MHz); // Set clock speed to 16MHz
    Serial.flush();

    SluSetPreEnterPowerDown(mypowerDown); // Set the pre-sleep function
    SluSetPostLeavePowerDown(mypowerUp); // Set the post-wake function



    // LOW to trigger the interrupt whenever the pin is low,
    // CHANGE to trigger the interrupt whenever the pin changes value
    // RISING to trigger when the pin goes from low to high,
    // FALLING for when the pin goes from high to low.
    attachInterrupt(digitalPinToInterrupt(2), myInteruptr, CHANGE);



}

void loop() {
    led_red.toggle();
    SluDelay(100);
    SluPowerDown();
}

