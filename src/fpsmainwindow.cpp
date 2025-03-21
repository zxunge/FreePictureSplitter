//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPLv3 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPLv3

#include "fpsmainwindow.h"
#include "ui_fpsmainwindow.h"

#include <QFile>
#include <QStyle>

#include <QWKWidgets/widgetwindowagent.h>
#include <widgetframe/windowbar.h>
#include <widgetframe/windowbutton.h>

fpsMainWindow::fpsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fpsMainWindow)
{
    // setAttribute(Qt::WA_DontCreateNativeAncestors);

    ui->setupUi(this);

    // After the initialization of 'ui'.
    installWindowAgent();
    setWindowTitle(tr("FreePictureSplitter"));
    loadStyleSheet(Light);
}

fpsMainWindow::~fpsMainWindow()
{
    delete ui;
}

bool fpsMainWindow::event(QEvent *event) {
    switch (event->type()) {
    case QEvent::WindowActivate: {
        auto menu = menuWidget();
        menu->setProperty("bar-active", true);
        style()->polish(menu);
        break;
    }

    case QEvent::WindowDeactivate: {
        auto menu = menuWidget();
        menu->setProperty("bar-active", false);
        style()->polish(menu);
        break;
    }

    default:
        break;
    }
    return QMainWindow::event(event);
}

void fpsMainWindow::closeEvent(QCloseEvent *event) {
    // if (!(qApp->keyboardModifiers() & Qt::ControlModifier)) {
    //     QTimer::singleShot(1000, this, &QWidget::show);
    // }
    event->accept();
}

void fpsMainWindow::installWindowAgent() {
    // 1. Setup window agent
    windowAgent = new QWK::WidgetWindowAgent(this);
    windowAgent->setup(this);

    // 2. Construct your title bar
    auto menuBar = [this]() {
        auto menuBar = new QMenuBar();

        // Virtual menu
        auto file = new QMenu(tr("File(&F)"), menuBar);
        file->addAction(new QAction(tr("New(&N)"), menuBar));
        file->addAction(new QAction(tr("Open(&O)"), menuBar));
        file->addSeparator();

        auto edit = new QMenu(tr("Edit(&E)"), menuBar);
        edit->addAction(new QAction(tr("Undo(&U)"), menuBar));
        edit->addAction(new QAction(tr("Redo(&R)"), menuBar));

        // Theme action
        auto darkAction = new QAction(tr("Enable dark theme"), menuBar);
        darkAction->setCheckable(true);
        connect(darkAction, &QAction::triggered, this, [this](bool checked) {
            loadStyleSheet(checked ? Dark : Light); //
        });
        connect(this, &fpsMainWindow::themeChanged, darkAction,
                [this, darkAction]() {
                    darkAction->setChecked(currentTheme == Dark); //
                });

#ifdef Q_OS_WIN
        auto dwmBlurAction = new QAction(tr("Enable DWM blur"), menuBar);
        dwmBlurAction->setCheckable(true);
        connect(dwmBlurAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(QStringLiteral("dwm-blur"),
                                                 checked)) {
                return;
            }
            setProperty("custom-style", checked);
            style()->polish(this);
        });

        auto acrylicAction =
            new QAction(tr("Enable acrylic material"), menuBar);
        acrylicAction->setCheckable(true);
        connect(acrylicAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(
                    QStringLiteral("acrylic-material"), true)) {
                return;
            }
            setProperty("custom-style", checked);
            style()->polish(this);
        });

        auto micaAction = new QAction(tr("Enable mica"), menuBar);
        micaAction->setCheckable(true);
        connect(micaAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(QStringLiteral("mica"),
                                                 checked)) {
                return;
            }
            setProperty("custom-style", checked);
            style()->polish(this);
        });

        auto micaAltAction = new QAction(tr("Enable mica alt"), menuBar);
        micaAltAction->setCheckable(true);
        connect(micaAltAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(QStringLiteral("mica-alt"),
                                                 checked)) {
                return;
            }
            setProperty("custom-style", checked);
            style()->polish(this);
        });
#elif defined(Q_OS_MAC)
        auto darkBlurAction = new QAction(tr("Dark blur"), menuBar);
        darkBlurAction->setCheckable(true);
        connect(darkBlurAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(QStringLiteral("blur-effect"),
                                                 "dark")) {
                return;
            }
            if (checked) {
                setProperty("custom-style", true);
                style()->polish(this);
            }
        });

        auto lightBlurAction = new QAction(tr("Light blur"), menuBar);
        lightBlurAction->setCheckable(true);
        connect(lightBlurAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(QStringLiteral("blur-effect"),
                                                 "light")) {
                return;
            }
            if (checked) {
                setProperty("custom-style", true);
                style()->polish(this);
            }
        });

        auto noBlurAction = new QAction(tr("No blur"), menuBar);
        noBlurAction->setCheckable(true);
        connect(noBlurAction, &QAction::toggled, this, [this](bool checked) {
            if (!windowAgent->setWindowAttribute(QStringLiteral("blur-effect"),
                                                 "none")) {
                return;
            }
            if (checked) {
                setProperty("custom-style", false);
                style()->polish(this);
            }
        });

        auto macStyleGroup = new QActionGroup(menuBar);
        macStyleGroup->addAction(darkBlurAction);
        macStyleGroup->addAction(lightBlurAction);
        macStyleGroup->addAction(noBlurAction);
