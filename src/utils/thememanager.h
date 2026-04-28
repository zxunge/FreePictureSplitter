// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QIcon>
#include <QAbstractButton>

#include <oclero/qlementine/style/QlementineStyle.hpp>
#include <oclero/qlementine/style/ThemeManager.hpp>

#include <tuple>
#include <string>

class QApplication;
class TitleBar;

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
    enum class Theme { Dark, Light };
    using SkinInfo =
            std::tuple<std::string, std::string,
                       Util::ThemeManager::Theme>; // Skin name, skin file's name, skin's theme

    Theme theme() const { return std::get<2>(m_skinInfo); }

    /*!
     * \brief getAvailableSkins
     * \details Get available skins names under skins directory.
     */
    static QStringList availableSkins();
    /*!
     * \brief setAppSkin
     * \param skinName Name of the skin, written in the header.
     * \details Set application's stylesheet to the corresponding file with name `skinName'.
     */
    bool setAppSkin(const std::string &skinName);

    static ThemeManager &instance();

    void setTitleBar(TitleBar *bar);
    TitleBar *titleBar() const { return m_titleBar; }

private:
    SkinInfo infoFromSkinName(const std::string &name);

private:
    explicit ThemeManager(QObject *parent = nullptr);
    ~ThemeManager();

    SkinInfo m_skinInfo;
    TitleBar *m_titleBar;
    ButtonHoverEventFilter *m_filter;
    oclero::qlementine::QlementineStyle *m_style;
    oclero::qlementine::ThemeManager *m_themeMgr;

signals:
    void themeChanged(Util::ThemeManager::Theme theme);
};

} // namespace Util
Q_DECLARE_METATYPE(Util::ThemeManager::Theme)

#endif // THEMEMANAGER_H
