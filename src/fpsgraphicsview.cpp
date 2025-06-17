// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsgraphicsview.h"

#include <QMouseEvent>
#include "debugutil.h"

fpsGraphicsView::fpsGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    m_hruler = new fpsRulerBar(this, Qt::Horizontal);
    m_vruler = new fpsRulerBar(this, Qt::Vertical);
    m_box = new fpsCornerBox(this);
    setViewport(new QWidget);
    setMouseTracking(true);
    setAttribute(Qt::WA_DeleteOnClose);

    // Set background color
    QPalette palette{ viewport()->palette() };
    palette.setColor(QPalette::Base, QColor(192, 192, 192));
    viewport()->setPalette(palette);

    // Connected for dragging support
    connect(m_hruler, &fpsRulerBar::dragStarted, this, &fpsGraphicsView::handleDragStarted);
    connect(m_hruler, &fpsRulerBar::dragMoved, this, &fpsGraphicsView::handleDragMoved);
    connect(m_hruler, &fpsRulerBar::dragFinished, this, &fpsGraphicsView::handleDragFinished);
    connect(m_vruler, &fpsRulerBar::dragStarted, this, &fpsGraphicsView::handleDragStarted);
    connect(m_vruler, &fpsRulerBar::dragMoved, this, &fpsGraphicsView::handleDragMoved);
    connect(m_vruler, &fpsRulerBar::dragFinished, this, &fpsGraphicsView::handleDragFinished);
}

fpsGraphicsView::~fpsGraphicsView()
{
    for (auto l : m_plines)
        delete l;
}

void fpsGraphicsView::setImage(QImage img)
{
    QGraphicsScene *scene{ new QGraphicsScene };
    scene->addPixmap(QPixmap::fromImage(img));
    setScene(scene);
}

void fpsGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pt{ mapToScene(event->pos()) };
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
    double lower_x{ factor * (viewbox.left() - offset.x()) };
    double upper_x{ factor * (viewbox.right() - RULER_SIZE - offset.x()) };
    m_hruler->setRange(lower_x, upper_x, upper_x - lower_x);
    m_hruler->update();

    double lower_y{ -factor * (viewbox.top() - offset.y()) };
    double upper_y{ -factor * (viewbox.bottom() - RULER_SIZE - offset.y()) };

    m_vruler->setRange(lower_y, upper_y, upper_y - lower_y);
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

void fpsGraphicsView::addFloatingLine(Qt::Orientation orientation, const QPoint &pos)
{
    if (!scene())
        return;

    QPointer<fpsFloatingLine> fl{ new fpsFloatingLine(this, orientation) };
    fl->updateLine(pos);
    fl->show();
    m_plines.push_back(fl);
    int i{ m_plines.size() - 1 };
    connect(fl, &fpsFloatingLine::lineDestroyed, this, [fl, this, i]() {
        fl->deleteLater();
        this->m_plines.remove(i);
    });
}

void fpsGraphicsView::addFloatingLine(fpsFloatingLine *fl)
{
    if (!scene())
        return;

    m_plines.push_back(fl);
    int i{ m_plines.size() - 1 };
    connect(fl, &fpsFloatingLine::lineDestroyed, this, [fl, this, i]() {
        fl->deleteLater();
        this->m_plines.remove(i);
    });
}

void fpsGraphicsView::removeAllFloatingLines()
{
    for (auto l : m_plines)
        delete l;
    m_plines.clear();
}

void fpsGraphicsView::handleDragStarted(const QPoint &startPos)
{
    if (!scene())
        return;

    m_dragStartPos = startPos;
    setCursor(QCursor(Qt::CrossCursor));

    // Create a temporary line
    if (qobject_cast<fpsRulerBar *>(sender())->getOritation() == Qt::Horizontal) {
        m_tempLine = new fpsFloatingLine(this, Qt::Horizontal);
        m_tempLine->updateLine(0, startPos.y());
    } else {
        m_tempLine = new fpsFloatingLine(this, Qt::Vertical);
        m_tempLine->updateLine(startPos.x(), 0);
    }

    m_tempLine->show();
}

void fpsGraphicsView::handleDragMoved(const QPoint &currentPos)
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

void fpsGraphicsView::handleDragFinished(const QPoint &endPos, bool isReal)
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

    addFloatingLine(m_tempLine);

    // Set to null (not deleting)
    m_tempLine = nullptr;
    setCursor(QCursor(Qt::ArrowCursor));
}
