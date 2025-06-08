//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) 2024 2025 zxunge
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later


#include "fpsgraphicsview.h"
#include "fpsrulerbar.h"

#include <QMouseEvent>

fpsGraphicsView::fpsGraphicsView(QWidget *parent)
    : QGraphicsView(parent)
{
    m_hruler = new fpsRulerBar(this, Qt::Horizontal);
    m_vruler = new fpsRulerBar(this, Qt::Vertical);
    m_box = new fpsCornerBox(this);
    setViewport(new QWidget);

    setAttribute(Qt::WA_DeleteOnClose);
}

void fpsGraphicsView::setImage(QImage img)
{
    QGraphicsScene *scene { new QGraphicsScene };
    scene->addPixmap(QPixmap::fromImage(img));
    setScene(scene);
}

void fpsGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pt { mapToScene(event->pos()) };
    m_hruler->updatePosition(event->pos());
    m_vruler->updatePosition(event->pos());
    Q_EMIT positionChanged(pt.x(), pt.y());
    QGraphicsView::mouseMoveEvent(event);
}

void fpsGraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);

    this->setViewportMargins(RULER_SIZE - 1, RULER_SIZE - 1, 0, 0);
    m_hruler->resize(this->size().width() - RULER_SIZE - 1, RULER_SIZE);
    m_hruler->move(RULER_SIZE, 0);
    m_vruler->resize(RULER_SIZE, this->size().height() - RULER_SIZE - 1);
    m_vruler->move(0, RULER_SIZE);

    m_box->resize(RULER_SIZE, RULER_SIZE);
    m_box->move(0,0);
    updateRuler();
}

void fpsGraphicsView::scrollContentsBy(int dx, int dy)
{
    QGraphicsView::scrollContentsBy(dx,dy);
    updateRuler();
}

void fpsGraphicsView::updateRuler()
{
    if (!scene())
        return;

    QRectF viewbox { this->rect() };
    QPointF offset { mapFromScene(scene()->sceneRect().topLeft()) };
    double factor {  1.0 / transform().m11() };
    double lower_x { factor * (viewbox.left() - offset.x()) };
    double upper_x { factor * (viewbox.right() - RULER_SIZE - offset.x()) };
    m_hruler->setRange(lower_x, upper_x, upper_x - lower_x);
    m_hruler->update();

    double lower_y { -factor * (viewbox.top() - offset.y()) };
    double upper_y { -factor * (viewbox.bottom() - RULER_SIZE - offset.y()) };

    m_vruler->setRange(lower_y, upper_y, upper_y - lower_y);
    m_vruler->update();
}

void fpsGraphicsView::zoomIn()
{
    scale(1.2, 1.2);
    updateRuler();
}

void fpsGraphicsView::zoomOut()
{
    scale(1 / 1.2, 1 / 1.2);
    updateRuler();
}
