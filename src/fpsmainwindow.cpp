//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsmainwindow.h"
#include "ui_fpsmainwindow.h"

fpsMainWindow::fpsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fpsMainWindow)
{
    ui->setupUi(this);
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

fpsMainWindow::~fpsMainWindow()
{
    delete ui;
}

void fpsMainWindow::on_actionOpen_triggered()
{

}

