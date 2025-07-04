// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsprogressdialog.h"
#include "ui_fpsprogressdialog.h"

fpsProgressDialog::fpsProgressDialog(QWidget *parent, int total)
    : QDialog(parent), ui(new Ui::fpsProgressDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background-color: qlineargradient(x1:0, y1:0.5, x2:1, y2:0.5, stop:0 #c5f3ff, "
                  "stop:1 #77b7fe); border-radius: 15%;");
    m_total = total;
    ui->progressBar->setRange(0, total);
}

fpsProgressDialog::~fpsProgressDialog()
{
    delete ui;
}

void fpsProgressDialog::proceed(int elapsed)
{
    ui->progressBar->setValue(elapsed);
    ui->labRemaining->setText(QString::number(m_total - elapsed));
}
