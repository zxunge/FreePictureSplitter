// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef PREFERENCESWIDGET_H
#define PREFERENCESWIDGET_H

#include <QWidget>
#include <QColor>

class QEvent;

namespace Ui {
class PreferencesWidget;
}

class PreferencesWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit PreferencesWidget(QWidget *parent = nullptr);
    ~PreferencesWidget();

protected:
    void changeEvent(QEvent *e) override;

private:
    Ui::PreferencesWidget *ui;
};

#endif // PREFERENCESWIDGET_H
