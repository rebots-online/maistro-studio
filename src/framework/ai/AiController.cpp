#include "AiController.h"
#include "AiMusicTypes.h" // Include our new types

// Notation and Engraving headers
#include "notation/inotation.h"
#include "notation/inotationselection.h"
#include "notation/inotationinteraction.h" // For INotationInteraction to get selection
#include "engraving/dom/note.h"
#include "engraving/dom/chord.h"        // Note is part of a Chord
#include "engraving/dom/score.h"        // To get score context if needed (e.g. time signatures, tempo)
#include "engraving/dom/segment.h"      // Chord is in a Segment. Note::tick() is absolute.
#include "engraving/dom/fraction.h"     // For tick types
#include "global/globalcontext.h"       // Potential way to get current notation
#include "project/iproject.h"           // Project might give current notation
#include "multiinstances/iprojectprovider.h" // To get current project

#include <QDebug>
#include <QTimer>
#include <vector> // For std::vector<char> dummy data


namespace Muse {
namespace Ai {

// Helper function to attempt to get the current INotationSelection
// This is a placeholder for a more robust mechanism.
mu::notation::INotationSelectionPtr getCurrentNotationSelection() {
    // Attempt to get the project provider, then the current project, then its notation, then interaction, then selection
    auto projectProvider = muse::globalContext()->ioc()->tryResolve<mu::multiinstances::IProjectProvider>();
    if (projectProvider) {
        mu::project::IProjectPtr currentProject = projectProvider->currentProject();
        if (currentProject) {
            mu::notation::INotationPtr notation = currentProject->notation();
            if (notation && notation->interaction()) {
                return notation->interaction()->selection();
            } else if (notation) {
                 qWarning() << "AiController: Got INotation for current project, but INotationInteraction is null.";
            } else {
                qWarning() << "AiController: Current project's INotation is null.";
            }
        } else {
            qWarning() << "AiController: No current project found via IProjectProvider.";
        }
    } else {
        qWarning() << "AiController: IProjectProvider could not be resolved from global context.";
    }
    qWarning() << "AiController: Could not obtain INotationSelection from current project context. Returning nullptr.";
    return nullptr;
}

MusicSelectionContext AiController::extractMusicSelection() {
    MusicSelectionContext context;
    context.debugInfo = "Selection extraction initiated.";

    mu::notation::INotationSelectionPtr selectionInterface = getCurrentNotationSelection();

    if (!selectionInterface) {
        context.debugInfo = "Error: Could not obtain INotationSelection interface. Cannot extract notes.";
        qWarning() << "AiController::extractMusicSelection:" << QString::fromStdString(context.debugInfo);
        // To allow testing the rest of the flow without a live selection:
        // MusicalEvent dummyEvent1 = {60, 0, 1920, 0, 0};
        // context.events.push_back(dummyEvent1);
        // context.debugInfo += " Added 1 dummy note for testing.";
        return context;
    }

    if (selectionInterface->isNone()) {
        context.debugInfo = "Selection is 'None' (no elements selected). No notes to extract.";
        qWarning() << "AiController::extractMusicSelection:" << QString::fromStdString(context.debugInfo);
        return context;
    }

    std::vector<mu::engraving::Note*> selectedNotes = selectionInterface->notes();

    if (selectedNotes.empty()) {
        context.debugInfo = "Selection does not contain any notes directly (e.g., range selection over rests or other elements).";
        qWarning() << "AiController::extractMusicSelection:" << QString::fromStdString(context.debugInfo);
        return context;
    }

    qDebug() << "AiController::extractMusicSelection: Found" << selectedNotes.size() << "notes in the selection.";

    for (mu::engraving::Note* note : selectedNotes) {
        if (!note) {
            qWarning() << "AiController::extractMusicSelection: Encountered a null Note* in selectedNotes. Skipping.";
            continue;
        }
        mu::engraving::Chord* chord = note->chord();
        if (!chord) {
            qWarning() << "AiController::extractMusicSelection: Note with pitch" << note->pitch() << "has no parent Chord object. Skipping.";
            continue;
        }

        MusicalEvent event;
        event.pitchMidi = note->pitch();
        event.voice = chord->voice();
        event.staffIndex = chord->staffIdx();
        event.startTimeTicks = note->tick().ticks(); // Absolute tick from score start
        event.durationTicks = note->playTicksFraction().ticks(); // Actual sounding duration

        context.events.push_back(event);
    }

    if (!context.events.empty()) {
        context.debugInfo = "Successfully extracted " + std::to_string(context.events.size()) + " musical events.";
    } else if (!selectedNotes.empty()) {
        context.debugInfo = "Warning: Notes were found in selection, but no events extracted (e.g., notes lacked parent chord).";
    }

    // TODO: Extract KeySignature and Tempo from score context for the selection
    // This would involve getting the mu::engraving::Score* (e.g. from note->score() or selectionInterface->element()->score())
    // Then using the first event's staffIndex and startTimeTicks to query:
    // score->staff(firstEvent.staffIndex)->keySigEvent(mu::Fraction(firstEvent.startTimeTicks)).key().fifths();
    // score->tempo(mu::Fraction(firstEvent.startTimeTicks)).toDouble();
    // These would populate context.keySignatureFifths and context.tempoBPM.

    return context;
}

AiController::AiController(QObject *parent) : QObject(parent),
    m_musicGenerator(nullptr),
    m_dataConverter(new AiMusicDataConverter()),
    m_customModelPath("None selected") // Initialize custom model path
{
    qDebug() << "AiController created. AiMusicDataConverter instantiated. Custom model path initial: " << m_customModelPath;
}

AiController::~AiController() {
    qDebug() << "AiController destroyed.";
    delete m_dataConverter; // Clean up m_dataConverter
    // delete m_musicGenerator; // Clean up m_musicGenerator if it were instantiated
}

void AiController::generateMusic(const QString& style, int tempo, const QString& key, int length) {
    qDebug() << "AiController::generateMusic received request - Style:" << style
             << "Tempo:" << tempo
             << "Key:" << key
             << "Length:" << length << "measures";

    QString statusMessage = QString("Generation process started for Style: %1, Tempo: %2 BPM, Key: %3, Length: %4 measures...")
                                .arg(style)
                                .arg(tempo)
                                .arg(key)
                                .arg(length);
    emit generationStarted(statusMessage);

    // Simulate AI model processing and data conversion
    QTimer::singleShot(1500, this, [this, style, key]() {
        bool success = true; // Assume success for now
        QString resultMessage;

        if (success) {
            resultMessage = QString("Placeholder: Music generation finished for %1 in %2 key.").arg(style).arg(key);
            emit generationComplete(true, resultMessage);

            // Create dummy AI data
            std::vector<char> dummyAiData = {'D', 'U', 'M', 'M', 'Y', ' ', 'A', 'I', ' ', 'D', 'A', 'T', 'A'};
            // In a real scenario, this data would come from m_musicGenerator
            // if (m_musicGenerator) { dummyAiData = m_musicGenerator->getGeneratedMusicData(); }


            // Call converter methods
            qDebug() << "AiController: Calling AiMusicDataConverter methods...";
            Muse::Engraving::Score* scoreData = m_dataConverter->convertToScore(dummyAiData);
            void* playbackData = m_dataConverter->convertToPlaybackStream(dummyAiData); // Assuming void* as per previous setup

            qDebug() << "AiController: Conversion results - Score data at:"
                     << (scoreData ? QString("0x%1").arg(QString::number((quintptr)scoreData, 16)) : "nullptr")
                     << "Playback data at:"
                     << (playbackData ? QString("0x%1").arg(QString::number((quintptr)playbackData, 16)) : "nullptr");

            QString playbackReadyMessage = QString("Processed data for playback. Score: %1, Playback: %2.")
                                               .arg(scoreData ? QString("0x%1").arg(QString::number((quintptr)scoreData, 16)) : "nullptr")
                                               .arg(playbackData ? QString("0x%1").arg(QString::number((quintptr)playbackData, 16)) : "nullptr");
            emit musicDataReadyForPlayback(playbackReadyMessage);

        } else {
            resultMessage = "Placeholder: AI model failed to generate music.";
            emit generationComplete(false, resultMessage);
        }
    });
}

void AiController::initiateCustomModelSelection() {
    qDebug() << "AiController::initiateCustomModelSelection called - simulating model selection.";
    // In a real application, this would trigger a QFileDialog or similar.
    // The dialog would then call setCustomModelPath upon successful selection.
    QString dummyModelPath = "/mnt/models/custom_ai_model_v3_final.gguf"; // Example dummy path
    // Simulate a slight delay before "selection" completes and calls setCustomModelPath
    QTimer::singleShot(100, this, [this, dummyModelPath]() {
        setCustomModelPath(dummyModelPath);
    });
}

void AiController::setCustomModelPath(const QString& modelPath) {
    qDebug() << "AiController::setCustomModelPath called with path:" << modelPath;
    if (m_customModelPath != modelPath) {
        // TODO: SECURITY - When loading the model from m_customModelPath, ensure robust path validation
        // (e.g., check for directory traversal, ensure it's a valid/expected filetype or directory structure,
        // check permissions, and consider sandboxing model loading/execution if possible).
        // Also, consider privacy implications if the path itself is sensitive and gets logged extensively.
        m_customModelPath = modelPath;
        emit customModelPathChanged(m_customModelPath); // Notify QML of the change
        // Provide general feedback via an existing signal
        emit generationComplete(true, QString("Custom AI model path updated to: %1").arg(m_customModelPath));
        qDebug() << "Custom model path set to:" << m_customModelPath;
    } else {
        qDebug() << "Custom model path is already set to:" << modelPath;
        // Still emit generationComplete so UI can show a message if desired
        emit generationComplete(true, QString("Custom AI model path remains: %1").arg(m_customModelPath));
    }
}

void AiController::initiateStyleAnalysisFileSelection() {
    qDebug() << "AiController::initiateStyleAnalysisFileSelection called - simulating file selection.";
    QString dummyFilePath = "/path/to/placeholder_style_file.musicxml"; // Define dummyFilePath

    // This is where C++ logic to open a native file dialog would be triggered.
    // For example, using QFileDialog.
    // After a file is selected, another signal might be emitted with the file path,
    // or the path might be processed directly here to start analysis.
    emit styleAnalysisFileSelectionRequested(QString("Simulated file selection: %1. Passing to processing.").arg(dummyFilePath));

    // Directly call the processing method with the dummy path
    processStyleAnalysisFile(dummyFilePath);
}

void AiController::processStyleAnalysisFile(const QString& filePath) {
    // TODO: SECURITY - When implementing file parsing for style analysis (e.g., MusicXML, MIDI),
    // use robust libraries designed to handle potentially malformed or malicious files.
    // Implement thorough input validation, size checks, and resource limits during parsing
    // and processing to prevent denial-of-service, crashes, or other exploits.
    // Consider sandboxing the parsing process if possible.
    // Also, be mindful of privacy if file content is logged or transmitted.
    qDebug() << "AiController::processStyleAnalysisFile called with path:" << filePath;

    emit generationStarted(QString("Processing style from file: %1...").arg(filePath));

    // TODO: Here you would eventually:
    // 1. Validate the file path (security check: ensure it's a safe path, not pointing to system files etc.).
    // 2. Read and parse the music file (using secure parsers).
    // 3. Perform AI style analysis on the parsed data.
    // 4. Store the resulting style profile securely.

    // Simulate processing delay and completion
    QTimer::singleShot(1000, this, [this, filePath]() { // Shorter delay for this simulation
        qDebug() << "Placeholder: Style analysis for file" << filePath << "would happen here.";
        emit generationComplete(true, QString("Placeholder: Style analysis of %1 finished. Style profile would be ready.").arg(filePath));
    });
}

void AiController::receiveMidiNoteForFeedback(int pitch, int velocity) {
    qDebug() << "AiController::receiveMidiNoteForFeedback called with pitch:" << pitch << "velocity:" << velocity;

    QString feedbackMessage;
    if (pitch == m_targetMidiNote) {
        feedbackMessage = QString("Correct! (Note: %1, Velocity: %2)").arg(pitch).arg(velocity);
    } else if (pitch < m_targetMidiNote) {
        feedbackMessage = QString("Too Low. (Played: %1, Target: %2, Velocity: %3)").arg(pitch).arg(m_targetMidiNote).arg(velocity);
    } else { // pitch > m_targetMidiNote
        feedbackMessage = QString("Too High. (Played: %1, Target: %2, Velocity: %3)").arg(pitch).arg(m_targetMidiNote).arg(velocity);
    }
    emit pitchFeedbackUpdate(feedbackMessage);
}

} // namespace Ai
} // namespace Muse

