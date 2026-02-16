// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "thememanager.h"

#include "utils/hovereventfilter.h"

using namespace Qt::Literals::StringLiterals;

namespace Util {

#define ICON_CLOSE_DARK QIcon(u":/windowBar/windowBar/close-dark.svg"_s)
#define ICON_CLOSE_LIGHT QIcon(u":/windowBar/windowBar/close-light.svg"_s)
#define ICON_MAX_DARK QIcon(u":/windowBar/windowBar/max-dark.svg"_s)
#define ICON_MAX_LIGHT QIcon(u":/windowBar/windowBar/max-light.svg"_s)
#define ICON_MIN_DARK QIcon(u":/windowBar/windowBar/min-dark.svg"_s)
#define ICON_MIN_LIGHT QIcon(u":/windowBar/windowBar/min-light.svg"_s)
#define ICON_NORMAL_DARK QIcon(u":/windowBar/windowBar/normal-dark.svg"_s)
#define ICON_NORMAL_LIGHT QIcon(u":/windowBar/windowBar/normal-light.svg"_s)
#define ICON_PIN_DARK QIcon(u":/windowBar/windowBar/pin-dark.svg"_s)
#define ICON_PIN_LIGHT QIcon(u":/windowBar/windowBar/pin-light.svg"_s)
#define ICON_PIN_FILL_DARK QIcon(u":/windowBar/windowBar/pin-fill-dark.svg"_s)
#define ICON_PIN_FILL_LIGHT QIcon(u":/windowBar/windowBar/pin-fill-light.svg"_s)

ThemeManager::ThemeManager(QObject *parent) : QObject(parent), m_theme(Theme::Light), m_closeBtn(nullptr)
{
    m_filter = new ButtonHoverEventFilter(ICON_CLOSE_DARK, ICON_CLOSE_LIGHT, this);
}

ThemeManager::~ThemeManager()
{
    delete m_filter;
}

void ThemeManager::setTheme(Theme theme)
{
    m_theme = theme;
    Q_EMIT themeChanged(theme);

    if (theme == Theme::Light) {
        m_filter->setEnterIcon(ICON_CLOSE_DARK);
        m_filter->setLeaveIcon(ICON_CLOSE_LIGHT);
    } else {
        m_filter->setEnterIcon(ICON_CLOSE_LIGHT);
        m_filter->setLeaveIcon(ICON_CLOSE_DARK);
    }
}

ThemeManager &ThemeManager::instance()
{
    static ThemeManager instance;
    return instance;
}

void ThemeManager::setCloseButton(QAbstractButton *btn)
{
    if (m_closeBtn)
        m_closeBtn->installEventFilter(nullptr);
    m_closeBtn = btn;
    m_closeBtn->installEventFilter(m_filter);
}

} // namespace Util
