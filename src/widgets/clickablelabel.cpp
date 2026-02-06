// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "clickablelabel.h"
#include <QMouseEvent>

ClickableLabel::ClickableLabel(QWidget *parent) : QLabel(parent) { }

void ClickableLabel::mousePressEvent(QMouseEvent *e)
{
    Q_EMIT clicked();
    QLabel::mousePressEvent(e);
}
