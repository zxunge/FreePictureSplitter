// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsrulerbar.h"
#include <QFrame>
#include <QPaintEvent>
#include <QPainter>
#include <cmath>
#include <array>

struct RulerMetric
{
    double ruler_scale[16];
    int    subdivide[5];
};

// Ruler metric for general use.
static const RulerMetric ruler_metric_general {
    { 1, 2, 5, 10, 25, 50, 100, 250, 500, 1'000, 2'500, 5'000, 10'000, 25'000,
      50'000, 100'000 },
    { 1, 5, 10, 50, 100 }
};

// Ruler metric for inch scales.
static const RulerMetric ruler_metric_inches { { 1, 2, 4, 8, 16, 32, 64, 128,
                                                 256, 512, 1'024, 2'048, 4'096,
                                                 8'192, 16'384, 32'768 },
                                               { 1, 2, 4, 8, 16 } };

fpsRulerBar::fpsRulerBar(QWidget *parent, Qt::Orientation direction)
    : QWidget(parent), m_faceColor(0xFF, 0xFF, 0xFF), m_lower {}, m_upper {},
      m_maxsize {}, m_lastPos { QPoint(0, 0) }, m_direction { direction }
{
    QFont f(font());
    f.setBold(false);
    f.setPixelSize(10);
    setFont(f);
    setMouseTracking(true);
}

void fpsRulerBar::setRange(double lower, double upper, double max_size)
{
    m_lower   = lower;
    m_upper   = upper;
    m_maxsize = max_size;
}

void fpsRulerBar::updatePosition(const QPoint &pos)
{
    m_lastPos = pos;
    update();
}

void fpsRulerBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    QRect    rulerRect { rect() };
    painter.fillRect(rulerRect, m_faceColor);

    if (m_direction == Qt::Horizontal)
        painter.drawLine(rulerRect.bottomLeft(), rulerRect.bottomRight());
    else
        painter.drawLine(rulerRect.topRight(), rulerRect.bottomRight());

    drawTicker(&painter);
    drawPos(&painter);
}

