// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpssettingsdialog.h"
#include "ui_fpssettingsdialog.h"

fpsSettingsDialog::fpsSettingsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::fpsSettingsDialog)
{
    ui->setupUi(this);
    ui->frColor->setAutoFillBackground(true);
    ui->frColor->setPalette(QPalette(Qt::green));
}

fpsSettingsDialog::~fpsSettingsDialog()
{
    delete ui;
}
