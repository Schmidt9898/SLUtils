#ifndef POWER_H
#define POWER_H

#include <avr/sleep.h> // Include AVR sleep library
#include <avr/power.h>
// Changing the clock prescaler significantly affect the mcu power consumption

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

enum class SluClockSpeed : uint8_t
{
    _16MHz = 0,  // 16MHz / 1 = 16MHz
    _8MHz = 1,   // 16MHz / 2 = 8MHz
    _4MHz = 2,   // 16MHz / 4 = 4MHz
    _2MHz = 3,   // 16MHz / 8 = 2MHz
    _1MHz = 4,   // 16MHz / 16 = 1MHz
    _500kHz = 5, // 16MHz / 32 = 500kHz
    _250kHz = 6, // 16MHz / 64 = 250kHz
    _125kHz = 7, // 16MHz / 128 = 125kHz
    _62kHz = 8,  // 16MHz / 256 = 62.5kHz
};

inline uint16_t GetClockDevisionFactor()
{
    // Get the clock division factor based on the current CLKPR register value
    return (1 << (CLKPR & 0x0F)); // Return the clock division factor
}

/**
 * @brief Set the clock speed of the microcontroller by changing the prescaler bits in the CLKPR register.
 * Note: Changing the clock speed will affect the timing of all functions that rely on the system clock. Eg. delay(), Serial, etc.
 * @param speed Closck speed to set.
 * @return uint16_t The new clock division factor. Maximum value is 256.
 */
uint16_t SluSetClockSpeed(SluClockSpeed speed)
{
    // Set the clock speed by changing the prescaler bits in the CLKPR register
    uint8_t _speed = static_cast<uint8_t>(speed) & 0x0F; // Mask to 4 bits
    CLKPR = (1 << CLKPCE);                               // Enable change
    CLKPR = _speed;                                      // Set the clock speed

    return GetClockDevisionFactor(); // Return the new clock division factor
}

void SluDelay(long ms)
{
    // Check if ms with devision is less than 1a
    uint16_t _cdf = GetClockDevisionFactor(); // Get the current clock division factor
    if ((ms / _cdf) < 1)
    {
        // If the delay is less than 1ms, use a microsecond delay
        delayMicroseconds((1000 * ms) / _cdf); // Delay function with time scaling
        return;
    }
    delay(ms / _cdf); // Delay function with time scaling
}

// SLEEP_MODE_IDLE         0
// SLEEP_MODE_PWR_DOWN     1
// SLEEP_MODE_PWR_SAVE     2
// SLEEP_MODE_ADC          3
// SLEEP_MODE_STANDBY      4
// SLEEP_MODE_EXT_STANDBY  5

typedef void (*functionPtr)(void);

functionPtr preEnterPowerDown = nullptr;
functionPtr postLeavePowerDown = nullptr;

void SluSetPreEnterPowerDown(functionPtr func)
{
    preEnterPowerDown = func; // Set the pre-sleep function pointer
}
void SluSetPostLeavePowerDown(functionPtr func)
{
    postLeavePowerDown = func; // Set the post-wake function pointer
}

void _powerdown()
{
    // ADCSRA &= ~_BV(ADEN);        // ADC ki
    power_twi_disable(); // Disable I2C
    power_adc_disable(); // Disable Analog to Digital Converter (ADC)
    power_spi_disable(); // Disable Serial Peripheral Interface (SPI)
    // power_timer0_disable();      // System timer, responsible for the wdt interrupt, without this it won't work
    power_timer1_disable(); // Disable 16bit timer
    power_timer2_disable(); // disable async timer
    // power_usart_disable();       // Disable USART
}
void _powerup()
{
    power_twi_enable(); // Enable I2C
    power_adc_enable(); // Enable ADC
    power_spi_enable(); // Enable SPI
    // power_timer0_enable();       // Enable timer0
    power_timer1_enable(); // Enable 16bit timer
    power_timer2_enable(); // Enable async timer
    // power_usart_enable();        // Enable USART
}

/**
 * @brief Put the microcontroller into sleep mode to save power. Only external interrupts can wake it up.
 * Note: You can configure
 */
void SluPowerDown()
{
    // Set the sleep mode to power down
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode to power down

    if (preEnterPowerDown != nullptr)
    {
        preEnterPowerDown(); // Call the pre-sleep function if set
    }

    sleep_enable();  // Enable sleep mode
    sleep_cpu();     // Enter sleep mode
    sleep_disable(); // Disable sleep mode after waking up

    if (postLeavePowerDown != nullptr)
    {
        postLeavePowerDown(); // Call the post-wake function if set
    }
}

#endif
