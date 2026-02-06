// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef FANCYTABBARSTYLE_H
#define FANCYTABBARSTYLE_H

#include <QProxyStyle>
#include <QStyleOptionTab>

class FancyTabBarStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit FancyTabBarStyle(QStyle *style = nullptr);

    QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size,
                           const QWidget *widget) const override;

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget) const override;

private:
    void drawTabBarTab(const QStyleOptionTab *option, QPainter *painter,
                       const QWidget *widget) const;
};

#endif // FANCYTABBARSTYLE_H
