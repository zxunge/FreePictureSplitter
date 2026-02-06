// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "graphicsview.h"
#include "utils/colors.h"

#include <QMouseEvent>
#include <QPixmap>
#include <QColor>

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    m_hruler = new RulerBar(this, Qt::Horizontal);
    m_vruler = new RulerBar(this, Qt::Vertical);
    m_box = new CornerBox(this);
    setRulersVisibility(false);

    setViewport(new QWidget);
    setMouseTracking(true);
    setAttribute(Qt::WA_DeleteOnClose);

    // Connected for dragging support
    connect(m_hruler, &RulerBar::dragStarted, this, &GraphicsView::dragStarted);
    connect(m_hruler, &RulerBar::dragMoved, this, &GraphicsView::dragMoved);
    connect(m_hruler, &RulerBar::dragFinished, this, &GraphicsView::dragFinished);
    connect(m_vruler, &RulerBar::dragStarted, this, &GraphicsView::dragStarted);
    connect(m_vruler, &RulerBar::dragMoved, this, &GraphicsView::dragMoved);
    connect(m_vruler, &RulerBar::dragFinished, this, &GraphicsView::dragFinished);
}

GraphicsView::~GraphicsView()
{
    // dtor
}

void GraphicsView::showPixmap(const QPixmap &pixmap, bool adaptive)
{
    if (scene())
        scene()->deleteLater();
    QGraphicsScene *scene{ new QGraphicsScene };
    scene->addPixmap(pixmap);
    setScene(scene);

    setRulersVisibility(true);
    updateRuler();
    removeAllDraggableLines();

    if (adaptive) { // Set an adaptive background for GraphicsView based on the image's color
        const QColor dominant{ Util::getDominantColor(pixmap) };
        const QColor bgColor{ Util::getContrastColor(dominant) };

        viewport()->setStyleSheet(QString("background-color: %1;").arg(bgColor.name()));
    }
}

void GraphicsView::clearScene()
{
    if (scene())
        scene()->deleteLater();
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);

    QPointF pt{ mapToScene(event->pos()) };
    m_hruler->updatePosition(event->pos());
    m_vruler->updatePosition(event->pos());
    Q_EMIT positionChanged(pt.x(), pt.y());
}

void GraphicsView::resizeEvent(QResizeEvent *event)
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

    Q_FOREACH (auto l, m_plines)
        l->updateLine();
}

void GraphicsView::scrollContentsBy(int dx, int dy)
{
    QGraphicsView::scrollContentsBy(dx, dy);
    updateRuler();
    Q_FOREACH (auto l, m_plines)
        l->updateLine();
}

void GraphicsView::updateRuler()
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

void GraphicsView::zoomIn()
{
    scale(ZOOM_RATIO, ZOOM_RATIO);
    updateRuler();
    Q_FOREACH (auto l, m_plines)
        l->updateLine();
}

void GraphicsView::zoomOut()
{
    scale(1 / ZOOM_RATIO, 1 / ZOOM_RATIO);
    updateRuler();
    Q_FOREACH (auto l, m_plines)
        l->updateLine();
}

void GraphicsView::addDraggableLine(Qt::Orientation orientation, const QPoint &pos)
{
    if (!scene())
        return;

    QPointer<DraggableLine> fl{ new DraggableLine(orientation, this) };
    fl->updateLine(pos);
    fl->show();
    connect(fl, &DraggableLine::userDestruction, this, &GraphicsView::lineDestruction);
    m_plines.push_back(fl);
}

void GraphicsView::addDraggableLine(DraggableLine *fl)
{
    if (!scene())
        return;

    connect(fl, &DraggableLine::userDestruction, this, &GraphicsView::lineDestruction);
    m_plines.push_back(fl);
}

void GraphicsView::removeAllDraggableLines()
{
    Q_FOREACH (auto l, m_plines)
        l->deleteLater();
    m_plines.clear();
}

void GraphicsView::dragStarted(const QPoint &startPos)
{
    if (!scene())
        return;

    m_dragStartPos = startPos;
    setCursor(QCursor(Qt::CrossCursor));

    // Create a temporary line
    if (qobject_cast<RulerBar *>(sender())->orientation() == Qt::Horizontal) {
        m_tempLine = new DraggableLine(Qt::Horizontal, this);
        m_tempLine->updateLine(0, startPos.y());
    } else {
        m_tempLine = new DraggableLine(Qt::Vertical, this);
        m_tempLine->updateLine(startPos.x(), 0);
    }

    m_tempLine->show();
}

void GraphicsView::dragMoved(const QPoint &currentPos)
{
    if (!m_tempLine)
        return;

    // Avoid being moved out of its parent
    int x{ qBound(0, currentPos.x(), width() - LINE_SIZE) };
    int y{ qBound(0, currentPos.y(), height() - LINE_SIZE) };

    if (qobject_cast<RulerBar *>(sender())->orientation() == Qt::Horizontal)
        m_tempLine->updateLine(0, y);
    else
        m_tempLine->updateLine(x, 0);
}

void GraphicsView::dragFinished(const QPoint &endPos, bool isReal)
{
    if (!m_tempLine)
        return;

    // In the receiver, ensure the line
    // is not moved out of the visualized part of the scene.
    if (!isReal || !scene()->sceneRect().contains(mapToScene(viewport()->mapFromParent(endPos)))) {
        // Clear temporary widget
        m_tempLine->deleteLater();
        m_tempLine = nullptr;
        setCursor(QCursor(Qt::ArrowCursor));
        return;
    }

    addDraggableLine(m_tempLine);

    // Set to null (not deleting)
    m_tempLine = nullptr;
    setCursor(QCursor(Qt::ArrowCursor));
}

void GraphicsView::lineDestruction()
{
    DraggableLine *sd{ qobject_cast<DraggableLine *>(sender()) };
    if (sd) {
        for (qsizetype i{}; i != m_plines.size(); ++i)
            if (m_plines[i]->id() == sd->id()) {
                m_plines[i]->deleteLater();
                m_plines.remove(i);
                break;
            }
    }
}
