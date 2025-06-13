// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef FPSSETTINGSDIALOG_H
#define FPSSETTINGSDIALOG_H

#include <QDialog>

namespace Ui
{
class fpsSettingsDialog;
}

class fpsSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit fpsSettingsDialog(QWidget *parent = nullptr);
    ~fpsSettingsDialog();

private:
    Ui::fpsSettingsDialog *ui;
};

#endif      // FPSSETTINGSDIALOG_H
