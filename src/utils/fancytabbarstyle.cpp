// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "fancytabbarstyle.h"

#include <QPainter>
#include <QTabBar>
#include <QFontMetrics>
#include <QApplication>

FancyTabBarStyle::FancyTabBarStyle(QStyle *style) : QProxyStyle(style) { }

QSize FancyTabBarStyle::sizeFromContents(ContentsType type, const QStyleOption *option,
                                         const QSize &size, const QWidget *widget) const
{
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);

    if (type == QStyle::CT_TabBarTab) {
        const QStyleOptionTab *tabOption = qstyleoption_cast<const QStyleOptionTab *>(option);
        if (tabOption
            && (tabOption->shape == QTabBar::RoundedWest || tabOption->shape == QTabBar::RoundedEast
                || tabOption->shape == QTabBar::TriangularWest
                || tabOption->shape == QTabBar::TriangularEast)) {

            int textWidth = option->fontMetrics.horizontalAdvance(tabOption->text);
            int textHeight = option->fontMetrics.height();

            int iconSizeInt = pixelMetric(PM_TabBarIconSize, option, widget);
            QSize iconSize(iconSizeInt, iconSizeInt);

            int contentWidth = qMax(iconSize.width(), textWidth);
            int totalWidth = contentWidth + 24;

            int totalHeight = iconSize.height() + textHeight + 16 + 8;

            s = QSize(totalHeight, totalWidth);

            s.setWidth(qMax(s.width(), 80));
            s.setHeight(qMax(s.height(), 60));
        }
    }
    return s;
}

void FancyTabBarStyle::drawControl(ControlElement element, const QStyleOption *option,
                                   QPainter *painter, const QWidget *widget) const
{
    if (element == CE_TabBarTabLabel) {
        if (const QStyleOptionTab *tabOption = qstyleoption_cast<const QStyleOptionTab *>(option)) {
            if (tabOption->shape == QTabBar::RoundedWest || tabOption->shape == QTabBar::RoundedEast
                || tabOption->shape == QTabBar::TriangularWest
                || tabOption->shape == QTabBar::TriangularEast) {
                drawTabBarTab(tabOption, painter, widget);
                return;
            }
        }
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}

void FancyTabBarStyle::drawTabBarTab(const QStyleOptionTab *option, QPainter *painter,
                                     const QWidget *widget) const
{
    Q_ASSERT(option);
    Q_ASSERT(painter);

    painter->save();
    QRect rect = option->rect;

    // Obtain colors from QSS
    QColor bgColor;
    QColor textColor;
    QColor borderColor;

    // Use colors defined in QPalette
    QPalette palette = option->palette;

    if (option->state & State_Selected) {
        bgColor = palette.color(QPalette::Highlight);
        textColor = palette.color(QPalette::HighlightedText);
        borderColor = bgColor.darker(120);
    } else if (option->state & State_MouseOver) {
        bgColor = palette.color(QPalette::Highlight);
        bgColor.setAlpha(50);
        textColor = palette.color(QPalette::Text);
        borderColor = palette.color(QPalette::Mid);
    } else {
        bgColor = palette.color(QPalette::Window);
        textColor = palette.color(QPalette::Text);
        borderColor = palette.color(QPalette::Mid);
    }

    painter->fillRect(rect, bgColor);
    painter->setPen(borderColor);
    painter->drawRect(rect.adjusted(0, 0, -1, -1));

    QRect contentRect = rect.adjusted(8, 8, -8, -8);

    int iconTop = contentRect.top();
    if (!option->icon.isNull()) {
        int iconSizeInt = pixelMetric(PM_TabBarIconSize, option, widget);
        QSize iconSize(iconSizeInt, iconSizeInt);

        // Enlarge the icons
        if (iconSizeInt < 32)
            iconSize = QSize(32, 32);

        int iconX = contentRect.left() + (contentRect.width() - iconSize.width()) / 2;
        int iconY = iconTop;

        QRect iconRect(iconX, iconY, iconSize.width(), iconSize.height());

        QIcon::Mode mode = QIcon::Normal;
        if (!(option->state & State_Enabled))
            mode = QIcon::Disabled;
        else if (option->state & State_Selected)
            mode = QIcon::Selected;
        else if (option->state & State_MouseOver)
            mode = QIcon::Active;

        QIcon::State state = (option->state & State_On) ? QIcon::On : QIcon::Off;
        option->icon.paint(painter, iconRect, Qt::AlignCenter, mode, state);

        iconTop = iconRect.bottom() + 8;
    }

    if (!option->text.isEmpty()) {
        painter->setPen(textColor);

        QFont font = painter->font();
        if (font.pointSize() < 9) {
            font.setPointSize(9);
            painter->setFont(font);
        }

        QRect textRect = contentRect;
        textRect.setTop(iconTop);
        textRect.setBottom(contentRect.bottom());

        QTextOption textOption(Qt::AlignHCenter | Qt::AlignVCenter);
        textOption.setWrapMode(QTextOption::WordWrap);

        painter->drawText(textRect, option->text, textOption);
    }
    painter->restore();
}
