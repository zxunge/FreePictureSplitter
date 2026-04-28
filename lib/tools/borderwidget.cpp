// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "borderwidget.h"

#include <QPainter>
#include <QPainterPath>

BorderWidget::BorderWidget(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_OpaquePaintEvent, false);
}

void BorderWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addRoundedRect(rect(), m_radius, m_radius);

    painter.fillPath(path, m_bgColor);
}
