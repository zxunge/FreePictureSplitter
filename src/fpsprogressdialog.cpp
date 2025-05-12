//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsprogressdialog.h"
#include "ui_fpsprogressdialog.h"

fpsProgressDialog::fpsProgressDialog(QWidget *parent, int total) :
    QDialog(parent),
    ui(new Ui::fpsProgressDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background-color: rgb(196,170,192);");
    m_total = total;
    ui->progressBar->setRange(0, total);
}

fpsProgressDialog::~fpsProgressDialog()
{
    delete ui;
}

void fpsProgressDialog::on_proceed(int elapsed)
{
    ui->progressBar->setValue(elapsed);
    ui->labRemaining->setText(QString::number(m_total - elapsed));
}
