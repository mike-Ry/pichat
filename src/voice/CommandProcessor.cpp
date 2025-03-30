#include "include/voice/CommandProcessor.h"
#include <algorithm>
#include <cctype>

CommandProcessor::CommandProcessor() {
}

CommandProcessor::~CommandProcessor() {
}

bool CommandProcessor::initialize() {
    // No initialization needed for this basic implementation
    return true;
}

void CommandProcessor::registerCommand(const std::string& command, std::function<void()> handler) {
    // Convert to lowercase for case-insensitive comparison
    std::string lowerCommand = command;
    std::transform(lowerCommand.begin(), lowerCommand.end(), lowerCommand.begin(),
        [](unsigned char c) { return std::tolower(c); });

    commandHandlers[lowerCommand] = handler;
}

bool CommandProcessor::processCommand(const std::string& command) {
    // Convert to lowercase for case-insensitive comparison
    std::string lowerCommand = command;
    std::transform(lowerCommand.begin(), lowerCommand.end(), lowerCommand.begin(),
        [](unsigned char c) { return std::tolower(c); });

    // Check for exact match first
    auto it = commandHandlers.find(lowerCommand);
    if (it != commandHandlers.end()) {
        it->second(); // Call the handler
        return true;
    }

    // Check for partial matches (if command is part of a longer phrase)
    for (const auto& [registeredCommand, handler] : commandHandlers) {
        if (lowerCommand.find(registeredCommand) != std::string::npos) {
            handler(); // Call the handler
            return true;
        }
    }

    return false;
}