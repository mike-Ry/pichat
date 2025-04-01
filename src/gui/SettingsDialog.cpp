// src/gui/SettingsDialog.cpp
#include "include/gui/SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "include/config/ConfigManager.h"

SettingsDialog::SettingsDialog(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    loadSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::loadSettings() {
    ConfigManager& configManager = ConfigManager::getInstance();

    // 假设我们有一个API Key输入框
    if (ui->apiKeyEdit) {
        ui->apiKeyEdit->setText(QString::fromStdString(configManager.getApiKey()));
    }
}

void SettingsDialog::saveSettings() {
    ConfigManager& configManager = ConfigManager::getInstance();

    // 假设我们有一个API Key输入框
    if (ui->apiKeyEdit) {
        configManager.setApiKey(ui->apiKeyEdit->text().toStdString());
    }
}

void SettingsDialog::on_saveButton_clicked() {
    saveSettings();
    accept();
}

void SettingsDialog::on_cancelButton_clicked() {
    reject();
}
