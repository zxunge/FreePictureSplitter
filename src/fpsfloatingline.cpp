//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) 2024 2025 zxunge
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsfloatingline.h"
#include <QSizePolicy>

fpsFloatingLine::fpsFloatingLine(QWidget *parent, Orientation orientation)
    : QPushButton(parent)
{
    setStyleSheet("background-color: rgb(0, 0, 0);");
    m_orientation = orientation;
    if (m_orientation == Horizontal)
    {
        setMaximumHeight(3);
        setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed));
    }
    else
    {
        setMaximumWidth(3);
        setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum));
    }
    raise(); // Move to top
}

void fpsFloatingLine::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        this->pressPoint = event->pos();
}

void fpsFloatingLine::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton){
        this->move(this->mapToParent(event->pos() - this->pressPoint));

        // Avoid being moved out of parent
        if(this->mapToParent(this->rect().topLeft()).x() <= 0){
            this->move(0, this->pos().y());
        }
        if(this->mapToParent(this->rect().bottomRight()).x() >= this->parentWidget()->rect().width()){
            this->move(this->parentWidget()->rect().width() - this->width(), this->pos().y());
        }
        if(this->mapToParent(this->rect().topLeft()).y() <= 0){
            this->move(this->pos().x(), 0);
        }
        if(this->mapToParent(this->rect().bottomRight()).y() >= this->parentWidget()->rect().height()){
            this->move(this->pos().x(), this->parentWidget()->rect().height() - this->height());
        }
    }
}

/* void fpsFloatingLine::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (m_orientation == Horizontal)
        setFixedSize(parentWidget()->width(), maximumHeight());
    else
        setFixedSize(maximumWidth(), parentWidget()->height());
} */
