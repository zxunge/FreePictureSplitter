// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsbatchdialog.h"
#include "ui_fpsbatchdialog.h"

fpsBatchDialog::fpsBatchDialog(QWidget *parent) : QDialog(parent), ui(new Ui::fpsBatchDialog)
{
    ui->setupUi(this);
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

fpsBatchDialog::~fpsBatchDialog()
{
    delete ui;
}
