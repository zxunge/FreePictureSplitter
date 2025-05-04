#include "fpssettingsdialog.h"
#include "ui_fpssettingsdialog.h"

fpsSettingsDialog::fpsSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fpsSettingsDialog)
{
    ui->setupUi(this);
}

fpsSettingsDialog::~fpsSettingsDialog()
{
    delete ui;
}
