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
// clang-format Language: Cpp

#ifndef FPSRULERBAR_H
#define FPSRULERBAR_H

#include <QWidget>

constexpr int MINIMUM_INCR{ 5 };
constexpr int RULER_SIZE{ 24 };

class fpsCornerBox : public QWidget
{
    Q_OBJECT
public:
    explicit fpsCornerBox(QWidget *parent = nullptr);

    void setFaceColor(const QColor &color)
    {
        m_faceColor = color;
        update();
    }
    QColor getFaceColor() const { return m_faceColor; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor m_faceColor;
};

class fpsRulerBar : public QWidget
{
    Q_OBJECT
public:
    explicit fpsRulerBar(QWidget *parent = nullptr, Qt::Orientation direction = Qt::Horizontal);

    void setFaceColor(const QColor &color)
    {
        m_faceColor = color;
        update();
    }
    QColor getFaceColor() const { return m_faceColor; }

    void setOrientation(Qt::Orientation orientation) { m_direction = orientation; }
    Qt::Orientation getOritation() const { return m_direction; }

    void setRange(double lower, double upper, double max_size);
    void updatePosition(const QPoint &pos);

protected:
    void paintEvent(QPaintEvent *event) override;
    void drawTicker(QPainter *painter);
    void drawPos(QPainter *painter);

signals:
    // Definitions of dragging signals:
    // Starting, moving and ending
    void dragStarted(const QPoint &startPos);
    void dragMoved(const QPoint &currentPos);
    void dragFinished(const QPoint &endPos, bool isReal);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    bool m_dragging{ false };
    bool m_moved{ false };
    QPoint m_dragStartPos;

protected:
    Qt::Orientation m_direction;
    QPoint m_lastPos;
    QColor m_faceColor;

    double m_lower;
    double m_upper;
    double m_maxsize;
};

#endif // FPSRULERBAR_H
