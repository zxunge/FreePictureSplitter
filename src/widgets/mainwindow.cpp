// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "mainwindow.h"
#include "singlewidget.h"
#include "batchwidget.h"
#include "preferenceswidget.h"
#include "aboutdialog.h"
#include "fancytabwidget.h"
#include "clickablelabel.h"
#include "toolbutton.h"
#include "titlebar.h"

#include "utils/jsonconfigitems.h"
#include "utils/stdpaths.h"
#include "utils/thememanager.h"
#include "utils/globals.h"

#include <QEvent>
#include <QPainterPath>
#include <QCloseEvent>
#include <QToolButton>
#include <QLabel>
#include <QIcon>
#include <QTabWidget>
#include <QTabBar>
#include <QStyle>
#include <QStatusBar>
#include <QFile>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QTimer>

#include <QWKWidgets/widgetwindowagent.h>
#include <widgetframe/windowbutton.h>
#include <oclero/qlementine/style/Theme.hpp>

using namespace Qt::Literals::StringLiterals;
using namespace Util;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setAttribute(Qt::WA_DontCreateNativeAncestors);

    // Construct UI
    setMinimumSize({ 800, 600 });
    setWindowTitle(App::Constants::APP_NAME);
    setCentralWidget(new QWidget(this));

    QVBoxLayout *verticalLayout = new QVBoxLayout(centralWidget());
    verticalLayout->setSpacing(0);

    m_twgt = new FancyTabWidget(centralWidget());
    verticalLayout->addWidget(m_twgt);

    statusBar()->setSizeGripEnabled(false);
    statusBar()->setObjectName("statusBar");
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    createTabs();

    setObjectName("MainWindow");
    centralWidget()->setObjectName("centralWidget");
    m_twgt->setObjectName("tabWidget");

    // Version label & progress bar
    ClickableLabel *labMark = new ClickableLabel(m_twgt);
    labMark->setAutoFillBackground(true);
    labMark->setStyleSheet(u"background-color: #8A2BE2; color: white;"_s);
    labMark->setText(App::Constants::APP_VERSION_STR);
    labMark->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    m_twgt->setBottomCornerWidget(labMark);
    m_pbar = new QProgressBar(this);
    m_pbar->setMaximumWidth(80);
    m_pbar->setVisible(false);
    connect(labMark, &ClickableLabel::clicked, this, [this] {
        AboutDialog dlg(this);
        dlg.exec();
    });

    // Task menu & button
    m_taskMenu = new QMenu(this);
    ToolButton *tbtnTask = new ToolButton(this);
    tbtnTask->setToolTip(tr("Tasks"));
    tbtnTask->setMinimumHeight(32);
    tbtnTask->setText(QChar(0x2630));
    connect(tbtnTask, &QToolButton::clicked, this, [this] { m_taskMenu->exec(QCursor::pos()); });
    statusBar()->addPermanentWidget(m_pbar);
    statusBar()->addPermanentWidget(tbtnTask);

    installWindowAgent();

    connect(&Util::ThemeManager::instance(), &Util::ThemeManager::themeChanged, this,
            &MainWindow::themeChanged);

    QFile layoutFile(Util::dataDir() % '/' % Util::LAYOUT_FILE_NAME);
    if (layoutFile.open(QIODevice::ReadOnly))
        restoreGeometry(layoutFile.readAll());

    ThemeManager::instance().setTitleBar(m_titleBar);
}

void MainWindow::themeChanged(const oclero::qlementine::Theme *theme)
{
    if (ThemeManager::Theme::Dark == ThemeManager::instance().theme()) {
        m_twgt->setTab(0, QIcon(u":/controls/controls/32x32/image-dark.svg"_s),
                       tr("Single Splitting"));
        m_twgt->setTab(1, QIcon(u":/controls/controls/32x32/image_multiple-dark.svg"_s),
                       tr("Batch Splitting"));
        m_twgt->setTab(2, QIcon(u":/controls/controls/32x32/settings-dark.svg"_s),
                       tr("Preferences"));
    } else {
        m_twgt->setTab(0, QIcon(u":/controls/controls/32x32/image.svg"_s), tr("Single Splitting"));
        m_twgt->setTab(1, QIcon(u":/controls/controls/32x32/image_multiple.svg"_s),
                       tr("Batch Splitting"));
        m_twgt->setTab(2, QIcon(u":/controls/controls/32x32/settings.svg"_s), tr("Preferences"));
    }
}

