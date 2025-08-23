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

#ifndef HOVEREVENT_H
#define HOVEREVENT_H

#include <QObject>
#include <QEvent>
#include <QIcon>
#include <QAbstractButton>

using namespace Qt::Literals::StringLiterals;

namespace Util {

class ButtonHoverEvent : public QObject
{
    Q_OBJECT

public:
    explicit ButtonHoverEvent(const QIcon &enter, const QIcon &leave, QObject *parent = nullptr) 
        : m_enterIcon(enter), 
          m_leaveIcon(leave),
          QObject(parent) {}

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        QAbstractButton *btn{ qobject_cast<QAbstractButton *>(obj) };
        Q_ASSERT(btn);
        switch (event->type()) {
        case QEvent::HoverEnter:
            btn->setIcon(m_enterIcon);
            return true;

        case QEvent::HoverLeave:
            btn->setIcon(m_leaveIcon);
            return true;

        default:
            break;
        }
        return QObject::eventFilter(obj, event);
    }

private:
    QIcon m_enterIcon;
    QIcon m_leaveIcon;
};

} // namespace Util

#endif // HOVEREVENT_H