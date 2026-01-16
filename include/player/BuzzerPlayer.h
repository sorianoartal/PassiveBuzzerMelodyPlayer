#pragma once

#include <stdint.h>
#include "player/IBuzzerBackend.h"
#include "core/Types.h"
#include "Timer/Delay.h"
#include "FSM/States.h"


/**
 * @brief Buzzer Player class that uses a backend to play melodies
 *  It's the Melody scheduler(FSM) and controller 
 * 
 * @details
 * This class manages the playback of melodies using a provided IBuzzerBackend implementation.
 * It handles the timing and sequencing of notes in the melody.
 * 
 */
class BuzzerPlayer
{
    public:

        /// @brief Constructor for BuzzerPlayer
        /// @param IBuzzerBackend Reference to an hardware wave form generation implementation
        BuzzerPlayer(IBuzzerBackend& hwBackend);
        
        /// @brief Default destructor
        ~BuzzerPlayer() = default;        

        /// @brief Function that starts playing a melody
        /// @param melody - Reference to the melody to be played
        /// @param loop - Whether to loop the melody after it finishes
        void play(const Melody& melody, bool loop = false);

        
        /// @brief Implementation for stopping the buzzer
        void stop()  ;

        /// @brief Check if the buzzer is currently playing
        /// @return true if the buzzer is playing, false otherwise
        bool isPlaying() const;        

        /// @brief Update the buzzer state, should be called periodically
        void update();


    private:

    // === helper private functions ===

    /// @brief Retrieve the current step being played
    /// @return Reference to the current step
    const Step& getCurrentStep() const;

    /// @brief Advance to the next step in the melody
    void advanceToNextStep();

    // === private members ===

    IBuzzerBackend& hwBackend_;     // Reference to the buzzer backend implementation

    const Melody* melody_;              // The melody to be played 
  
    size_t  melodyStepIdx_;             // Current melody step index 

    bool looping_;                       // Whether to loop the melody
    Delay stepDelay_;                   // Delay for the current step

    fsm::State state_;                 // Current state of the player FSM
    
};