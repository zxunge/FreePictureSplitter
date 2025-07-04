// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef FPSSETTINGSDIALOG_H
#define FPSSETTINGSDIALOG_H

#include <QDialog>
#include <QColor>

namespace Ui {
class fpsSettingsDialog;
}

class fpsSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit fpsSettingsDialog(QWidget *parent = nullptr);
    ~fpsSettingsDialog();

private slots:
    void on_buttonBox_accepted();

    void on_cbxLocation_currentIndexChanged(int index);

    void on_cbxFormats_currentTextChanged(const QString &text);

    void on_btnSelectColor_clicked();

    void on_buttonBox_rejected();

    void on_rbtnSpecified_toggled(bool checked);

    void on_chbGrid_toggled(bool checked);

private:
    Ui::fpsSettingsDialog *ui;

    QColor m_color;
};

#endif // FPSSETTINGSDIALOG_H
