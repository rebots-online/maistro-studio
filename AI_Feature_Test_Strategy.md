# Test Strategy for AI Features (mAIstro)

This document outlines the testing strategy for the newly added AI features in mAIstro, focusing on the `AiController` C++ class and the `AiGenerationPage.qml` UI.

**I. General Approach:**

*   **Combined Testing:** Employ a mix of C++ unit tests (using the Qt Test framework or a similar C++ testing tool) and manual QML/UI interaction tests.
*   **Automated UI Tests:** Where feasible, QML-level automated tests (e.g., using Qt Test's QML capabilities) should be considered for repetitive UI checks.
*   **Signal/Slot Verification:** A key aspect will be verifying the correct emission of signals from `AiController` and their handling by the QML UI, ensuring data flows correctly and UI updates as expected.
*   **Mocking/Simulation:** For initial C++ tests, especially where dependencies like a full notation engine or actual AI models are not yet integrated, mocking or simulating these dependencies will be crucial.
*   **Iterative Testing:** As backend functionality (actual AI processing, file handling, selection extraction) becomes more complete, tests will need to be expanded and refined.

**II. Unit Testing (C++) - `AiController` Focus:**

These tests should ideally be created within the existing test structure of the project (e.g., in `src/framework/ai/tests/`).

*   **`generateMusic(const QString& style, int tempo, const QString& key, int length)`:**
    *   **Valid Inputs:** Test with a set of typical valid parameters (e.g., "Classical", 120, "C Major", 32).
        *   Verify `generationStarted` is emitted once with a status message reflecting these parameters.
        *   Verify `generationComplete(true, ...)` is emitted once with a success message.
        *   Verify `musicDataReadyForPlayback` is emitted once with the expected (simulated) data description.
    *   **Edge Cases for Parameters:**
        *   Tempo: Min/max values (e.g., 40, 240).
        *   Length: Min/max values (e.g., 8, 128).
        *   Style/Key: Empty strings (if allowed by UI, though ComboBoxes prevent this), very long strings (if input was free-text).
    *   **Signal Emission Order:** Ensure signals are emitted in the correct sequence: `generationStarted` -> `generationComplete` -> `musicDataReadyForPlayback`.

*   **`cocreateWithSelection(const QString& style, int tempo, const QString& key)`:**
    *   (Assumes `extractMusicSelection` is either fully working and mockable, or this test is deferred/adapted).
    *   **Mocked `MusicSelectionContext`:**
        *   **Empty Selection:** Simulate `extractMusicSelection` returning a context with an empty `events` vector. Verify `generationComplete(false, ...)` is emitted with a "No selection" or similar error, and no `musicDataReadyForPlayback`.
        *   **Single Note Selection:** Simulate one `MusicalEvent`. Verify `generationStarted` (with "1 selected event(s)"), `generationComplete(true, ...)`, and `musicDataReadyForPlayback` are emitted with co-creation specific messages.
        *   **Multiple Notes Selection:** Simulate multiple `MusicalEvent`s. Verify signals as above, with correct event count in messages.
    *   **Parameter Handling:** Test with valid style, tempo, key parameters.

*   **`extractMusicSelection()`:**
    *   (This is the most complex to unit test without a live notation environment or extensive mocking of notation interfaces).
    *   **Mock `INotationSelection`:**
        *   Simulate `selectionInterface->notes()` returning an empty vector. Verify `MusicSelectionContext.events` is empty and `debugInfo` indicates no notes.
        *   Simulate `selectionInterface->notes()` returning a vector with one mock `mu::engraving::Note` (and its parent `Chord`). Verify correct extraction of pitch, start/duration ticks, voice, and staff index into the `MusicalEvent`.
        *   Simulate multiple mock notes with varying properties.
        *   Simulate `selectionInterface->isNone()` returning true.
        *   Simulate `getCurrentNotationSelection()` returning `nullptr`.
    *   **Placeholder Test:** If full mocking is too complex initially, a test that calls it and checks `debugInfo` for the "Could not obtain INotationSelection" message can be a starting point.

*   **`initiateStyleAnalysisFileSelection()` & `processStyleAnalysisFile(const QString& filePath)`:**
    *   **`initiateStyleAnalysisFileSelection()`:**
        *   Verify `styleAnalysisFileSelectionRequested` is emitted with the simulated file path message.
        *   Verify it subsequently calls `processStyleAnalysisFile` (e.g., by mocking `processStyleAnalysisFile` or checking for its side-effects like subsequent signal emissions).
    *   **`processStyleAnalysisFile(filePath)`:**
        *   Call with a test file path string.
        *   Verify `generationStarted` is emitted with a message containing the file path.
        *   Verify `generationComplete(true, ...)` is emitted with a message indicating analysis of that file is done.

*   **`initiateCustomModelSelection()` & `setCustomModelPath(const QString& modelPath)`:**
    *   **`initiateCustomModelSelection()`:**
        *   Verify it calls `setCustomModelPath` (e.g., by checking for `customModelPathChanged` emission).
    *   **`setCustomModelPath(modelPath)`:**
        *   Call with a test path. Verify `m_customModelPath` (if accessible via a getter or friend class) is updated.
        *   Verify `customModelPathChanged` signal is emitted with the correct path.
        *   Verify `generationComplete(true, ...)` is emitted with a status update.
        *   Call with the same path again; verify `customModelPathChanged` is not emitted again, but `generationComplete` might still be (as per current implementation).

*   **`receiveMidiNoteForFeedback(int pitch, int velocity)`:**
    *   Test with `pitch == m_targetMidiNote` (default 60). Verify `pitchFeedbackUpdate` emits "Correct!".
    *   Test with `pitch < m_targetMidiNote` (e.g., 59). Verify "Too Low." message.
    *   Test with `pitch > m_targetMidiNote` (e.g., 61). Verify "Too High." message.
    *   Verify velocity is included in the message.

**III. UI Testing (QML) - `AiGenerationPage.qml` Focus:**

These tests will initially be manual, with potential for automation later.

*   **Parameter Controls:**
    *   Visually verify `styleComboBox`, `tempoSpinBox`, `keyComboBox`, `lengthSpinBox` are present.
    *   Check default values (e.g., tempo 120, length 32, first style/key selected).
    *   Verify ranges for SpinBoxes (tempo 40-240, length 8-128).
    *   Verify model content for ComboBoxes (expected styles and keys are listed).

*   **"Generate Music" Button:**
    *   Click button with default parameters.
    *   Observe `statusTextArea`:
        *   Shows "Generation process started..." message.
        *   Button becomes disabled.
        *   After a delay, button becomes enabled.
        *   Shows "Operation Successful: Placeholder: Music generation finished..." message.
        *   Shows "Music data ready: Processed data for playback..." message.
    *   Change parameters and repeat; verify new parameters are used in the status messages.

*   **"Co-create with Current Selection" Button:**
    *   Click button.
    *   Observe `statusTextArea`:
        *   If no score/selection active (simulated by `extractMusicSelection` returning empty): Shows "Co-creation failed: No musical selection..." message. Button should re-enable quickly.
        *   If selection is simulated as present: Shows "Co-creation started for X selected event(s)...". Button becomes disabled.
        *   After a delay, button becomes enabled.
        *   Shows "Operation Successful: Placeholder: Co-creation finished..." message.
        *   Shows "Music data ready: Processed co-created data..." message.

*   **"Load File for Style Analysis" Button:**
    *   Click button.
    *   Observe `statusTextArea`:
        *   Shows "Simulated file selection: /path/to/placeholder_style_file.musicxml. Auto-processing...".
        *   Then, "Processing style from file: /path/to/placeholder_style_file.musicxml...". Button may disable/re-enable.
        *   After a delay, "Operation Successful: Placeholder: Style analysis of /path/to/placeholder_style_file.musicxml finished...".

*   **Custom Model UI:**
    *   Verify `Label` "Custom Model:" and the read-only `TextField` (initially "None selected") are present.
    *   Click "Select Custom Model" button.
    *   Observe `customModelPathTextField` updates to "/mnt/models/custom_ai_model_v3_final.gguf".
    *   Observe `statusTextArea` updates to "Custom AI model path updated to: /mnt/models/custom_ai_model_v3_final.gguf".

*   **Pitch Feedback UI:**
    *   Verify `Label` "Music Education: Pitch Feedback" and the feedback label (`pitchFeedbackLabel`) are present.
    *   Verify initial text of `pitchFeedbackLabel` mentions the target note (Middle C / 60).
    *   Click "Test Low (B59)" button. Verify `pitchFeedbackLabel` shows "Too Low. (Played: 59, Target: 60...)".
    *   Click "Test Target (C60)" button. Verify `pitchFeedbackLabel` shows "Correct! (Note: 60...)".
    *   Click "Test High (C#61)" button. Verify `pitchFeedbackLabel` shows "Too High. (Played: 61, Target: 60...)".

**IV. Integration Points (Conceptual):**

*   **UI to Controller:** Manually verify that changing parameters in QML (style, tempo, etc.) results in those specific values being logged by the `AiController` methods when action buttons are clicked.
*   **Controller to Converter:** Verify (via logs) that `AiController` calls the `AiMusicDataConverter` methods (`convertToScore`, `convertToPlaybackStream`) with some form of data (currently dummy data).
*   **Selection to Controller:** (Crucial for co-creation) Once live selection is reliably passed to `extractMusicSelection`, verify that selecting different notes/passages in a score results in different `MusicalEvent` data being logged by `AiController`.

**V. Security/Privacy Comment Verification:**

*   Manually inspect `AiController.cpp`.
*   Confirm the `// TODO: SECURITY - ...` comment exists in `setCustomModelPath` regarding path validation and model loading.
*   Confirm the `// TODO: SECURITY - ...` comment exists in `processStyleAnalysisFile` regarding secure file parsing and input validation.

This strategy provides a comprehensive starting point for testing the AI features as they evolve.
