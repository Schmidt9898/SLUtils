#ifndef SLU_INPUT
#define SLU_INPUT

#include <EEPROM.h>

// Define EEPROM size based on the detected board
#if defined(__AVR_ATmega328P__)  // Arduino Uno, Nano
    #define EEPROM_SIZE 1024
#elif defined(__AVR_ATmega2560__)  // Arduino Mega 2560
    #define EEPROM_SIZE 4096
#elif defined(__AVR_ATmega32U4__)  // Arduino Leonardo
    #define EEPROM_SIZE 1024
#elif defined(ESP32)  // ESP32 (uses flash memory, emulated EEPROM)
    #define EEPROM_SIZE 512  // Default size (can be changed in EEPROM.begin(size))
#else
    #define EEPROM_SIZE 0  // Unknown board, no EEPROM defined
#endif




EEPROM.write(0, 0);
EEPROM.update(0, 0);
EEPROM.put(0, 0);
EEPROM.get(0, 0);

#endif
