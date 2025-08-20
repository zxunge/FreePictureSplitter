/*
 * This file is a part of FreePictureSplitter, a fast and easy-to-use picture splitter.
 * Copyright (c) 2024 2025 zxunge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef LEAVEEVENT_H
#define LEAVEEVENT_H

#include <QWidget>
#include <QTimer>
#include <QRect>
#include <QScreen>
#include <QCursor>
#include <QCoreApplication>
#include <QtAssert>

namespace Util {

// It's a Qt issue that if a QAbstractButton::clicked triggers a window's maximization,
// the button remains to be hovered until the mouse move. As a result, we need to
// manually send leave events to the button.
static inline void emulateLeaveEvent(QWidget *widget) {
    Q_ASSERT(widget);

    QTimer::singleShot(0, widget, [widget]() {
        const QScreen *screen{ widget->screen() };
        const QPoint globalPos{ QCursor::pos(screen) };
        if (!QRect(widget->mapToGlobal(QPoint{0, 0}), widget->size()).contains(globalPos)) {
            QCoreApplication::postEvent(widget, new QEvent(QEvent::Leave));
            if (widget->testAttribute(Qt::WA_Hover)) {
                const QPoint localPos{ widget->mapFromGlobal(globalPos) };
                const QPoint scenePos{ widget->window()->mapFromGlobal(globalPos) };
                static constexpr const QPoint oldPos{};
                const Qt::KeyboardModifiers modifiers{ QGuiApplication::keyboardModifiers() };
                const auto event{
                    new QHoverEvent(QEvent::HoverLeave, scenePos, globalPos, oldPos, modifiers) };
                Q_UNUSED(localPos);
                QCoreApplication::postEvent(widget, event);
            }
        }
    });
}

} // namespace Util

#endif // LEAVEEVENT_H
