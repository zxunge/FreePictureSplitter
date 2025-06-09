//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) 2024 2025 zxunge
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FPSMAINWINDOW_H
#define FPSMAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QRect>
#include <QVector>

namespace Ui {
class fpsMainWindow;
}
class fpsBatchDialog;

class fpsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit fpsMainWindow(QWidget *parent = nullptr);
    ~fpsMainWindow();

signals:
    void proceed(int elapsed);

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

private:
    Ui::fpsMainWindow *ui;
    fpsBatchDialog *m_batchDlg;

    QImage m_originalImage;
    QVector<QVector<QRect>> m_rects;
    QString m_fileName;
};

#endif // FPSMAINWINDOW_H
