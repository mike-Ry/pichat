#pragma once

#include <string>
#include <vector>
#include <mutex>

// 使用 enum class 而不是普通 enum
enum class ErrorLevel {
    INFO,
    WARNING,
    ERROR,
    FATAL
};

// Error record structure
struct ErrorRecord {
    ErrorLevel level;
    std::string message;
    std::string context;
    std::string timestamp;
};

// Error handler class
class ErrorHandler {
public:
    static ErrorHandler& getInstance();
    void logError(ErrorLevel level, const std::string& message, const std::string& context = "");
    void logInfo(const std::string& message, const std::string& context = "");
    void logWarning(const std::string& message, const std::string& context = "");
    void logError(const std::string& message, const std::string& context = "");
    void logFatal(const std::string& message, const std::string& context = "");
    const ErrorRecord* getLastError() const;
    std::vector<ErrorRecord> getAllErrors() const;
    void clearErrors();

private:
    ErrorHandler();
    ErrorHandler(const ErrorHandler&) = delete;
    ErrorHandler& operator=(const ErrorHandler&) = delete;
    std::vector<ErrorRecord> errorRecords;
    mutable std::mutex errorMutex;
    std::string getCurrentTimestamp() const;
};
