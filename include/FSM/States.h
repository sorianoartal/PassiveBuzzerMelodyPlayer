#pragma once

namespace fsm
{
    /**
     * @brief Player Finite State Machine States
     * 
     * @details
     * A melody is play in a sequence of steps. Each step has a duration and a frequency.
     * So this FSM enum will give the states for the Buzzer Player class.
     */
    enum class State
    {
        IDLE,          // System is idle
        START_STEP,    // Starting a new step on the melody
        PLAYING_STEP,  // Currently playing a step of the melody
        ADVANCE_STEP   // Advancing to the next step
    };

} // namespace fsm