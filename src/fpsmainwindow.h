//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later


#ifndef FPSMAINWINDOW_H
#define FPSMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class fpsMainWindow;
}

namespace QWK {
    class WidgetWindowAgent;
    class StyleAgent;
} // namespace QWK

class fpsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit fpsMainWindow(QWidget *parent = nullptr);
    ~fpsMainWindow();

    enum Theme {
        Dark,
        Light,
               };
    Q_ENUM(Theme)

Q_SIGNALS:
    void themeChanged();

protected:
    bool event(QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void installWindowAgent();
    void loadStyleSheet(Theme theme);

    Theme currentTheme{};

    QWK::WidgetWindowAgent *windowAgent;

    Ui::fpsMainWindow *ui;
};

#endif // FPSMAINWINDOW_H
