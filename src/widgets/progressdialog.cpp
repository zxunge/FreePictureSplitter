// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge

#include "progressdialog.h"
#include "ui_progressdialog.h"

#include <QMessageBox>
#include <qpushbutton.h>

using namespace Qt::Literals::StringLiterals;

ProgressDialog::ProgressDialog(int total, QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint), ui(new Ui::ProgressDialog), m_cancelled(false)
{
    ui->setupUi(this);
    ui->progressBar->setFormat(u"%p%"_s);
    ui->progressBar->setAlignment(Qt::AlignCenter);

    m_total = total;
    ui->progressBar->setRange(0, total);
    ui->labRemaining->setText(QString::number(m_total));

    connect(ui->btnCancel, &QPushButton::clicked, this, [this] {
        m_cancelled = true;
        Q_EMIT cancelled();
    });
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::proceed(int elapsed)
{
    ui->progressBar->setValue(elapsed);
    ui->labRemaining->setText(QString::number(m_total - elapsed));
}