void fpsRulerBar::drawTicker(QPainter *painter)
{
    QRect allocation { this->rect() };
    int   width { (m_direction == Qt::Horizontal) ? allocation.width()
                                                  : allocation.height() },
        height { (m_direction == Qt::Horizontal) ? allocation.height()
                                                 : allocation.width() };
    int    length {}, ideal_length {};
    double lower { m_lower },
        upper { m_upper };  /* Upper and lower limits, in ruler units */
    double       increment; /* Number of pixels per unit */
    uint         scale;     /* Number of units per major unit */
    double       start, end, cur;
    char         unit_str[32];
    QFontMetrics fm(font());
    int          digit_height { fm.height() };
    int          text_size;
    int          pos;
    double       max_size { m_maxsize };
    RulerMetric  ruler_metric {
        ruler_metric_general
    }; /* The metric to use for this unit system */

    if ((upper - lower) == 0) return;

    increment = static_cast<double>(width) / (upper - lower);

    scale = ceil(max_size);
    sprintf(unit_str, "%d", scale);
    text_size = strlen(unit_str) * digit_height + 1;
    for (scale = 0; scale != std::size(ruler_metric.ruler_scale); scale++)
        if (ruler_metric.ruler_scale[scale] * fabs(increment) > 2 * text_size)
            break;
    if (scale == std::size(ruler_metric.ruler_scale))
        scale = std::size(ruler_metric.ruler_scale) - 1;

    for (int i { std::size(ruler_metric.subdivide) - 1 }; i >= 0; --i)
    {
        double subd_incr;
        if (scale == 1 && i == 1)
            subd_incr = 1.0;
        else
            subd_incr = static_cast<double>(ruler_metric.ruler_scale[scale]) /
                        ruler_metric.subdivide[i];

        if (subd_incr * fabs(increment) <= MINIMUM_INCR) continue;

        ideal_length = height / (i + 1) - 1;

        if (ideal_length > ++length) length = ideal_length;
        if (lower < upper)
        {
            start = floor(lower / subd_incr) * subd_incr;
            end   = ceil(upper / subd_incr) * subd_incr;
        }
        else
        {
            start = floor(upper / subd_incr) * subd_incr;
            end   = ceil(lower / subd_incr) * subd_incr;
        }

        int tick_index {};
        for (cur = start; cur <= end; cur += subd_incr)
        {
            pos = static_cast<int>(qRound((cur - lower) * increment + 1e-12));
            if (m_direction == Qt::Horizontal)
            {
                QRect rt(pos, height - length, 1, length);
                painter->drawLine(rt.topLeft(), rt.bottomLeft());
            }
            else
            {
                QRect rt(height - length, pos, length, 1);
                painter->drawLine(rt.topLeft(), rt.topRight());
            }

            double label_spacing_px { fabs(
                increment *
                static_cast<double>(ruler_metric.ruler_scale[scale]) /
                ruler_metric.subdivide[i]) };

            if (i == 0 &&
                (label_spacing_px > 6 * digit_height || tick_index % 2 == 0 ||
                 cur == 0) &&
                (label_spacing_px > 3 * digit_height || tick_index % 4 == 0 ||
                 cur == 0))
            {
                if (qAbs(static_cast<int>(cur)) >= 2'000 &&
                    (static_cast<int>(cur) / 1'000) * 1'000 ==
                        static_cast<int>(cur))
                    sprintf(unit_str, "%dk", static_cast<int>(cur) / 1'000);
                else
                    sprintf(unit_str, "%d", static_cast<int>(cur));

                int w { fm.horizontalAdvance(unit_str) };
                if (m_direction == Qt::Horizontal)
                    painter->drawText(pos + 2, allocation.top(), w, RULER_SIZE,
                                      Qt::AlignLeft | Qt::AlignTop, unit_str);
                else
                {
                    QRect textRect(-w / 2, -RULER_SIZE / 2, w, RULER_SIZE);
                    painter->save();
                    painter->translate(4, pos + w / 2 + 2);
                    painter->rotate(90);
                    painter->drawText(textRect, Qt::AlignRight, unit_str);
                    painter->restore();
                }
            }
            ++tick_index;
        }
    }
}

void fpsRulerBar::drawPos(QPainter *painter)
{
    int    x, y;
    int    width, height;
    int    bs_width, bs_height;
    QRect  allocation { this->rect() };
    double position;
    double lower { m_lower };
    double upper { m_upper };

    if (m_direction == Qt::Horizontal)
    {
        width      = allocation.width();
        height     = allocation.height();
        bs_width   = height / 2 + 2;
        bs_width  |= 1; /* make sure it's odd */
        bs_height  = bs_width / 2 + 1;
        position = lower + (upper - lower) * m_lastPos.x() / allocation.width();
    }
    else
    {
        width      = allocation.height();
        height     = allocation.width();
        bs_height  = width / 2 + 2;
        bs_height |= 1; /* make sure it's odd */
        bs_width   = bs_height / 2 + 1;
        position =
            lower + (upper - lower) * m_lastPos.y() / allocation.height();
    }
    if ((bs_width > 0) && (bs_height > 0))
    {
        double increment {};
        if (m_direction == Qt::Horizontal)
        {
            increment = static_cast<double>(width) / (upper - lower);
            x = qRound((position - lower) * increment) + bs_width / 2 - 1;
            y = (height + bs_height) / 2;
            painter->drawLine(m_lastPos.x(), 0, m_lastPos.x(), height);
        }
        else
        {
            increment = static_cast<double>(height) / (upper - lower);
            x         = (width + bs_width) / 2;
            y = qRound((position - lower) * increment) + (bs_height) / 2 - 1;
            painter->drawLine(0, m_lastPos.y(), width, m_lastPos.y());
        }
    }
}

void fpsRulerBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging     = true;
        m_dragStartPos = event->pos();         // Coordinate in fpsRulerBar
        Q_EMIT dragStarted(
            mapToParent(m_dragStartPos));      // Convert to the parent window
    }
    QWidget::mousePressEvent(event);
}

void fpsRulerBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging)
    {
        Q_EMIT dragMoved(
            mapToParent(event->pos()));      // Convert to the parent
        m_moved = true;                      // Avoid clicking only
    }

    QWidget::mouseMoveEvent(event);
}

void fpsRulerBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_dragging && m_moved && event->button() == Qt::LeftButton)
    {
        m_dragging = false;
        if ((m_direction == Qt::Horizontal && event->pos().y() > height()) ||
            (m_direction == Qt::Vertical && event->pos().x() > width()))
            Q_EMIT dragFinished(mapToParent(event->pos()),
                                true);      // Convert to the parent
        else
            Q_EMIT dragFinished(mapToParent(event->pos()), false);
    }
    else
        Q_EMIT dragFinished(mapToParent(event->pos()), false);
    QWidget::mouseReleaseEvent(event);
}

fpsCornerBox::fpsCornerBox(QWidget *parent) : QWidget(parent)
{
    // ctor
}

void fpsCornerBox::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);
    painter.fillRect(rect(), QColor(192, 192, 192));

    painter.setPen(Qt::DashLine);
    painter.drawLine(rect().center().x(), rect().top(), rect().center().x(),
                     rect().bottom());
    painter.drawLine(rect().left(), rect().center().y(), rect().right(),
                     rect().center().y());

    painter.drawLine(rect().topRight(), rect().bottomRight());
    painter.drawLine(rect().bottomLeft(), rect().bottomRight());
}
