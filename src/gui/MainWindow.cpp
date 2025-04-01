#include "include/gui/MainWindow.h"
#include "ui_MainWindow.h"
#include "include/gui/SettingsDialog.h"
#include <QScrollBar>
#include <QKeyEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    api(new DeepSeekAPI(this))
{
    ui->setupUi(this);
    setupConnections();

    // Display welcome message
    appendMessage("PiChat", "Welcome to PiChat! How can I help you today?");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupConnections() {
    // Connect send button
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::on_sendButton_clicked);

    // Connect API response signal
    connect(api, &DeepSeekAPI::responseReceived, this, &MainWindow::onResponseReceived);

    // Connect new chat button
    connect(ui->newChatButton, &QPushButton::clicked, this, [this]() {
        chatHistory.clear();
        ui->chatDisplay->clear();
        appendMessage("PiChat", "Starting a new conversation. How can I help you?");
        });

    // Connect settings button
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::on_actionSettings_triggered);

    // Connect menu actions
    connect(ui->actionNew_Chat, &QAction::triggered, ui->newChatButton, &QPushButton::click);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::on_actionSettings_triggered);
    connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);

    // Connect about action
    connect(ui->actionAbout, &QAction::triggered, this, [this]() {
        QMessageBox::about(this, "About PiChat",
            "PiChat v1.0\n\nAn intelligent chat assistant powered by DeepSeek AI.\n\n"
            "(C) 2023 PiChat Team");
        });

    // Monitor input text for height adjustments
    connect(ui->messageInput, &QTextEdit::textChanged, this, [this]() {
        // Limit text input area height
        if (ui->messageInput->document()->size().height() > 80) {
            ui->messageInput->setMaximumHeight(100);
        }
        else {
            ui->messageInput->setMaximumHeight(ui->messageInput->document()->size().height() + 20);
        }
        });

    // Install event filter for Enter key handling
    ui->messageInput->installEventFilter(this);
}

void MainWindow::on_sendButton_clicked() {
    QString message = ui->messageInput->toPlainText().trimmed();
    if (message.isEmpty()) return;

    // Display user message
    appendMessage("You", message);

    // Clear input field
    ui->messageInput->clear();

    // Send message to API
    api->sendMessage(message.toStdString(), chatHistory);

    // Update history for API
    chatHistory.push_back(Message("user", message.toStdString()));
}

void MainWindow::onResponseReceived(const QString& response) {
    // 显示助理回复 - 现在直接使用QString，无需转换
    appendMessage("PiChat", response);

    // 更新历史记录
    chatHistory.push_back(Message("assistant", response.toStdString()));
}
void MainWindow::appendMessage(const QString& sender, const QString& message) {
    QTextCursor cursor(ui->chatDisplay->document());
    cursor.movePosition(QTextCursor::End);

    QTextBlockFormat blockFormat;
    blockFormat.setTopMargin(10);
    cursor.insertBlock(blockFormat);

    QTextCharFormat senderFormat;
    senderFormat.setFontWeight(QFont::Bold);

    // 根据发送者设置不同的文本颜色
    if (sender == "You") {
        senderFormat.setForeground(QColor(42, 130, 218));
    }
    else {
        senderFormat.setForeground(QColor(0, 170, 127));
    }

    cursor.insertText(sender + ": ", senderFormat);

    QTextCharFormat messageFormat;
    cursor.insertText(message, messageFormat);

    // 滚动到最新消息
    ui->chatDisplay->verticalScrollBar()->setValue(ui->chatDisplay->verticalScrollBar()->maximum());
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (obj == ui->messageInput && event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return && !(keyEvent->modifiers() & Qt::ShiftModifier)) {
            on_sendButton_clicked();
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::on_actionSettings_triggered() {
    // Open settings dialog
    SettingsDialog dialog(this);
    dialog.exec();
}
