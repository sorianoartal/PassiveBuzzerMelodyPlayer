#pragma     once
#include <Arduino.h>

#include "../music/Notes.h"
#include "../music/Durations.h"
#include "../music/Score.h"
#include "../presetTones/PresetId.h"

/**
 * @brief Preset tones definitions
 * 
 * @details This namespace contains preset tones defined as musical scores.
 * These can be used for common sound effects like success, error, notification, etc.
 * 
 */
 namespace presets {
    using namespace score;
    
    // Preset tone: Success
    static const score::ScoreNote TONE_SUCCESS[] = {
        {notes::C5, durations::Eighth},
        {notes::E5, durations::Eighth},
        {notes::G5, durations::Quarter},
        {notes::C6, durations::Half}
    };

    /**
     * @brief Get the Success Tone Score View object
     * 
     * @details To play the success tone, use this function to get the ScoreView.
     *  So it can be play as a succession of notes.
     *  
     * @return the tone as a ScoreView
     */
    inline ScoreView success()
    {
        return ScoreView{TONE_SUCCESS, sizeof(TONE_SUCCESS) / sizeof(score::ScoreNote)};
    }

    // =========================================================================
    // Preset tone: Error
    static const score::ScoreNote TONE_ERROR[] = {
        {notes::C6, durations::Eighth},
        {notes::Gs5_Ab5, durations::Eighth},
        {notes::E5, durations::Quarter},
        {notes::C5, durations::Half}
    };
    inline const ScoreView error()
    {
        return ScoreView{TONE_ERROR, sizeof(TONE_ERROR) / sizeof(score::ScoreNote)};
    }

    // =========================================================================
    // Preset tone: Notification
    static const score::ScoreNote TONE_NOTIFICATION[] = {
        {notes::E5, durations::Sixteenth},
        {notes::G5, durations::Sixteenth},
        {notes::C6, durations::Eighth},
        {notes::G5, durations::Eighth}
    };
    inline const ScoreView notification()
    {
        return ScoreView{TONE_NOTIFICATION, sizeof(TONE_NOTIFICATION) / sizeof(score::ScoreNote)};
    }

    // =========================================================================
    // Preset tone: Warning
    static const score::ScoreNote TONE_WARNING[] = {
        {notes::C5, durations::Eighth},
        {notes::D5, durations::Eighth},
        {notes::E5, durations::Eighth},
        {notes::D5, durations::Eighth},
        {notes::C5, durations::Quarter}
    };
    inline const ScoreView warning()
    {
        return ScoreView{TONE_WARNING, sizeof(TONE_WARNING) / sizeof(score::ScoreNote)};
    }

    // =========================================================================
    // Preset tone: Startup
    static const score::ScoreNote TONE_STARTUP[] = {
        {notes::G4, durations::Eighth},
        {notes::C5, durations::Eighth},
        {notes::E5, durations::Eighth},
        {notes::G5, durations::Eighth},
        {notes::C6, durations::Quarter}
    };
    inline const ScoreView startup()
    {
        return ScoreView{TONE_STARTUP, sizeof(TONE_STARTUP) / sizeof(score::ScoreNote)};
    }
    
    // =========================================================================
    // Preset tone: Shutdown
    static const score::ScoreNote TONE_SHUTDOWN[] = {
        {notes::C6, durations::Quarter},
        {notes::G5, durations::Eighth},
        {notes::E5, durations::Eighth},
        {notes::C5, durations::Eighth},
        {notes::G4, durations::Eighth}
    };
    inline const ScoreView shutdown()
    {
        return ScoreView{TONE_SHUTDOWN, sizeof(TONE_SHUTDOWN) / sizeof(score::ScoreNote)};
    }

    // =========================================================================
    // Preset tone: Button click
    static const score::ScoreNote TONE_BUTTON_CLICK[] = {
        {notes::E5, durations::Sixteenth},
        {notes::G5, durations::Sixteenth}
    };
    inline const ScoreView buttonClick()
    {
        return ScoreView{TONE_BUTTON_CLICK, sizeof(TONE_BUTTON_CLICK) / sizeof(score::ScoreNote)};
    }


    // Add more preset tones as needed



    // ===  TO SELECT PRESETS BY ID ===

    /**
     * @brief Get the Preset ScoreView By Id object
     * 
     * @param id - Preset id
     * @return score::ScoreView - struct that hold a reference to the actual data(ScoreNote*) and size 
     */
    inline score::ScoreView getPresetById(PresetId id)
    {
        switch (id)
        {
            case PresetId::Success:         return success();
            case PresetId::Error:           return  error();
            case PresetId::Notification:    return notification();
            case PresetId::Warning:         return warning();
            case PresetId::Startup:         return startup();
            case PresetId::Shutdown:        return shutdown();
            case PresetId::ButtonClick:     return buttonClick();
            default:  break;
        }
    }
}