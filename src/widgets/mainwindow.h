// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>

namespace QWK {
class WidgetWindowAgent;
} // namespace QWK
class QEvent;
class QStatusBar;
class QCloseEvent;
class QProgressBar;
class FancyTabWidget;
class QMenu;
class TitleBar;
namespace oclero::qlementine {
class Theme;
} // namespace oclero::qlementine

class MainWindow final : public QMainWindow
{
    Q_OBJECT
   
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

    QProgressBar *progressBar() const { return m_pbar; }

private:
    void themeChanged(const oclero::qlementine::Theme *theme);
    void createTabs();
    void installWindowAgent();

protected:
    void closeEvent(QCloseEvent *e) override;
    void changeEvent(QEvent *e) override;

private:
    QWK::WidgetWindowAgent *m_windowAgent;

    QProgressBar *m_pbar;
    FancyTabWidget *m_twgt;
    QMenu *m_taskMenu;
    TitleBar *m_titleBar;
};

#endif // MAINWINDOW_H
