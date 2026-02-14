// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "singlewidget.h"
#include "batchwidget.h"
#include "preferenceswidget.h"
#include "fancytabwidget.h"
#include "aboutdialog.h"
#include "clickablelabel.h"
#include "globaldefs.h"

#include "utils/jsonconfigitems.h"
#include "utils/leaveevent.h"
#include "utils/thememanager.h"
#include "utils/stdpaths.h"

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

    SingleWidget *pgSingle{ new SingleWidget(this) };
    BatchWidget *pgBatch{ new BatchWidget(this) };
    PreferencesWidget *pgPref{ new PreferencesWidget(this) };
    ui->tabWidget->addTab(pgSingle, QIcon(u":/controls/controls/32x32/image.svg"_s),
                          tr("Single Splitting"));
    ui->tabWidget->addTab(pgBatch, QIcon(u":/controls/controls/32x32/image_multiple.svg"_s),
                          tr("Batch Splitting"));
    ui->tabWidget->addTab(pgPref, QIcon(u":/controls/controls/32x32/settings.svg"_s),
                          tr("Preferences"));
    ui->tabWidget->setCurrentIndex(g_appConfig.dialog.lastEnteredIndex);

    QIcon icon(u":/icons/version.ico"_s);
    ClickableLabel *labMark{ new ClickableLabel(ui->tabWidget) };
    labMark->resize(32, 32);
    labMark->setPixmap(icon.pixmap(icon.actualSize(QSize(32, 32))));
    ui->statusBar->addPermanentWidget(labMark);
    connect(labMark, &ClickableLabel::clicked, this, [this] {
        AboutDialog dlg(this);
        dlg.exec();
    });
    connect(ui->actionExit, &QAction::triggered, this, [this] { close(); });

    installWindowAgent();

    QFile layoutFile(Util::dataDir() % LAYOUT_FILENAME);
    if (layoutFile.open(QIODevice::ReadOnly))
        restoreGeometry(layoutFile.readAll());
}

MainWindow::~MainWindow()
{
    delete ui;
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
    ThemeManager::instance().setTheme(ThemeManager::Theme::Light);
    ThemeManager::instance().setCloseButton(closeButton);

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
    QFile layoutFile(Util::dataDir() % LAYOUT_FILENAME);
    if (layoutFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        layoutFile.write(saveGeometry());

    g_appConfig.dialog.lastEnteredIndex = ui->tabWidget->currentIndex();

    QMainWindow::closeEvent(e);
}
