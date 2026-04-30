// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "toolbutton.h"
#include "utils/thememanager.h"

#include <QPainter>
#include <QEnterEvent>
#include <QApplication>
#include <QStyleOptionButton>

#include <oclero/qlementine/style/Theme.hpp>

ToolButton::ToolButton(QWidget *parent)
    : QToolButton(parent), m_animation(new QPropertyAnimation(this, "hoverFactor"))
{
    m_normalColor = palette().button().color();
    m_hoverColor = m_normalColor.darker(120);
    m_pressedColor = m_normalColor.darker(150);
    m_checkedColor = m_normalColor.lighter(110);

    m_animation->setEasingCurve(QEasingCurve::InOutQuad);
    m_animation->setDuration(Util::ThemeManager::instance().currentAppTheme().animationDuration);
    connect(m_animation, &QPropertyAnimation::valueChanged, this,
            QOverload<>::of(&QWidget::update));
    connect(&Util::ThemeManager::instance(), &Util::ThemeManager::themeChanged, this,
            &ToolButton::themeChanged);
}

void ToolButton::setNormalColor(const QColor &color)
{
    m_normalColor = color;
    update();
}

void ToolButton::setHoverColor(const QColor &color)
{
    m_hoverColor = color;
    update();
}

void ToolButton::setPressedColor(const QColor &color)
{
    m_pressedColor = color;
    update();
}

void ToolButton::setCheckedColor(const QColor &color)
{
    m_checkedColor = color;
    update();
}

void ToolButton::setHoverFactor(qreal factor)
{
    if (qFuzzyCompare(m_hoverFactor, factor))
        return;
    m_hoverFactor = factor;
    update();
}

void ToolButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor bgColor;
    if (isDown())
        bgColor = m_pressedColor;
    else if (isChecked())
        bgColor = m_checkedColor;
    else
        bgColor = interpolateColor(m_normalColor, m_hoverColor, m_hoverFactor);

    painter.setBrush(bgColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(rect(), 9, 9);

    QToolButton::paintEvent(event);
}

void ToolButton::enterEvent(QEnterEvent *event)
{
    startHoverAnimation(true);
    QToolButton::enterEvent(event);
}

void ToolButton::leaveEvent(QEvent *event)
{
    startHoverAnimation(false);
    QToolButton::leaveEvent(event);
}

void ToolButton::startHoverAnimation(bool hovered)
{
    m_animation->stop();
    m_animation->setStartValue(m_hoverFactor);
    m_animation->setEndValue(hovered ? 1.0 : 0.0);
    m_animation->start();
}

QColor ToolButton::interpolateColor(const QColor &from, const QColor &to, qreal ratio) const
{
    return QColor(from.red() + (to.red() - from.red()) * ratio,
                  from.green() + (to.green() - from.green()) * ratio,
                  from.blue() + (to.blue() - from.blue()) * ratio,
                  from.alpha() + (to.alpha() - from.alpha()) * ratio);
}

void ToolButton::themeChanged(const oclero::qlementine::Theme *theme)
{
    setNormalColor(theme->neutralColor);
    setHoverColor(theme->neutralColorHovered);
    setPressedColor(theme->neutralColorPressed);
    setCheckedColor(theme->neutralColorPressed);
    m_animation->setDuration(Util::ThemeManager::instance().currentAppTheme().animationDuration);
}