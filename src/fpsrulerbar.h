// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
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

protected:
    void paintEvent(QPaintEvent *event) override;
};

class fpsRulerBar : public QWidget
{
    Q_OBJECT
public:
    explicit fpsRulerBar(QWidget *parent = nullptr, Qt::Orientation direction = Qt::Horizontal);

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
