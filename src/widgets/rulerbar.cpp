// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "rulerbar.h"

#include <QFrame>
#include <QPaintEvent>
#include <QGraphicsView>
#include <QPainter>

#include <cmath>
#include <array>

struct RulerMetric
{
    double rulerScale[16];
    int subdivide[5];
};

// Ruler metric for general use.
static const RulerMetric rulerMetricGeneral{ { 1, 2, 5, 10, 25, 50, 100, 250, 500, 1'000, 2'500,
                                               5'000, 10'000, 25'000, 50'000, 100'000 },
                                             { 1, 5, 10, 50, 100 } };

// Ruler metric for inch scales.
static const RulerMetric rulerMetricInches{ { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1'024, 2'048,
                                              4'096, 8'192, 16'384, 32'768 },
                                            { 1, 2, 4, 8, 16 } };

RulerBar::RulerBar(QWidget *parent, Qt::Orientation orientation)
    : QWidget{ parent },
      m_faceColor{ parent->palette().color(QPalette::Window) },
      m_lower{},
      m_upper{},
      m_maxSize{},
      m_lastPos{ QPoint(0, 0) },
      m_orientation{ orientation }
{
    QFont f(font());
    f.setBold(false);
    f.setPixelSize(10);
    setFont(f);
    setMouseTracking(true);
}

void RulerBar::setRange(double lower, double upper, double maxSize)
{
    m_lower = lower;
    m_upper = upper;
    m_maxSize = maxSize;
}

void RulerBar::updatePosition(const QPoint &pos)
{
    m_lastPos = pos;
    update();
}

void RulerBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QRect rulerRect{ rect() };
    painter.fillRect(rulerRect, m_faceColor);

    if (m_orientation == Qt::Horizontal) {
        painter.drawLine(rulerRect.bottomLeft(), rulerRect.bottomRight());
        painter.drawLine(rulerRect.topLeft(), rulerRect.topRight());
        painter.drawLine(rulerRect.topLeft(), rulerRect.bottomLeft());
    } else {
        painter.drawLine(rulerRect.topRight(), rulerRect.bottomRight());
        painter.drawLine(rulerRect.topLeft(), rulerRect.bottomLeft());
        painter.drawLine(rulerRect.topLeft(), rulerRect.topRight());
    }

    drawTicker(&painter);
    drawPos(&painter);
}

