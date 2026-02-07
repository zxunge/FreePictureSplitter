// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef FANCYTABWIDGET_H
#define FANCYTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>

class FancyTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit FancyTabBar() { }

protected:
    virtual QSize tabSizeHint(int index) const override;
    virtual void paintEvent(QPaintEvent *) override;
};

class FancyTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit FancyTabWidget(QWidget *parent = nullptr) : QTabWidget(parent)
    {
        setTabBar(new FancyTabBar());
    }
};

#endif // FANCYTABWIDGET_H
