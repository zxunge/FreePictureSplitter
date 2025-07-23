/*
 * This file is a part of FreePictureSplitter, a fast and easy-to-use picture splitter.
 * Copyright (c) 2024 2025 zxunge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsprogressdialog.h"
#include "ui_fpsprogressdialog.h"

using namespace Qt::Literals::StringLiterals;

fpsProgressDialog::fpsProgressDialog(QWidget *parent, int total)
    : QDialog(parent), ui(new Ui::fpsProgressDialog), m_cancelled(false)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    ui->progressBar->setFormat(u"%p%"_s);
    ui->progressBar->setAlignment(Qt::AlignCenter);

    m_total = total;
    ui->progressBar->setRange(0, total);
    ui->labRemaining->setText(QString::number(m_total));
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

void fpsProgressDialog::on_btnCancel_clicked()
{
    m_cancelled = true;
    Q_EMIT cancelled();
}
