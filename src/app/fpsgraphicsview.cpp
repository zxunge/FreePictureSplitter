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

#include "fpsgraphicsview.h"
#include "colors.h"

#include <QMouseEvent>
#include <QPixmap>
#include <QColor>
#include <QRandomGenerator64>

fpsGraphicsView::fpsGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    m_hruler = new fpsRulerBar(this, Qt::Horizontal);
    m_vruler = new fpsRulerBar(this, Qt::Vertical);
    m_box = new fpsCornerBox(this);
    m_hruler->setVisible(false);
    m_vruler->setVisible(false);
    m_box->setVisible(false);

    setViewport(new QWidget);
    setMouseTracking(true);
    setAttribute(Qt::WA_DeleteOnClose);

    // Connected for dragging support
    connect(m_hruler, &fpsRulerBar::dragStarted, this, &fpsGraphicsView::dragStarted);
    connect(m_hruler, &fpsRulerBar::dragMoved, this, &fpsGraphicsView::dragMoved);
    connect(m_hruler, &fpsRulerBar::dragFinished, this, &fpsGraphicsView::dragFinished);
    connect(m_vruler, &fpsRulerBar::dragStarted, this, &fpsGraphicsView::dragStarted);
    connect(m_vruler, &fpsRulerBar::dragMoved, this, &fpsGraphicsView::dragMoved);
    connect(m_vruler, &fpsRulerBar::dragFinished, this, &fpsGraphicsView::dragFinished);
}

fpsGraphicsView::~fpsGraphicsView()
{
    // dtor
}

void fpsGraphicsView::showPixmap(const QPixmap &pixmap, bool adaptive)
{
    if (scene())
        delete scene();
    QGraphicsScene *scene{ new QGraphicsScene };
    scene->addPixmap(pixmap);
    setScene(scene);

    m_hruler->setVisible(true);
    m_vruler->setVisible(true);
    m_box->setVisible(true);
    updateRuler();
    removeAllDraggableLines();

    if (adaptive) { // Set an adaptive background for GraphicsView based on the image's color
        const QColor dominant{ Util::getDominantColorHSVWeighted(pixmap) };
        const QColor bgColor{ Util::getContrastColor(dominant) };

        viewport()->setStyleSheet(QString("background-color: %1;").arg(bgColor.name()));
    }
}

void fpsGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);

    QPointF pt{ mapToScene(event->pos()) };
    m_hruler->updatePosition(event->pos());
    m_vruler->updatePosition(event->pos());
    Q_EMIT positionChanged(pt.x(), pt.y());
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
    m_box->move(0, 0);
    updateRuler();

    for (auto l : m_plines)
        l->updateLine();
}

void fpsGraphicsView::scrollContentsBy(int dx, int dy)
{
    QGraphicsView::scrollContentsBy(dx, dy);
    updateRuler();
    for (auto l : m_plines)
        l->updateLine();
}

void fpsGraphicsView::updateRuler()
{
    if (!scene())
        return;

    QRectF viewbox{ this->rect() };
    QPointF offset{ mapFromScene(scene()->sceneRect().topLeft()) };
    double factor{ 1.0 / transform().m11() };
    double lowerX{ factor * (viewbox.left() - offset.x()) };
    double upperX{ factor * (viewbox.right() - RULER_SIZE - offset.x()) };
    m_hruler->setRange(lowerX, upperX, upperX - lowerX);
    m_hruler->update();

    double lowerY{ -factor * (viewbox.top() - offset.y()) };
    double upperY{ -factor * (viewbox.bottom() - RULER_SIZE - offset.y()) };

    m_vruler->setRange(lowerY, upperY, upperY - lowerY);
    m_vruler->update();
}

void fpsGraphicsView::zoomIn()
{
    scale(ZOOM_RATIO, ZOOM_RATIO);
    updateRuler();
    for (auto l : m_plines)
        l->updateLine();
}

void fpsGraphicsView::zoomOut()
{
    scale(1 / ZOOM_RATIO, 1 / ZOOM_RATIO);
    updateRuler();
    for (auto l : m_plines)
        l->updateLine();
}

void fpsGraphicsView::addDraggableLine(Qt::Orientation orientation, const QPoint &pos)
{
    if (!scene())
        return;

    QPointer<fpsDraggableLine> fl{ new fpsDraggableLine(this, orientation) };
    fl->updateLine(pos);
    fl->setId(QRandomGenerator64::global()->generate());
    fl->show();
    connect(fl, &fpsDraggableLine::userDestruction, this, &fpsGraphicsView::lineDestruction);
    m_plines.push_back(fl);
}

void fpsGraphicsView::addDraggableLine(fpsDraggableLine *fl)
{
    if (!scene())
        return;

    fl->setId(QRandomGenerator64::global()->generate());
    connect(fl, &fpsDraggableLine::userDestruction, this, &fpsGraphicsView::lineDestruction);
    m_plines.push_back(fl);
}

void fpsGraphicsView::removeAllDraggableLines()
{
    for (auto l : m_plines)
        l->deleteLater();
    m_plines.clear();
}

void fpsGraphicsView::dragStarted(const QPoint &startPos)
{
    if (!scene())
        return;

    m_dragStartPos = startPos;
    setCursor(QCursor(Qt::CrossCursor));

    // Create a temporary line
    if (qobject_cast<fpsRulerBar *>(sender())->getOritation() == Qt::Horizontal) {
        m_tempLine = new fpsDraggableLine(this, Qt::Horizontal);
        m_tempLine->updateLine(0, startPos.y());
    } else {
        m_tempLine = new fpsDraggableLine(this, Qt::Vertical);
        m_tempLine->updateLine(startPos.x(), 0);
    }

    m_tempLine->show();
}

void fpsGraphicsView::dragMoved(const QPoint &currentPos)
{
    if (!m_tempLine)
        return;

    // Avoid being moved out of its parent
    int x{ qBound(0, currentPos.x(), width() - LINE_SIZE) };
    int y{ qBound(0, currentPos.y(), height() - LINE_SIZE) };

    if (qobject_cast<fpsRulerBar *>(sender())->getOritation() == Qt::Horizontal)
        m_tempLine->updateLine(0, y);
    else
        m_tempLine->updateLine(x, 0);
}

void fpsGraphicsView::dragFinished(const QPoint &endPos, bool isReal)
{
    if (!m_tempLine)
        return;

    // In the receiver, ensure the line
    // is not moved out of the visualized part of the scene.
    if (!isReal || !scene()->sceneRect().contains(mapToScene(viewport()->mapFromParent(endPos)))) {
        // Clear temporary widget
        delete m_tempLine;
        m_tempLine = nullptr;
        setCursor(QCursor(Qt::ArrowCursor));
        return;
    }

    addDraggableLine(m_tempLine);

    // Set to null (not deleting)
    m_tempLine = nullptr;
    setCursor(QCursor(Qt::ArrowCursor));
}

void fpsGraphicsView::lineDestruction()
{
    fpsDraggableLine *sd{ qobject_cast<fpsDraggableLine *>(sender()) };
    if (sd) {
        for (qsizetype i{}; i != m_plines.size(); ++i)
            if (m_plines[i]->id() == sd->id()) {
                m_plines[i]->deleteLater();
                m_plines.remove(i);
                break;
            }
    }
}
