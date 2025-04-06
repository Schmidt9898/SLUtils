



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

void setup()
{
    InitSerial();
    println("Start test");
    Serial.flush();
    CLKPR = 0x80;
    CLKPR |= (1 << CLKPS0) | (1 << CLKPS1); // Set prescaler to 8 for 2MHz clock
    pinMode(2, INPUT_PULLUP);  // Make sure pin 2 is input

    #if 0
    // Configure INT0 to trigger on the **falling edge**
    EICRA |= (1 << ISC01);  // Trigger interrupt on falling edge (INT0)
    EICRA &= ~(1 << ISC00); // Falling edge
    
    // Enable external interrupt 0 (INT0)
    EIMSK |= (1 << INT0);   // Enable interrupt on INT0
    #endif
    
    cli();  // Disable global interrupts
    #if 1
    
    // Set up Timer2 as async
    /*
    TIMSK2 = 0;                          // Disable Timer2 interrupts temporarily
    ASSR = (1 << AS2) | (1 << EXCLK);                   // Enable asynchronous mode for Timer2
    //set initial counter value
    TCNT2 = 0;                          // Set initial counter value to 0
    TCCR2A = (1 << COM2B1) | (1 << COM2B0) | (1 << WGM21);           // CTC mode
    TCCR2B = (1 << WGM22);               // CTC mode
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);// Prescaler 128 for 32.768 kHz crystal
    OCR2B = 10;                         // Compare match value for ~1-second interval
    while (ASSR & ((1 << TCN2UB) | (1 << TCR2BUB) | (1 << TCR2AUB) | (1 << OCR2BUB))); // Wait for updates
    TIFR2  = (1 << OCF2B);              // Clear any pending interrupts
    TIMSK2 = (1 << OCIE2B);             // Enable Timer2 Compare Match B interrupt
    */
   
    TIMSK2 = 0;                          // Disable Timer2 interrupts temporarily

    //ASSR = (1 << EXCLK);                   // Enable external clock for Timer2
    ASSR = (1 << AS2);                   // Enable asynchronous mode for Timer2
    //set initial counter value
    TCNT2 = 0;                          // Set initial counter value to 0
    TCCR2A = (1 << COM2A1) | (1 << COM2A0) | (1 << WGM21);           // CTC mode
    TCCR2B = (1 << WGM22);               // CTC mode
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);// Prescaler 1024 for 32.768 kHz crystal
    OCR2A = 255;                         // Compare match value for ~1-second interval
    while (ASSR & ((1 << TCN2UB) | (1 << TCR2BUB) | (1 << TCR2AUB) | (1 << OCR2AUB))); // Wait for updates
    TIFR2  = (1 << OCF2A);              // Clear any pending interrupts
    TIMSK2 = (1 << OCIE2A);             // Enable Timer2 Compare Match B interrupt



    #else
    //Disable timer2 interrupts
    TIMSK2  = 0;
    //Enable asynchronous mode
    ASSR  = (1<<AS2);
    //set initial counter value
    TCNT2=0;
    //set prescaller 128
    TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20);
    //wait for registers update
    while (ASSR & ((1<<TCN2UB)|(1<<TCR2BUB)));
    //clear interrupt flags
    TIFR2  = (1<<TOV2);
    //enable TOV2 interrupt
    TIMSK2  = (1<<TOIE2);
    #endif
    
    sei(); // Enable global interrupts


}

unsigned int last_time = 0;

ISR(TIMER2_COMPA_vect)
{
    print("delay:");
    println(millis() - last_time);
    last_time = millis(); // Store the current time in milliseconds
    led_green.toggle(); // Toggle the red LED
}
ISR(TIMER2_COMPB_vect)
{
    //TIFR2  = (1 << OCF2B);              // Clear any pending interrupts
    //TIMSK2 = (1 << OCIE2B);             // Enable Timer2 Compare Match B interrupt
    println("int");
    led_red.toggle(); // Toggle the red LED
    asm volatile("nop"::);
}
//Overflow ISR
ISR(TIMER2_OVF_vect)
{
    //Toggle pin PD0 every second
    //println("ovc");
    led_red.toggle(); // Toggle the red LED
    //delay(1000); // Wait for 1 second
    asm volatile("nop"::);
    //_delay_us(10);
}
ISR(INT0_vect) {
    // This interrupt service routine is triggered by the external signal on INT0
    led_green.toggle(); // Toggle the green LED
}
void loop() {

     //Before going to sleep, configure the sleep mode
     set_sleep_mode(SLEEP_MODE_PWR_SAVE); // Set to Idle mode (CPU off, peripherals on)
     sleep_enable();  // Enable sleep mode
     // Enter sleep mode
     sleep_mode();  // Sleep until interrupt wakes up
     // After waking up, disable sleep mode
     sleep_disable();

    // Code execution continues here after wake-up
    //led_red.turn(false);
    //println("continues after wake-up");
    //Serial.flush();
    delay(3000);  // Do something after waking up
}




/*
void setupTimer1_1s() {
  TCCR1A = 0;
  TCCR1B = 0;

  TCNT1 = 0;

  // 16 MHz / 1024 = 15625 ticks per second
  OCR1A = 15624;

  // CTC mode
  TCCR1B |= (1 << WGM12);

  // Prescaler = 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);

  // Enable interrupt on compare match
  TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
  // This is called every 1 second
  PORTB ^= (1 << 5); // Toggle pin 13 (onboard LED)
}
*/