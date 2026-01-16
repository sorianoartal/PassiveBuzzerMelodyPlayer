/**
 * @file main.cpp
 * @author David Soriano Artal 
 * @brief This app reproduces melodies using a passive buzzer connected to an Arduino-compatible board.
 * 
 * @details
 * The application is structured into several key components:
 * - **ArduinoToneBackend**: This class handles the low-level hardware interactions to generate PWM signals for sound output through the buzzer.
 * - **MelodyBuilder**: This class provides a fluent interface to construct melodies using musical notation, allowing users to define notes and rests in a way that resembles traditional sheet music.
 * - **BuzzerPlayer**: This class manages the playback of melodies, coordinating with the hardware backend to play notes in sequence and handle looping if required.
 * The main program initializes these components, builds a melody (either from presets or custom definitions), and starts playback. The loop function continuously updates the player to ensure smooth operation.
 * 
 * - How notes are interpreted and converted into frequencies and durations:
 *  The MelodyBuilder class translates musical notation (e.g., quarter notes, eighth notes) into specific frequencies (in Hz) and durations (in milliseconds) based on a defined tempo (beats per minute). 
 *  This allows users to compose melodies in a familiar format while the system handles the conversion to parameters suitable for playback.
 * 
 * @version 0.1
 * @date 2026-01-16
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#include <Arduino.h>

#include "backends/ArduinoToneBackend.h"  // Hardware abstraction for PWM(50% duty cycle) square wave generator
#include "config/Config.h"                // App config: Buzzer pin
#include "builder/MelodyBuilder.h"        // DataModel layer: To generate a melody the player can execute
#include "core/Types.h"                   // What the player actually. Sheet music notes in the digital realm
#include "player/BuzzerPlayer.h"          // Engine class( Schedule + Presets)
#include "presetTones/Presets.h"          // Preset stored tones( success, warning, error ...)
#include "logger/Logger.h"                // For debugging 
#include "../lib/avr_algorithms.h"


// Buffer where store the compose Melody(sequence of Steps)
Step melodyStepsBuffer[config::MAX_BUFFER_MELODY_STEP_SIZE];

// PWM wave generator: What actually generate the signal
ArduinoToneBackend hwBackend(config::BUZZER_PIN);

// Place holder where store the melody sequence of steps the builder creates 
Melody melody= {};

// DataModel instance:  Translate a real world music sheet into a sequence of step tha the Player understand(Digital realm -> Step{freqHz,durationMs})
MelodyBuilder builder(melodyStepsBuffer, config::MAX_BUFFER_MELODY_STEP_SIZE);

// Payer consumes the melody build: 
//  1. Start a Square wave using hwBackend at the specific note frequency
//  2. waits for the note to finish
//  3. Move to the next Step on the melody
BuzzerPlayer player(hwBackend);


void setup() {
 
 // Init Serial monitor 
 Serial.begin(115200);
 while (!Serial){};
 LOGI("Booting...");
 

//---   OPTION A : Presets  --- 
/*
A) Uncomment this block to Play a preset Score sequence(recommended for common UI sounds)
// Play preset
 melody = builder.clearMelody(true)
    .setTempo(120)
    .gap(20)
    .appendScore(presets::getPresetById(PresetId::Startup))
    .build(); 

*/

////////////////////////////////////////////////////////////
 
// ---  OPTION B : Compose melody on the fly(DSL style) ---
melody = builder.clearMelody(true)
  .setTempo(76)
  .gap(15)
  .compose([](MelodyBuilder& mBuilder){
    
    // Phrase A 
    mBuilder.addNote(notes::G5, durations::Quarter)
     .addNote(notes::D5, durations::Quarter)
     .addNote(notes::B5, durations::Quarter)

     .addNote(notes::G5, durations::Eighth)
     .addNote(notes::D5, durations::Eighth)
     .addNote(notes::C5, durations::Eighth)
     .addNote(notes::B5, durations::Eighth)
     .addNote(notes::A5, durations::Eighth)
     .addNote(notes::G5, durations::Eighth)

     .addNote(notes::G5, durations::Eighth)
     .addNote(notes::Fs5_Gb5, durations::Eighth)
     .addNote(notes::E5, durations::Eighth)
     .addNote(notes::D5, durations::Eighth)

     // Phrase pause (bigger than the gap)
     .addNote(notes::REST, durations::Eighth);

    // Phrase B
    mBuilder.addNote(notes::G5, durations::Quarter)
     .addNote(notes::A5, durations::Quarter)
     .addNote(notes::B5, durations::Quarter)

     // optional tiny breath (try with/without)
     .addNote(notes::REST, durations::Eighth)

     .addNote(notes::D5, durations::Eighth)
     .addNote(notes::C5, durations::Eighth)
     .addNote(notes::B5, durations::Eighth)
     .addNote(notes::A5, durations::Eighth)
     .addNote(notes::G5, durations::Eighth)

     .addNote(notes::D5, durations::Half); }) // strong “arrival”

  .build();

  // Debug: Check if we build the melody and dump the compiled steps
  LOGI("Builder ok= %d steps=%lu", builder.ok() , (unsigned)builder.size());
  for (size_t i = 0; i < melody.count && i < 10; ++i) {
    LOGD("Step[%u] f=%u ms=%lu", (unsigned)i, melody.steps[i].freqHz, (unsigned long)melody.steps[i].durationMs);
  }
////////////////////////////////////////////////////////////

// ---  OPTION C: Read Melody from custom Score arrays( for sheet music store in memory)
/*
static const score::ScoreNote customMelody[] = {
  {notes::E4, durations::Eighth},
  { notes::D4, durations::Eighth },
  { notes::C4, durations::Quarter }
};

melody = builder.clearMelody(true)
  .setTempo(120)
  .gap(20)
  .appendScore(customMelody,sizeof(customMelody)/sizeof(customMelody[0]))
  .build();
*/

/////////////////////////////////////////////////////////////
 
 //  Play the melody created for the builder
 player.play(melody,true);

}

void loop() 
{

 player.update();   // Must to be call often to run the FSM(non-blocking)

}

