#ifndef FPSWINDOW_H
#define FPSWINDOW_H

#include <QGoodWindow>
#include "fpscentralwidget.h"

class QMenuBar;

class fpsWindow : public QGoodWindow
{
    Q_OBJECT
public:
    fpsWindow(QWidget *parent = nullptr);
    ~fpsWindow();

private:
    fpsCentralWidget *wgt;
    QMenuBar *menuBar;
};

#endif // FPSWINDOW_H
