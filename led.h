#ifndef BASE_LED
#define BASE_LED

#include <stdint.h>

//TODO: turn of all led

/**
 * Object controlling assigned GPIO as a led.
 */
class SluLed
{
	uint8_t pin;
	bool isOn = false;

public:
	void turn(bool b)
	{
		isOn = b;
		digitalWrite(pin, b);
	};
	void toggle()
	{
		isOn = !isOn;
		digitalWrite(pin, isOn);
	};
	SluLed(uint8_t pin_)
	{
		pin = pin_;
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	};
	SluLed() { pin = -1; };
};

#endif