// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(int total = 0, QWidget *parent = nullptr);
    ~ProgressDialog();

    bool isCancelled() const { return m_cancelled; }

signals:
    void cancelled();

public slots:
    void proceed(int elapsed);

private:
    Ui::ProgressDialog *ui;

    int m_total;
    bool m_cancelled;
};

#endif // PROGRESSDIALOG_H
