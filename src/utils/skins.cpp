// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge

#include "skins.h"
#include "stdpaths.h"

#include <QFile>
#include <QMessageBox>
#include <QApplication>
#include <QDir>
#include <QMetaObject>
#include <QStyleFactory>

using namespace Qt::Literals::StringLiterals;

namespace Util {

QStringList getAvailableSkins()
{
    QStringList list;
    QDir skinDir(Util::getSkinsDir());
    Q_FOREACH (const auto skin, skinDir.entryList(QStringList{ u"*.skin"_s }, QDir::Files)) {
        // Obtain the skin's name
        QFile file(Util::getSkinsDir() + u"/"_s + skin);
        if (file.open(QFile::ReadOnly)) {
            QTextStream in(&file);
            const QString header{ in.readLine() };
            const QString placeholder{ in.readLine() };
            // A valid skin file, whose 'header' is NAME,PALETTE
            if (!header.isEmpty() && placeholder == u"--@@##BEGIN##@@--"_s)
                list.push_back(header.split(u","_s)[0]);
        }
    }
    return list;
}

bool setAppSkin(QApplication *app, const QString &skinName)
{
    QFile styleFile;

    // Search for a skin file
    QDir skinDir(Util::getSkinsDir());

    Q_FOREACH (const auto fileName, skinDir.entryList(QStringList{ u"*.skin"_s }, QDir::Files)) {
        styleFile.setFileName(Util::getSkinsDir() + u"/"_s + fileName);
        if (styleFile.open(QFile::ReadOnly)) {
            QTextStream in(&styleFile);
            const QString header{ in.readLine() };
            const QString placeholder{ in.readLine() };
            if (header.split(u","_s)[0] == skinName && placeholder == u"--@@##BEGIN##@@--"_s) {
                const QStringList headerParts{ header.split(u","_s) };
                // Retrieve palette info from header
                if (headerParts.size() == 1) // Not changing the palette
                    app->setPalette(QPalette());
                else if (headerParts.size() == 2)
                    app->setPalette(QPalette(headerParts[1]));
                else { // Corrupted skin file
                    styleFile.close();
                    return false;
                }

                // The stylesheets cannot be applied directly,
                // we need to do some path-conversion.
                QString ss{ in.readAll() };
                ss.replace(u"@SKINS_DIR@"_s, Util::getSkinsDir());
                app->setStyle(QStyleFactory::create(u"fusion"_s));
                app->setStyleSheet(ss);
                styleFile.close();
                return true;
            } else
                styleFile.close();
        }
    }
    return false;
}

} // namespace Util
