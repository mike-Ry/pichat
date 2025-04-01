// include/utils/DeepSeekAPI.h
#pragma once

#include <string>
#include <vector>
#include <QObject>
#include <QString>
#include <functional>
#include "include/common/Message.h"

// 确保std::string可以在Qt信号槽系统中使用
Q_DECLARE_METATYPE(std::string)

// 声明API函数
std::string chatCompletion(
    const std::string& apiKey,
    const std::vector<Message>& messages,
    const std::string& model = "deepseek-chat",
    float temperature = 0.7,
    int maxTokens = 1000
);

std::string streamingChatCompletion(
    const std::string& apiKey,
    const std::vector<Message>& messages,
    std::function<void(const std::string&)> callback,
    const std::string& model = "deepseek-chat",
    float temperature = 0.7,
    int maxTokens = 1000
);

// Chat Session class to manage conversation with DeepSeek
class ChatSession {
public:
    ChatSession();

    bool initialize(const std::string& apiKey);

    std::string sendMessage(const std::string& message);

    std::string sendMessageStreaming(
        const std::string& message,
        std::function<void(const std::string&)> callback
    );

    void clearHistory();

    // Make chat history accessible for GUI
    const std::vector<Message>& getHistory() const;

private:
    std::string apiKey;
    std::vector<Message> history;
    std::string model = "deepseek-chat"; // Updated model name
};

class DeepSeekAPI : public QObject {
    Q_OBJECT

public:
    explicit DeepSeekAPI(QObject* parent = nullptr);
    ~DeepSeekAPI();

    // 发送消息到API
    void sendMessage(const std::string& message, const std::vector<Message>& history);

signals:
    // 响应接收信号 - 使用QString而非std::string避免类型转换问题
    void responseReceived(const QString& response);

private:
    std::string apiKey;
    bool getApiKey();
    std::string sendRequest(const std::string& message, const std::vector<Message>& history);
};
