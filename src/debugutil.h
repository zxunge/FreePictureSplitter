// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef DEBUGUTIL_H
#define DEBUGUTIL_H

#include <QDebug>

#define fpsDebug(X)                                                            \
    qDebug() << "[" << __FILE_NAME__ << ": " << __LINE__ << ": "               \
             << __FUNCTION__ << "]" << #X << " = " << X

#define fpsDebugStr(X)                                                         \
    qDebug() << "[" << __FILE_NAME__ << ": " << __LINE__ << ": "               \
             << __FUNCTION__ << "]" << X

#endif      // DEBUGUTIL_H
