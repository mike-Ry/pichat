#pragma once

#include <string>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>

// Forward declarations
class SpeechRecognizer;
class TextToSpeech;
class CommandProcessor;

/**
 * @class VoiceManager
 * @brief Manages voice input/output operations
 *
 * This class coordinates speech recognition, text-to-speech,
 * and command processing functionality.
 */
class VoiceManager {
public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the VoiceManager instance
     */
    static VoiceManager& getInstance();

    /**
     * @brief Initialize the voice subsystem
     * @return true if initialization is successful
     */
    bool initialize();

    /**
     * @brief Start listening for voice commands
     * @param callback Function to call when text is recognized
     */
    void startListening(std::function<void(const std::string&)> callback);

    /**
     * @brief Stop listening for voice commands
     */
    void stopListening();

    /**
     * @brief Speak the provided text
     * @param text Text to convert to speech
     */
    void speak(const std::string& text);

    /**
     * @brief Process a command and determine if it's a system command
     * @param command The command text to process
     * @return true if command was handled, false otherwise
     */
    bool processCommand(const std::string& command);

    /**
     * @brief Check if the system is currently listening
     * @return true if listening
     */
    bool isListening() const;

    /**
     * @brief Clean up resources
     */
    void shutdown();

private:
    // Private constructor for singleton pattern
    VoiceManager();
    ~VoiceManager();

    // Delete copy constructor and assignment operator
    VoiceManager(const VoiceManager&) = delete;
    VoiceManager& operator=(const VoiceManager&) = delete;

    // Components
    std::unique_ptr<SpeechRecognizer> speechRecognizer;
    std::unique_ptr<TextToSpeech> textToSpeech;
    std::unique_ptr<CommandProcessor> commandProcessor;

    // Listening state
    std::atomic<bool> listening;
    std::thread listeningThread;

    // Listening loop function
    void listeningLoop(std::function<void(const std::string&)> callback);
};