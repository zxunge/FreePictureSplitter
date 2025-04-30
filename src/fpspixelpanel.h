#ifndef FPSPIXELPANEL_H
#define FPSPIXELPANEL_H

#include <QWidget>

namespace Ui {
class fpsPixelPanel;
}

class fpsPixelPanel : public QWidget
{
    Q_OBJECT

public:
    explicit fpsPixelPanel(QWidget *parent = nullptr);
    ~fpsPixelPanel();

private:
    Ui::fpsPixelPanel *ui;
};

#endif // FPSPIXELPANEL_H
