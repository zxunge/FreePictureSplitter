// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef FPSGRAPHICSVIEW_H
#define FPSGRAPHICSVIEW_H

#include "fpsfloatingline.h"

class fpsRulerBar;
class fpsCornerBox;
class QMouseEvent;

constexpr double ZOOM_RATIO{ 1.2 }; // Ratio used when zooming in/out.

class fpsGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    fpsGraphicsView(QWidget *parent);
    ~fpsGraphicsView();

    void zoomIn();
    void zoomOut();

    void showPixmap(const QPixmap &pixmap); // Show an image on the GraphicsView.
    // Create a line using direction and initial position. pos -> whole GraphicsView
    void addFloatingLine(Qt::Orientation ori, const QPoint &pos);
    void addFloatingLine(fpsFloatingLine *fl); // Add an existing line to list.
    void removeAllFloatingLines();

    QVector<QPointer<fpsFloatingLine>> getFloatingLines() const { return m_plines; }

signals:
    void positionChanged(int x, int y);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void scrollContentsBy(int dx, int dy) override;
    void updateRuler();

private:
    fpsRulerBar *m_hruler;
    fpsRulerBar *m_vruler;
    fpsCornerBox *m_box;
    QVector<QPointer<fpsFloatingLine>> m_plines;

private slots:
    void handleDragStarted(const QPoint &startPos);
    void handleDragMoved(const QPoint &currentPos);
    void handleDragFinished(const QPoint &endPos, bool isReal);

private:
    fpsFloatingLine *m_tempLine{ nullptr }; // Temporary line widget
    QPoint m_dragStartPos; // Position where dragging starts
};

#endif // FPSGRAPHICSVIEW_H
