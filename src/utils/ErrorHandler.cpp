#include "include/utils/NewErrorHandler.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

ErrorHandler::ErrorHandler() {
    // Initialize with empty error records
}

ErrorHandler& ErrorHandler::getInstance() {
    static ErrorHandler instance;
    return instance;
}

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
    case ErrorLevel::INFO:
        std::cout << "[INFO] " << message << std::endl;
        break;
    case ErrorLevel::WARNING:
        std::cout << "[WARNING] " << message << std::endl;
        break;
    case ErrorLevel::ERROR:
        std::cerr << "[ERROR] " << message << std::endl;
        break;
    case ErrorLevel::FATAL:
        std::cerr << "[FATAL] " << message << std::endl;
        break;
    }
}

void ErrorHandler::logInfo(const std::string& message, const std::string& context) {
    logError(ErrorLevel::INFO, message, context);
}

void ErrorHandler::logWarning(const std::string& message, const std::string& context) {
    logError(ErrorLevel::WARNING, message, context);
}

void ErrorHandler::logError(const std::string& message, const std::string& context) {
    logError(ErrorLevel::ERROR, message, context);
}

void ErrorHandler::logFatal(const std::string& message, const std::string& context) {
    logError(ErrorLevel::FATAL, message, context);
}

const ErrorRecord* ErrorHandler::getLastError() const {
    std::lock_guard<std::mutex> lock(errorMutex);

    if (errorRecords.empty()) {
        return nullptr;
    }

    return &errorRecords.back();
}

std::vector<ErrorRecord> ErrorHandler::getAllErrors() const {
    std::lock_guard<std::mutex> lock(errorMutex);
    return errorRecords;
}

void ErrorHandler::clearErrors() {
    std::lock_guard<std::mutex> lock(errorMutex);
    errorRecords.clear();
}

std::string ErrorHandler::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}