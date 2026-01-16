The application is structured into several key components:

- **ArduinoToneBackend**: This class handles the low-level hardware interactions to generate PWM signals for sound output through the buzzer.
- **MelodyBuilder**: This class provides a fluent interface to construct melodies using musical notation, allowing users to define notes and rests in a way that resembles traditional sheet music.
- **BuzzerPlayer**: This class manages the playback of melodies, coordinating with the hardware backend to play notes in sequence and handle looping if required.

The main program initializes these components, builds a melody (either from presets or custom definitions), and starts playback. The loop function continuously updates the player to ensure smooth operation.
 
 - How notes are interpreted and converted into frequencies and durations:
    *  The MelodyBuilder class translates musical notation (e.g., quarter notes, eighth notes) into specific frequencies (in Hz) and durations (in milliseconds) based on a defined tempo (beats per minute). 
   *  This allows users to compose melodies in a familiar format while the system handles the conversion to parameters suitable for playback.
