// include/gui/MainWindow.h
#pragma once

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QMessageBox>
#include "include/common/Message.h"
#include "include/utils/DeepSeekAPI.h"

class SettingsDialog;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void appendMessage(const QString& sender, const QString& message);

private slots:
    void on_sendButton_clicked();
    void onResponseReceived(const QString& response);
    void on_actionSettings_triggered();

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    void setupConnections();

    Ui::MainWindow* ui;
    DeepSeekAPI* api;
    std::vector<Message> chatHistory;
};
