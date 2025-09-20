// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jsonconfigitems.h"
#include "leaveevent.h"
#include "config.h"
#include "hovereventfilter.h"
#include "singlewidget.h"
#include "batchwidget.h"
#include "preferenceswidget.h"
#include "aboutdialog.h"
#include "clickablelabel.h"
#include "stdpaths.h"

#include <QEvent>
#include <QCloseEvent>
#include <QAction>
#include <QToolButton>
#include <QLabel>
#include <QIcon>
#include <QStyle>
#include <QStatusBar>
#include <QStackedWidget>
#include <QFile>

#include <QWKWidgets/widgetwindowagent.h>
#include <widgetframe/windowbar.h>
#include <widgetframe/windowbutton.h>

using namespace Qt::Literals::StringLiterals;
using namespace Util;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    setAttribute(Qt::WA_DontCreateNativeAncestors);
    ui->setupUi(this);

    // Add pages
    int index{};

    SingleWidget *pgSingle{ new SingleWidget() };
    connect(pgSingle, &SingleWidget::message, ui->statusBar, &QStatusBar::showMessage);
    index = ui->wgtMain->addWidget(pgSingle);
    connect(ui->tbtnSingle, &QToolButton::clicked, this, [this, index](bool checked) {
        if (checked)
            ui->wgtMain->setCurrentIndex(index);
    });
    if (index == appConfig.dialog.lastEnteredIndex) {
        ui->tbtnSingle->setChecked(true);
        ui->wgtMain->setCurrentIndex(index);
    }

    BatchWidget *pgBatch{ new BatchWidget() };
    connect(pgBatch, &BatchWidget::message, ui->statusBar, &QStatusBar::showMessage);
    index = ui->wgtMain->addWidget(pgBatch);
    connect(ui->tbtnBatch, &QToolButton::clicked, this, [this, index](bool checked) {
        if (checked)
            ui->wgtMain->setCurrentIndex(index);
    });
    if (index == appConfig.dialog.lastEnteredIndex) {
        ui->tbtnBatch->setChecked(true);
        ui->wgtMain->setCurrentIndex(index);
    }

    PreferencesWidget *pgPref{ new PreferencesWidget() };
    index = ui->wgtMain->addWidget(pgPref);
    connect(ui->tbtnPref, &QToolButton::clicked, this, [this, index](bool checked) {
        if (checked)
            ui->wgtMain->setCurrentIndex(index);
    });
    if (index == appConfig.dialog.lastEnteredIndex) {
        ui->tbtnPref->setChecked(true);
        ui->wgtMain->setCurrentIndex(index);
    }

    connect(ui->labMark, &ClickableLabel::clicked, this, [this] {
        AboutDialog dlg(this);
        dlg.exec();
    });

    ui->labMark->setText(fpsVersionFull);
    installWindowAgent();

    QFile layoutFile(Util::getDataDir() % Util::LAYOUT_FILENAME);
    if (layoutFile.open(QIODevice::ReadOnly))
        restoreState(layoutFile.readAll());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::installWindowAgent()
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

bool MainWindow::event(QEvent *event)
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

void MainWindow::closeEvent(QCloseEvent *e)
{
    QFile layoutFile(Util::getDataDir() % Util::LAYOUT_FILENAME);
    if (layoutFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        layoutFile.write(saveState());

    QMainWindow::closeEvent(e);
}
