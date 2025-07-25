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
