// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

    QProgressBar *progressBar() const { return m_pbar; }
    QTabWidget *tabWidget() const { return m_twgt; }

private:
    void createTabs();
    void installWindowAgent();

protected:
    bool event(QEvent *event) override;
    void closeEvent(QCloseEvent *e) override;
    void changeEvent(QEvent *e) override;

private:
    QWK::WidgetWindowAgent *m_windowAgent;
    QProgressBar *m_pbar;
    QTabWidget *m_twgt;
};

#endif // MAINWINDOW_H
