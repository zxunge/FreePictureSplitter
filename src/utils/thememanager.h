// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QIcon>
#include <QAbstractButton>

namespace Util {

class ButtonHoverEventFilter;

/*!
 * \brief The ThemeManager class
 * \include utils/thememanager.h
 */
class ThemeManager : public QObject
{
    Q_OBJECT
public:
    explicit ThemeManager(QObject *parent = nullptr);
    ~ThemeManager();

    enum class Theme { Dark, Light };
    Theme theme() const { return m_theme; }
    void setTheme(Theme theme);

    static ThemeManager &instance();

    // Close buttons
    void setCloseButton(QAbstractButton *btn);
    QAbstractButton *closeButton() const { return m_closeBtn; }

private:
    Theme m_theme;
    QAbstractButton *m_closeBtn;
    ButtonHoverEventFilter *m_filter;

signals:
    void themeChanged(Util::ThemeManager::Theme theme);
};

} // namespace Util

#endif // THEMEMANAGER_H
