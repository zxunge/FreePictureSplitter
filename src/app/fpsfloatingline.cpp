/*
 * This file is a part of FreePictureSplitter, a fast and easy-to-use picture splitter.
 * Copyright (c) 2024 2025 zxunge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsfloatingline.h"

#include <QSizePolicy>
#include <QMouseEvent>
#include <QCursor>
#include <QGraphicsView>
#include <QColor>
#include <QPalette>

fpsFloatingLine::fpsFloatingLine(QGraphicsView *parent, Qt::Orientation orientation,
                                 const QPoint &pos)
    : QPushButton(parent)
{
    setAutoFillBackground(true);
    setPalette(QPalette(QColor(50, 205, 50)));

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
                Q_EMIT userDestruction();
            move(0, mapToParent(event->pos()).y());
            m_pressed = false;
        } else if (m_pressed && m_orientation == Qt::Vertical) {
            m_scenePos =
                    parent->mapToScene(parent->viewport()->mapFromParent(mapToParent(event->pos())))
                            .x();
            if (m_scenePos >= parent->scene()->width() || m_scenePos <= 0)
                Q_EMIT userDestruction();
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
