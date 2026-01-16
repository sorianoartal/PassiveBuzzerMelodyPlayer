#include "builder/MelodyBuilder.h"


/**
 * @brief Construct a new Melody Builder:: Melody Builder object
 * 
 * @param buffer - buffer to store the melody steps
 * @param capacity - number of steps the buffer can hold
 */
MelodyBuilder::MelodyBuilder(Step* buffer, size_t capacity): 
    buffer_(buffer), 
    capacity_(capacity),
    length_(0),
    ctx_(MelodyContext{}),
    ok_((buffer_ != nullptr) && (capacity_ > 0))
{};

/**
 * @brief Reset the melody builder to initial clean sheet melody state
 * 
 * @para toDefault - if true, reset to default musical parameters (tempo=120bpm, gap=0ms)
 * @return MelodyBuilder& - Reference to the current MelodyBuilder instance
 */
MelodyBuilder& MelodyBuilder::clearMelody(bool toDefault)
{
    // 1. reset length to zero
    length_ = 0;

    // 2. reset context to default if requested
    if (toDefault) ctx_ = MelodyContext{};

    // 3. clear notes buffer pointer and capacity check
    ok_ = (buffer_ != nullptr) && (capacity_ > 0);

    return *this;
}

/**
 * @brief Set the tempo of the melody being built
 * 
 * @details tempo is defined in beats per minute (BPM), where a beat is a quarter note.
 * knowing the tempo we can convert each note from the real sheet music notation(denom) to duration in milliseconds.
 * 
 * @param bpm - Tempo in beats per minute
 * @return MelodyBuilder& - Reference to the current MelodyBuilder instance
 */
MelodyBuilder& MelodyBuilder::setTempo(int bpm)
{
    // Validate tempo
    if(bpm <= 0 || bpm > 300) ok_ = false; // invalid tempo
    else ctx_.bpm = bpm;

    return *this;
}

/**
 * @brief Gap in milliseconds between notes in the melody
 * 
 * @details
 * This function sets the articulation gap between consecutive notes in the melody.
 * 
 * @param gapMs - Gap in milliseconds
 * @return MelodyBuilder& - Reference to the current MelodyBuilder instance
 */
MelodyBuilder& MelodyBuilder::gap(uint16_t gapMs)
{
    // Validate gap
    if( gapMs > 1000) ok_ = false; // invalid gap
    else ctx_.gapMs = gapMs;
    
    return *this;
}



/**
 * @brief AppendScore from an array of ScoreNote
 * 
 * @details
 * This function appends multiple ScoreNote to the melody being built.
 * the sheet music is composed of an array of ScoreNote, each containing a frequency in Hz and a duration in musical notation.
 * 
 * @param score - Pointer to the array of ScoreNote
 * @param count - Number of ScoreNotes in the array
 * @return MelodyBuilder& - Reference to the current MelodyBuilder instance
 */
MelodyBuilder& MelodyBuilder::appendScore(const score::ScoreNote *score, size_t count)
{
    //Validate input
    if(!score && count>0)
    {
        ok_ = false;
        return *this;
    }

    // Iterate through each ScoreNote in the array and add it to the melody
    avr_algorithms::for_each_indexed(score, count, [this](const score::ScoreNote &note, size_t index) {
        // Step1: if there is still capacity to add more notes
        if (ok_)
        {
            // add the store note to the melody
            addNote(note.hz, note.denom);   
        }
    });

    return *this;
}

/**
* @brief Use this function to append from a ScoreView(Ideal for Presets) 
* 
* @details
* A ScoreView does not owns date is just a struct that hold a reference to a ScoreNote*(data) and number of notes stored
* 
* @param view -A view of a musical score for Preset tones
* @return MelodyBuilder& - Reference to the current MelodyBuilder instance
*/
MelodyBuilder& MelodyBuilder::appendScore(score::ScoreView view)
{
    appendScore(view.data, view.count);
}


/**
 * @brief Add a note to the melody being built
 * 
 * @details
 * This function adds a musical note to the melody by converting the musical notation (denom) to duration in milliseconds.
 * It uses the current tempo set in the MelodyContext to perform this conversion.
 * 
 * @param hz - Frequency in Hz of the note
 * @param denom - Duration in musical notation (e.g., quarter note, half note)
 * @return MelodyBuilder& - Reference to the current MelodyBuilder instance
 */
MelodyBuilder &MelodyBuilder::addNote(uint16_t hz,uint8_t denom)
{
    // Convert musical notation to duration in milliseconds using the current tempo
    uint32_t durationMs = denomToMs_(denom, ctx_.bpm);

    // Validate duration    
    if (durationMs ==0)
    {
        ok_ = false;
        return *this;
    }
    
    // Silence is allow so hz==0 means rest
    if(hz==0)
    {
        // Push the step to the melody buffer
        pushStep_(hz, durationMs);
        return *this;
    }

    // Enforce minimum playable duration
    uint32_t playMs = durationMs;
    uint32_t restMs = 0;

    // if there is a gap between notes, add it to the duration
    if(hz>0 && ctx_.gapMs > 0 && playMs > MelodyContext::MIN_PLAY_MS)
    {
        // 1. compute the max gap allow so the tone is audible
        uint32_t maxGap = playMs - MelodyContext::MIN_PLAY_MS;      

        // 2. clamp the restMs to the maxGap if the requested gap is too large
        restMs = (ctx_.gapMs > maxGap) ? maxGap : ctx_.gapMs;
        playMs -= restMs;
    }

    // if there is a gap to leave we split the note into play + rest
    pushStep_(hz, playMs);                  // Push the step to the melody buffer
    if(restMs > 0) pushStep_(0, restMs);    // add rest if there is a gap

    LOGD("addNote hz=%u denom=%u total=%lu play=%lu rest=%lu",
     hz, denom, (unsigned long)durationMs, (unsigned long)playMs, (unsigned long)restMs
    );


    return *this;
}

