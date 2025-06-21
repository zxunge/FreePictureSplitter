// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef FPSFLOATINGLINE_H
#define FPSFLOATINGLINE_H

#include "fpsrulerbar.h"

class QMouseEvent;
class QEvent;
class QGraphicsView;

constexpr int LINE_SIZE{ 2 };

class fpsFloatingLine : public QPushButton
{
    Q_OBJECT
public:
    // According to Qt documentation, the following 2 'pos'es
    // refers to the position in whole GraphicsView.
    explicit fpsFloatingLine(QGraphicsView *parent = nullptr,
                             Qt::Orientation orientation = Qt::Horizontal,
                             const QPoint &pos = QPoint(0, RULER_SIZE));

    void updateLine(const QPoint &pos); // Move on GraphicsView and update scenePos; pos -> whole
                                        // GraphicsView

    void updateLine(int x, int y) { updateLine(QPoint(x, y)); } // Overloaded for convenience

    void updateLine(); // Update itself through existing scenePos

    int getScenePos() const { return m_scenePos; }

    // We assume that you specify the correct value for different directions. pos -> scene
    void setScenePos(int pos);

    Qt::Orientation getOrientation() const { return m_orientation; }

protected:
    // event->pos() -> fpsFloatingLine itself
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool event(QEvent *event) override;

signals:
    void lineDestroyed();

private:
    bool m_pressed{ false };
    Qt::Orientation m_orientation;
    int m_scenePos; // Position in a scene (V:x or H:y)
};

#endif // FPSFLOATINGLINE_H
