#include "include/gui/SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "include/config/ConfigManager.h"

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog) {

    ui->setupUi(this);

    // Load existing settings
    ConfigManager& config = ConfigManager::getInstance();
    ui->apiKeyEdit->setText(QString::fromStdString(config.getApiKey()));
}

SettingsDialog::~SettingsDialog() = default;

void SettingsDialog::on_buttonBox_accepted() {
    // Save settings
    ConfigManager& config = ConfigManager::getInstance();
    config.setApiKey(ui->apiKeyEdit->text().trimmed().toStdString());

    accept();
}

void SettingsDialog::on_buttonBox_rejected() {
    reject();
}
