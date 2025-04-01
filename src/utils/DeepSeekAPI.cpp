// src/utils/DeepSeekAPI.cpp
#include "include/utils/DeepSeekAPI.h"
#include "include/config/ConfigManager.h"
#include "include/utils/ErrorHandler.h"
#include <QThread>
#include <QDebug>
#include <QMetaType>

// ChatSession 类的实现
ChatSession::ChatSession() : apiKey("") {}

bool ChatSession::initialize(const std::string& apiKey) {
    this->apiKey = apiKey;
    return !apiKey.empty();
}

std::string ChatSession::sendMessage(const std::string& message) {
    // Add user message to history
    history.push_back(Message("user", message));

    // Get response from API
    std::string response = chatCompletion(apiKey, history);

    // Add assistant message to history
    history.push_back(Message("assistant", response));

    return response;
}

std::string ChatSession::sendMessageStreaming(
    const std::string& message,
    std::function<void(const std::string&)> callback
) {
    // Add user message to history
    history.push_back(Message("user", message));

    // Get streaming response from API
    std::string response = streamingChatCompletion(apiKey, history, callback);

    // Add assistant message to history
    history.push_back(Message("assistant", response));

    return response;
}

void ChatSession::clearHistory() {
    history.clear();
}

const std::vector<Message>& ChatSession::getHistory() const {
    return history;
}

// DeepSeekAPI 类的实现
DeepSeekAPI::DeepSeekAPI(QObject* parent) : QObject(parent) {
    // 注册std::string类型以便在信号槽机制中使用
    qRegisterMetaType<std::string>("std::string");
    getApiKey();
}

DeepSeekAPI::~DeepSeekAPI() {
}

bool DeepSeekAPI::getApiKey() {
    ConfigManager& configManager = ConfigManager::getInstance();
    apiKey = configManager.getApiKey();
    if (apiKey.empty()) {
        // 如果没有设置API KEY，使用演示模式
        apiKey = "demo_key";
        return false;
    }
    return true;
}

void DeepSeekAPI::sendMessage(const std::string& message, const std::vector<Message>& history) {
    // 创建线程处理API请求，避免阻塞UI
    QThread* thread = QThread::create([this, message, history]() {
        std::string response = sendRequest(message, history);
        // 使用QString转换，避免类型问题
        emit responseReceived(QString::fromStdString(response));
        });

    // 设置线程完成后自动删除
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}

std::string DeepSeekAPI::sendRequest(const std::string& message, const std::vector<Message>& history) {
    qDebug() << "Sending request to API with message:" << QString::fromStdString(message);

    // 如果API key为空，则使用演示模式
    if (apiKey.empty()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 模拟网络延迟
        return "API key not set. Please configure your API key in settings. You said: " + message;
    }

    try {
        // 使用外部声明的chatCompletion函数调用真实的API
        // 注意：我们需要创建一个新的history副本，包含当前message
        std::vector<Message> newHistory = history;
        if (newHistory.empty() || newHistory.back().role != "user" || newHistory.back().content != message) {
            newHistory.push_back(Message("user", message));
        }
        std::string response = ::chatCompletion(apiKey, newHistory);
        qDebug() << "API Response:" << QString::fromStdString(response);
        return response;
    }
    catch (const std::exception& e) {
        std::string errorMsg = std::string("Error in API request: ") + e.what();
        qDebug() << "API Error:" << QString::fromStdString(errorMsg);
        return errorMsg;
    }
}
