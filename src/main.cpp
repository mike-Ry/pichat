#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <functional>
#include <future>
#include <mutex>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <filesystem>

// Qt includes
#include <QtWidgets/QApplication>
#include <QtCore/QCommandLineParser>
#include <QtWidgets/QMessageBox>
#include <QPalette>
#include <QColor>
#include <QIcon>

#ifdef _WIN32
#include <Windows.h>
#else
#include <signal.h>
#include <unistd.h>
#endif

#include "include/config/ConfigManager.h"
#include "include/utils/ErrorHandler.h"
#include "include/cli/CLIManager.h"
#include "include/voice/VoiceManager.h"
#include "include/voice/CommandProcessor.h"
#include "include/voice/TextToSpeech.h"
#include "include/gui/MainWindow.h"
#include "include/common/Message.h"
#include "include/utils/DeepSeekAPI.h" // 确保引入此头文件

// 调试输出设置
void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    const char* file = context.file ? context.file : "";
    const char* function = context.function ? context.function : "";

    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
        abort();
    }
}

using json = nlohmann::json;

// Global flag for service mode
bool g_running = true;

// Signal handler for service mode
#ifdef _WIN32
BOOL WINAPI consoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT || signal == CTRL_BREAK_EVENT) {
        g_running = false;
        return TRUE;
    }
    return FALSE;
}
#else
void signalHandler(int signum) {
    g_running = false;
}
#endif

// Service mode
void runService() {
    // Setup signal handling
#ifdef _WIN32
    SetConsoleCtrlHandler(consoleHandler, TRUE);
#else
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);
#endif

    std::cout << "PiChat service started" << std::endl;

    // Main service loop
    while (g_running) {
        // In a real application, this would handle incoming requests
        // This is just a placeholder
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "PiChat service stopped" << std::endl;
}

// Interactive mode with DeepSeek API
void runInteractive() {
    std::cout << "PiChat Interactive Mode" << std::endl;
    std::cout << "Type 'exit' to quit, 'clear' to clear chat history" << std::endl;

    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    ConfigManager& configManager = ConfigManager::getInstance();

    // Get API key
    std::string apiKey = configManager.getApiKey();
    if (apiKey.empty()) {
        errorHandler.logError("API key not set. Use --set-key to configure.");
        std::cerr << "Error: API key not set. Use --set-key to configure." << std::endl;
        return;
    }

    // Initialize chat session
    ChatSession chatSession;
    if (!chatSession.initialize(apiKey)) {
        errorHandler.logError("Failed to initialize chat session. Check your API key.");
        std::cerr << "Error: Failed to initialize chat session. Check your API key." << std::endl;
        return;
    }

    // Basic interactive loop
    std::string input;
    while (true) {
        std::cout << "\nYou: ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        if (input == "clear") {
            chatSession.clearHistory();
            std::cout << "Chat history cleared." << std::endl;
            continue;
        }

        if (!input.empty()) {
            std::cout << "PiChat: ";

            // Try non-streaming first
            std::string response = chatSession.sendMessage(input);
            std::cout << response << std::endl;

            // Use streaming API if you prefer real-time responses (currently disabled to test non-streaming first)
            /*
            chatSession.sendMessageStreaming(input, [](const std::string& chunk) {
                std::cout << chunk << std::flush;
            });
            std::cout << std::endl;
            */
        }
    }
}

