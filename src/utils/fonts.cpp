// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge

#include "fonts.h"

#include <QGuiApplication>
#include <QFont>

using namespace Qt::Literals::StringLiterals;

namespace Util {

void setAppFont(QLocale locale, QGuiApplication *app)
{
    QFont font{ app->font() };
    font.setPointSize(9);
    app->setFont(font);
}

} // namespace Util
