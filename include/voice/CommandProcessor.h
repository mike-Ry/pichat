#pragma once

#include <string>
#include <unordered_map>
#include <functional>

/**
 * @class CommandProcessor
 * @brief Processes voice commands
 */
class CommandProcessor {
public:
    CommandProcessor();
    ~CommandProcessor();

    /**
     * @brief Initialize the command processor
     * @return true if initialization is successful
     */
    bool initialize();

    /**
     * @brief Register a command handler
     * @param command The command phrase
     * @param handler The function to call when command is recognized
     */
    void registerCommand(const std::string& command, std::function<void()> handler);

    /**
     * @brief Process a command string
     * @param command Command to process
     * @return true if a command was recognized and handled
     */
    bool processCommand(const std::string& command);

private:
    // Map of commands to handlers
    std::unordered_map<std::string, std::function<void()>> commandHandlers;
};