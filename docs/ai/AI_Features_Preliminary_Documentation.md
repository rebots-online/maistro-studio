# mAIstro AI Features - Preliminary Documentation

**Version:** (Pre-Alpha / Prototyping Stage)
**Date:** (Current Date - to be filled by system)

## 1. Introduction

This document provides a preliminary overview of the mAIstro AI features initiative. These features aim to integrate artificial intelligence capabilities into the mAIstro music notation software to assist users in composition, arrangement, music education, and style analysis.

The features are currently in a foundational framework and prototyping stage. Most of the backend AI processing is simulated, and the UI provides an initial interface for these planned capabilities. This document outlines what is currently accessible and what is planned for developers and future users.

## 2. AI Features Overview

All AI-related UI elements are currently centralized on the "AI Music Generation" page, accessible from the application's Home Page under the "AI Music" section.

### AI Music Generation
*   **Purpose:** To generate new musical passages or pieces based on user-defined parameters.
*   **UI Location:** "AI Music Generation" section on the AI page.
*   **Controls:**
    *   Style (ComboBox: Classical, Jazz, Rock, etc.)
    *   Tempo (SpinBox: 40-240 BPM)
    *   Key (ComboBox: Common major and minor keys)
    *   Length (SpinBox: 8-128 measures)
*   **Current Usage:** Users can select parameters and click the "Generate Music" button. The system will log the request, and the UI will display status messages indicating the (simulated) start and completion of the generation process.
*   **Note:** The actual music generation by an AI model is currently simulated. Placeholder data is processed internally.

### Music Co-creation
*   **Purpose:** To enable users to collaborate with an AI by providing a musical selection (e.g., a few measures or a melodic line) which the AI can then develop, harmonize, or otherwise enhance based on selected parameters.
*   **UI Location:** "Co-create with Current Selection" button on the AI page.
*   **Current Usage:** Users can (conceptually) select a portion of their score. Clicking the button uses the same Style, Tempo, and Key parameters from the UI. The system then (simulated) processes this selection.
*   **Note:** The extraction of musical data from the current score selection is implemented at a basic level (pitch, timing, voice, staff for selected notes). The AI enhancement/co-creation process itself is simulated.

### Style Transfer
*   **Purpose:** To analyze the stylistic elements (e.g., harmony, rhythm, melody contours) of one musical piece and apply that style to another piece or a user's composition.
*   **UI Location:** "Load File for Style Analysis" button on the AI page.
*   **Current Usage:** Clicking the button simulates a file selection process. A placeholder file path is used, and the system logs that it would begin analysis.
*   **Note:** Actual file dialog opening, file parsing, and AI style analysis are not yet implemented. The process is simulated.

### Custom AI Models
*   **Purpose:** To allow advanced users or researchers to integrate and use their own pre-trained AI models for generation or other AI tasks within mAIstro.
*   **UI Location:** "Custom Model:" label, a read-only text field displaying the path, and a "Select Custom Model" button on the AI page.
*   **Current Usage:** Clicking the "Select Custom Model" button simulates a file/directory selection process, and a placeholder path is displayed in the text field.
*   **Note:** The actual loading, validation, and execution of custom AI models are not yet implemented.

### Music Education - Pitch Feedback
*   **Purpose:** To provide real-time feedback on pitch accuracy for users playing a MIDI instrument, aiding in ear training and performance practice.
*   **UI Location:** "Music Education: Pitch Feedback" section on the AI page, including a feedback label.
*   **Current Usage:** The UI includes test buttons to simulate MIDI input for specific notes (B59, C60, C#61). The feedback label updates to show "Correct!", "Too Low.", or "Too High." relative to a fixed target note (Middle C - MIDI note 60).
*   **Note:** Live MIDI input from an instrument is not yet connected to this feature. The feedback logic is based on a predefined target.

## 3. Key Backend Components (For Developers)

The C++ backend logic for these AI features is primarily being developed within a new framework module:

*   **Module Path:** `src/framework/ai/`
*   **`AiController` (`AiController.h`, `AiController.cpp`):**
    *   A QML-accessible class that acts as the bridge between the QML UI and the C++ backend.
    *   Handles user requests from the UI (e.g., generate music, select file).
    *   Manages the (currently simulated) workflow for AI tasks.
    *   Emits signals to update the UI with status messages or results.
    *   Includes methods for selection extraction (`extractMusicSelection`) and placeholders for file/model path handling.
*   **`IAiMusicGenerator` (`IAiMusicGenerator.h`):**
    *   A C++ interface defining the contract for AI music generation models. Currently a placeholder.
*   **`AiMusicDataConverter` (`AiMusicDataConverter.h`, `AiMusicDataConverter.cpp`):**
    *   A class intended to convert data between the AI model's output format and mAIstro's internal data structures (e.g., `engraving::Score`, playback event lists). Currently, its conversion methods are placeholders.
*   **`AiMusicTypes` (`AiMusicTypes.h`):**
    *   Contains C++ structs (`MusicalEvent`, `MusicSelectionContext`) for representing extracted musical selection data in a structured way for potential AI processing.

## 4. Future Development & Considerations

*   **AI Model Integration:** The most significant next step is the integration of actual AI models for generation, co-creation, and style analysis. The current backend is primarily simulated.
*   **Build Environment:** Current CI/VM environment issues with `apt-get update` are blocking successful builds, which in turn hinders some aspects of development and full testing (e.g., native file dialogs, full C++ test cycles). Resolving these is a priority.
*   **Selection Extraction:** The `extractMusicSelection` method in `AiController` has basic functionality. It needs to be made more robust, including proper error handling and potentially deeper integration with the notation context to get richer musical information (tempo, key signatures from score). Tick-to-beat time conversion needs full implementation.
*   **File Handling:** Actual implementation of file dialogs (for style analysis and custom models) and secure file parsing is required.
*   **Security & Privacy:** As noted by `TODO` comments in `AiController.cpp`, robust security measures for file handling (path validation, secure parsing) and custom model loading (validation, sandboxing) are critical and must be implemented. Privacy of user data (scores, selected models) must be ensured.
*   **MIDI Integration:** For the pitch feedback feature, live MIDI input needs to be routed to the `AiController`.
*   **Error Handling:** More granular error reporting from backend to UI.
*   **User Experience:** The UI is currently basic. Refinements will be needed as features mature.

## 5. How to Access (Conceptual)

1.  Launch mAIstro.
2.  From the Home Page, navigate to the "AI Music" section in the side menu.
3.  This will open the "AI Music Generation" page (`AiGenerationPage.qml`) where all current AI feature UI elements are located.

This document will be updated as development progresses.
