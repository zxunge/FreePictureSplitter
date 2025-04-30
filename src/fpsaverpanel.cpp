#include "fpsaverpanel.h"
#include "ui_fpsaverpanel.h"

fpsAverPanel::fpsAverPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fpsAverPanel)
{
    ui->setupUi(this);
}

fpsAverPanel::~fpsAverPanel()
{
    delete ui;
}
