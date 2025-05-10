//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsfloatingline.h"

fpsFloatingLine::fpsFloatingLine(QWidget *parent) : QPushButton(parent){}

void fpsFloatingLine::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        this->raise(); //将此按钮移动到顶层显示
        this->pressPoint = event->pos();
    }
}

void fpsFloatingLine::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton){
        this->move(this->mapToParent(event->pos() - this->pressPoint));

        //防止按钮移出父窗口
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
