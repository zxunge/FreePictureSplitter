// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef MISC_H
#define MISC_H

#include <QApplication>

class MainWindow;

namespace Util {

inline MainWindow *getMainWindow()
{
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (MainWindow *mainWin = qobject_cast<MainWindow *>(w))
            return mainWin;
    Q_ASSERT(false);
    return nullptr;
}

} // namespace Util

#endif // MISC_H
