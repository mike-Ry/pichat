#include "include/utils/ErrorHandler.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

/**
 * @brief Constructor for ErrorHandler
 *
 * Initializes the ErrorHandler with empty error records.
 */
ErrorHandler::ErrorHandler() {
    // Initialize with empty error records
}

/**
 * @brief Get the singleton instance of ErrorHandler
 *
 * @return Reference to the ErrorHandler instance
 */
ErrorHandler& ErrorHandler::getInstance() {
    static ErrorHandler instance;
    return instance;
}

/**
 * @brief Log an error with level and context
 *
 * @param level Error severity level
 * @param message Error message
 * @param context Additional context information
 */
void ErrorHandler::logError(ErrorLevel level, const std::string& message, const std::string& context) {
    std::lock_guard<std::mutex> lock(errorMutex);

    ErrorRecord record;
    record.level = level;
    record.message = message;
    record.context = context;
    record.timestamp = getCurrentTimestamp();

    errorRecords.push_back(record);

    // Output to console based on level
    switch (level) {
    case ErrorLevel::EL_INFO:
        std::cout << "[INFO] " << message << std::endl;
        break;
    case ErrorLevel::EL_WARNING:
        std::cout << "[WARNING] " << message << std::endl;
        break;
    case ErrorLevel::EL_ERROR:
        std::cerr << "[ERROR] " << message << std::endl;
        break;
    case ErrorLevel::EL_FATAL:
        std::cerr << "[FATAL] " << message << std::endl;
        break;
    }
}

/**
 * @brief Log an error message
 *
 * @param message Error message
 * @param context Additional context information
 */
void ErrorHandler::logError(const std::string& message, const std::string& context) {
    logError(ErrorLevel::EL_ERROR, message, context);
}

/**
 * @brief Log an info message
 *
 * @param message Information message
 * @param context Additional context information
 */
void ErrorHandler::logInfo(const std::string& message, const std::string& context) {
    logError(ErrorLevel::EL_INFO, message, context);
}

/**
 * @brief Log a warning message
 *
 * @param message Warning message
 * @param context Additional context information
 */
void ErrorHandler::logWarning(const std::string& message, const std::string& context) {
    logError(ErrorLevel::EL_WARNING, message, context);
}

/**
 * @brief Log a fatal error message
 *
 * @param message Fatal error message
 * @param context Additional context information
 */
void ErrorHandler::logFatal(const std::string& message, const std::string& context) {
    logError(ErrorLevel::EL_FATAL, message, context);
}

/**
 * @brief Get the most recent error
 *
 * @return The most recent error record, or nullptr if no errors
 */
const ErrorRecord* ErrorHandler::getLastError() const {
    std::lock_guard<std::mutex> lock(errorMutex);

    if (errorRecords.empty()) {
        return nullptr;
    }

    return &errorRecords.back();
}

/**
 * @brief Get all logged errors
 *
 * @return Vector of all error records
 */
std::vector<ErrorRecord> ErrorHandler::getAllErrors() const {
    std::lock_guard<std::mutex> lock(errorMutex);
    return errorRecords;
}

/**
 * @brief Clear all logged errors
 */
void ErrorHandler::clearErrors() {
    std::lock_guard<std::mutex> lock(errorMutex);
    errorRecords.clear();
}

/**
 * @brief Get the current timestamp
 *
 * @return Current timestamp as a string
 */
std::string ErrorHandler::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    // 使用线程安全的方式获取时间
    struct tm timeinfo;
#if defined(_MSC_VER)
    localtime_s(&timeinfo, &time);
#else
    localtime_r(&time, &timeinfo);
#endif

    std::stringstream ss;
    ss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
vo