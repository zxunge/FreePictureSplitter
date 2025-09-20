// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef HOVEREVENTFILTER_H
#define HOVEREVENTFILTER_H

#include <QObject>
#include <QEvent>
#include <QIcon>
#include <QAbstractButton>

namespace Util {

/*!
 * \brief The ButtonHoverEventFilter class
 * \details An event filter that changes the icon of a button when hovered.
 */
class ButtonHoverEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit ButtonHoverEventFilter(const QIcon &enter, const QIcon &leave,
                                    QObject *parent = nullptr)
        : m_enterIcon(enter), m_leaveIcon(leave), QObject(parent)
    {
    }
    ~ButtonHoverEventFilter() { }

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

#endif // HOVEREVENTFILTER_H