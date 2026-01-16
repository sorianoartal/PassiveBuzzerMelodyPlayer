#pragma once

#include <stdint.h>

/**
 * @brief Note duration definitions in musical notation
 * 
 * @details note duration are not translated to "milliseconds" here, as that depends on the tempo (BPM) of the piece.
 * - Tempo(BPM) defines the time base signature:
 *  * Most often the BPM refers to quarter notes per minute, but this can vary based on time signature and musical context.
 *  * e.g for 120bpm: A quarter note lasts 1 beat -> 120bpm = 0.5 seconds per beat -> Quarter note = 500ms, Half note = 1000ms, Whole note = 2000ms, etc.
 * 
 * - Later to translate these durations to actual time (milliseconds), you can use the formula:
 *  *   duration_in_ms = (60000 / BPM) * (4 / note_duration)
 *  * where note_duration is one of the defined durations in this namespace (e.g., Quarter = 4, Half = 2, etc.)
 * 
 * @note This namespace only defines the relative durations in musical notation.
 * @see https://en.wikipedia.org/wiki/Note_value
 * 
 */
namespace durations {

    // Note duration definitions
    constexpr uint8_t Whole       = 1;     // WHOLE note = 4 * quarters(beats)
    constexpr uint8_t Half        = 2;     // HALF note = 2 * quarters(beats)   
    constexpr uint8_t Quarter     = 4;     // QUARTER note = 1 * quarter(beat)
    constexpr uint8_t Eighth      = 8;     // EIGHTH note = 0.5 * quarter(beat)
    constexpr uint8_t Sixteenth   = 16;    // SIXTEENTH note = 0.25 * quarter(beat)
    constexpr uint8_t ThirtySecond = 32;   // THIRTY-SECOND note = 0.125 * quarter(beat)


} // end namespace durations