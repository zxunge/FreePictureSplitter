// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef LEAVEEVENT_H
#define LEAVEEVENT_H

#include <QWidget>
#include <QTimer>
#include <QRect>
#include <QScreen>
#include <QCursor>
#include <QCoreApplication>
#include <QHoverEvent>
#include <QEvent>
#include <QtAssert>

namespace Util {

/*!
 * \brief emulateLeaveEvent
 * \details It's a Qt issue that if a QAbstractButton::clicked triggers a window's maximization,
 * the button remains to be hovered until the mouse move. As a result, we need to
 * manually send leave events to the button.
 */
static inline void emulateLeaveEvent(QWidget *widget)
{
    Q_ASSERT(widget);

    QTimer::singleShot(0, widget, [widget]() {
        const QScreen *screen{ widget->screen() };
        const QPoint globalPos{ QCursor::pos(screen) };
        if (!QRect(widget->mapToGlobal(QPoint{ 0, 0 }), widget->size()).contains(globalPos)) {
            QCoreApplication::postEvent(widget, new QEvent(QEvent::Leave));
            if (widget->testAttribute(Qt::WA_Hover)) {
                const QPoint localPos{ widget->mapFromGlobal(globalPos) };
                const QPoint scenePos{ widget->window()->mapFromGlobal(globalPos) };
                static constexpr const QPoint oldPos{};
                const Qt::KeyboardModifiers modifiers{ QGuiApplication::keyboardModifiers() };
                const auto event{ new QHoverEvent(QEvent::HoverLeave, scenePos, globalPos, oldPos,
                                                  modifiers) };
                Q_UNUSED(localPos);
                QCoreApplication::postEvent(widget, event);
            }
        }
    });
}

} // namespace Util

#endif // LEAVEEVENT_H
