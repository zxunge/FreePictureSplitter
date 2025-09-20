// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
} // namespace Ui
namespace QWK {
class WidgetWindowAgent;
} // namespace QWK
class QEvent;
class QCloseEvent;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

private:
    void installWindowAgent();

protected:
    bool event(QEvent *event) override;
    void closeEvent(QCloseEvent *e) override;

private:
    Ui::MainWindow *ui;
    QWK::WidgetWindowAgent *m_windowAgent;
};

#endif // MAINWINDOW_H
