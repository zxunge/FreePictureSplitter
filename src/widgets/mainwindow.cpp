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

#include "utils/jsonconfigitems.h"
#include "utils/stdpaths.h"
#include "utils/thememanager.h"
#include "utils/globals.h"

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
#include <QVBoxLayout>
#include <QProgressBar>

#include <oclero/qlementine/style/Theme.hpp>

using namespace Qt::Literals::StringLiterals;
using namespace Util;

MainWindow::MainWindow(QWidget *parent) : FramelessWidget(parent)
{
    setAttribute(Qt::WA_DontCreateNativeAncestors);

    // Construct UI
    setMinimumSize({ 800, 600 });
    setCentralWidget(new QWidget(this));

    QVBoxLayout *verticalLayout = new QVBoxLayout(centralWidget());
    verticalLayout->setSpacing(0);

    m_twgt = new FancyTabWidget(centralWidget());
    verticalLayout->addWidget(m_twgt);

    m_statusBar = new QStatusBar();
    statusBar()->setSizeGripEnabled(false);
    statusBar()->setObjectName("statusBar");
    verticalLayout->addWidget(m_statusBar);
    verticalLayout->setContentsMargins(5, 0, 5, 5);

    createTabs();

    setObjectName("MainWindow");
    centralWidget()->setObjectName("centralWidget");
    m_twgt->setObjectName("tabWidget");

    setWindowTitle(qAppName());
    titleBar()->setTitleText(qAppName());
    titleBar()->setBackgroundColor(Qt::white);
    setBackgroundColor(QColor(255, 255, 255, 255));
    setRadius(12);
    setBlurRadius(12);

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

    connect(&Util::ThemeManager::instance(), &Util::ThemeManager::themeChanged, this,
            &MainWindow::themeChanged);

    QFile layoutFile(Util::dataDir() % '/' % Util::LAYOUT_FILE_NAME);
    if (layoutFile.open(QIODevice::ReadOnly))
        FramelessWidget::restoreGeometry(layoutFile.readAll());

    ThemeManager::instance().setTitleBar(titleBar());
}

void MainWindow::themeChanged(const oclero::qlementine::Theme *theme)
{
    Q_UNUSED(theme)
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
