// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "thememanager.h"
#include "stdpaths.h"
#include "titlebar.h"

#include "utils/hovereventfilter.h"
#include "utils/jsonconfigitems.h"

#include <QFile>
#include <QApplication>
#include <QDir>
#include <QMetaObject>
#include <QTabBar>
#include <QColor>

#include <oclero/qlementine.hpp>

#include <optional>

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

ThemeManager::ThemeManager(QObject *parent)
    : m_titleBar(nullptr),
      oclero::qlementine::ThemeManager(new oclero::qlementine::QlementineStyle(qApp), parent)
{
    m_filter = new ButtonHoverEventFilter(ICON_CLOSE_DARK, QIcon(), this);

    style()->setAnimationsEnabled(true);
    style()->setAutoIconColor(oclero::qlementine::AutoIconColor::TextColor);
    loadDirectory(Util::skinsDir());
}

ThemeManager::~ThemeManager()
{
    delete m_filter;
}

QStringList ThemeManager::availableSkins()
{
    QStringList list;
    for (const auto theme : themes())
        list.append(theme.meta.name);
    return list;
}

bool ThemeManager::setAppSkin(const std::string &skinName)
{
    if (-1 == themeIndex(QString::fromStdString(skinName)))
        return false;

    setCurrentTheme(QString::fromStdString(skinName));
    // Infer theme (dark or light) from the colors
    QColor color = themes().at(currentThemeIndex()).backgroundColorMain2;
    if (color.red() * 0.2126 + color.green() * 0.7152 + color.blue() * 0.0722 <= 127.5)
        m_theme = Theme::Dark;
    else
        m_theme = Theme::Light;
    qApp->setStyle(style());
    setTitleBar(m_titleBar);
    emit themeChanged(&themes().at(currentThemeIndex()));
    return true;
}

ThemeManager &ThemeManager::instance()
{
    static ThemeManager instance;
    return instance;
}

void ThemeManager::setTitleBar(TitleBar *bar)
{
    if (m_titleBar)
        m_titleBar->closeButton()->removeEventFilter(m_filter);
    m_titleBar = bar;
    if (theme() == Theme::Light) {
        m_titleBar->closeButton()->setIcon(ICON_CLOSE_LIGHT);
        m_titleBar->minButton()->setIcon(ICON_MIN_LIGHT);
        m_titleBar->maxButton()->setIcon(ICON_MAX_LIGHT);
        m_filter->setLeaveIcon(ICON_CLOSE_LIGHT);
    } else {
        m_titleBar->closeButton()->setIcon(ICON_CLOSE_DARK);
        m_titleBar->minButton()->setIcon(ICON_MIN_DARK);
        m_titleBar->maxButton()->setIcon(ICON_MAX_DARK);
        m_filter->setLeaveIcon(ICON_CLOSE_DARK);
    }
    m_titleBar->closeButton()->installEventFilter(m_filter);
    connect(m_titleBar, &TitleBar::maximizedStateChanged, this, [this](bool is) {
        if (is)
            m_titleBar->maxButton()->setIcon(theme() == Theme::Dark ? ICON_NORMAL_DARK
                                                                    : ICON_NORMAL_LIGHT);
        else
            m_titleBar->maxButton()->setIcon(theme() == Theme::Dark ? ICON_MAX_DARK
                                                                    : ICON_MAX_LIGHT);
    });
}

} // namespace Util
