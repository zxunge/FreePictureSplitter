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

#include "config.h"
#include "fpsaboutdialog.h"

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QPropertyAnimation>
#include <QPixmap>
#include <QDesktopServices>
#include <QUrl>

using namespace Qt::Literals::StringLiterals;

fpsAboutDialog::fpsAboutDialog(QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    auto mainLayout{ new QVBoxLayout(this) };

    auto iconLabel{ new QLabel(this) };
    iconLabel->setPixmap(QPixmap(u":/icons/fps.ico"_s).scaled(64, 64, Qt::KeepAspectRatio));

    auto titleLabel{ new QLabel(fpsAppName, this) };
    auto titleFont{ titleLabel->font() };
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    auto headerLayout{ new QHBoxLayout() };
    headerLayout->addWidget(iconLabel);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    auto versionLabel{ new QLabel(
            tr("<strong>Version: %1</strong><br />An efficient, small but powerful picture "
               "splitting application.<br />Most common formats are supported.")
                    .arg(fpsVersionFull),
            this) };
    versionLabel->setAlignment(Qt::AlignCenter);

    auto copyrightLabel{ new QLabel(u"© 2024, 2025 zxunge\nLicensed under: GPLv3\n"
                                    "This program is free software: you can redistribute "
                                    "it and/or modify it under "
                                    "the terms of the GNU General Public License as "
                                    "published by the Free Software "
                                    "Foundation, either version 3 of the License, or (at "
                                    "your option) any later version.\n"
                                    "This program is distributed in the hope that it "
                                    "will be useful, but WITHOUT ANY "
                                    "WARRANTY; without even the implied warranty of "
                                    "MERCHANTABILITY or FITNESS FOR A "
                                    "PARTICULAR PURPOSE.  See the GNU General Public "
                                    "License for more details.\n"
                                    "You should have received a copy of the GNU General "
                                    "Public License along with this program.  If not, "
                                    "see <https://www.gnu.org/licenses/>.\n"_s,
                                    this) };
    copyrightLabel->setAlignment(Qt::AlignCenter);
    copyrightLabel->adjustSize();
    copyrightLabel->setWordWrap(true);

    auto linkLabel{ new QLabel(tr("<a href=\"%1\">GitHub Repository</a>").arg(fpsHomepageUrl),
                               this) };
    linkLabel->setOpenExternalLinks(true);
    linkLabel->setAlignment(Qt::AlignCenter);

    auto closeButton{ new QPushButton(tr("Close"), this) };
    connect(closeButton, &QPushButton::clicked, this, [this]() { this->close(); });

    mainLayout->addLayout(headerLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(versionLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(copyrightLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(linkLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(closeButton);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);

    setStyleSheet(
            u"QDialog { background-color: qlineargradient(x1:0, y1:0, "
            "x2:1, y2:1, stop:0 #ffffff, stop:1 #3d59ab); border-radius: 15%; }"
            "QLabel { color: #333; margin: 5px; }"
            "QLabel#title { color: #2c3e50; }"
            "QPushButton { color: black; background: rgba(255, 255, 255, 0.7);"
            "border: 1px solid rgba(0, 0, 0, 0.073); border-bottom: 1px solid rgba(0, 0, 0, 0.183);"
            "border-radius: 5px;"
            "padding: 5px 12px 6px 12px; outline: none; }"_s);

    // Graphical Effects
    auto animation{ new QPropertyAnimation(this, "windowOpacity"_ba) };
    animation->setDuration(300);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start();
}