void RulerBar::drawTicker(QPainter *painter)
{
    QRect allocation{ rect() };
    int width{ (m_orientation == Qt::Horizontal) ? allocation.width() : allocation.height() },
            height{ (m_orientation == Qt::Horizontal) ? allocation.height() : allocation.width() };
    int length{}, idealLength{};
    double lower{ m_lower }, upper{ m_upper }; // Upper and lower limits, in ruler units
    double increment{}; // Number of pixels per unit
    unsigned int scale{}; // Number of units per major unit
    double start{}, end{}, cur{};
    char unitStr[32]{};
    QFontMetrics fm(font());
    int digitHeight{ fm.height() };
    int textSize{};
    int pos{};
    RulerMetric rulerMetric{ rulerMetricGeneral }; // The metric to use for this unit system

    if (upper == lower)
        return;

    increment = static_cast<double>(width) / (upper - lower);

    scale = ceil(m_maxSize);
    sprintf(unitStr, "%d", scale);
    textSize = strlen(unitStr) * digitHeight + 1;
    for (scale = 0; scale != std::size(rulerMetric.rulerScale); ++scale)
        if (rulerMetric.rulerScale[scale] * fabs(increment) > 2 * textSize)
            break;

    if (scale == std::size(rulerMetric.rulerScale))
        scale = std::size(rulerMetric.rulerScale) - 1;

    for (int i{ std::size(rulerMetric.subdivide) - 1 }; i >= 0; --i) {
        double subdIncr{};
        if (scale == 1 && i == 1)
            subdIncr = 1.0;
        else
            subdIncr =
                    static_cast<double>(rulerMetric.rulerScale[scale]) / rulerMetric.subdivide[i];

        if (subdIncr * fabs(increment) <= MINIMUM_INCR)
            continue;

        idealLength = height / (i + 1) - 1;

        if (idealLength > ++length)
            length = idealLength;
        if (lower < upper) {
            start = floor(lower / subdIncr) * subdIncr;
            end = ceil(upper / subdIncr) * subdIncr;
        } else {
            start = floor(upper / subdIncr) * subdIncr;
            end = ceil(lower / subdIncr) * subdIncr;
        }

        int tick_index{};
        for (cur = start; cur <= end; cur += subdIncr) {
            pos = static_cast<int>(qRound((cur - lower) * increment + 1e-12));
            if (m_orientation == Qt::Horizontal) {
                QRect rt(pos, height - length, 1, length);
                painter->drawLine(rt.topLeft(), rt.bottomLeft());
            } else {
                QRect rt(height - length, pos, length, 1);
                painter->drawLine(rt.topLeft(), rt.topRight());
            }

            double pxLabelSpacing{ fabs(increment
                                        * static_cast<double>(rulerMetric.rulerScale[scale])
                                        / rulerMetric.subdivide[i]) };

            if (i == 0 && (pxLabelSpacing > 6 * digitHeight || tick_index % 2 == 0 || cur == 0)
                && (pxLabelSpacing > 3 * digitHeight || tick_index % 4 == 0 || cur == 0)) {
                if (qAbs(static_cast<int>(cur)) >= 2'000
                    && (static_cast<int>(cur) / 1'000) * 1'000 == static_cast<int>(cur))
                    sprintf(unitStr, "%dk", static_cast<int>(cur) / 1'000);
                else
                    sprintf(unitStr, "%d", static_cast<int>(cur));

                int w{ fm.horizontalAdvance(unitStr) };
                if (m_orientation == Qt::Horizontal)
                    painter->drawText(pos + 2, allocation.top(), w, RULER_SIZE,
                                      Qt::AlignLeft | Qt::AlignTop, unitStr);
                else {
                    QRect textRect(-w / 2, -RULER_SIZE / 2, w, RULER_SIZE);
                    painter->save();
                    painter->translate(4.0, pos + w / 2.0 + 2.0);
                    painter->rotate(90);
                    painter->drawText(textRect, Qt::AlignRight, unitStr);
                    painter->restore();
                }
            }
            ++tick_index;
        }
    }
}

void RulerBar::drawPos(QPainter *painter)
{
    int x{}, y{};
    int width{}, height{};
    int bsWidth{}, bsHeight{};
    QRect allocation{ rect() };
    double position{};
    double lower{ m_lower };
    double upper{ m_upper };

    if (m_orientation == Qt::Horizontal) {
        width = allocation.width();
        height = allocation.height();
        bsWidth = height / 2 + 2;
        bsWidth |= 1; // Make sure it's odd
        bsHeight = bsWidth / 2 + 1;
        position = lower + (upper - lower) * m_lastPos.x() / allocation.width();
    } else {
        width = allocation.height();
        height = allocation.width();
        bsHeight = width / 2 + 2;
        bsHeight |= 1; // Make sure it's odd
        bsWidth = bsHeight / 2 + 1;
        position = lower + (upper - lower) * m_lastPos.y() / allocation.height();
    }
    if ((bsWidth > 0) && (bsHeight > 0)) {
        double increment{};
        if (m_orientation == Qt::Horizontal) {
            increment = static_cast<double>(width) / (upper - lower);
            x = qRound((position - lower) * increment) + bsWidth / 2 - 1;
            y = (height + bsHeight) / 2;
            painter->drawLine(m_lastPos.x(), 0, m_lastPos.x(), height);
        } else {
            increment = static_cast<double>(height) / (upper - lower);
            x = (width + bsWidth) / 2;
            y = qRound((position - lower) * increment) + (bsHeight) / 2 - 1;
            painter->drawLine(0, m_lastPos.y(), width, m_lastPos.y());
        }
    }
}

void RulerBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragStartPos = event->pos(); // Coordinate in RulerBar
        Q_EMIT dragStarted(mapToParent(m_dragStartPos)); // Convert to the parent window
    }
    QWidget::mousePressEvent(event);
}

void RulerBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging) {
        Q_EMIT dragMoved(mapToParent(event->pos())); // Convert to the parent
        m_moved = true; // Avoid clicking only
    }

    QWidget::mouseMoveEvent(event);
}

void RulerBar::mouseReleaseEvent(QMouseEvent *event)
{
    // Ensure that the line is moved out of ruler bar
    if (m_dragging && m_moved && event->button() == Qt::LeftButton) {
        m_dragging = false;
        if ((m_orientation == Qt::Horizontal && event->pos().y() >= height())
            || (m_orientation == Qt::Vertical && event->pos().x() >= width()))
            Q_EMIT dragFinished(mapToParent(event->pos()),
                                true); // Convert to the parent
        else
            Q_EMIT dragFinished(mapToParent(event->pos()), false);
    } else
        Q_EMIT dragFinished(mapToParent(event->pos()), false);
    QWidget::mouseReleaseEvent(event);
}

CornerBox::CornerBox(QWidget *parent)
    : QWidget{ parent }, m_faceColor{ parent->palette().color(QPalette::Window) }
{
    // ctor
}

void CornerBox::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.fillRect(rect(), m_faceColor);

    painter.setPen(Qt::DashLine);
    painter.drawLine(rect().center().x(), rect().top(), rect().center().x(), rect().bottom());
    painter.drawLine(rect().left(), rect().center().y(), rect().right(), rect().center().y());
}
