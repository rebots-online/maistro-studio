#pragma once

#include <vector>
#include <string> // For potential future string members like debugInfo

// Forward declare engraving types if they were to be included directly in these structs
// For now, these structs are self-contained or use basic types.

namespace Muse {
namespace Ai {

struct MusicalEvent {
    int pitchMidi;        // MIDI pitch (0-127)
    long long startTimeTicks; // Raw start time in score ticks (as per mu::Fraction::ticks())
    long long durationTicks;  // Raw duration in score ticks (as per mu::Fraction::ticks())
    int voice;            // Voice number (0-3 typically)
    int staffIndex;       // 0-based staff index in the score

    // Placeholder for future: double startTimeBeats;
    // Placeholder for future: double durationBeats;
    // Placeholder for future: std::string noteName; // e.g., "C#4"
    // Placeholder for future: std::vector<std::string> articulations;
};

struct MusicSelectionContext {
    std::vector<MusicalEvent> events;
    std::string debugInfo;      // To store any messages/errors during extraction, or summary.

    // Placeholders for future context information:
    // int keySignatureFifths;   // e.g., 0 for C, 1 for G, -1 for F
    // double tempoBPM;
    // int timeSigNumerator;
    // int timeSigDenominator;
    // bool isSelectionContiguous;
};

} // namespace Ai
} // namespace Muse
