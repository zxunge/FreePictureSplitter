// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef SINGLEWIDGET_H
#define SINGLEWIDGET_H

#include "core/imagehandler.h"

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

private:
    void openPicture();
    void savePictures();
    void resetSplitLines();

Q_SIGNALS:
    void message(const QString &msg, int timeout = 0);

private:
    Ui::SingleWidget *ui;

    QImageReader m_imgReader;
    Core::RectList m_rects;
};

#endif // SINGLEWIDGET_H