#endif

        // Real menu
        auto settings = new QMenu(tr("Settings(&S)"), menuBar);
        settings->addAction(darkAction);

#ifdef Q_OS_WIN
        settings->addSeparator();
        settings->addAction(dwmBlurAction);
        settings->addAction(acrylicAction);
        settings->addAction(micaAction);
        settings->addAction(micaAltAction);
#elif defined(Q_OS_MAC)
        settings->addAction(darkBlurAction);
        settings->addAction(lightBlurAction);
        settings->addAction(noBlurAction);
#endif

        menuBar->addMenu(file);
        menuBar->addMenu(edit);
        menuBar->addMenu(settings);
        return menuBar;
    }();
    menuBar->setObjectName(QStringLiteral("win-menu-bar"));

    auto titleLabel = new QLabel();
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName(QStringLiteral("win-title-label"));

#ifndef Q_OS_MAC
    auto iconButton = new QWK::WindowButton();
    iconButton->setObjectName(QStringLiteral("icon-button"));
    iconButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto minButton = new QWK::WindowButton();
    minButton->setObjectName(QStringLiteral("min-button"));
    minButton->setProperty("system-button", true);
    minButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto maxButton = new QWK::WindowButton();
    maxButton->setCheckable(true);
    maxButton->setObjectName(QStringLiteral("max-button"));
    maxButton->setProperty("system-button", true);
    maxButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto closeButton = new QWK::WindowButton();
    closeButton->setObjectName(QStringLiteral("close-button"));
    closeButton->setProperty("system-button", true);
    closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
#endif

    auto windowBar = new QWK::WindowBar();
#ifndef Q_OS_MAC
    windowBar->setIconButton(iconButton);
    windowBar->setMinButton(minButton);
    windowBar->setMaxButton(maxButton);
    windowBar->setCloseButton(closeButton);
#endif
    windowBar->setMenuBar(menuBar);
    windowBar->setTitleLabel(titleLabel);
    windowBar->setHostWidget(this);

    windowAgent->setTitleBar(windowBar);
#ifndef Q_OS_MAC
    windowAgent->setSystemButton(QWK::WindowAgentBase::WindowIcon, iconButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, minButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Maximize, maxButton);
    windowAgent->setSystemButton(QWK::WindowAgentBase::Close, closeButton);
#endif
    windowAgent->setHitTestVisible(menuBar, true);

#ifdef Q_OS_MAC
    windowAgent->setSystemButtonAreaCallback([](const QSize &size) {
        static constexpr const int width = 75;
        return QRect(QPoint(size.width() - width, 0),
                     QSize(width, size.height())); //
    });
#endif

    setMenuWidget(windowBar);

// 3. Adds simulated mouse events to the title bar buttons
#ifdef Q_OS_WINDOWS
    // Emulate Window system menu button behaviors
    connect(iconButton, &QAbstractButton::clicked, windowAgent,
            [this, iconButton] {
                iconButton->setProperty("double-click-close", false);

                // Pick a suitable time threshold
                // QTimer::singleShot(75, windowAgent, [this, iconButton]() {
                //     if (iconButton->property("double-click-close").toBool())
                //         return;
                //     windowAgent->showSystemMenu(iconButton->mapToGlobal(QPoint{0,
                //     iconButton->height()}));
                // });
            });
    connect(iconButton, &QWK::WindowButton::doubleClicked, this,
            [iconButton, this]() {
                iconButton->setProperty("double-click-close", true);
                close();
            });
#endif

#ifndef Q_OS_MAC
    connect(windowBar, &QWK::WindowBar::minimizeRequested, this,
            &QWidget::showMinimized);
    connect(windowBar, &QWK::WindowBar::maximizeRequested, this,
            [this, maxButton](bool max) {
                if (max) {
                    showMaximized();
                } else {
                    showNormal();
                }

                // It's a Qt issue that if a QAbstractButton::clicked triggers a
                // window's maximization, the button remains to be hovered until
                // the mouse move. As a result, we need to manually send leave
                // events to the button. emulateLeaveEvent(maxButton);
            });
    connect(windowBar, &QWK::WindowBar::closeRequested, this, &QWidget::close);
#endif
}

void fpsMainWindow::loadStyleSheet(Theme theme) {
    if (!styleSheet().isEmpty() && theme == currentTheme)
        return;
    currentTheme = theme;

    if (QFile qss(theme == Dark ? QStringLiteral(":/qss/dark-style.qss")
                                : QStringLiteral(":/qss/light-style.qss"));
        qss.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setStyleSheet(QString::fromUtf8(qss.readAll()));
        Q_EMIT themeChanged();
    }
}

