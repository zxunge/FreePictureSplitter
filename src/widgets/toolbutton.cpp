// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "toolbutton.h"

#include <QPainter>
#include <QEvent>
#include <QEnterEvent>
#include <QEasingCurve>

using namespace Qt::Literals::StringLiterals;

ToolButton::ToolButton(QWidget *parent)
    : QToolButton(parent), m_animation(new QPropertyAnimation(this, "hoverOpacity"_ba))
{
    setMouseTracking(true);
    m_animation->setEasingCurve(QEasingCurve::InOutQuad);
    m_animation->setDuration(200);
    setStyleSheet(u"border: none;"_s);
    connect(m_animation, &QPropertyAnimation::valueChanged, this,
            QOverload<>::of(&QWidget::update));
}

ToolButton::~ToolButton()
{
    delete m_animation;
}

void ToolButton::setHoverOpacity(qreal opacity)
{
    if (qFuzzyCompare(m_hoverOpacity, opacity))
        return;
    m_hoverOpacity = opacity;
    update();
}

void ToolButton::setOverlayColor(const QColor &color)
{
    if (m_overlayColor == color)
        return;
    m_overlayColor = color;
    update();
}

void ToolButton::enterEvent(QEnterEvent *event)
{
    m_isHovered = true;
    startHoverAnimation(true);
    QToolButton::enterEvent(event);
}

void ToolButton::leaveEvent(QEvent *event)
{
    m_isHovered = false;
    startHoverAnimation(false);
    QToolButton::leaveEvent(event);
}

void ToolButton::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::EnabledChange && !isEnabled()) {
        m_animation->stop();
        setHoverOpacity(0.0);
    }
    QToolButton::changeEvent(event);
}

void ToolButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);
    if (qFuzzyIsNull(m_hoverOpacity))
        return;

    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setOpacity(m_hoverOpacity);
    painter.fillRect(rect(), m_overlayColor);
}

void ToolButton::startHoverAnimation(bool hovered)
{
    m_animation->stop();
    m_animation->setStartValue(m_hoverOpacity);
    m_animation->setEndValue(hovered ? 1.0 : 0.0);
    m_animation->start();
}
