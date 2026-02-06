// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "draggableline.h"

#include <QSizePolicy>
#include <QMouseEvent>
#include <QCursor>
#include <QGraphicsView>
#include <QColor>
#include <QPalette>
#include <QToolTip>
#include <QRandomGenerator64>

using namespace Qt::Literals::StringLiterals;

void DraggableLine::updateLine(const QPoint &pos)
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

void DraggableLine::updateLine()
{
    QGraphicsView *parent{ qobject_cast<QGraphicsView *>(parentWidget()) };
    // Moving according to the stored scenePos
    if (m_orientation == Qt::Horizontal) {
        resize(parentWidget()->width(), LINE_SIZE);
        move(0, parent->viewport()->mapTo(parent, parent->mapFromScene(0, m_scenePos)).y());
    } else {
        resize(LINE_SIZE, parentWidget()->height());
        move(parent->viewport()->mapTo(parent, parent->mapFromScene(m_scenePos, 0)).x(), 0);
    }
}

void DraggableLine::setScenePos(int pos)
{
    m_scenePos = pos;
    updateLine();
}

void DraggableLine::init(Qt::Orientation orientation, const QPoint &pos)
{
    QGraphicsView *parent{ qobject_cast<QGraphicsView *>(parentWidget()) };

    setAutoFillBackground(true);
    setStyleSheet(u"background-color: rgb(50, 205, 50);"_s);

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
    setId(QRandomGenerator64::global()->generate());
}

void DraggableLine::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_pressed = true;
    QPushButton::mousePressEvent(event);
}

void DraggableLine::mouseMoveEvent(QMouseEvent *event)
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

void DraggableLine::mouseReleaseEvent(QMouseEvent *event)
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

bool DraggableLine::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::Enter:
        setStyleSheet(u"background-color: rgba(50, 205, 50, 0.5);"_s); // Highlighting
        setCursor(QCursor(m_orientation == Qt::Horizontal ? Qt::SizeVerCursor : Qt::SizeHorCursor));
        QToolTip::showText(QCursor::pos(), QString::number(m_scenePos), this);
        break;
    case QEvent::Leave:
        setCursor(QCursor(Qt::ArrowCursor));
        setStyleSheet(u"background-color: rgb(50, 205, 50);"_s);
        break;
    default:
        break;
    }
    return QPushButton::event(event);
}
