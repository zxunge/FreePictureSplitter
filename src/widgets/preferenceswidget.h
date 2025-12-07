// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef PREFERENCESWIDGET_H
#define PREFERENCESWIDGET_H

#include <QWidget>
#include <QColor>

namespace Ui {
class PreferencesWidget;
}
class QMainWindow;

class PreferencesWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit PreferencesWidget(QMainWindow *parent = nullptr);
    ~PreferencesWidget();

    void saveChanges(); // This does not apply skin changes.

private:
    Ui::PreferencesWidget *ui;
    QColor m_color;
};

#endif // PREFERENCESWIDGET_H
