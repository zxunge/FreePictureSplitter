// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef DEBUGUTIL_H
#define DEBUGUTIL_H

#include <QtCore/qdebug.h>

#define fpsDebug(X) qDebug() << "[" << __FUNCTION__ << "]" << #X << " = " << X

#define fpsDebugStr(X) qDebug() << "[" << __FUNCTION__ << "]" << X

#endif // DEBUGUTIL_H
