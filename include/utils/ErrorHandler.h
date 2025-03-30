#pragma once

#include <string>
#include <vector>
#include <mutex>

// 使用枚举类，避免名称冲突
enum class ErrorLevel {
    EL_INFO,
    EL_WARNING,
    EL_ERROR,
    EL_FATAL
};

// 错误记录结构
struct ErrorRecord {
    ErrorLevel level;
    std::string message;
    std::string context;
    std::string timestamp;
};

// 错误处理类
class ErrorHandler {
public:
    // 获取单例实例
    static ErrorHandler& getInstance();

    // 记录错误信息
    void logError(const std::string& message, const std::string& context = "");

    // 记录指定级别的错误信息
    void logError(ErrorLevel level, const std::string& message, const std::string& context = "");

    // 记录信息级别的消息
    void logInfo(const std::string& message, const std::string& context = "");

    // 记录警告级别的消息
    void logWarning(const std::string& message, const std::string& context = "");

    // 记录致命错误消息
    void logFatal(const std::string& message, const std::string& context = "");

    // 获取最近的错误记录
    const ErrorRecord* getLastError() const;

    // 获取所有错误记录
    std::vector<ErrorRecord> getAllErrors() const;

    // 清除所有错误记录
    void clearErrors();

private:
    // 私有构造函数，实现单例模式
    ErrorHandler();
    ~ErrorHandler() = default;
    ErrorHandler(const ErrorHandler&) = delete;
    ErrorHandler& operator=(const ErrorHandler&) = delete;

    // 错误记录列表
    std::vector<ErrorRecord> errorRecords;

    // 线程安全互斥锁
    mutable std::mutex errorMutex;

    // 获取当前时间戳
    std::string getCurrentTimestamp() const;
};
