#pragma once

#include <string>
#include <vector>
#include <mutex>

// ʹ��ö���࣬�������Ƴ�ͻ
enum class ErrorLevel {
    EL_INFO,
    EL_WARNING,
    EL_ERROR,
    EL_FATAL
};

// �����¼�ṹ
struct ErrorRecord {
    ErrorLevel level;
    std::string message;
    std::string context;
    std::string timestamp;
};

// ��������
class ErrorHandler {
public:
    // ��ȡ����ʵ��
    static ErrorHandler& getInstance();

    // ��¼������Ϣ
    void logError(const std::string& message, const std::string& context = "");

    // ��¼ָ������Ĵ�����Ϣ
    void logError(ErrorLevel level, const std::string& message, const std::string& context = "");

    // ��¼��Ϣ�������Ϣ
    void logInfo(const std::string& message, const std::string& context = "");

    // ��¼���漶�����Ϣ
    void logWarning(const std::string& message, const std::string& context = "");

    // ��¼����������Ϣ
    void logFatal(const std::string& message, const std::string& context = "");

    // ��ȡ����Ĵ����¼
    const ErrorRecord* getLastError() const;

    // ��ȡ���д����¼
    std::vector<ErrorRecord> getAllErrors() const;

    // ������д����¼
    void clearErrors();

private:
    // ˽�й��캯����ʵ�ֵ���ģʽ
    ErrorHandler();
    ~ErrorHandler() = default;
    ErrorHandler(const ErrorHandler&) = delete;
    ErrorHandler& operator=(const ErrorHandler&) = delete;

    // �����¼�б�
    std::vector<ErrorRecord> errorRecords;

    // �̰߳�ȫ������
    mutable std::mutex errorMutex;

    // ��ȡ��ǰʱ���
    std::string getCurrentTimestamp() const;
};
