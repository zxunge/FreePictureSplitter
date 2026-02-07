// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "fancytabwidget.h"

#include <QStylePainter>
#include <QStyleOptionTab>
#include <QStyleOption>

QSize FancyTabBar::tabSizeHint(int index) const
{
    QSize size{ QTabBar::tabSizeHint(index) };
    size.transpose();
    return size;
}

void FancyTabBar::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)

    QStylePainter painter(this);
    QStyleOptionTab tabOpt;

    for (int i{}; i < count(); ++i) {
        initStyleOption(&tabOpt, i);
        painter.drawControl(QStyle::CE_TabBarTabShape, tabOpt);
        painter.save();

        QSize size{ tabOpt.rect.size() };
        size.transpose();
        QRect rect(QPoint(), size);
        rect.moveCenter(tabOpt.rect.center());
        tabOpt.rect = rect;

        QPoint point{ tabRect(i).center() };
        painter.translate(point);
        // West
        painter.rotate(90);
        painter.translate(-point);
        painter.drawControl(QStyle::CE_TabBarTabLabel, tabOpt);
        painter.restore();
    }
}
