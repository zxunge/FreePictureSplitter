// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef SKINOPTIONSDIALOG_H
#define SKINOPTIONSDIALOG_H

#include "utils/thememanager.h"
#include <QDialog>

class SkinOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    SkinOptionsDialog(QWidget *parent = nullptr);
    Util::ThemeManager::SkinInfo skinInfo() const { return m_info; }

private:
    Util::ThemeManager::SkinInfo m_info;
};

#endif // SKINOPTIONSDIALOG_H
