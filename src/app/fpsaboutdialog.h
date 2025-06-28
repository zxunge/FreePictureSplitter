// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef FPSABOUTDIALOG_H
#define FPSABOUTDIALOG_H

#include <QDialog>

class fpsAboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit fpsAboutDialog(QWidget *parent = nullptr);

signals:
};

#endif // FPSABOUTDIALOG_H
