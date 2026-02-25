// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "mainwindow.h"
#include "singlewidget.h"
#include "batchwidget.h"
#include "preferenceswidget.h"
#include "aboutdialog.h"
#include "auxtabbutton.h"
#include "clickablelabel.h"

#include "utils/jsonconfigitems.h"
#include "utils/leaveevent.h"
#include "utils/thememanager.h"
#include "utils/stdpaths.h"
#include "utils/misc.h"

#include <QEvent>
#include <QCloseEvent>
#include <QToolButton>
#include <QLabel>
#include <QIcon>
#include <QTabWidget>
#include <QTabBar>
#include <QStyle>
#include <QStatusBar>
#include <QFile>
#include <QHBoxLayout>
#include <QProgressBar>

#include <QWKWidgets/widgetwindowagent.h>
#include <widgetframe/windowbar.h>
#include <widgetframe/windowbutton.h>

using namespace Qt::Literals::StringLiterals;
using namespace Util;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setAttribute(Qt::WA_DontCreateNativeAncestors);

    // Construct UI
    setMinimumSize(QSize(800, 600));
    setCentralWidget(new QWidget(this));
    QHBoxLayout *horizontalLayout{ new QHBoxLayout(centralWidget()) };
    horizontalLayout->setSpacing(0);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    m_twgt = new QTabWidget(centralWidget());
    m_twgt->setTabPosition(QTabWidget::TabPosition::West);
    m_twgt->setTabShape(QTabWidget::TabShape::Rounded);

    horizontalLayout->addWidget(m_twgt);

    setStatusBar(new QStatusBar());
    statusBar()->setSizeGripEnabled(false);
    statusBar()->setObjectName("statusBar");

    createTabs();

    setObjectName("MainWindow");
    centralWidget()->setObjectName("centralWidget");
    m_twgt->setObjectName("tabWidget");
    setWindowTitle(qAppName());

    QIcon icon(u":/icons/version.ico"_s);
    ClickableLabel *labMark{ new ClickableLabel(m_twgt) };
    labMark->resize(32, 32);
    labMark->setPixmap(icon.pixmap(icon.actualSize(QSize(32, 32))));
    m_pbar = new QProgressBar(this);
    m_pbar->setMaximumWidth(80);
    statusBar()->addPermanentWidget(m_pbar);
    statusBar()->addPermanentWidget(labMark);
    m_pbar->setVisible(false);

    connect(labMark, &ClickableLabel::clicked, this, [this] {
        AboutDialog dlg(this);
        dlg.exec();
    });

    installWindowAgent();

    QFile layoutFile(Util::dataDir() % '/' % Util::LAYOUT_FILE_NAME);
    if (layoutFile.open(QIODevice::ReadOnly))
        restoreGeometry(layoutFile.readAll());
}

void MainWindow::createTabs()
{
    SingleWidget *pgSingle{ new SingleWidget(this) };
    BatchWidget *pgBatch{ new BatchWidget(this) };
    PreferencesWidget *pgPref{ new PreferencesWidget(this) };
    m_twgt->addTab(pgSingle, QString());
    m_twgt->addTab(pgBatch, QString());
    m_twgt->addTab(pgPref, QString());
    AuxTabButton *btn0{ new AuxTabButton(QPixmap(u":/controls/controls/32x32/image.svg"_s),
                                         tr("Single Splitting")) };
    m_twgt->tabBar()->setTabButton(0, QTabBar::ButtonPosition::RightSide, btn0);
    AuxTabButton *btn1{ new AuxTabButton(QPixmap(u":/controls/controls/32x32/image_multiple.svg"_s),
                                         tr("Batch Splitting")) };
    m_twgt->tabBar()->setTabButton(1, QTabBar::ButtonPosition::RightSide, btn1);
    AuxTabButton *btn2{ new AuxTabButton(QPixmap(u":/controls/controls/32x32/settings.svg"_s),
                                         tr("Preferences")) };
    m_twgt->tabBar()->setTabButton(2, QTabBar::ButtonPosition::RightSide, btn2);
    m_twgt->setCurrentIndex(g_appConfig.dialog.lastEnteredIndex);
    m_twgt->setAutoFillBackground(true);
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
    QFile layoutFile(Util::dataDir() % '/' % Util::LAYOUT_FILE_NAME);
    if (layoutFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        layoutFile.write(saveGeometry());

    g_appConfig.dialog.lastEnteredIndex = m_twgt->currentIndex();

    QMainWindow::closeEvent(e);
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    if (e->type() == QEvent::LanguageChange) {
        qobject_cast<AuxTabButton *>(
                m_twgt->tabBar()->tabButton(0, QTabBar::ButtonPosition::RightSide))
                ->setText(tr("Single Splitting"));
        qobject_cast<AuxTabButton *>(
                m_twgt->tabBar()->tabButton(1, QTabBar::ButtonPosition::RightSide))
                ->setText(tr("Batch Splitting"));
        qobject_cast<AuxTabButton *>(
                m_twgt->tabBar()->tabButton(2, QTabBar::ButtonPosition::RightSide))
                ->setText(tr("Preferences"));

        // TODO@26/02/25 Add code to update each bar's size to show words correctly.
        m_twgt->tabBar()->updateGeometry();
        m_twgt->adjustSize();
    }
}
