// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef RULERBAR_H
#define RULERBAR_H

#include <QWidget>

constexpr int MINIMUM_INCR = 5;
constexpr int RULER_SIZE = 24;

namespace oclero::qlementine {
class Theme;
} // namespace oclero::qlementine

class CornerBox : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor faceColor READ faceColor WRITE setFaceColor)
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor)
public:
    explicit CornerBox(QWidget *parent = nullptr);

    void setFaceColor(const QColor &color)
    {
        m_faceColor = color;
        update();
    }
    QColor faceColor() const { return m_faceColor; }

    QColor lineColor() const;
    void setLineColor(const QColor &lineColor);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void themeChanged(const oclero::qlementine::Theme *theme);
    QColor m_faceColor;
    QColor m_lineColor;
};

class RulerBar : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor faceColor READ faceColor WRITE setFaceColor)
public:
    explicit RulerBar(QWidget *parent = nullptr, Qt::Orientation orientation = Qt::Horizontal);

    void setFaceColor(const QColor &color)
    {
        m_faceColor = color;
        update();
    }
    QColor faceColor() const { return m_faceColor; }

    void setOrientation(Qt::Orientation orientation) { m_orientation = orientation; }
    Qt::Orientation orientation() const { return m_orientation; }

    void setRange(double lower, double upper, double maxSize);
    void updatePosition(const QPoint &pos);

protected:
    void paintEvent(QPaintEvent *event) override;
    void drawTicker(QPainter *painter);
    void drawPos(QPainter *painter);

signals:
    /*!
     * \brief Definitions of dragging signals:
     * \brief Starting, moving and ending
     */
    void dragStarted(const QPoint &startPos);
    void dragMoved(const QPoint &currentPos);
    void dragFinished(const QPoint &endPos, bool isReal);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void themeChanged(const oclero::qlementine::Theme *theme);
    bool m_dragging = false;
    bool m_moved = false;
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