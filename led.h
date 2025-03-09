#ifndef BASE_LED
#define BASE_LED

#include <stdint.h>

/*
#ifndef NUMBER_OF_LEDS
#error Please define NUMBER_OF_LEDS 
// Example:
// #define NUMBER_OF_LEDS 2
#endif
*/

/**
 * Object controlling assigned GPIO as a led.
 */
class SLed {
	uint8_t pin;
	bool isOn = false;
	public:
	void turn(bool b){
		isOn=b;
		digitalWrite(pin,b);
	};
	SLed(uint8_t pin_){
		pin=pin_;
		pinMode(pin, OUTPUT);
		digitalWrite(pin,LOW);
	};
	SLed(){pin=-1;};
};

/*

uint8_t _led_counter = 0;
LED* _all_led[NUMBER_OF_LEDS];


#define SetUpLed(NAME,PIN) \
LED NAME = LED(PIN); \
_all_led[_led_counter++] = &NAME;


void InitAllLeds() {
	for (uint8_t li = 0; li < NUMBER_OF_LEDS; li++)
	{
		_all_led[li]->
	}
}
*/


#endif