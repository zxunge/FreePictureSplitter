// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef SINGLEWIDGET_H
#define SINGLEWIDGET_H

#include "core/imagedocument.h"

#include <QWidget>

namespace Ui {
class SingleWidget;
}
class QEvent;

class SingleWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit SingleWidget(QWidget *parent = nullptr);
    ~SingleWidget();

private:
    void openPicture();
    void savePictures();
    void resetSplitLines();
    void closePicture();

protected:
    void changeEvent(QEvent *e) override;

private:
    Ui::SingleWidget *ui;
    Core::ImageDocument *m_imgDoc;
};

#endif // SINGLEWIDGET_H
