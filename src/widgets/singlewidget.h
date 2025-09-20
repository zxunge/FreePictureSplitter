// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef SINGLEWIDGET_H
#define SINGLEWIDGET_H

#include "imagehandler.h"

#include <QWidget>
#include <QImageReader>

namespace Ui {
class SingleWidget;
}

class SingleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SingleWidget(QWidget *parent = nullptr);
    ~SingleWidget();

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_btnReset_clicked();

    void on_rbtnSize_toggled(bool checked);

    void on_rbtnAver_toggled(bool checked);

    void on_actionZoomIn_triggered();

    void on_actionZoomOut_triggered();

    void on_rbtnManual_toggled(bool checked);

signals:
    void message(const QString &msg, int timeout = 0);

private:
    Ui::SingleWidget *ui;

    QImageReader m_imgReader;
    Util::RectList m_rects;
};

#endif // SINGLEWIDGET_H
