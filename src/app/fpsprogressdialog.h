// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef FPSPROGRESSDIALOG_H
#define FPSPROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class fpsProgressDialog;
}

class fpsProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit fpsProgressDialog(QWidget *parent = nullptr, int total = 0);
    ~fpsProgressDialog();

    bool isCancelled() const { return m_cancelled; }

signals:
    void cancelled();

public slots:
    void proceed(int elapsed);

private slots:
    void on_btnCancel_clicked();

private:
    Ui::fpsProgressDialog *ui;

    int m_total;
    bool m_cancelled;
};

#endif // FPSPROGRESSDIALOG_H
