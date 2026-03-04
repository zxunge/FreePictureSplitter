// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QToolButton>
#include <QPropertyAnimation>

class ToolButton : public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(qreal hoverOpacity READ hoverOpacity WRITE setHoverOpacity)

public:
    explicit ToolButton(QWidget *parent = nullptr);
    ~ToolButton();

    qreal hoverOpacity() const { return m_hoverOpacity; }
    void setHoverOpacity(qreal opacity);

    void setOverlayColor(const QColor &color);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *event) override;

private:
    void startHoverAnimation(bool hovered);

    qreal m_hoverOpacity = 0.0;
    QPropertyAnimation *m_animation;
    QColor m_overlayColor = QColor(255, 255, 255, 80);
    bool m_isHovered = false;
};

#endif // TOOLBUTTON_H
