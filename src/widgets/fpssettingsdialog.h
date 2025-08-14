/*
 * This file is a part of FreePictureSplitter, a fast and easy-to-use picture splitter.
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

#ifndef FPSSETTINGSDIALOG_H
#define FPSSETTINGSDIALOG_H

#include <QDialog>
#include <QColor>

namespace Ui {
class fpsSettingsDialog;
} // namespace Ui
namespace QWK {
class WidgetWindowAgent;
} // namespace QWK

class fpsSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit fpsSettingsDialog(QWidget *parent = nullptr);
    ~fpsSettingsDialog();

private slots:
    void on_buttonBox_accepted();

    void on_cbxLocation_currentIndexChanged(int index);

    void on_cbxFormats_currentTextChanged(const QString &text);

    void on_btnSelectColor_clicked();

    void on_buttonBox_rejected();

    void on_rbtnSpecified_toggled(bool checked);

    void on_chbGrid_toggled(bool checked);

    void on_tbtnAppearance_toggled(bool checked);

    void on_tbtnOutput_toggled(bool checked);

    void on_tbtnBrowse_clicked();
    
private:
    void installWindowAgent();

private:
    Ui::fpsSettingsDialog *ui;
    QWK::WidgetWindowAgent *m_windowAgent;

    QColor m_color;
};

#endif // FPSSETTINGSDIALOG_H
