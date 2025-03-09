#ifndef SLU_INPUT
#define SLU_INPUT


enum ButtonState {
  None = 0,
  Pressed = 1,
  Released = 3,
  Down = 4
};


class Button
{
	uint8_t pin;
	bool isdown = false;
	ButtonState GetState()
	{
		bool val = !digitalRead(pin);
		if(isdown ^ val)
		{
			isdown = val;
			delay(50); //  button transient noise canceling delay.
			if(isdown)
				return ButtonState::Pressed;
			else
				return ButtonState::Released;
		}
		if(isdown)
			return ButtonState::Down;
		return ButtonState::None;
	};
	Button(int pin_)
	{
		pin = pin_;
		pinMode(pin, INPUT_PULLUP); // active low button
	};
	Button() { pin = -1; };
};











#endif
