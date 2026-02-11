// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "errorlogdialog.h"
#include "ui_errorlogdialog.h"

#include <QTableWidgetItem>

ErrorLogDialog::ErrorLogDialog(QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint), ui(new Ui::ErrorLogDialog)
{
    ui->setupUi(this);
}

ErrorLogDialog::ErrorLogDialog(const QList<ErrorInfo> &infolist, QWidget *parent)
    : ErrorLogDialog(parent)
{
    addErrorInfo(infolist);
}

ErrorLogDialog::~ErrorLogDialog()
{
    delete ui;
}

void ErrorLogDialog::addErrorInfo(const QList<ErrorInfo> &infolist)
{
    Q_FOREACH (auto &info, infolist) {
        int curRows{ ui->tblErrors->rowCount() };
        ui->tblErrors->insertRow(curRows);
        ui->tblErrors->setItem(curRows, 0, new QTableWidgetItem(std::get<0>(info)));
        ui->tblErrors->setItem(curRows, 1, new QTableWidgetItem(std::get<1>(info)));
        ui->tblErrors->setItem(curRows, 2, new QTableWidgetItem(std::get<2>(info)));
    }
}

QList<ErrorLogDialog::ErrorInfo> ErrorLogDialog::errorInfoList() const
{
    QList<ErrorInfo> infolist;
    for (int row{}; row < ui->tblErrors->rowCount(); ++row)
        infolist.push_back(std::make_tuple(ui->tblErrors->item(row, 0)->text(),
                                           ui->tblErrors->item(row, 1)->text(),
                                           ui->tblErrors->item(row, 2)->text()));

    return infolist;
}
