#pragma once

#include "../core/Types.h"
#include "../music/Score.h"
#include "../lib/avr_algorithms.h"
#include "../music/Notes.h"
#include "../logger/Logger.h"


/**
 * @brief MelodyBuilder class for constructing melodies step by step.
 * 
 * @details 
 * The MelodyBuilder class provides a convenient interface for building melodies
 * by adding notes and rests in both musical notation and raw time-based formats.
 * It allows setting global parameters such as tempo and articulation gap between notes.
 * The built melody can be retrieved as a Melody object for playback or further processing.
 * 
 * @note Provide different options to compose a melody:
 * 1. Using addNote/addRest methods to build melody step by step.
 * 2. Using appendScore to add multiple ScoreNote from an array or a reader lambda.
 * 3. Using compose method with a DSL-style lambda for on-the-fly composition.
 * 
 * Example usage:
 * 
 * // option 1: build melody using MelodyBuilder
 * MelodyBuilder builder(buffer, capacity);
 * builder.setTempo(120)
 *        .gap(50)
 *        .addNote(440, duration::Quarter)
 *        .addRest(duration::Eighth)
 *        .addNote(494, duration::Quarter);
 * 
 * Melody melody = builder.build();
 * 
 * // Option2: to append score from an array of ScoreNote
 * builder.clearMelody()
 *        .tempo(140)
 *        .gap(20)
 *        .appendScore(getPreset(Preset::Success));
 * 
 * Melody melody = builder.build();
 * 
 * // Option3: to compose melody:
 * builder.clearMelody()
 *       .tempo(100)
 *       .gap(30)
 *       .compose([](auto& myMelody){
 *          myMelody.addNote(523, duration::Half)
 *                  .addNote(587, duration::Quarter)
 *                  .addNote(659, duration::Quarter)
 *                  .addRest(duration::Eighth);
 *          });   
 * 
 * Melody melody = builder.build();
 */
class MelodyBuilder {
public:
    /**
     * @brief Construct a new Melody Builder object
     * 
     * @param buffer 
     * @param capacity 
     */
    MelodyBuilder(Step* buffer, size_t capacity);

    // default destructor    
    ~MelodyBuilder() = default;

    // reset the melody builder to empty state
    MelodyBuilder& clearMelody(bool toDefault);

    // Global musical parameters for the melody
    MelodyBuilder& setTempo(int bpm);                   // beat = quarter note
    MelodyBuilder& gap(uint16_t gapMs);                 // articulation gap between notes in milliseconds   

    // Core musical primitives(sheet-music realm)
    MelodyBuilder& addNote(uint16_t hz, uint8_t denom);   // converts denom->ms using tempo, applies gap
    MelodyBuilder& addRest(uint8_t denom);                // rest in musical notation, converts denom->ms using tempo, applies gap

    // Raw primitives(digital-audio realm)
    MelodyBuilder& addToneMs(uint16_t hz, uint32_t durationMs);
    MelodyBuilder& addRestMs(uint32_t durationMs);


    // ============================= COMPOSE MELODY OPTIONS ==================================================

    // OPTION 1: AppendScore from an array of ScoreNote 
    /**
     * @brief AppendScore from an array of ScoreNote
     * 
     * @details 
     * This function appends multiple ScoreNote to the melody being built.
     * Each ScoreNote consists of a frequency in Hz and a duration in musical notation.
     * The function converts each ScoreNote to its corresponding Step (frequency and duration in milliseconds)
     * and adds it to the melody.
     * 
     * @param score - Pointer to the array of ScoreNote to append
     * @param count - Number of ScoreNotes in the array
     * @return MelodyBuilder& - Reference to the current MelodyBuilder instance
     */
    MelodyBuilder& appendScore(const score::ScoreNote* score, size_t count);
    
    // OPTION 2: Append the Score as a ScoreView
    /**
     * @brief Use this function to append from a ScoreView(Ideal for Presets) 
     * 
     * @details
     * A ScoreView does not owns date is just a struct that hold a reference to a ScoreNote*(data) and number of notes stored
     * 
     * @param view -A view of a musical score for Preset tones
     * @return MelodyBuilder& - Reference to the current MelodyBuilder instance
     */
    MelodyBuilder& appendScore(score::ScoreView view);

    // OPTION 3: AppendScore using reader lambda
    /**
     * @brief AppendScore from a reader lambda function 
     * 
     * @details 
     * works for read save melody from flash or other non-linear storage( PROGMEM, EEPROM, SD card, etc)
     * 
     * @tparam Reader - Type of the callable reader function 
     * @param reader  - The function to read ScoreNote at given index
     * @param count   - Number of ScoreNotes to read  
     * @return MelodyBuilder& - Reference to the current MelodyBuilder instance
     */
    template<typename Reader>
    MelodyBuilder& appendScore(Reader&& reader, size_t count)
    {
        avr_algorithms::for_each_indexed(buffer_, length_, [&](Step& step, size_t index){
            
            // Step1: if there is still capacity to add more notes 
            if(ok_)
            {   
                // Step2: read from the reader lambda function
                score::ScoreNote note = reader(index);

                // Step3: add the store note to the melody
                addNote(note.hz, note.denom);
            }
        });
        return *this;
    }

    // OPTION4: compose on the fly(DSL style)
    /**
     * @brief Compose melody using a DSL-style lambda function
     * 
     * @details 
     * This function allows composing a melody on the fly using a lambda function.
     * The lambda function receives a reference to the MelodyBuilder instance, allowing
     * for chaining calls to addNote and addRest in a more readable manner.
     * 
     * @tparam Func - Type of the callable function to compose the melody
     * @param func  - The function that defines the melody composition
     * @return MelodyBuilder& - Reference to the current MelodyBuilder instance
     */
    template<typename Func>
    MelodyBuilder& compose(Func&& func)
    {   
        // func receives MelodyBuilder& to chain calls to addNote/addRest
        func(*this);
        return *this;
    }

    // ==================================================================================================
    
    // Build the melody
    Melody build() const; 

    // --- For retrieve status: Debugging  ---

    /// @brief to check if the build is done correctly not overflow capacity
    /// @return Flag that check there is more capacity to add notes
    bool ok()const;

    /// @brief Return the current number of Steps stored 
    /// @return melody length
    size_t size()const;

private:
    // push Step{freqHz, durMs} into buffer
    bool pushStep_(uint16_t hz, uint32_t durationMs);

    // converts denom + bpm -> the total slot duration for a note in milliseconds(no gap applied here)
    uint32_t denomToMs_(uint8_t denom, uint16_t bpm);

private:    

    Step* buffer_;              // Pointer to the buffer where the melody steps are stored
    size_t capacity_;           // Maximum number of steps the buffer can hold
    size_t length_ ;            // Current number of steps in the melody
    MelodyContext ctx_;         // Context for melody playback (tempo, gap, etc.)

    bool ok_;                   // Flag to indicate if we have capacity to add more steps   

}; // MelodyBuilder