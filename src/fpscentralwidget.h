#ifndef FPSCENTRALWIDGET_H
#define FPSCENTRALWIDGET_H

#include <QGoodWindow>
#include <QGoodCentralWidget>

namespace Ui {
class fpsCentralWidget;
}

class fpsCentralWidget : public QGoodCentralWidget
{
    Q_OBJECT

public:
    explicit fpsCentralWidget(QGoodWindow *parent = nullptr);
    ~fpsCentralWidget();

private:
    Ui::fpsCentralWidget *ui;
};

#endif // FPSCENTRALWIDGET_H
