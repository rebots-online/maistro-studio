#pragma once

#include <QObject>
#include <QString>

#include "AiMusicDataConverter.h"
#include "AiMusicTypes.h" // New music types

// Forward declarations for notation classes to avoid heavy includes in header
namespace mu {
    namespace notation {
        class INotation;
        class INotationSelection;
        // Forward declare other specific types if their pointers/references are stored in AiController
    }
    namespace engraving {
        class Note; // If we were to store Note* etc.
    }
}


namespace Muse {
namespace Ai {

class IAiMusicGenerator; // Forward declaration

class AiController : public QObject {
    Q_OBJECT
public:
    explicit AiController(QObject *parent = nullptr);
    ~AiController();

    Q_INVOKABLE void generateMusic(const QString& style, int tempo, const QString& key, int length);
    Q_INVOKABLE void cocreateWithSelection(const QString& style, int tempo, const QString& key); // Signature changed

signals:
    void generationStarted(const QString& statusMessage);
    void generationComplete(bool success, const QString& resultMessageOrError);
    void musicDataReadyForPlayback(const QString& descriptionOfData);

private:
    MusicSelectionContext extractMusicSelection(); // New private helper

    IAiMusicGenerator* m_musicGenerator;
    AiMusicDataConverter* m_dataConverter;
};

} // namespace Ai
} // namespace Muse
