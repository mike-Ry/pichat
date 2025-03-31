#include "include/gui/MainWindow.h"
#include "ui_MainWindow.h"
#include "include/gui/SettingsDialog.h"
#include "include/config/ConfigManager.h"
#include <QMessageBox>
#include <QTime>
#include <QScrollBar>
#include <QSettings>
#include <thread>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , api(new DeepSeekAPI()) {

    ui->setupUi(this);
    setupConnections();
    loadSettings();

    // Set window title
    setWindowTitle("PiChat - DeepSeek AI Chat Interface");

    // Initialize API with key from config
    ConfigManager& config = ConfigManager::getInstance();
    api->setApiKey(config.getApiKey());
}

MainWindow::~MainWindow() = default;

void MainWindow::setupConnections() {
    // Connect enter key in messageInput to send button
    connect(ui->messageInput, &QLineEdit::returnPressed, this, &MainWindow::on_sendButton_clicked);
}

void MainWindow::on_sendButton_clicked() {
    QString message = ui->messageInput->text().trimmed();
    if (message.isEmpty()) {
        return;
    }

    // Display user message
    appendMessage("You", message);

    // Clear input field
    ui->messageInput->clear();

    // Add to chat history
    std::string userMessage = message.toStdString();
    chatHistory.emplace_back("user", userMessage);

    // Show "Thinking..." indicator
    ui->statusBar->showMessage("DeepSeek AI is thinking...");

    // Send message to API in a separate thread
    std::thread apiThread([this, userMessage]() {
        try {
            std::string response = api->sendMessage(chatHistory);

            // Add to chat history
            chatHistory.emplace_back("assistant", response);

            // Update UI in the main thread
            QMetaObject::invokeMethod(this, "onResponseReceived",
                Q::QueuedConnection,
                Q_ARG(std::string, response));
        }
        catch (const std::exception& e) {
            QString errorMsg = "Error: " + QString::fromStdString(e.what());
            QMetaObject::invokeMethod(this, "appendMessage",
                Q::QueuedConnection,
                Q_ARG(QString, "System"),
                Q_ARG(QString, errorMsg));
            QMetaObject::invokeMethod(ui->statusBar, "clearMessage", Q::QueuedConnection);
        }
        });

    // Detach thread to run independently
    apiThread.detach();
}

void MainWindow::onResponseReceived(const std::string& response) {
    // Clear thinking status
    ui->statusBar->clearMessage();

    // Display assistant message
    appendMessage("DeepSeek AI", QString::fromStdString(response));
}

void MainWindow::appendMessage(const QString& sender, const QString& message) {
    // Format timestamp
    QString timestamp = QTime::currentTime().toString("[hh:mm:ss]");

    // Format message
    QString formattedMsg;
    if (sender == "You") {
        formattedMsg = QString("<div style='margin-bottom:5px;'><span style='color:blue;font-weight:bold;'>%1 %2:</span> %3</div>")
            .arg(timestamp, sender, message.toHtmlEscaped());
    }
    else if (sender == "DeepSeek AI") {
        formattedMsg = QString("<div style='margin-bottom:5px;'><span style='color:green;font-weight:bold;'>%1 %2:</span> %3</div>")
            .arg(timestamp, sender, message.toHtmlEscaped());
    }
    else {
        formattedMsg = QString("<div style='margin-bottom:5px;'><span style='color:red;font-weight:bold;'>%1 %2:</span> %3</div>")
            .arg(timestamp, sender, message.toHtmlEscaped());
    }

    // Append to chat display
    ui->chatDisplay->append(formattedMsg);

    // Scroll to bottom
    QScrollBar* scrollbar = ui->chatDisplay->verticalScrollBar();
    scrollbar->setValue(scrollbar->maximum());
}

void MainWindow::on_actionSettings_triggered() {
    SettingsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // Reload settings if dialog was accepted
        loadSettings();
    }
}

void MainWindow::on_actionClear_triggered() {
    // Clear chat display
    ui->chatDisplay->clear();

    // Clear chat history
    chatHistory.clear();
}

void MainWindow::on_actionExit_triggered() {
    close();
}

void MainWindow::loadSettings() {
    ConfigManager& config = ConfigManager::getInstance();
    api->setApiKey(config.getApiKey());

    // Load window settings
    QSettings settings("PiChat", "GUI");
    if (settings.contains("mainWindow/geometry")) {
        restoreGeometry(settings.value("mainWindow/geometry").toByteArray());
    }
}

void MainWindow::saveSettings() {
    // Save window settings
    QSettings settings("PiChat", "GUI");
    settings.setValue("mainWindow/geometry", saveGeometry());
}
