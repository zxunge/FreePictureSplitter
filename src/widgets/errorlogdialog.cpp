// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "errorlogdialog.h"
#include "ui_errorlogdialog.h"
#include "globaldefs.h"

#include "utils/jsonconfigitems.h"

#include <QTableWidgetItem>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

using namespace Qt::Literals::StringLiterals;

ErrorLogDialog::ErrorLogDialog(QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint), ui(new Ui::ErrorLogDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, [&, this](QAbstractButton *button) {
        if (ui->buttonBox->standardButton(button) == QDialogButtonBox::SaveAll) {
            QString dir{ QFileDialog::getExistingDirectory(
                    this, tr("Choose a directory to save the error logs."),
                    g_appConfig.dialog.lastSavedToDir.empty()
                            ? QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation)
                            : QString::fromStdString(g_appConfig.dialog.lastSavedToDir)) };
            if (dir.isEmpty())
                return;
            g_appConfig.dialog.lastSavedToDir = dir.toStdString();

            // Save logs to file
            QFile file(dir % ERRORLOG_FILENAME);
            QTextStream ts(&file);
            if (file.open(QIODevice::ReadWrite | QIODevice::Truncate))
                for (int row{}; row < ui->tblErrors->rowCount(); ++row)
                    ts << ui->tblErrors->item(row, 0)->text() << u" "_s
                       << ui->tblErrors->item(row, 1)->text() << Qt::endl;
            else {
                QMessageBox::warning(
                        nullptr, fpsAppName,
                        QObject::tr("Error writing to file %1.").arg(dir % ERRORLOG_FILENAME),
                        QMessageBox::Close);
                return;
            }
            file.close();
        }
    });
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
    }
}

QList<ErrorLogDialog::ErrorInfo> ErrorLogDialog::errorInfoList() const
{
    QList<ErrorInfo> infolist;
    for (int row{}; row < ui->tblErrors->rowCount(); ++row)
        infolist.push_back(std::make_tuple(ui->tblErrors->item(row, 0)->text(),
                                           ui->tblErrors->item(row, 1)->text()));

    return infolist;
}

int ErrorLogDialog::exec()
{
    if (ui->tblErrors->rowCount() == 0) // No rows?
        return QDialog::Accepted;
    else
        return QDialog::exec();
}

void ErrorLogDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}
