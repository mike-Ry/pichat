#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>

/**
 * @struct Command
 * @brief Represents a CLI command
 */
struct Command {
    std::string name;
    std::string description;
    std::function<int(const std::vector<std::string>&)> handler;
};

/**
 * @class CLIManager
 * @brief Manages command-line interface operations
 *
 * This class handles parsing command-line arguments and executing
 * the appropriate commands.
 */
class CLIManager {
public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the CLIManager instance
     */
    static CLIManager& getInstance();

    /**
     * @brief Parse command-line arguments and execute appropriate command
     * @param argc Argument count
     * @param argv Argument values
     * @return Exit code
     */
    int parseAndExecute(int argc, char* argv[]);

    /**
     * @brief Register a command
     * @param name Command name
     * @param description Command description
     * @param handler Command handler function
     */
    void registerCommand(const std::string& name,
        const std::string& description,
        std::function<int(const std::vector<std::string>&)> handler);

    /**
     * @brief Print usage information
     */
    void printUsage() const;

    /**
     * @brief Get application name
     * @return Application name
     */
    std::string getAppName() const;

    /**
     * @brief Set application name
     * @param name Application name
     */
    void setAppName(const std::string& name);

private:
    // Private constructor for singleton pattern
    CLIManager();

    // Delete copy constructor and assignment operator
    CLIManager(const CLIManager&) = delete;
    CLIManager& operator=(const CLIManager&) = delete;

    // Application name
    std::string appName;

    // Registered commands
    std::unordered_map<std::string, Command> commands;

    // Default handler for unknown commands
    int handleUnknownCommand(const std::string& command);

    // Helper methods for service management
    bool isServiceRunning();
    bool startService();
    bool stopService();
};