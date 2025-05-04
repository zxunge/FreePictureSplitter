#ifndef FPSSETTINGSDIALOG_H
#define FPSSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class fpsSettingsDialog;
}

class fpsSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit fpsSettingsDialog(QWidget *parent = nullptr);
    ~fpsSettingsDialog();

private:
    Ui::fpsSettingsDialog *ui;
};

#endif // FPSSETTINGSDIALOG_H
