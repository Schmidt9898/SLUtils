#define __USE_SERIAL__

#include "SLUtils.h"

SluLed led_red(13);

ISR(WDT_vect) {
    // We just use it to wake up from sleep
}


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


void setup() {

    InitSerial();
    println("Start lowpower test");
    Serial.flush();

    SluSetPreEnterPowerDown(mypowerDown); // Set the pre-sleep function
    SluSetPostLeavePowerDown(mypowerUp); // Set the post-wake function

    EnableInterruptWdt(WDT_P_1000MS); // Enable the Watchdog Timer with a 1 second timeout period
    //EnableResetWdt(WDT_P_1000MS);
}

void loop() {
    SluPowerDown(); // Enter sleep mode
    ResetWdt(); // Reset the watchdog timer to prevent a reset
    // Code execution continues here after wake-up
    println("Woked up from sleep");
    Serial.flush();
    led_red.turn(true);
    delay(500);
    led_red.turn(false);
}