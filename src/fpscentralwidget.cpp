#include "fpscentralwidget.h"
#include "ui_fpscentralwidget.h"
#include <QFileDialog>
#include <QDebug>
#include <QColor>
#include <QPainter>
#include <QStyleOption>
#include <QStringLiteral>
#include "qjsonconfig.h"

extern QJSONConfig cfg;

fpsCentralWidget::fpsCentralWidget(QGoodWindow *parent) :
    QGoodCentralWidget(parent),
    ui(new Ui::fpsCentralWidget)
{
    ui->setupUi(this);
}

fpsCentralWidget::~fpsCentralWidget()
{
    delete ui;
}
