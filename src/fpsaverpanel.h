#ifndef FPSAVERPANEL_H
#define FPSAVERPANEL_H

#include <QWidget>

namespace Ui {
class fpsAverPanel;
}

class fpsAverPanel : public QWidget
{
    Q_OBJECT

public:
    explicit fpsAverPanel(QWidget *parent = nullptr);
    ~fpsAverPanel();

private:
    Ui::fpsAverPanel *ui;
};

#endif // FPSAVERPANEL_H
