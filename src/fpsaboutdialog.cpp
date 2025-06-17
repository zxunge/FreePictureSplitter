// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "config.h"
#include "fpsaboutdialog.h"
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDesktopServices>
#include <QUrl>
#include <QFont>
#include <QPropertyAnimation>

fpsAboutDialog::fpsAboutDialog(QWidget *parent) : QDialog{ parent }
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *iconLabel = new QLabel(this);
    iconLabel->setPixmap(QPixmap(":/icons/fps.ico").scaled(64, 64, Qt::KeepAspectRatio));

    QLabel *titleLabel = new QLabel(fpsAppName, this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->addWidget(iconLabel);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    QLabel *versionLabel =
            new QLabel(tr("<strong>Version: ") + fpsAppVersionFull
                               + tr("</strong><br />An efficient, small but powerful "
                                    "picture splitting application.<br />")
                               + tr("Most common formats are supported."),
                       this);
    versionLabel->setAlignment(Qt::AlignCenter);

    QLabel *copyrightLabel =
            new QLabel(tr("© 2024, 2025 zxunge\n") + tr("Licensed under: GPLv3\n")
                               + tr("This program is free software: you can redistribute "
                                    "it and/or modify it under "
                                    "the terms of the GNU General Public License as "
                                    "published by the Free Software "
                                    "Foundation, either version 3 of the License, or (at "
                                    "your option) any later version.\n")
                               + tr("This program is distributed in the hope that it "
                                    "will be useful, but WITHOUT ANY "
                                    "WARRANTY; without even the implied warranty of "
                                    "MERCHANTABILITY or FITNESS FOR A "
                                    "PARTICULAR PURPOSE.  See the GNU General Public "
                                    "License for more details.\n")
                               + tr("You should have received a copy of the GNU General "
                                    "Public License along with this program.  If not, "
                                    "see <https://www.gnu.org/licenses/>.\n"),
                       this);
    copyrightLabel->setAlignment(Qt::AlignCenter);
    copyrightLabel->setWordWrap(true);

    QLabel *linkLabel =
            new QLabel(tr("<a href=\"") + fpsHomepageUrl + tr("\">GitHub Repository</a>"), this);
    linkLabel->setOpenExternalLinks(true);
    linkLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addLayout(headerLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(versionLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(copyrightLabel);
    mainLayout->addWidget(linkLabel);

    setStyleSheet("QDialog { background-color: qlineargradient(x1:0, y1:0, "
                  "x2:1, y2:1, stop:0 #ffffff, stop:1 #3d59ab); }"
                  "QLabel { color: #333; margin: 5px; }"
                  "QLabel#title { color: #2c3e50; }");
    setFixedSize(600, 400);

    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(300);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->start();
}
