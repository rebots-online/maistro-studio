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
    Q_INVOKABLE void cocreateWithSelection(const QString& style, int tempo, const QString& key);
    Q_INVOKABLE void initiateStyleAnalysisFileSelection();
    Q_INVOKABLE void processStyleAnalysisFile(const QString& filePath);
    Q_INVOKABLE void initiateCustomModelSelection();
    Q_INVOKABLE void setCustomModelPath(const QString& modelPath);
    Q_INVOKABLE void receiveMidiNoteForFeedback(int pitch, int velocity); // New

signals:
    void generationStarted(const QString& statusMessage);
    void generationComplete(bool success, const QString& resultMessageOrError);
    void musicDataReadyForPlayback(const QString& descriptionOfData);
    void styleAnalysisFileSelectionRequested(const QString& message);
    void customModelPathChanged(const QString& newPath);
    void pitchFeedbackUpdate(const QString& feedbackMessage); // New

private:
    MusicSelectionContext extractMusicSelection();

    IAiMusicGenerator* m_musicGenerator;
    AiMusicDataConverter* m_dataConverter;
    QString m_customModelPath;
    int m_targetMidiNote = 60; // Middle C, C++11 in-class initialization
};

} // namespace Ai
} // namespace Muse
