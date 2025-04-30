#include "fpspixelpanel.h"
#include "ui_fpspixelpanel.h"

fpsPixelPanel::fpsPixelPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fpsPixelPanel)
{
    ui->setupUi(this);
}

fpsPixelPanel::~fpsPixelPanel()
{
    delete ui;
}

