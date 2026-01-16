#include "backends/ArduinoToneBackend.h"

/**
 * @brief Construct a new Arduino Tone Backend:: Arduino Tone Backend object
 * 
 * @param pin - buzzer pin where the PWM wave would be generated
 */
ArduinoToneBackend::ArduinoToneBackend(uint8_t pin): buzzerPin_(pin){}

/**
 * @brief Config Arduino pin on which to generate the tone as OUTPUT 
 * 
 */
void ArduinoToneBackend::begin()
{
    pinMode(buzzerPin_,OUTPUT);
}

/**
 * @brief Generates a square wave of the specified frequency(and 50% duty cycle)
 * on the settting pin.
 * 
 * @see https://docs.arduino.cc/language-reference/en/functions/advanced-io/tone/
 * 
 */
void ArduinoToneBackend::start(uint16_t frequencyHz)
{       
    LOGI("tone pin=%u f=%u", buzzerPin_, frequencyHz);

    tone(buzzerPin_,frequencyHz);
}

/**
 * @brief Stops the generation of the square wave trigger by calling start() 
 * on the specify pin
 */
void ArduinoToneBackend::stop()
{
    LOGI("noTone pin=%u", buzzerPin_);
    noTone(buzzerPin_);
}