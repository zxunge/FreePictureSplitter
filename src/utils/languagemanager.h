// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QStringList>

#include <memory>

class QTranslator;

namespace Util {

class LanguageManager
{
public:
    static LanguageManager &instance();

    /*!
     * \brief Installs the translators on the application for Qt and FPS. Should be
     * called again when the language changes.
     */
    void installTranslators();

    /*!
     * \brief Returns the available languages as a list of country codes.
     */
    static QStringList availableLanguages();

private:
    LanguageManager();
    ~LanguageManager() = default;

    std::unique_ptr<QTranslator> m_qtTranslator;
    std::unique_ptr<QTranslator> m_appTranslator;
};

} // namespace Util

#endif // LANGUAGEMANAGER_H
