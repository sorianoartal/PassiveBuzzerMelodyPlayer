#include "player/BuzzerPlayer.h"


/**
 * @brief Construct a new Buzzer Player:: Buzzer Player object
 * 
 * @param hwBackend - Reference to an hardware wave form generation implementation
 */
BuzzerPlayer::BuzzerPlayer(IBuzzerBackend& hwBackend): 
hwBackend_(hwBackend),
melody_(nullptr),
melodyStepIdx_(0),
looping_(false),
stepDelay_(Delay(0)),
state_(fsm::State::IDLE)
{
    stepDelay_.init();
};


/**
 * @brief Loads the melody and arm the Player so it's start to play back the
 * melody in hte next update() call.
 * 
 * @param melody - Reference to the melody to be played
 * @param loop - Whether to loop the melody after it finishes
 */
void BuzzerPlayer::play(const Melody &melody, bool loop)
{   
    LOGI("play count=%u", (unsigned)melody.count);

    // 1. Check if we are already playing a melody
    if(isPlaying()) stop();         // Stop current playback if any

    // 2. Store the melody and loop flag
    melody_ = &melody;
    looping_ = loop;

    // 3. Reset the step index and set the state to start playing
    melodyStepIdx_ = 0;

    // 4. Set the FSM state to START_STEP to begin playback in the next update
    state_ = fsm::State::START_STEP;
}

/**
 * @brief Stop the current playing melody and reset the scheduler
 * 
 */
void BuzzerPlayer::stop()
{
    // 1.- Call the hardware backend to stop the buzzer
    hwBackend_.stop();

    // 2.Clear the active melody
    melody_ = nullptr;

    // 3. Reset states
    looping_ = false;
    melodyStepIdx_ = 0;

    // 4. Set the FSM state to IDLE
    state_ = fsm::State::IDLE;

    // 5.- Stop the timer so won't fired later
    stepDelay_.stopDelay();
}

/**
 * @brief Gives the rest of the app a simple query to know if we are playing a melody
 * 
 * @note
 * Why is usefull?
 *  - UI: If you need to show on a display if we are playing
 *  - logic: prevent re-trigger, so you can decide if you whether to queue something
 *  - Enter in power save mode if you are not playing
 * 
 * @return true - if the buzzer is playing
 * @return false - if we are in IDLE state so it's not playing a melody
 */
bool BuzzerPlayer::isPlaying() const
{
    return (state_ != fsm::State::IDLE); 
}

/**
 * @brief This function is the Engine of the FSM.
 * Progress the FSM states without blocking, base on timer
 * 
 * @details
 *  - update(): must be call ofter (in the main loop() or at least every few milliseconds)
 *  - Not heavy work is done here , just small constant work and return quickly
 */
void BuzzerPlayer::update()
{
    using namespace fsm;    

    switch (state_)
    {
        case State::IDLE :
        {            
            // Do nothing
            break;
        }

        case State::START_STEP:
        {
            // 1. Get the melody step we need to play
            const Step& mStep = getCurrentStep();

            // 2. Check if we need to play a note of is a REST
            if (mStep.freqHz > 0) hwBackend_.start(mStep.freqHz);       // Play note
            else hwBackend_.stop();                                     // REST == playing a silence
               
            // 3. Arm timer
            stepDelay_.init(mStep.durationMs * 1000UL);                 // Delay uses Us      

            LOGI("step idx=%u f=%u ms=%lu",
                (unsigned)melodyStepIdx_, (unsigned)mStep.freqHz, (unsigned long)mStep.durationMs
            );


            // 4. Move to wait until finish playin current step
            state_ = fsm::State::PLAYING_STEP;
            break;    
        }       
        
        case State::PLAYING_STEP:
        {
            // If Note duration elapsed we advance to the next melody Step.
            if(stepDelay_.isDelayTimeElapsed())
                
                state_ = fsm::State::ADVANCE_STEP;
                
                LOGI("step idx=%u f=%u ms=%lu",
                    (unsigned)melodyStepIdx_, (unsigned)getCurrentStep().freqHz, (unsigned long)getCurrentStep().durationMs
                );

            break;
        }
        

        case State::ADVANCE_STEP:
        {
            advanceToNextStep();
            break;
        }

        default:
        {
            break;
        }
    }
}



//////////////////////////////  PRIVATE HELPERS    ////////////////////////////////////////////////

/**
 * @brief Gets the current step we are playing in a melody(sequence of steps)
 * 
 * @return const Step& - current step that's been playing
 */
const Step& BuzzerPlayer::getCurrentStep() const
{
    return melody_->steps[melodyStepIdx_];
}

/**
 * @brief This function advance the to the next musical note(Step) on the melody
 * 
 * @details 
 *  1. It validates that there is a melody(!nullptr) and the possible overflow:
 *      - If we end the current melody(overflow) and looping is enable -> arm player to start the melody again
 *      - If we reached the end -> stop playing and ensure reset state
 *  2. otherwise advance to the next step(No edge case) 
 */
void BuzzerPlayer::advanceToNextStep()
{
    // 1. Increment idx of the melody steps
    ++melodyStepIdx_;

    // 2. Handle overflow: Validate melody and if we overflow
    if((melody_ != nullptr) && melodyStepIdx_ >= melody_->count)
    {
        // If looping is true and we averFlow -> start again the melody from the beginning
        if (melody_ && looping_)
        {
            melodyStepIdx_ = 0;
            state_ = fsm::State::START_STEP;
        }
        else
        {
            // We finish and looping_ is disable -> stop Player
            stop(); // Ensure hardware backend stops and reset state.
        }
        return;        
    }

    // 3. Advance to the next step of the melody
    state_ = fsm::State::START_STEP;

}


