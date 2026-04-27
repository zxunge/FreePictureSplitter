// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "framelesswidget.h"

class QEvent;
class QStatusBar;
class QCloseEvent;
class QProgressBar;
class FancyTabWidget;
class QMenu;

class MainWindow final : public FramelessWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

    QProgressBar *progressBar() const { return m_pbar; }
    QStatusBar *statusBar() const { return m_statusBar; }

private:
    void createTabs();

protected:
    void closeEvent(QCloseEvent *e) override;
    void changeEvent(QEvent *e) override;

private:
    QProgressBar *m_pbar;
    QStatusBar *m_statusBar;
    FancyTabWidget *m_twgt;
    QMenu *m_taskMenu;
};

#endif // MAINWINDOW_H
