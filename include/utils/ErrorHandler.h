#pragma once

#include <string>
#include <vector>
#include <mutex>

/**
 * @enum ErrorLevel
 * @brief Enumeration of error severity levels
 */
enum class ErrorLevel {
    INFO,
    WARNING,
    ERROR,
    FATAL
};

/**
 * @struct ErrorRecord
 * @brief Records an error occurrence
 */
struct ErrorRecord {
    ErrorLevel level;
    std::string message;
    std::string context;
    std::string timestamp;
};

/**
 * @class ErrorHandler
 * @brief Centralized error handling for the application
 *
 * This class provides methods for logging and handling errors
 * throughout the application.
 */
class ErrorHandler {
public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the ErrorHandler instance
     */
    static ErrorHandler& getInstance();

    /**
     * @brief Log an error
     * @param level Error severity level
     * @param message Error message
     * @param context Additional context information
     */
    void logError(ErrorLevel level, const std::string& message, const std::string& context = "");

    /**
     * @brief Log an info message
     * @param message Information message
     * @param context Additional context information
     */
    void logInfo(const std::string& message, const std::string& context = "");

    /**
     * @brief Log a warning message
     * @param message Warning message
     * @param context Additional context information
     */
    void logWarning(const std::string& message, const std::string& context = "");

    /**
     * @brief Log an error message
     * @param message Error message
     * @param context Additional context information
     */
    void logError(const std::string& message, const std::string& context = "");

    /**
     * @brief Log a fatal error message
     * @param message Fatal error message
     * @param context Additional context information
     */
    void logFatal(const std::string& message, const std::string& context = "");

    /**
     * @brief Get the most recent error
     * @return The most recent error record, or nullptr if no errors
     */
    const ErrorRecord* getLastError() const;

    /**
     * @brief Get all logged errors
     * @return Vector of all error records
     */
    std::vector<ErrorRecord> getAllErrors() const;

    /**
     * @brief Clear all logged errors
     */
    void clearErrors();

private:
    // Private constructor for singleton pattern
    ErrorHandler();

    // Delete copy constructor and assignment operator
    ErrorHandler(const ErrorHandler&) = delete;
    ErrorHandler& operator=(const ErrorHandler&) = delete;

    // Error records
    std::vector<ErrorRecord> errorRecords;

    // Mutex for thread safety
    mutable std::mutex errorMutex;

    // Helper functions
    std::string getCurrentTimestamp() const;
};