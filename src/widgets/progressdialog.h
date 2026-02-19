// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(int total = 0, QWidget *parent = nullptr);
    ~ProgressDialog();

    bool isCancelled() const { return m_cancelled; }

Q_SIGNALS:
    void cancelled();

public Q_SLOTS:
    void proceed(int elapsed);
    void proceed();
    void setRange(int minimum, int maximum);
    void setValue(int value);

private:
    Ui::ProgressDialog *ui;

    int m_total;
    bool m_cancelled;
};

#endif // PROGRESSDIALOG_H
