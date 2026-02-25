// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef MISC_H
#define MISC_H

#include "widgets/mainwindow.h"

#include <QApplication>
#include <QStringLiteral>

namespace Util {

inline MainWindow *getMainWindow()
{
    Q_FOREACH (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow *mainWin = qobject_cast<MainWindow *>(w))
            return mainWin;
    Q_ASSERT(false);
    return nullptr;
}

const char CONFIG_FILE_NAME[] = "conf.json";
const char LAYOUT_FILE_NAME[] = "geometry.layout";
const char LOG_FILE_NAME[] = "app.log";
const char ERRORLOG_FILE_NAME[] = "error.log";

} // namespace Util

#endif // MISC_H
