// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include "draggableline.h"

#include <QGraphicsView>
#include <QImage>
#include <QtCore/qvector.h>
#include <QtCore/qpointer.h>

class RulerBar;
class CornerBox;
class QMouseEvent;

constexpr double ZOOM_RATIO{ 1.2 }; // Ratio used when zooming in/out.

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QWidget *parent = nullptr);
    ~GraphicsView();

    void zoomIn();
    void zoomOut();

    void showPixmap(const QPixmap &pixmap,
                    bool adaptive = true); // Show an image on the GraphicsView.
    void clearScene();
    // Create a line using direction and initial position. pos -> whole GraphicsView
    void addDraggableLine(Qt::Orientation ori, const QPoint &pos);
    void addDraggableLine(DraggableLine *fl); // Add an existing line to list.
    void removeAllDraggableLines();
    const QVector<QPointer<DraggableLine>> &getDraggableLines() const { return m_plines; }

    void setRulersVisibility(bool visible)
    {
        m_hruler->setVisible(visible);
        m_vruler->setVisible(visible);
        m_box->setVisible(visible);
    }

public slots:
    void lineDestruction();

Q_SIGNALS:
    void positionChanged(int x, int y);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void scrollContentsBy(int dx, int dy) override;
    void updateRuler();

private slots:
    void dragStarted(const QPoint &startPos);
    void dragMoved(const QPoint &currentPos);
    void dragFinished(const QPoint &endPos, bool isReal);

private:
    RulerBar *m_hruler;
    RulerBar *m_vruler;
    CornerBox *m_box;
    QVector<QPointer<DraggableLine>> m_plines;

    DraggableLine *m_tempLine{ nullptr }; // Temporary line widget
    QPoint m_dragStartPos; // Position where dragging starts
};

#endif // GRAPHICSVIEW_H
