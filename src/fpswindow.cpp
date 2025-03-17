#include "fpswindow.h"
#include "qmenu.h"
#include "qmenubar.h"
#include <QGoodCentralWidget>
#include <QMenuBar>
#include <QMenu>

fpsWindow::fpsWindow(QWidget *parent)
    : QGoodWindow(parent)
{
    wgt = new fpsCentralWidget(this);

    //Function that sets the state holder theme
    auto func_theme = [=]{
        qGoodStateHolder->setCurrentThemeDark(QGoodWindow::isSystemThemeDark());
    };

    //React to state holder theme change and apply our dark or light theme to the whole app
    connect(qGoodStateHolder, &QGoodStateHolder::currentThemeChanged, this, [=]{
        if (qGoodStateHolder->isCurrentThemeDark())
        {
            QGoodWindow::setAppDarkTheme();
            setNativeDarkModeEnabledOnWindows(true);
        }
        else
        {
            QGoodWindow::setAppLightTheme();
            setNativeDarkModeEnabledOnWindows(false);
        }
    });

    //React to system theme change
    connect(this, &QGoodWindow::systemThemeChanged, this, func_theme);

    //Set the central widget of QGoodWindow
    setCentralWidget(wgt);

    //Set initial theme
    func_theme();

    // We need to set up the menu bar manually
    menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar->addMenu(fileMenu);

}

fpsWindow::~fpsWindow()
{
    delete wgt;
    delete menuBar;
}