/**
 * @brief Add a rest to the melody being built
 * 
 * @param denom - Duration in musical notation (e.g., quarter note, half note)
 * @return MelodyBuilder& - Reference to the current MelodyBuilder instance
 */
MelodyBuilder &MelodyBuilder::addRest(uint8_t denom)
{
    // Convert musical notation to duration in milliseconds using the current tempo
    uint32_t durationMs = denomToMs_(denom, ctx_.bpm);

    // Push the rest step to the melody buffer
    pushStep_(0, durationMs); // 0 Hz for rest

    return *this;
}

/**
 * @brief Add a tone with specified frequency and duration in milliseconds to the melody being built
 * 
 * @param hz - Frequency in Hz of the tone
 * @param durationMs - Duration in milliseconds
 * @return MelodyBuilder& - Reference to the current MelodyBuilder instance
 */
MelodyBuilder& MelodyBuilder::addToneMs(uint16_t hz, uint32_t durationMs)
{
    // Push the tone step to the melody buffer
    pushStep_(hz, durationMs);

    return *this;
}


/**
 * @brief Add a rest with specified duration in milliseconds to the melody being built
 * 
 * @param durationMs - Duration in milliseconds
 * @return MelodyBuilder& - Reference to the current MelodyBuilder instance
 */
MelodyBuilder& MelodyBuilder::addRestMs(uint32_t durationMs)
{
    // Push the rest step to the melody buffer
    pushStep_(0, durationMs); // 0 Hz for rest

    return *this;
}

/**
 * @brief Build a melody
 * @details
 * This function constructs a Melody object from the steps added to the MelodyBuilder. 
 *  
 * @return Melody 
 */
Melody MelodyBuilder::build() const
{
    return Melody{buffer_, length_};
}

/**
 * @brief Check if the builder did not overflow it's capacity so more Steps can be added
 * 
 * @return true - If there still capacity to store more Steps
 * @return false - if we overflow it's capacity
 */
bool MelodyBuilder::ok()const
{
    return ok_;
}

/**
 * @brief Get the current size of the buffer
 * 
 * @return size_t - Number of Step currently stored on the builder buffer
 */
size_t MelodyBuilder::size()const
{
    return length_;
}

//                                  === Private methods =====

/**
 * @brief Push a step to the melody buffer if there is capacity
 * 
 * @details
 * This is a private helper function used to add a Step (frequency and duration) to the melody buffer.
 * 
 * What it does:
 *  1. check if there is enough capacity in the buffer to add a new step
 *  2. if there is capacity, it adds the step and increments the length
 *  3. if there is no capacity, it marks the builder as not ok
 * 
 * 
 * @param hz- Frequency in Hz 
 * @param durationMs - Duration in milliseconds
 * @return bool - true if the step was added successfully, false if there was no capacity
 */
bool MelodyBuilder::pushStep_(uint16_t hz, uint32_t durationMs)
{
    if (length_ < capacity_)
    {
        LOGD("pushStep len=%u f=%u ms=%lu", (unsigned)length_, hz, (unsigned long)durationMs);
        buffer_[length_++] = Step{hz, durationMs};
        return true;
    }
    else
    {
        LOGE("pushStep overflow len=%u cap=%u", (unsigned)length_, (unsigned)capacity_);
        ok_ = false; // mark as not ok if we exceed capacity
        return false;
    }
}



/**
 * @brief Convert musical notation (denom) to duration in milliseconds
 * 
 * @details How the conversion is done:
 *  - Each note duration in musical notation (denom) or rhythm is store as a relation to the number of beats
 *      WHOLE = 1 (4 beats), HALF = 2 (2 beats), QUARTER = 4 (1 beat), EIGHTH = 8 (0.5 beat), etc.
 *  - So knowing the tempo of the sheet music (in BPM), we can calculate the duration in milliseconds for each note. 
 *  - The formula to convert denom to milliseconds is:
 *          beatsMs = 60000/BPM ; this is how many milliseconds a beat lasts
 *  - Then, we also know how many beats a note has, so the duration in milliseconds for a note is:
 *         note_durationMs = beatsMs * noteBeats;
 * 
 *  - So, combining the two formulas, we get:
 *          note_durationMs = (60000 / BPM) * (4 / denom)
 * 
 * @param denom 
 * @param bpm 
 * @return MelodyBuilder& 
 */
uint32_t MelodyBuilder::denomToMs_(uint8_t denom, uint16_t bpm)
{
    // Validate inputs
    if (denom == 0 || bpm == 0) {
        ok_ = false;
        return 0;
    }

    const uint32_t numerator = 60000UL * 4UL;                 // 240000
    const uint32_t divisor   = (uint32_t)bpm * (uint32_t)denom;

    uint32_t noteMs = numerator / divisor;  // note_durationMs = (60000 / BPM) * (4 / denom)

    // never return 0ms (avoids zero-length steps at high BPM / small notes)
    if (noteMs == 0) noteMs = 1;

    LOGD("denomToMs bpm=%u denom=%u -> %lu", bpm, denom, (unsigned long)noteMs);

    return noteMs;
}