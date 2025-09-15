/*
 * This file is part of FreePictureSplitter, a fast and easy-to-use picture splitter.
 * Copyright (c) 2024 2025 zxunge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef PREFERENCESWIDGET_H
#define PREFERENCESWIDGET_H

#include <QWidget>
#include <QColor>

namespace Ui {
class PreferencesWidget;
}
class QAbstractButton;

class PreferencesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PreferencesWidget(QWidget *parent = nullptr);
    ~PreferencesWidget();

    void saveChanges(); // This does not apply skin changes.

private slots:
    void on_cbxLocation_currentIndexChanged(int index);

    void on_cbxFormats_currentTextChanged(const QString &text);

    void on_btnSelectColor_clicked();

    void on_rbtnSpecified_toggled(bool checked);

    void on_chbGrid_toggled(bool checked);

    void on_tbtnAppearance_toggled(bool checked);

    void on_tbtnOutput_toggled(bool checked);

    void on_tbtnBrowse_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::PreferencesWidget *ui;
    QColor m_color;
};

#endif // PREFERENCESWIDGET_H
