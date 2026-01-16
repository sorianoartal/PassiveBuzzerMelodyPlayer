#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * @brief What the MCU actually plays
 * @details
 * A Step consists on a signal with a frequency in Hz and a duration in milliseconds.
 * 
 * Example:
 * A Step with freqHz = 440 and durationMs = 1000 means "play a 440 Hz tone for 1 second".
 * 
 */
struct Step
{
    uint16_t freqHz;
    uint32_t durationMs;
};

/**
 * @brief A melody consists of a sequence of multiple Steps
 * 
 * @details
 *  A simple monophonic melody(one tone at a time) can be described as a sequence of events:
 *  1. Pitch (frequency in Hz): which note to play A4,C5, etc.
 *  2. Duration (in milliseconds): how long to play the note.
 *  3. (Optional) Gap between notes (in milliseconds): silence between notes.
 *  4. Tempo (in BPM): speed of the melody. Tempo relates to the time that duration of notes playing(how many beats occur in a given time frame - usually a minute). 
 */
struct Melody
{
    const Step* steps;
    size_t count;
};

/**
 * @brief Context for playing a melody
 * 
 */
struct MelodyContext
{
    uint16_t bpm    = 120;  // default BPM = 120
    uint16_t gapMs  = 0;    // default gap between notes = 0 ms

    static constexpr uint16_t MIN_PLAY_MS = 10 ; // minimum playable duration for a note in milliseconds so there are a min oscillation to actually hear the tone
};


