#ifndef FPSMAINWINDOW_H
#define FPSMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class fpsMainWindow;
}

class fpsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit fpsMainWindow(QWidget *parent = nullptr);
    ~fpsMainWindow();

private:
    Ui::fpsMainWindow *ui;
};

#endif // FPSMAINWINDOW_H
