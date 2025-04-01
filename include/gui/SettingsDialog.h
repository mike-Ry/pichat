// include/gui/SettingsDialog.h
#pragma once

#include <QDialog>
#include <QString>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog();

private slots:
    void on_saveButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::SettingsDialog* ui;
    void loadSettings();
    void saveSettings();
};
