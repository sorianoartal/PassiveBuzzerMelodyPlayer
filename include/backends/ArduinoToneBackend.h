#pragma once

#include "player/IBuzzerBackend.h"
#include "logger/Logger.h"

#ifdef ARDUINO
    #include <Arduino.h>
#endif

/**
 * @brief Backend square wave Square wave Generator
 * 
 */
class ArduinoToneBackend: public IBuzzerBackend
{
    private:

        uint8_t buzzerPin_;

    public:

    ArduinoToneBackend(uint8_t pin);

    void begin();

    // === Implemented method form IBuzzerBackend ===
    
    /// @brief 
    /// @param frequencyHz 
    void start(uint16_t frequencyHz) override;

    /// @brief 
    void stop() override;

};