void MainWindow::createTabs()
{
    SingleWidget *pgSingle = new SingleWidget(this);
    BatchWidget *pgBatch = new BatchWidget(this);
    PreferencesWidget *pgPref = new PreferencesWidget(this);
    m_twgt->insertTab(0, pgSingle, QIcon(u":/controls/controls/32x32/image.svg"_s),
                      tr("Single Splitting"));
    m_twgt->insertTab(1, pgBatch, QIcon(u":/controls/controls/32x32/image_multiple.svg"_s),
                      tr("Batch Splitting"));
    m_twgt->insertTab(2, pgPref, QIcon(u":/controls/controls/32x32/settings.svg"_s),
                      tr("Preferences"));
    m_twgt->setCurrentIndex(g_appConfig.dialog.lastEnteredIndex);
    m_twgt->setAutoFillBackground(true);
}

void MainWindow::installWindowAgent()
{
    // Setup window agent
    m_windowAgent = new QWK::WidgetWindowAgent(this);
    m_windowAgent->setup(this);

    // Construct window bar
    m_titleBar = new TitleBar();
    auto titleLabel = new QLabel();
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName(u"win-title-label"_s);

    auto iconButton = new QWK::WindowButton();
    iconButton->setObjectName(u"icon-button"_s);
    iconButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto pinButton = new QWK::WindowButton();
    pinButton->setCheckable(true);
    pinButton->setObjectName(QStringLiteral("pin-button"));
    pinButton->setProperty("system-button", true);
    pinButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    m_titleBar->setHostWidget(this);
    m_titleBar->setTitleLabel(titleLabel);
    m_titleBar->setIconButton(iconButton);
    m_titleBar->setPinButton(pinButton);
    m_windowAgent->setTitleBar(m_titleBar);

    // Set properties
    m_windowAgent->setHitTestVisible(pinButton, true);
    m_windowAgent->setSystemButton(QWK::WindowAgentBase::WindowIcon, iconButton);
    m_windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, m_titleBar->minButton());
    m_windowAgent->setSystemButton(QWK::WindowAgentBase::Maximize, m_titleBar->maxButton());
    m_windowAgent->setSystemButton(QWK::WindowAgentBase::Close, m_titleBar->closeButton());
    connect(m_titleBar, &TitleBar::pinRequested, this, [this, pinButton](bool pin) {
        if (isHidden() || isMinimized() || isMaximized() || isFullScreen())
            return;

        setWindowFlag(Qt::WindowStaysOnTopHint, pin);
        show();
        pinButton->setChecked(pin);
    });
    connect(m_titleBar, &TitleBar::minimizeRequested, this, &QWidget::showMinimized);
    connect(m_titleBar, &TitleBar::maximizeRequested, this, [this](bool max) {
        if (max)
            showMaximized();
        else
            showNormal();

        // It's a Qt issue that if a QAbstractButton::clicked triggers a window's maximization,
        // the button remains to be hovered until the mouse move. As a result, we need to
        // manually send leave events to the button.
        QWidget *widget = m_titleBar->maxButton();
        if (!widget) {
            return;
        }
        QTimer::singleShot(0, widget, [widget]() {
            const QScreen *screen = widget->screen();
            const QPoint globalPos = QCursor::pos(screen);
            if (!QRect(widget->mapToGlobal(QPoint{ 0, 0 }), widget->size()).contains(globalPos)) {
                QCoreApplication::postEvent(widget, new QEvent(QEvent::Leave));
                if (widget->testAttribute(Qt::WA_Hover)) {
                    const QPoint localPos = widget->mapFromGlobal(globalPos);
                    const QPoint scenePos = widget->window()->mapFromGlobal(globalPos);
                    static constexpr const auto oldPos = QPoint{ };
                    const Qt::KeyboardModifiers modifiers = QGuiApplication::keyboardModifiers();
                    const auto event = new QHoverEvent(QEvent::HoverLeave, scenePos, globalPos,
                                                       oldPos, modifiers);
                    Q_UNUSED(localPos);
                    QCoreApplication::postEvent(widget, event);
                }
            }
        });
    });
    connect(m_titleBar, &TitleBar::closeRequested, this, &QWidget::close);
    setMenuWidget(m_titleBar);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QFile layoutFile(Util::dataDir() % '/' % Util::LAYOUT_FILE_NAME);
    if (layoutFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        layoutFile.write(saveGeometry());

    g_appConfig.dialog.lastEnteredIndex = m_twgt->currentIndex();

    QWidget::closeEvent(e);
}

void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    if (e->type() == QEvent::LanguageChange)
        themeChanged(&ThemeManager::instance().themes().at(
                ThemeManager::instance().currentThemeIndex()));
}
