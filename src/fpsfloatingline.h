//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FPSFLOATINGLINE_H
#define FPSFLOATINGLINE_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>

class fpsFloatingLine : public QPushButton
{
    Q_OBJECT
public:
    explicit fpsFloatingLine(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QPoint pressPoint;
};

#endif // FPSFLOATINGLINE_H
