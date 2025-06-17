// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsfloatingline.h"
#include "debugutil.h"

#include <QSizePolicy>
#include <QMouseEvent>
#include <QCursor>
#include <QGraphicsView>

fpsFloatingLine::fpsFloatingLine(QGraphicsView *parent, Qt::Orientation orientation,
                                 const QPoint &pos)
    : QPushButton(parent)
{
    setStyleSheet("background-color: rgb(50, 205, 50);");
    m_orientation = orientation;
    if (m_orientation == Qt::Horizontal) {
        setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
        resize(parent->width(), LINE_SIZE);
        move(0, pos.y());
        m_scenePos = parent->mapToScene(parent->viewport()->mapFrom(parent, pos)).toPoint().y();
    } else {
        setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding));
        resize(LINE_SIZE, parent->height());
        move(pos.x(), 0);
        m_scenePos = parent->mapToScene(parent->viewport()->mapFrom(parent, pos)).toPoint().x();
    }
    raise(); // Move to top
}

void fpsFloatingLine::updateLine(const QPoint &pos)
{
    QGraphicsView *parent{ qobject_cast<QGraphicsView *>(parentWidget()) };
    if (m_orientation == Qt::Horizontal) {
        resize(parentWidget()->width(), LINE_SIZE);
        m_scenePos = parent->mapToScene(parent->viewport()->mapFrom(parent, pos)).toPoint().y();
        move(0, pos.y());
    } else {
        resize(LINE_SIZE, parentWidget()->height());
        m_scenePos = parent->mapToScene(parent->viewport()->mapFrom(parent, pos)).toPoint().x();
        move(pos.x(), 0);
    }
}

void fpsFloatingLine::updateLine()
{
    QGraphicsView *parent{ qobject_cast<QGraphicsView *>(parentWidget()) };
    // Moving through the stored scenePos
    if (m_orientation == Qt::Horizontal) {
        resize(parentWidget()->width(), LINE_SIZE);
        move(0, parent->viewport()->mapTo(parent, parent->mapFromScene(0, m_scenePos)).y());
    } else {
        resize(LINE_SIZE, parentWidget()->height());
        move(parent->viewport()->mapTo(parent, parent->mapFromScene(m_scenePos, 0)).x(), 0);
    }
}

void fpsFloatingLine::setScenePos(int pos)
{
    m_scenePos = pos;
    updateLine();
}

void fpsFloatingLine::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_pressed = true;
    QPushButton::mousePressEvent(event);
}

void fpsFloatingLine::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        QGraphicsView *parent{ qobject_cast<QGraphicsView *>(parentWidget()) };
        if (m_pressed && m_orientation == Qt::Horizontal) {
            move(0, mapToParent(event->pos()).y());
            m_scenePos =
                    parent->mapToScene(parent->viewport()->mapFromParent(mapToParent(event->pos())))
                            .y();
        } else if (m_pressed && m_orientation == Qt::Vertical) {
            move(mapToParent(event->pos()).x(), 0);
            m_scenePos =
                    parent->mapToScene(parent->viewport()->mapFromParent(mapToParent(event->pos())))
                            .x();
        }
    }
    QPushButton::mouseMoveEvent(event);
}

void fpsFloatingLine::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QGraphicsView *parent{ qobject_cast<QGraphicsView *>(parentWidget()) };
        if (m_pressed && m_orientation == Qt::Horizontal) {
            m_scenePos =
                    parent->mapToScene(parent->viewport()->mapFromParent(mapToParent(event->pos())))
                            .y();
            if (m_scenePos >= parent->scene()->height() || m_scenePos <= 0)
                Q_EMIT lineDestroyed();
            move(0, mapToParent(event->pos()).y());
            m_pressed = false;
        } else if (m_pressed && m_orientation == Qt::Vertical) {
            m_scenePos =
                    parent->mapToScene(parent->viewport()->mapFromParent(mapToParent(event->pos())))
                            .x();
            if (m_scenePos >= parent->scene()->width() || m_scenePos <= 0)
                Q_EMIT lineDestroyed();
            move(mapToParent(event->pos()).x(), 0);
            m_pressed = false;
        }
    }
    QPushButton::mouseReleaseEvent(event);
}

bool fpsFloatingLine::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        setStyleSheet("background-color: rgba(50, 205, 50, 0.5);"); // Highlighting
        setCursor(QCursor(m_orientation == Qt::Horizontal ? Qt::SizeVerCursor : Qt::SizeHorCursor));
        break;
    case QEvent::Leave:
        setCursor(QCursor(Qt::ArrowCursor));
        setStyleSheet("background-color: rgb(50, 205, 50);");
        break;
    default:
        break;
    }
    return QPushButton::event(event);
}
