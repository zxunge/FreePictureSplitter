// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QList>

#include <oclero/qlementine/style/ThemeManager.hpp>

#include <string>

class QApplication;
class TitleBar;

namespace Util {

class ButtonHoverEventFilter;

/*!
 * \brief The ThemeManager class
 * \include utils/thememanager.h
 */
class ThemeManager : public oclero::qlementine::ThemeManager
{
    Q_OBJECT
public:
    enum class Theme { Dark, Light };

    Theme theme() const { return m_theme; }

    /*!
     * \brief availableSkins
     * \details Get available skins names under skins directory.
     */
    QStringList availableSkins();
    /*!
     * \brief setAppSkin
     * \param skinName Name of the skin, written in the header.
     * \details Set application's stylesheet to the corresponding file with name `skinName'.
     */
    bool setAppSkin(const std::string &skinName);

    static ThemeManager &instance();

    void setTitleBar(TitleBar *bar);
    TitleBar *titleBar() const { return m_titleBar; }

    QList<oclero::qlementine::Theme>::const_reference currentAppTheme() const;

private:
    explicit ThemeManager(QObject *parent = nullptr);
    ~ThemeManager();

    Theme m_theme; // It will be determined each time the theme is changed.
    TitleBar *m_titleBar;
    ButtonHoverEventFilter *m_filter;

signals:
    void themeChanged(const oclero::qlementine::Theme *theme);
};

} // namespace Util
Q_DECLARE_METATYPE(Util::ThemeManager)

#endif // THEMEMANAGER_H
