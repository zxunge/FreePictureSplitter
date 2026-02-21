// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "thememanager.h"
#include "stdpaths.h"

#include "utils/hovereventfilter.h"
#include "utils/jsonconfigitems.h"

#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QDir>
#include <QMetaObject>
#include <QStyleFactory>

#include <phantomstyle.h>

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
    : QObject(parent), m_skinInfo(std::make_tuple("", "", Theme::Light)), m_closeBtn(nullptr)
{
    m_filter = new ButtonHoverEventFilter(ICON_CLOSE_DARK, QIcon(), this);
}

ThemeManager::~ThemeManager()
{
    delete m_filter;
}

QStringList ThemeManager::availableSkins()
{
    QStringList list;
    // We DO NOT check the existence of the skin files.
    for (const auto &skin : g_appConfig.skinList)
        list.push_back(QString::fromStdString(std::get<0>(skin)));
    return list;
}

bool ThemeManager::setAppSkin(const std::string &skinName)
{
    m_skinInfo = infoFromSkinName(skinName);

    QFile styleFile;
    styleFile.setFileName(Util::skinsDir() + u"/"_s
                          + QString::fromStdString(std::get<1>(m_skinInfo)));
    if (styleFile.open(QFile::ReadOnly)) {
        QTextStream in(&styleFile);
        // The stylesheets cannot be applied directly,
        // we need to do some path-conversion.
        QString ss{ in.readAll() };
        ss.replace(u"@SKINS_DIR@"_s, Util::skinsDir());
        // Set a new PhantomStyle
        PhantomStyle *style{ new PhantomStyle() };
        qApp->setStyle(style);
        qApp->setStyleSheet(ss);
        styleFile.close();

        if (std::get<2>(m_skinInfo) == Theme::Light)
            m_filter->setLeaveIcon(ICON_CLOSE_LIGHT);
        else
            m_filter->setLeaveIcon(ICON_CLOSE_DARK);
        Q_EMIT themeChanged(std::get<2>(m_skinInfo));

        return true;
    }
    return false;
}

ThemeManager &ThemeManager::instance()
{
    static ThemeManager instance;
    return instance;
}

void ThemeManager::setCloseButton(QAbstractButton *btn)
{
    if (m_closeBtn)
        m_closeBtn->removeEventFilter(m_filter);
    m_closeBtn = btn;
    m_closeBtn->installEventFilter(m_filter);
}

ThemeManager::SkinInfo ThemeManager::infoFromSkinName(const std::string &name)
{
    for (const auto &skin : g_appConfig.skinList)
        if (std::get<0>(skin) == name)
            return skin;
    return std::make_tuple("", "", Theme::Light);
}

} // namespace Util
