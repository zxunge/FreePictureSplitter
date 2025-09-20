// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef CLICKABLELABEL
#define CLICKABLELABEL

#include <QLabel>

class QMouseEvent;

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent = nullptr);
    ~ClickableLabel() { }

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *e) override;
};

#endif // CLICKABLELABEL
