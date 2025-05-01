//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FPSBATCHDIALOG_H
#define FPSBATCHDIALOG_H

#include <QDialog>

namespace Ui {
class fpsBatchDialog;
}

class fpsBatchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit fpsBatchDialog(QWidget *parent = nullptr);
    ~fpsBatchDialog();

private:
    Ui::fpsBatchDialog *ui;
};

#endif // FPSBATCHDIALOG_H
