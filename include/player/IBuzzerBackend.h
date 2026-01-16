#pragma once

#include <stdint.h>
#include "logger/Logger.h"

/**
 * @brief Interface for Buzzer Backend implementations
 * 
 * This interface defines the necessary methods for a buzzer backend that can play tones.
 * Implementations of this interface should provide the actual hardware-specific logic to control the buzzer.
 * 
 * @note this is the hardware abstraction layer for the buzzer functionality.
 *  So we provide an abstraction for how the squared wave
 *  - Using hardware PWM
 *  - Using software PWM
 *  - Using Arduino tone()/noTone() functions...
 */
class IBuzzerBackend
{
    public:
    
    /// @brief Virtual destructor to proper clean up
    virtual ~IBuzzerBackend() = default;

    /// @brief Start playing a tone at the specified frequency.
    virtual void start(uint16_t frequencyHz) = 0;
    
    /// @brief Stop playing the tone. Silence buffer
    virtual void stop() = 0;

};