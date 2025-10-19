// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef SKINS_H
#define SKINS_H

#include <QString>
#include <QStringList>

class QApplication;

namespace Util {

inline namespace Skin {

/*!
 * \brief getAvailableSkins
 * \details Get available skins names under skins directory.
 */
QStringList getAvailableSkins();

/*!
 * \brief setAppSkin
 * \param app Pointer to the QApplication instance.
 * \param skinName Name of the skin, written in the header.
 * \details Set application's stylesheet to the corresponding file with name `skinName'.
 */
bool setAppSkin(QApplication *app, const QString &skinName);

} // namespace Skin

} // namespace Util

#endif // SKINS_H
