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

#ifndef FPSMAINWINDOW_H
#define FPSMAINWINDOW_H

#include "imagehandler.h"
#include <QMainWindow>
#include <QImageReader>
#include <QImageWriter>

namespace Ui {
class fpsMainWindow;
}
class fpsBatchDialog;
class fpsAboutDialog;
class fpsSettingsDialog;

class fpsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit fpsMainWindow(QWidget *parent = nullptr);
    ~fpsMainWindow();

signals:
    void splitProceed(int elapsed);

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

    void on_actionBatch_triggered();

    void on_actionSettings_triggered();

    void on_actionHomepage_triggered();

    void on_actionAbout_triggered();

    void on_btnReset_clicked();

    void on_rbtnSize_toggled(bool checked);

    void on_rbtnAver_toggled(bool checked);

    void on_actionZoomIn_triggered();

    void on_actionZoomOut_triggered();

    void on_rbtnManual_toggled(bool checked);

private:
    Ui::fpsMainWindow *ui;

    QImageReader m_imgReader;
    RectList m_rects;
};

#endif // FPSMAINWINDOW_H
