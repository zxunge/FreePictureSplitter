// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>
#include <QPropertyAnimation>

namespace oclero::qlementine {
class Theme;
} // namespace oclero::qlementine

class ToolButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(qreal hoverFactor READ hoverFactor WRITE setHoverFactor)
    Q_PROPERTY(QColor normalColor READ normalColor WRITE setNormalColor)
    Q_PROPERTY(QColor hoverColor READ hoverColor WRITE setHoverColor)
    Q_PROPERTY(QColor pressedColor READ pressedColor WRITE setPressedColor)
    Q_PROPERTY(QColor checkedColor READ checkedColor WRITE setCheckedColor)

public:
    explicit ToolButton(QWidget *parent = nullptr);

    void setNormalColor(const QColor &color);
    QColor normalColor() const { return m_normalColor; }
    void setHoverColor(const QColor &color);
    QColor hoverColor() const { return m_hoverColor; }
    void setPressedColor(const QColor &color);
    QColor pressedColor() const { return m_pressedColor; }
    void setCheckedColor(const QColor &color);
    QColor checkedColor() const { return m_checkedColor; }

    qreal hoverFactor() const { return m_hoverFactor; }
    void setHoverFactor(qreal factor);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void startHoverAnimation(bool hovered);
    QColor interpolateColor(const QColor &from, const QColor &to, qreal ratio) const;
    void themeChanged(const oclero::qlementine::Theme *theme);

    QPropertyAnimation *m_animation;
    qreal m_hoverFactor = 0.0;

    QColor m_normalColor;
    QColor m_hoverColor;
    QColor m_pressedColor;
    QColor m_checkedColor;
};

#endif // TOOLBUTTON_H