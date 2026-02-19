// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
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
class QProgressBar;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow &get();
    QProgressBar *progressBar() const { return m_pbar; }

private:
    void installWindowAgent();

protected:
    bool event(QEvent *event) override;
    void closeEvent(QCloseEvent *e) override;

private:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;
    QWK::WidgetWindowAgent *m_windowAgent;
    QProgressBar *m_pbar;
};

#endif // MAINWINDOW_H
