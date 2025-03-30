#pragma once

#include <string>

/**
 * @class TextToSpeech
 * @brief Handles text-to-speech conversion using Windows SAPI
 */
class TextToSpeech {
public:
    TextToSpeech();
    ~TextToSpeech();

    /**
     * @brief Initialize the text-to-speech engine
     * @return true if initialization is successful
     */
    bool initialize();

    /**
     * @brief Convert text to speech and play it
     * @param text Text to speak
     * @return true if successful
     */
    bool speak(const std::string& text);

    /**
     * @brief Clean up resources
     */
    void shutdown();

private:
    // Implementation details
    void* espeakInstance;
};