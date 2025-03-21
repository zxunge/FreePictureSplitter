#include "fpsmainwindow.h"
#include "ui_fpsmainwindow.h"

#include <QFile>
#include <QStyle>

#include <QWKWidgets/widgetwindowagent.h>

fpsMainWindow::fpsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fpsMainWindow)
{
    ui->setupUi(this);
    auto agent = new QWK::WidgetWindowAgent(this);
    agent->setup(this);

    setWindowTitle(QStringLiteral("FreePictureSplitter"));
}

fpsMainWindow::~fpsMainWindow()
{
    delete ui;
}
