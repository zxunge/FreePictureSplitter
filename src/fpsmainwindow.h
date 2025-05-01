//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FPSMAINWINDOW_H
#define FPSMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class fpsMainWindow;
}

class fpsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit fpsMainWindow(QWidget *parent = nullptr);
    ~fpsMainWindow();

private slots:
    void on_actionOpen_triggered();

private:
    Ui::fpsMainWindow *ui;
};

#endif // FPSMAINWINDOW_H