void AiController::cocreateWithSelection(const QString& style, int tempo, const QString& key) { // Signature updated
    MusicSelectionContext selectionCtx = extractMusicSelection();

    qDebug() << "AiController::cocreateWithSelection called. Debug from extraction:" << QString::fromStdString(selectionCtx.debugInfo);

    if (selectionCtx.events.empty()) {
        qWarning() << "AiController::cocreateWithSelection: No musical events extracted from selection. Aborting co-creation.";
        // Update status message to reflect failure due to no selection
        QString failMessage = QString("Co-creation failed: No musical selection found or selection is empty. (%1)").arg(QString::fromStdString(selectionCtx.debugInfo));
        emit generationComplete(false, failMessage); // Use generationComplete to signal failure to UI
        return;
    }

    qDebug() << "AiController::cocreateWithSelection proceeding with" << selectionCtx.events.size() << "events. Style:" << style
             << "Tempo:" << tempo
             << "Key:" << key;

    QString statusMessage = QString("Co-creation started for %1 selected event(s) with style %2, %3 BPM, %4...")
                               .arg(selectionCtx.events.size())
                               .arg(style)
                               .arg(tempo)
                               .arg(key);
    emit generationStarted(statusMessage);

    // Simulate AI processing and data conversion for co-creation
    QTimer::singleShot(1800, this, [this, style, key, selectionCtx]() { // Capture selectionCtx by value (or relevant parts)
        bool success = true; // Assume success for now
        QString resultMessage;

        if (success) {
            resultMessage = QString("Placeholder: Co-creation finished for %1 event(s) with style %2.").arg(selectionCtx.events.size()).arg(style);
            emit generationComplete(true, resultMessage);

            // Create dummy co-created AI data based on the selection context (conceptually)
            // For example, concatenate data or use it as a seed.
            std::vector<char> dummyCocreatedData;
            for (const auto& event : selectionCtx.events) {
                // Simple transformation for dummy data
                dummyCocreatedData.push_back(static_cast<char>(event.pitchMidi % 128));
            }
            if (dummyCocreatedData.empty()) {
                dummyCocreatedData = {'E', 'M', 'P', 'T', 'Y', '_', 'S', 'E', 'L'};
            }


            qDebug() << "AiController (co-create): Calling AiMusicDataConverter methods with processed dummy data.";
            auto scoreData = m_dataConverter->convertToScore(dummyCocreatedData);
            auto playbackData = m_dataConverter->convertToPlaybackStream(dummyCocreatedData);

            qDebug() << "AiController (co-create): Conversion results - Score data at:"
                     << (scoreData ? QString("0x%1").arg(QString::number((quintptr)scoreData, 16)) : "nullptr")
                     << "Playback data at:"
                     << (playbackData ? QString("0x%1").arg(QString::number((quintptr)playbackData, 16)) : "nullptr");

            QString playbackReadyMessage = QString("Processed co-created data for playback. Score: %1, Playback: %2.")
                                               .arg(scoreData ? QString("0x%1").arg(QString::number((quintptr)scoreData, 16)) : "nullptr")
                                               .arg(playbackData ? QString("0x%1").arg(QString::number((quintptr)playbackData, 16)) : "nullptr");
            emit musicDataReadyForPlayback(playbackReadyMessage);

        } else {
            resultMessage = QString("Placeholder: AI model failed to co-create with the selection (%1 events).").arg(selectionCtx.events.size());
            emit generationComplete(false, resultMessage);
        }
    });
}
