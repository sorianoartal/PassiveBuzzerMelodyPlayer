#pragma once

#include <Arduino.h>
#include <stdio.h>
#include <stdarg.h>
#include "config/Config.h"

// Macros for Debugging 
#define LOGI(fmt, ...) LOG('I', fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) LOG('W', fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) LOG('E', fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) LOG('D', fmt, ##__VA_ARGS__)


/**
 * @brief 
 * 
 * 
 * @param label 
 * @return const char* 
 */
inline const char* logPrefix(char label)
{
    switch (label) {
        case 'I': return "[I]";
        case 'W': return "[W]";
        case 'E': return "[E]";
        case 'D': return "[D]";
        default:  return "[?]";
    }
}

/**
 * @brief 
 * 
 * @param label 
 * @param fmt 
 * @param ... 
 */
inline void LOG(char label, const char* fmt, ...)
{
    char buffer[config::debug::MAX_BUFFER_SIZE];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    Serial.print(logPrefix(label));
    Serial.println(buffer);
}