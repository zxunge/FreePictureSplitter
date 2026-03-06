// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>
#include <QPropertyAnimation>

class ToolButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(qreal hoverFactor READ hoverFactor WRITE setHoverFactor)

public:
    explicit ToolButton(QWidget *parent = nullptr);

    void setNormalColor(const QColor &color);
    void setHoverColor(const QColor &color);
    void setPressedColor(const QColor &color);
    void setCheckedColor(const QColor &color);

    qreal hoverFactor() const { return m_hoverFactor; }
    void setHoverFactor(qreal factor);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void startHoverAnimation(bool hovered);
    QColor interpolateColor(const QColor &from, const QColor &to, qreal ratio) const;

    QPropertyAnimation *m_animation;
    qreal m_hoverFactor = 0.0;

    QColor m_normalColor;
    QColor m_hoverColor;
    QColor m_pressedColor;
    QColor m_checkedColor;
};

#endif // TOOLBUTTON_H