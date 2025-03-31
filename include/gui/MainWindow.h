#pragma once

#include <QMainWindow>
#include <memory>
#include "../utils/DeepSeekAPI.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();
    void on_actionSettings_triggered();
    void on_actionClear_triggered();
    void on_actionExit_triggered();
    void onResponseReceived(const std::string& response);

private:
    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<DeepSeekAPI> api;
    std::vector<std::pair<std::string, std::string>> chatHistory;

    void setupConnections();
    void appendMessage(const QString& sender, const QString& message);
    void loadSettings();
    void saveSettings();
};
