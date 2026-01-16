#pragma once

#include <stdint.h>

/**
 * @brief Note frequency definitions in Hertz (Hz)
 * 
 * @see See https://muted.io/note-frequencies/ for reference
 * 
 */
namespace notes {

    // REST Note frequency: 0 Hz implies no sound 
    constexpr uint16_t REST = 0;
    
    // C Note frequencies in Hz from C0 to C8
    constexpr uint16_t C0  = 16;
    constexpr uint16_t C1 = 33;
    constexpr uint16_t C2 = 65;
    constexpr uint16_t C3 = 131;
    constexpr uint16_t C4 = 262;
    constexpr uint16_t C5 = 523;
    constexpr uint16_t C6 = 1047;
    constexpr uint16_t C7 = 2093;
    constexpr uint16_t C8 = 4186;

    // C# / Db Note frequencies in Hz from C#0/Db0 to C#8/Db8
    constexpr uint16_t Cs0_Db0  = 17;
    constexpr uint16_t Cs1_Db1 = 35;
    constexpr uint16_t Cs2_Db2 = 69;
    constexpr uint16_t Cs3_Db3 = 139;
    constexpr uint16_t Cs4_Db4 = 277;
    constexpr uint16_t Cs5_Db5 = 554;
    constexpr uint16_t Cs6_Db6 = 1109;
    constexpr uint16_t Cs7_Db7 = 2217;
    constexpr uint16_t Cs8_Db8 = 4435;    

    // D Note frequencies in Hz from D0 to D8
    constexpr uint16_t D0  = 18;
    constexpr uint16_t D1 = 37;
    constexpr uint16_t D2 = 73;
    constexpr uint16_t D3 = 147;
    constexpr uint16_t D4 = 294;
    constexpr uint16_t D5 = 587;
    constexpr uint16_t D6 = 1175;
    constexpr uint16_t D7 = 2349;
    constexpr uint16_t D8 = 4699;

    // D# / Eb Note frequencies in Hz from D#0/Eb0 to D#8/Eb8
    constexpr uint16_t Ds0_Eb0  = 19;
    constexpr uint16_t Ds1_Eb1 = 39;
    constexpr uint16_t Ds2_Eb2 = 77;
    constexpr uint16_t Ds3_Eb3 = 155;
    constexpr uint16_t Ds4_Eb4 = 311;
    constexpr uint16_t Ds5_Eb5 = 622;
    constexpr uint16_t Ds6_Eb6 = 1245;
    constexpr uint16_t Ds7_Eb7 = 2489;
    constexpr uint16_t Ds8_Eb8 = 4978;

    // E Note frequencies in Hz from E0 to E8
    constexpr uint16_t E0  = 21;
    constexpr uint16_t E1 = 41;
    constexpr uint16_t E2 = 82;
    constexpr uint16_t E3 = 165;
    constexpr uint16_t E4 = 330;
    constexpr uint16_t E5 = 659;
    constexpr uint16_t E6 = 1319;
    constexpr uint16_t E7 = 2637;
    constexpr uint16_t E8 = 5274;

    // F Note frequencies in Hz from F0 to F8
    constexpr uint16_t F0  = 22;
    constexpr uint16_t F1 = 44;
    constexpr uint16_t F2 = 87;
    constexpr uint16_t F3 = 175;
    constexpr uint16_t F4 = 349;
    constexpr uint16_t F5 = 698;
    constexpr uint16_t F6 = 1397;
    constexpr uint16_t F7 = 2794;
    constexpr uint16_t F8 = 5588;

    // F# / Gb Note frequencies in Hz from F#0/Gb0 to F#8/Gb8
    constexpr uint16_t Fs0_Gb0  = 23;
    constexpr uint16_t Fs1_Gb1 = 46;
    constexpr uint16_t Fs2_Gb2 = 92;
    constexpr uint16_t Fs3_Gb3 = 185;
    constexpr uint16_t Fs4_Gb4 = 370;
    constexpr uint16_t Fs5_Gb5 = 740;
    constexpr uint16_t Fs6_Gb6 = 1480;
    constexpr uint16_t Fs7_Gb7 = 2960;
    constexpr uint16_t Fs8_Gb8 = 5920;

    // G Note frequencies in Hz from G0 to G8
    constexpr uint16_t G0  = 25;
    constexpr uint16_t G1 = 49;
    constexpr uint16_t G2 = 98;
    constexpr uint16_t G3 = 196;
    constexpr uint16_t G4 = 392;
    constexpr uint16_t G5 = 784;
    constexpr uint16_t G6 = 1568;
    constexpr uint16_t G7 = 3136;
    constexpr uint16_t G8 = 6272;

    // G# / Ab Note frequencies in Hz from G#0/Ab0 to G#8/Ab8
    constexpr uint16_t Gs0_Ab0  = 26;
    constexpr uint16_t Gs1_Ab1 = 52;
    constexpr uint16_t Gs2_Ab2 = 104;
    constexpr uint16_t Gs3_Ab3 = 208;
    constexpr uint16_t Gs4_Ab4 = 415;
    constexpr uint16_t Gs5_Ab5 = 831;
    constexpr uint16_t Gs6_Ab6 = 1661;
    constexpr uint16_t Gs7_Ab7 = 3322;
    constexpr uint16_t Gs8_Ab8 = 6645;

    // A Note frequencies in Hz from A0 to A8
    constexpr uint16_t A0  = 27;
    constexpr uint16_t A1 = 55;
    constexpr uint16_t A2 = 110;
    constexpr uint16_t A3 = 220;
    constexpr uint16_t A4 = 440;        // Standard tuning pitch
    constexpr uint16_t A5 = 880;
    constexpr uint16_t A6 = 1760;
    constexpr uint16_t A7 = 3520;
    constexpr uint16_t A8 = 7040;

    // A# / Bb Note frequencies in Hz from A#0/Bb0 to A#8/Bb8
    constexpr uint16_t As0_Bb0  = 29;
    constexpr uint16_t As1_Bb1 = 58;
    constexpr uint16_t As2_Bb2 = 116;
    constexpr uint16_t As3_Bb3 = 233;
    constexpr uint16_t As4_Bb4 = 466;
    constexpr uint16_t As5_Bb5 = 932;
    constexpr uint16_t As6_Bb6 = 1865;
    constexpr uint16_t As7_Bb7 = 3729;
    constexpr uint16_t As8_Bb8 = 7459;

    // B Note frequencies in Hz from B0 to B8
    // B0 and B1 are named B0_ and B1_ to avoid conflict with Arduino's predefined macros
    constexpr uint16_t B0_  = 31;   
    constexpr uint16_t B1_ = 62;
    constexpr uint16_t B2 = 123;
    constexpr uint16_t B3 = 247;
    constexpr uint16_t B4 = 494;
    constexpr uint16_t B5 = 988;
    constexpr uint16_t B6 = 1976;
    constexpr uint16_t B7 = 3951;
    constexpr uint16_t B8 = 7902;

} // end namespace notes
    