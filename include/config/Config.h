#pragma once

#include <stdint.h>
#include <Arduino.h>

namespace config
{
    // buzzer pin
    constexpr uint8_t   BUZZER_PIN =  9;

    // max number of step the the melody can hold
    constexpr uint8_t MAX_BUFFER_MELODY_STEP_SIZE = 64;

    /// @brief for debugging 
    namespace debug
    {
        // Logger buffer max. size for Debugging
        constexpr size_t MAX_BUFFER_SIZE = 64;
    }
  
}

