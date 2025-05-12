//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

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

public slots:
    void on_proceed(int elapsed);

private:
    Ui::fpsProgressDialog *ui;
    int m_total;
};

#endif // FPSPROGRESSDIALOG_H
