// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "languagemanager.h"
#include "jsonconfigitems.h"
#include "stdpaths.h"

#include <QCoreApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QDirIterator>
#include <QLocale>
#include <QTranslator>

using namespace Qt::Literals::StringLiterals;

void Util::LanguageManager::installTranslators()
{
    m_qtTranslator = std::make_unique<QTranslator>();
    m_appTranslator = std::make_unique<QTranslator>();

    const QLocale locale{ g_appConfig.app.lang.empty()
                                  ? QLocale()
                                  : QLocale(QString::fromStdString(g_appConfig.app.lang)) };

    if (m_qtTranslator->load(locale, u"qt"_s, u"_"_s,
                             QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
        QCoreApplication::installTranslator(m_qtTranslator.get());

    if (m_appTranslator->load(locale, qAppName(), u"_"_s, Util::translationsDir()))
        QCoreApplication::installTranslator(m_appTranslator.get());
}

QStringList Util::LanguageManager::availableLanguages()
{
    QStringList langs;
    QStringList nameFilters;
    nameFilters.append(qAppName() % u"_*.qm"_s);

    QDirIterator iterator(Util::translationsDir(), nameFilters, QDir::Files | QDir::Readable);

    while (iterator.hasNext()) {
        iterator.next();
        // Cut off "FreePictureSplitter_" from the start
        langs.append(iterator.fileInfo().completeBaseName().mid(20));
    }
    return langs;
}

Util::LanguageManager::LanguageManager() : m_appTranslator(nullptr), m_qtTranslator(nullptr) { }

Util::LanguageManager &Util::LanguageManager::instance()
{
    static LanguageManager instance;
    return instance;
}
