// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef ERRORLOGDIALOG_H
#define ERRORLOGDIALOG_H

#include <QDialog>
#include <tuple>

namespace Ui {
class ErrorLogDialog;
}

class ErrorLogDialog : public QDialog
{
    Q_OBJECT

public:
    using ErrorInfo = std::tuple<QString, QString>; // For object description and error message

    explicit ErrorLogDialog(QWidget *parent = nullptr);
    explicit ErrorLogDialog(const QList<ErrorInfo> &infolist, QWidget *parent = nullptr);
    ~ErrorLogDialog();

    void addErrorInfo(const QList<ErrorInfo> &infolist);
    QList<ErrorInfo> errorInfoList() const;

    int exec() override; // Use our custom exec() to decide whether to show or not

private:
    Ui::ErrorLogDialog *ui;
};

#endif // ERRORLOGDIALOG_H
