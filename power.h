#ifndef POWER_H
#define POWER_H

// Atmega328P Clock Prescaler
// The clock prescaler divides the system clock frequency by a certain factor to reduce power consumption
/*
| CLKPS3 | CLKPS2 | CLKPS1 | CLKPS0 | Clock Division Factor |
|--------|--------|--------|--------|------------------------|
|   0    |   0    |   0    |   0    | 1                      |
|   0    |   0    |   0    |   1    | 2                      |
|   0    |   0    |   1    |   0    | 4                      |
|   0    |   0    |   1    |   1    | 8                      |
|   0    |   1    |   0    |   0    | 16                     |
|   0    |   1    |   0    |   1    | 32                     |
|   0    |   1    |   1    |   0    | 64                     |
|   0    |   1    |   1    |   1    | 128                    |
|   1    |   0    |   0    |   0    | 256                    |
|   1    |   0    |   0    |   1    | Reserved               |
|   1    |   0    |   1    |   0    | Reserved               |
|   1    |   0    |   1    |   1    | Reserved               |
|   1    |   1    |   0    |   0    | Reserved               |
|   1    |   1    |   0    |   1    | Reserved               |
|   1    |   1    |   1    |   0    | Reserved               |
|   1    |   1    |   1    |   1    | Reserved               |
*/

#define main_crystal_freq _16MHz // Main crystal frequency

enum class SluClockSpeed : uint8_t {
    _16MHz = 0, // 16MHz / 1 = 16MHz
    _8MHz = 1,  // 16MHz / 2 = 8MHz
    _4MHz = 2,  // 16MHz / 4 = 4MHz
    _2MHz = 3,  // 16MHz / 8 = 2MHz
    _1MHz = 4,  // 16MHz / 16 = 1MHz
    _500kHz = 5, // 16MHz / 32 = 500kHz
    _250kHz = 6, // 16MHz / 64 = 250kHz
    _125kHz = 7, // 16MHz / 128 = 125kHz
    _62kHz = 8,  // 16MHz / 256 = 62.5kHz
};

inline uint16_t GetClockDevisionFactor() {
    // Get the clock division factor based on the current CLKPR register value
    return (1 << (CLKPR & 0x0F)); // Return the clock division factor
}

/**
 * @brief Set the clock speed of the microcontroller by changing the prescaler bits in the CLKPR register.
 * Note: Changing the clock speed will affect the timing of all functions that rely on the system clock. Eg. delay(), Serial, etc.
 * @param speed Closck speed to set.
 * @return uint16_t The new clock division factor. Maximum value is 256.
 */
uint16_t SluSetClockSpeed(SluClockSpeed speed) {
    // Set the clock speed by changing the prescaler bits in the CLKPR register
    uint8_t _speed = static_cast<uint8_t>(speed) & 0x0F; // Mask to 4 bits
    CLKPR = (1 << CLKPCE); // Enable change
    CLKPR = _speed; // Set the clock speed

    return GetClockDevisionFactor(); // Return the new clock division factor
}

void SluDelay(long ms){
    // Check if ms with devision is less than 1a
    uint16_t _cdf = GetClockDevisionFactor(); // Get the current clock division factor
    if ( (ms / _cdf) < 1) {
        // If the delay is less than 1ms, use a microsecond delay
        delayMicroseconds((1000 * ms) / _cdf); // Delay function with time scaling
        return;
    }
    delay(ms / _cdf); // Delay function with time scaling
}

#endif

