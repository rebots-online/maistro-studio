#pragma once

#include "IAiMusicGenerator.h" // For std::vector<char> or other AI data type
#include "engraving/dom/score.h"    // Actual path to score.h

// Placeholder for playback data structures - update if incorrect
// #include "src/playback/PlaybackDataTypes.h" // Assuming a type like PlaybackStream exists

#include <vector>
#include <string> // Required by IAiMusicGenerator.h, good to have explicitly

// Forward declarations for types not fully included or hypothetical
namespace Muse {
namespace Playback {
// Hypothetical playback stream type, forward declare for now
class PlaybackStream;
}
}

namespace Muse {
namespace Ai {

class AiMusicDataConverter {
public:
    AiMusicDataConverter();
    ~AiMusicDataConverter();

    // Converts AI data to an engraving Score object
    Engraving::Score* convertToScore(const std::vector<char>& aiData);

    // Converts AI data to a playback stream
    // Using void* as a generic placeholder until PlaybackStream is defined
    void* convertToPlaybackStream(const std::vector<char>& aiData);
    // Alternative if we forward declare:
    // Playback::PlaybackStream* convertToPlaybackStream(const std::vector<char>& aiData);

private:
    // Private members if any
};

} // namespace Ai
} // namespace Muse
