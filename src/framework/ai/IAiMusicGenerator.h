#pragma once

#include <string>
#include <vector>

namespace Muse {
namespace Ai {

class IAiMusicGenerator {
public:
    virtual ~IAiMusicGenerator() = default;

    virtual bool setParameters(
        const std::string& style,
        int tempo,
        const std::string& key,
        int length) = 0;

    virtual bool generateMusic() = 0;
    // Using std::vector<char> as a placeholder for raw music data
    virtual std::vector<char> getGeneratedMusicData() = 0; 
};

} // namespace Ai
} // namespace Muse
