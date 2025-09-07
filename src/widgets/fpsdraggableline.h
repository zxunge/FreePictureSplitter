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

#ifndef FPSDRAGGABLELINE_H
#define FPSDRAGGABLELINE_H

#include "fpsrulerbar.h"
#include <QPushButton>
#include <qnamespace.h>
#include <qpoint.h>

class QMouseEvent;
class QEvent;
class QGraphicsView;

constexpr int LINE_SIZE{ 2 };

class fpsDraggableLine : public QPushButton
{
    Q_OBJECT
public:
    // According to Qt documentation, the following 2 'pos'es
    // refers to the position in whole GraphicsView.
    explicit fpsDraggableLine(Qt::Orientation orientation = Qt::Horizontal,
                              const QPoint &pos = QPoint(0, RULER_SIZE),
                              QGraphicsView *parent = nullptr);
    explicit fpsDraggableLine(QGraphicsView *parent = nullptr)
        : fpsDraggableLine(Qt::Horizontal, QPoint(0, RULER_SIZE), parent)
    {
    }
    explicit fpsDraggableLine(Qt::Orientation oritation = Qt::Horizontal,
                              QGraphicsView *parent = nullptr)
    {
        if (oritation == Qt::Horizontal)
            fpsDraggableLine(Qt::Horizontal, QPoint(0, RULER_SIZE), parent);
        else
            fpsDraggableLine(Qt::Vertical, QPoint(RULER_SIZE, 0), parent);
    }

    void updateLine(const QPoint &pos); // Move on GraphicsView and update scenePos; pos -> whole
                                        // GraphicsView

    void updateLine(int x, int y) { updateLine(QPoint(x, y)); } // Overloaded for convenience

    void updateLine(); // Update itself through existing scenePos

    int scenePos() const { return m_scenePos; }
    // We assume that you specify the correct value for different directions. pos -> scene
    void setScenePos(int pos);

    Qt::Orientation getOrientation() const { return m_orientation; }

    // An identifier, enabling us to find it if it is being destroyed.
    void setId(quint64 id) { m_id = id; }
    quint64 id() const { return m_id; }

signals:
    void userDestruction();

protected:
    // event->pos() -> fpsDraggableLine itself
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool event(QEvent *event) override;

private:
    bool m_pressed{ false };
    Qt::Orientation m_orientation;
    int m_scenePos; // Position in a scene (V:x or H:y)
    quint64 m_id; // An identifier, enabling us to find it.
};

#endif // FPSDRAGGABLELINE_H
