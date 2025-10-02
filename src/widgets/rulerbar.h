// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef RULERBAR_H
#define RULERBAR_H

#include <QWidget>

constexpr int MINIMUM_INCR{ 5 };
constexpr int RULER_SIZE{ 24 };

class CornerBox : public QWidget
{
    Q_OBJECT
public:
    explicit CornerBox(QWidget *parent = nullptr);

    void setFaceColor(const QColor &color)
    {
        m_faceColor = color;
        update();
    }
    QColor faceColor() const { return m_faceColor; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QColor m_faceColor;
};

class RulerBar : public QWidget
{
    Q_OBJECT
public:
    explicit RulerBar(QWidget *parent = nullptr, Qt::Orientation orientation = Qt::Horizontal);

    void setFaceColor(const QColor &color)
    {
        m_faceColor = color;
        update();
    }
    QColor faceColor() const { return m_faceColor; }

    void setOrientation(Qt::Orientation orientation) { m_orientation = orientation; }
    Qt::Orientation getOrientation() const { return m_orientation; }

    void setRange(double lower, double upper, double maxSize);
    void updatePosition(const QPoint &pos);

protected:
    void paintEvent(QPaintEvent *event) override;
    void drawTicker(QPainter *painter);
    void drawPos(QPainter *painter);

Q_SIGNALS:
    // Definitions of dragging Q_SIGNALS:
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
    Qt::Orientation m_orientation;
    QPoint m_lastPos;
    QColor m_faceColor;

    double m_lower;
    double m_upper;
    double m_maxSize;
};

#endif // RULERBAR_H
