/*
 * This file is part of FreePictureSplitter, a fast and easy-to-use picture splitter.
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
    // Create a line using direction and initial position. pos -> whole GraphicsView
    void addDraggableLine(Qt::Orientation ori, const QPoint &pos);
    void addDraggableLine(DraggableLine *fl); // Add an existing line to list.
    void removeAllDraggableLines();

    const QVector<QPointer<DraggableLine>> &getDraggableLines() const { return m_plines; }

public slots:
    void lineDestruction();

signals:
    void positionChanged(int x, int y);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void scrollContentsBy(int dx, int dy) override;
    void updateRuler();

private:
    RulerBar *m_hruler;
    RulerBar *m_vruler;
    CornerBox *m_box;
    QVector<QPointer<DraggableLine>> m_plines;

private slots:
    void dragStarted(const QPoint &startPos);
    void dragMoved(const QPoint &currentPos);
    void dragFinished(const QPoint &endPos, bool isReal);

private:
    DraggableLine *m_tempLine{ nullptr }; // Temporary line widget
    QPoint m_dragStartPos; // Position where dragging starts
};

#endif // GRAPHICSVIEW_H
