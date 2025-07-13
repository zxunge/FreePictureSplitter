// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "stdpaths.h"

namespace Util {

QString getInstallPrefix()
{
    QDir binDir(QCoreApplication::applicationDirPath());
    binDir.cdUp();
    return binDir.absolutePath();
}

QString getDataDir()
{
    QDir binDir(QCoreApplication::applicationDirPath());

#if defined(Q_OS_MAC)
    binDir.cdUp();
    binDir.cd("Resources");
#else
    binDir.cdUp();
    binDir.cd("share"); // The DATADIR of GNUInstallDirs is usually share
#endif

    return binDir.absolutePath();
}

QString getLibDir()
{
    QDir binDir(QCoreApplication::applicationDirPath());

#if defined(Q_OS_MAC)
    binDir.cdUp();
    binDir.cd("Library");
#else
    binDir.cdUp();
    binDir.cd("lib"); // The LIBDIR of GNUInstallDirs is usually lib
#endif

    return binDir.absolutePath();
}

QString getBinDir()
{
    QDir binDir(QCoreApplication::applicationDirPath());
    return binDir.absolutePath();
}

} // namespace Util
