#include "include/voice/VoiceManager.h"
#include "include/voice/SpeechRecognizer.h"
#include "include/voice/TextToSpeech.h"
#include "include/voice/CommandProcessor.h"
#include <iostream>
#include <chrono>

VoiceManager::VoiceManager() : listening(false) {
    speechRecognizer = std::make_unique<SpeechRecognizer>();
    textToSpeech = std::make_unique<TextToSpeech>();
    commandProcessor = std::make_unique<CommandProcessor>();
}

VoiceManager::~VoiceManager() {
    shutdown();
}

VoiceManager& VoiceManager::getInstance() {
    static VoiceManager instance;
    return instance;
}

bool VoiceManager::initialize() {
    // Initialize components
    if (!speechRecognizer->initialize()) {
        std::cerr << "Failed to initialize speech recognizer" << std::endl;
        return false;
    }

    if (!textToSpeech->initialize()) {
        std::cerr << "Failed to initialize text-to-speech" << std::endl;
        return false;
    }

    if (!commandProcessor->initialize()) {
        std::cerr << "Failed to initialize command processor" << std::endl;
        return false;
    }

    // Register built-in commands
    commandProcessor->registerCommand("turn on the lights", []() {
        std::cout << "Turning on the lights (simulated)" << std::endl;
        });

    commandProcessor->registerCommand("turn off the lights", []() {
        std::cout << "Turning off the lights (simulated)" << std::endl;
        });

    commandProcessor->registerCommand("what time is it", []() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::string timeStr = std::ctime(&time);
        std::cout << "The current time is " << timeStr;
        });

    return true;
}

void VoiceManager::startListening(std::function<void(const std::string&)> callback) {
    if (listening.load()) {
        return; // Already listening
    }

    listening.store(true);
    listeningThread = std::thread(&VoiceManager::listeningLoop, this, callback);
}

void VoiceManager::stopListening() {
    if (!listening.load()) {
        return; // Not listening
    }

    listening.store(false);
    if (listeningThread.joinable()) {
        listeningThread.join();
    }
}

void VoiceManager::speak(const std::string& text) {
    // Record start time for latency measurement
    auto startTime = std::chrono::high_resolution_clock::now();

    // Speak the text
    textToSpeech->speak(text);

    // Calculate latency
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Log latency if it exceeds threshold (3000ms = 3 seconds)
    if (duration > 3000) {
        std::cerr << "Warning: Audio response latency exceeded 3 seconds: " << duration << "ms" << std::endl;
    }
}

bool VoiceManager::processCommand(const std::string& command) {
    return commandProcessor->processCommand(command);
}

bool VoiceManager::isListening() const {
    return listening.load();
}

void VoiceManager::shutdown() {
    stopListening();

    if (speechRecognizer) {
        speechRecognizer->shutdown();
    }

    if (textToSpeech) {
        textToSpeech->shutdown();
    }
}

void VoiceManager::listeningLoop(std::function<void(const std::string&)> callback) {
    while (listening.load()) {
        // Prompt for input
        if (!speechRecognizer->startRecording()) {
            std::cerr << "Failed to start recording" << std::endl;
            break;
        }

        // Get input (keyboard simulation)
        std::string recognizedText = speechRecognizer->recognizeSpeech();

        if (!recognizedText.empty()) {
            // Check if it's "exit"
            if (recognizedText == "exit") {
                listening.store(false);
                break;
            }

            // Check if it's a system command
            if (processCommand(recognizedText)) {
                // Command was handled
                continue;
            }

            // Not a system command, pass to callback
            callback(recognizedText);
        }
    }
}