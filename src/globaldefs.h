// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef GLOBALDEFS_H
#define GLOBALDEFS_H

#include <QStringLiteral>
#include "mainwindow.h"

#define CONFIG_FILENAME QStringLiteral("/conf.json")
#define LAYOUT_FILENAME QStringLiteral("/geometry.layout")
#define LOG_FILENAME QStringLiteral("/app.log")

extern MainWindow *g_mainWnd;

#endif // GLOBALDEFS_H
