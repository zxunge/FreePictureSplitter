// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef BORDERWIDGET_H
#define BORDERWIDGET_H

#include <QWidget>

class BorderWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int borderRadius READ borderRadius WRITE setBorderRadius)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
public:
    explicit BorderWidget(QWidget *parent = nullptr);

    void setBorderRadius(int radius)
    {
        if (m_radius == radius)
            return;
        m_radius = radius;
        update();
    }
    int borderRadius() const { return m_radius; }

    const QColor &backgroundColor() const { return m_bgColor; }
    void setBackgroundColor(const QColor &bgColor)
    {
        m_bgColor = bgColor;
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override;

private:
    int m_radius;
    QColor m_bgColor;
};

#endif // BORDERWIDGET_H
