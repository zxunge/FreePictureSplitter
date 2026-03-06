// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>

class TaskManager : public QObject
{
    Q_OBJECT
public:
    explicit TaskManager(QObject *parent = nullptr);

signals:
};

#endif // TASKMANAGER_H
