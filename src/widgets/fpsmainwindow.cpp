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

#include "fpsmainwindow.h"
#include "ui_fpsmainwindow.h"
#include "jsonconfigitems.h"
#include "leaveevent.h"
#include "config.h"
#include "hovereventfilter.h"
#include "fpssinglewidget.h"
#include "fpsbatchwidget.h"
#include "fpspreferenceswidget.h"
#include "fpsaboutdialog.h"
#include "fpsclickablelabel.h"

#include <QEvent>
#include <QCloseEvent>
#include <QAction>
#include <QToolButton>
#include <QLabel>
#include <QIcon>
#include <QStyle>
#include <QStatusBar>
#include <QStackedWidget>

#include <QWKWidgets/widgetwindowagent.h>
#include <widgetframe/windowbar.h>
#include <widgetframe/windowbutton.h>

using namespace Qt::Literals::StringLiterals;
using namespace Util;

extern Config appConfig;

fpsMainWindow::fpsMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::fpsMainWindow)
{
    setAttribute(Qt::WA_DontCreateNativeAncestors);
    ui->setupUi(this);

    // Add pages
    int index{};

    // Must be first loaded
    fpsPreferencesWidget *pgPref{ new fpsPreferencesWidget() };
    index = ui->wgtMain->addWidget(pgPref);
    pgPref->setIndex(index);
    connect(ui->tbtnPref, &QToolButton::clicked, this, [this, index](bool checked) {
        if (checked)
            ui->wgtMain->setCurrentIndex(index);
    });
    if (index == appConfig.dialog.lastEnteredIndex) {
        ui->tbtnPref->setChecked(true);
        ui->wgtMain->setCurrentIndex(index);
    }

    fpsSingleWidget *pgSingle{ new fpsSingleWidget() };
    connect(pgSingle, &fpsSingleWidget::message, ui->statusBar, &QStatusBar::showMessage);
    index = ui->wgtMain->addWidget(pgSingle);
    connect(ui->tbtnSingle, &QToolButton::clicked, this, [this, index, &pgPref](bool checked) {
        if (ui->wgtMain->currentIndex() == pgPref->index())
            pgPref->changed();
        if (checked)
            ui->wgtMain->setCurrentIndex(index);
    });
    if (index == appConfig.dialog.lastEnteredIndex) {
        ui->tbtnSingle->setChecked(true);
        ui->wgtMain->setCurrentIndex(index);
    }

    fpsBatchWidget *pgBatch{ new fpsBatchWidget() };
    connect(pgBatch, &fpsBatchWidget::message, ui->statusBar, &QStatusBar::showMessage);
    index = ui->wgtMain->addWidget(pgBatch);
    connect(ui->tbtnBatch, &QToolButton::clicked, this, [this, index, &pgPref](bool checked) {
        if (ui->wgtMain->currentIndex() == pgPref->index())
            pgPref->changed();
        if (checked)
            ui->wgtMain->setCurrentIndex(index);
    });
    if (index == appConfig.dialog.lastEnteredIndex) {
        ui->tbtnBatch->setChecked(true);
        ui->wgtMain->setCurrentIndex(index);
    }

    connect(ui->labMark, &fpsClickableLabel::clicked, this, [this] {
        fpsAboutDialog dlg(this);
        dlg.exec();
    });

    ui->labMark->setText(fpsVersionFull);
    installWindowAgent();
}

fpsMainWindow::~fpsMainWindow()
{
    delete ui;
}

void fpsMainWindow::on_actionExit_triggered()
{
    close();
}

void fpsMainWindow::installWindowAgent()
{
    // Setup window agent
    m_windowAgent = new QWK::WidgetWindowAgent(this);
    m_windowAgent->setup(this);

    // Construct window bar
    auto windowBar{ new QWK::WindowBar() };
    auto titleLabel{ new QLabel() };
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName(u"win-title-label"_s);

    auto iconButton{ new QWK::WindowButton() };
    iconButton->setObjectName(u"icon-button"_s);
    iconButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto pinButton{ new QWK::WindowButton() };
    pinButton->setCheckable(true);
    pinButton->setObjectName(QStringLiteral("pin-button"));
    pinButton->setProperty("system-button", true);
    pinButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto minButton{ new QWK::WindowButton() };
    minButton->setObjectName(u"min-button"_s);
    minButton->setProperty("system-button", true);
    minButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto maxButton{ new QWK::WindowButton() };
    maxButton->setCheckable(true);
    maxButton->setObjectName(u"max-button"_s);
    maxButton->setProperty("system-button", true);
    maxButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto closeButton{ new QWK::WindowButton() };
    closeButton->setObjectName(u"close-button"_s);
    closeButton->setProperty("system-button", true);
    closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    windowBar->setHostWidget(this);
    windowBar->setTitleLabel(titleLabel);
    windowBar->setIconButton(iconButton);
    windowBar->setPinButton(pinButton);
    windowBar->setMinButton(minButton);
    windowBar->setMaxButton(maxButton);
    windowBar->setCloseButton(closeButton);
    m_windowAgent->setTitleBar(windowBar);

    // Set properties
    m_windowAgent->setHitTestVisible(pinButton, true);
    m_windowAgent->setSystemButton(QWK::WindowAgentBase::WindowIcon, iconButton);
    m_windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, minButton);
    m_windowAgent->setSystemButton(QWK::WindowAgentBase::Maximize, maxButton);
    m_windowAgent->setSystemButton(QWK::WindowAgentBase::Close, closeButton);
    connect(windowBar, &QWK::WindowBar::pinRequested, this, [this, pinButton](bool pin) {
        if (isHidden() || isMinimized() || isMaximized() || isFullScreen())
            return;

        setWindowFlag(Qt::WindowStaysOnTopHint, pin);
        show();
        pinButton->setChecked(pin);
    });
    connect(windowBar, &QWK::WindowBar::minimizeRequested, this, &QWidget::showMinimized);
    connect(windowBar, &QWK::WindowBar::maximizeRequested, this, [this, maxButton](bool max) {
        if (max)
            showMaximized();
        else
            showNormal();

        // It's a Qt issue that if a QAbstractButton::clicked triggers a window's maximization,
        // the button remains to be hovered until the mouse move. As a result, we need to
        // manually send leave events to the button.
        emulateLeaveEvent(maxButton);
    });
    connect(windowBar, &QWK::WindowBar::closeRequested, this, &QWidget::close);
    ButtonHoverEventFilter *filter{ new ButtonHoverEventFilter(
            QIcon(u":/windowBar/windowBar/close-dark.svg"_s),
            QIcon(u":/windowBar/windowBar/close-light.svg"_s), this) };
    closeButton->installEventFilter(filter);

    setMenuWidget(windowBar);
}

bool fpsMainWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::WindowActivate: {
        auto menu{ menuWidget() };
        if (!menu)
            break;
        menu->setProperty("bar-active", true);
        style()->polish(menu);
        break;
    }

    case QEvent::WindowDeactivate: {
        auto menu{ menuWidget() };
        if (!menu)
            break;
        menu->setProperty("bar-active", false);
        style()->polish(menu);
        break;
    }

    default:
        break;
    }
    return QMainWindow::event(event);
}

void fpsMainWindow::closeEvent(QCloseEvent *e)
{
    appConfig.dialog.lastEnteredIndex = ui->wgtMain->currentIndex();
    QMainWindow::closeEvent(e);
}
