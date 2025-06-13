// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef FPSFLOATINGLINE_H
#define FPSFLOATINGLINE_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>

class fpsFloatingLine : public QPushButton
{
    Q_OBJECT
public:
    explicit fpsFloatingLine(QWidget *parent             = nullptr,
                             Qt::Orientation orientation = Qt::Horizontal);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    // void paintEvent(QPaintEvent *event) override;

private:
    QPoint          pressPoint;
    Qt::Orientation m_orientation;
};

#endif      // FPSFLOATINGLINE_H