// 语音交互模式
void runVoice() {
    std::cout << "PiChat Voice Mode" << std::endl;
    std::cout << "Speak to interact with the AI, or say 'exit' to quit" << std::endl;

    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    ConfigManager& configManager = ConfigManager::getInstance();

    // 获取API密钥
    std::string apiKey = configManager.getApiKey();
    if (apiKey.empty()) {
        errorHandler.logError("API key not set. Use --set-key to configure.");
        std::cerr << "Error: API key not set. Use --set-key to configure." << std::endl;
        return;
    }

    // 使用单例获取语音管理器
    VoiceManager& voiceManager = VoiceManager::getInstance();
    if (!voiceManager.initialize()) {
        errorHandler.logError("Failed to initialize voice manager.");
        std::cerr << "Error: Failed to initialize voice manager." << std::endl;
        return;
    }

    // 初始化文本转语音和命令处理器
    TextToSpeech tts;
    if (!tts.initialize()) {
        errorHandler.logError("Failed to initialize text-to-speech.");
        std::cerr << "Error: Failed to initialize text-to-speech." << std::endl;
        return;
    }

    CommandProcessor cmdProcessor;
    cmdProcessor.initialize();

    // 初始化聊天会话
    ChatSession chatSession;
    if (!chatSession.initialize(apiKey)) {
        errorHandler.logError("Failed to initialize chat session. Check your API key.");
        std::cerr << "Error: Failed to initialize chat session. Check your API key." << std::endl;
        return;
    }

    // 注册命令
    cmdProcessor.registerCommand("exit", [&]() {
        std::cout << "Exiting voice mode..." << std::endl;
        g_running = false;
        });

    cmdProcessor.registerCommand("quit", [&]() {
        std::cout << "Exiting voice mode..." << std::endl;
        g_running = false;
        });

    // 设置信号处理
#ifdef _WIN32
    SetConsoleCtrlHandler(consoleHandler, TRUE);
#else
    signal(SIGINT, signalHandler);
#endif

    // 创建语音识别回调
    auto onSpeechRecognized = [&](const std::string& text) {
        if (text.empty()) return;

        std::cout << "You said: " << text << std::endl;

        // 检查是否是退出命令
        if (text == "exit" || text == "quit") {
            g_running = false;
            return;
        }

        // 处理命令
        bool commandHandled = cmdProcessor.processCommand(text);
        if (commandHandled) {
            return; // 命令已处理
        }

        // 发送消息到API
        std::string response = chatSession.sendMessage(text);
        std::cout << "PiChat: " << response << std::endl;

        // 使用TTS播放回答
        tts.speak(response);
        };

    // 开始监听
    g_running = true;
    voiceManager.startListening(onSpeechRecognized);

    std::cout << "Listening... Say something or 'exit' to quit" << std::endl;
    tts.speak("PiChat ready. Please speak.");

    // 主循环
    while (g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // 停止语音识别
    voiceManager.stopListening();
    std::cout << "Voice mode exited." << std::endl;
}

// GUI mode with modern UI
void runGui(int argc, char* argv[]) {
    // 确保图标文件存在

    // Create Qt application
    QApplication app(argc, argv);
    app.setApplicationName("PiChat");
    app.setOrganizationName("PiChat");

    // Set application style
    app.setStyle("Fusion");

    // Apply custom palette to beautify the interface
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    app.setPalette(darkPalette);

    // Set application stylesheet
    app.setStyleSheet(
        "QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }"
        "QMenu::item:selected { background-color: #2a82da; }"
        "QLineEdit { background-color: #1e1e1e; border: 1px solid #5c5c5c; padding: 2px; border-radius: 4px; }"
        "QPushButton { background-color: #2a82da; color: white; border-radius: 4px; padding: 6px; min-width: 80px; }"
        "QPushButton:hover { background-color: #3b93eb; }"
        "QPushButton:pressed { background-color: #1c71c9; }"
        "QTextEdit { background-color: #1e1e1e; border: 1px solid #5c5c5c; border-radius: 4px; }"
        "QScrollBar:vertical { background-color: #1e1e1e; width: 10px; margin: 0px; }"
        "QScrollBar::handle:vertical { background-color: #5c5c5c; min-height: 20px; border-radius: 5px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }"
        "QTabWidget::pane { border: 1px solid #5c5c5c; }"
        "QTabBar::tab { background-color: #2d2d2d; color: white; padding: 6px 12px; margin-right: 2px; }"
        "QTabBar::tab:selected { background-color: #2a82da; }"
        "QTabBar::tab:hover:!selected { background-color: #3c3c3c; }"
    );

    // Load application icon
    QIcon appIcon(":/icons/app_icon.png");
    if (!appIcon.isNull()) {
        app.setWindowIcon(appIcon);
    }

    // Create and show the main window
    MainWindow mainWindow;
    mainWindow.show();

    // Enter Qt event loop
    app.exec();
}

int main(int argc, char* argv[]) {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);

    // Initialize error handler
    ErrorHandler& errorHandler = ErrorHandler::getInstance();

    // Initialize ConfigManager
    ConfigManager& configManager = ConfigManager::getInstance();

    // Check command line arguments
    if (argc > 1) {
        std::string arg = argv[1];

        // Check if we're running in service mode
        if (arg == "--service") {
            runService();
            curl_global_cleanup();
            return 0;
        }

        // Check if we're running in interactive mode
        if (arg == "--interactive") {
            runInteractive();
            curl_global_cleanup();
            return 0;
        }

        // Check for voice mode flag
        if (arg == "--voice") {
            runVoice();
            curl_global_cleanup();
            return 0;
        }

        // Check for GUI mode flag
        if (arg == "--gui") {
            runGui(argc, argv);
            curl_global_cleanup();
            return 0;
        }

        // Process other CLI commands
        CLIManager& cliManager = CLIManager::getInstance();
        int result = cliManager.parseAndExecute(argc, argv);
        curl_global_cleanup();
        return result;
    }

    // Default to GUI mode if no arguments provided
    try {
        runGui(argc, argv);
    }
    catch (const std::exception& e) {
        errorHandler.logError(std::string("GUI error: ") + e.what());
        QMessageBox::critical(nullptr, "PiChat Error",
            QString("Failed to start GUI: %1").arg(e.what()));
    }

    // Clean up libcurl
    curl_global_cleanup();
    return 0;
}
