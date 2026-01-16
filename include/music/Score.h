#pragma once

#include <stdint.h>

/**
 * @brief Music score "sheet music" related definitions
 * 
 */
namespace score{

    /**
     * @brief A note in a musical score
     * @details Combines a note frequency and its duration in musical notation.
     * 
     */
    struct ScoreNote
    {
        uint16_t hz;    // Frequency of the note in Hz. note::REST for a rest (no sound) allowed
        uint8_t denom;  // Duration of the note in musical notation (e.g., duration::Quarter, duration::Half, etc.)
    };

    /**
     * @brief A view of a musical score for Preset tones
     * 
     * @note This struct does not own the data, it is just a view.
     * @details This is useful for passing around score data without copying it(e.g preset tones like success,error, ).
     * 
     */
    struct ScoreView
    {
        const ScoreNote* data;
        uint16_t count;
    };
    

    
}// end namespace score