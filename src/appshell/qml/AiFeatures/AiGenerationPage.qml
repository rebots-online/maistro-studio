import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Muse.UiComponents 1.0 // For Page
import Muse.Ai 1.0           // For AiController

Page {
    id: aiGenerationPage
    objectName: "aiGenerationPage"
    title: qsTr("AI Music Generation")
    accessibleName: title

    // Instantiate AiController
    AiController {
        id: aiController
        objectName: "aiController"

        onGenerationStarted: function(statusMessage) {
            console.log("QML received generationStarted:", statusMessage);
            statusTextArea.text = statusMessage;
            generateButton.enabled = false;
            cocreateButton.enabled = false; // Also disable co-create button
        }

        onGenerationComplete: function(success, resultMessageOrError) {
            console.log("QML received generationComplete - Success:", success, "Message:", resultMessageOrError);
            // statusTextArea.text = resultMessageOrError; // This message is about generic generation.
                                                        // musicDataReadyForPlayback will provide a more specific one for playback.
            if (!success) {
                statusTextArea.text = qsTr("Operation Failed: %1").arg(resultMessageOrError);
                console.error("Operation failed:", resultMessageOrError);
            } else {
                statusTextArea.text = qsTr("Operation Successful: %1").arg(resultMessageOrError);
                console.log("Operation successful:", resultMessageOrError);
            }
            generateButton.enabled = true;
            cocreateButton.enabled = true; // Re-enable co-create button
        }

        onMusicDataReadyForPlayback: function(descriptionOfData) {
            console.log("QML received musicDataReadyForPlayback:", descriptionOfData);
            statusTextArea.text += "\n" + qsTr("Music data ready: %1").arg(descriptionOfData);
        }

        onStyleAnalysisFileSelectionRequested: function(message) {
            console.log("QML received styleAnalysisFileSelectionRequested:", message);
            statusTextArea.text = message;
        }

        onCustomModelPathChanged: function(newPath) {
            console.log("QML received customModelPathChanged:", newPath);
            customModelPathTextField.text = newPath;
            // statusTextArea.text = qsTr("Custom model path updated: %1").arg(newPath); // This is now handled by generationComplete signal from C++
        }

        onPitchFeedbackUpdate: function(feedbackMessage) {
            console.log("QML received pitchFeedbackUpdate:", feedbackMessage);
            pitchFeedbackLabel.text = feedbackMessage;
        }
    }

    ScrollView {
        anchors.fill: parent
        clip: true
        contentWidth: parent.width

        ColumnLayout {
            id: mainLayout
            objectName: "mainLayout"
            width: parent.width
            anchors.margins: 20
            spacing: 15

            GridLayout {
                id: paramsGrid
                // ... (parameter controls as before)
                objectName: "paramsGrid"
                Layout.fillWidth: true
                columnSpacing: 10
                rowSpacing: 10
                columns: 2

                // --- Style ---
                Label { id: styleLabel; text: qsTr("Style:"); Layout.alignment: Qt.AlignRight | Qt.AlignVCenter }
                ComboBox {
                    id: styleComboBox; objectName: "styleComboBox"
                    model: ["Classical", "Jazz", "Rock", "Electronic", "Blues", "Folk", "Ambient", "Pop"]
                    currentIndex: 0; Layout.fillWidth: true; accessibleName: qsTr("Select musical style")
                }

                // --- Tempo ---
                Label { id: tempoLabel; text: qsTr("Tempo (BPM):"); Layout.alignment: Qt.AlignRight | Qt.AlignVCenter }
                SpinBox {
                    id: tempoSpinBox; objectName: "tempoSpinBox"
                    from: 40; to: 240; value: 120; stepSize: 1
                    Layout.fillWidth: true; accessibleName: qsTr("Set tempo in beats per minute")
                }

                // --- Key Signature ---
                Label { id: keyLabel; text: qsTr("Key:"); Layout.alignment: Qt.AlignRight | Qt.AlignVCenter }
                ComboBox {
                    id: keyComboBox; objectName: "keyComboBox"
                    model: [
                        "C Major", "G Major", "D Major", "A Major", "E Major", "B Major", "F# Major", "C# Major",
                        "F Major", "Bb Major", "Eb Major", "Ab Major", "Db Major", "Gb Major", "Cb Major",
                        "A Minor", "E Minor", "B Minor", "F# Minor", "C# Minor", "G# Minor", "D# Minor", "A# Minor",
                        "D Minor", "G Minor", "C Minor", "F Minor", "Bb Minor", "Eb Minor", "Ab Minor"
                    ]
                    currentIndex: 0; Layout.fillWidth: true; accessibleName: qsTr("Select key signature")
                }

                // --- Length ---
                Label { id: lengthLabel; text: qsTr("Length (measures):"); Layout.alignment: Qt.AlignRight | Qt.AlignVCenter }
                SpinBox {
                    id: lengthSpinBox; objectName: "lengthSpinBox"
                    from: 8; to: 128; value: 32; stepSize: 1
                    Layout.fillWidth: true; accessibleName: qsTr("Set desired length in measures (for full generation)")
                }

                // --- Custom Model Path Display ---
                Label {
                    id: customModelPathLabel
                    text: qsTr("Custom Model:")
                    Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                }
                TextField {
                    id: customModelPathTextField
                    objectName: "customModelPathTextField"
                    text: qsTr("None selected") // Initial text
                    readOnly: true
                    Layout.fillWidth: true
                    placeholderText: qsTr("Path to custom AI model will appear here")
                    accessibleName: qsTr("Path to the selected custom AI model")
                }
            } // End of GridLayout

            // --- Action Buttons Row ---
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 20

                // --- Generate Button ---
                Button {
                    id: generateButton
                    objectName: "generateButton"
                    text: qsTr("Generate Music")
                    font.pixelSize: 16
                    enabled: true
                    accessibleName: qsTr("Generate new music with selected parameters")

                    onClicked: {
                        console.log("Generate Music button clicked - calling AiController.generateMusic()");
                        aiController.generateMusic(styleComboBox.currentText, tempoSpinBox.value, keyComboBox.currentText, lengthSpinBox.value);
                    }
                }

                // --- Co-create Button ---
                Button {
                    id: cocreateButton
                    objectName: "cocreateButton"
                    text: qsTr("Co-create with Selection")
                    font.pixelSize: 16
                    enabled: true
                    accessibleName: qsTr("Co-create music based on the current score selection")

                    onClicked: {
                        console.log("Co-create with Current Selection button clicked - calling AiController.cocreateWithSelection()");

                        // var selectionPlaceholder = "Placeholder: User's musical selection from score"; // No longer passed

                        var style = styleComboBox.currentText;
                        var tempo = tempoSpinBox.value;
                        var key = keyComboBox.currentText;
                        // Length parameter is not part of cocreateWithSelection in AiController.
                        // The C++ side now handles selection extraction internally.

                        aiController.cocreateWithSelection(style, tempo, key); // Updated call
                    }
                }

                // --- Load File for Style Analysis Button ---
                Button {
                    id: loadFileStyleButton
                    objectName: "loadFileStyleButton"
                    text: qsTr("Load File for Style Analysis")
                    // Layout.alignment: Qt.AlignHCenter // Will be centered by RowLayout if it's the only item or if RowLayout has appropriate settings
                    font.pixelSize: 16
                    enabled: true // For now, always enabled. Could be tied to !generateButton.enabled
                    accessibleName: qsTr("Load a music file for AI style analysis")

                    onClicked: {
                        console.log("Load File for Style Analysis button clicked - calling AiController");
                        // statusTextArea.text = "Initiating file selection for style analysis..."; // Direct update removed, will be handled by signal
                        aiController.initiateStyleAnalysisFileSelection();
                    }
                }

                // --- Select Custom Model Button ---
                Button {
                    id: selectCustomModelButton
                    objectName: "selectCustomModelButton"
                    text: qsTr("Select Custom Model")
                    font.pixelSize: 16 // Consistent with other buttons
                    enabled: true // Manage as needed, similar to other buttons
                    accessibleName: qsTr("Select a custom AI model file or directory")

                    onClicked: {
                        console.log("Select Custom Model button clicked - calling AiController");
                        aiController.initiateCustomModelSelection();
                    }
                }
            } // End of RowLayout for buttons


            // --- Status/Preview Area ---
            TextArea {
                id: statusTextArea
                objectName: "statusTextArea"
                Layout.fillWidth: true
                Layout.topMargin: 10 // Add some space above the status area
                Layout.preferredHeight: 120
                text: qsTr("Ready.") // Simplified initial message
                readOnly: true
                wrapMode: Text.WordWrap
                // font.italic: true // Italicizing the whole status might be too much
                background: Rectangle {
                    color: theme.isDark ? "#252525" : "#f0f0f0"
                    border.color: theme.isDark ? "#333333" : "#cccccc"
                    radius: 3
                }
                accessibleName: qsTr("Status and output area for AI music generation")
            }

            // --- Pitch Feedback Section ---
            Label {
                id: pitchFeedbackSectionLabel
                text: qsTr("Music Education: Pitch Feedback")
                font.bold: true
                Layout.topMargin: 20 // Add some space before this new section
                Layout.columnSpan: 2 // If still in a GridLayout context, otherwise not needed in ColumnLayout
            }

            Label {
                id: pitchFeedbackLabel
                objectName: "pitchFeedbackLabel"
                text: qsTr("Pitch Feedback: Play a note (Middle C is target %1)").arg(aiController.m_targetMidiNote || 60) // Accessing C++ member directly not ideal, illustrative
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
                Layout.columnSpan: 2
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 10
                Layout.columnSpan: 2

                Button {
                    text: qsTr("Test Low (B59)")
                    onClicked: aiController.receiveMidiNoteForFeedback(59, 100)
                    accessibleName: qsTr("Test pitch feedback with note B natural below Middle C")
                }
                Button {
                    text: qsTr("Test Target (C60)")
                    onClicked: aiController.receiveMidiNoteForFeedback(60, 100)
                    accessibleName: qsTr("Test pitch feedback with Middle C")
                }
                Button {
                    text: qsTr("Test High (C#61)")
                    onClicked: aiController.receiveMidiNoteForFeedback(61, 100)
                    accessibleName: qsTr("Test pitch feedback with note C sharp above Middle C")
                }
            }
        } // End of ColumnLayout
    } // End of ScrollView
}
