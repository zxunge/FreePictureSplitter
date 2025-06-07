//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) 2024 2025 zxunge
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FPSRULERBAR_H
#define FPSRULERBAR_H

#include <QWidget>

class fpsCornerBox : public QWidget
{
    Q_OBJECT
public:
    explicit fpsCornerBox(QWidget * parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
};

class fpsRulerBar : public QWidget
{
    Q_OBJECT
public:
    explicit fpsRulerBar(QWidget *parent = nullptr, Qt::Orientation direction = Qt::Horizontal);

    void setOrientation(Qt::Orientation orientation);
    void setRange(double lower, double upper, double max_size);
    void updatePosition(const QPoint & pos);

protected:
    void paintEvent(QPaintEvent *event) override;
    void drawTicker(QPainter *painter);
    void drawPos(QPainter *painter);

protected:
    Qt::Orientation m_direction;
    QPoint m_lastPos;
    QColor m_faceColor;

    double m_lower;
    double m_upper;
    double m_maxsize;

};

#endif // FPSRULERBAR_H
