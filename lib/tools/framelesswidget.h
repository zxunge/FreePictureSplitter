// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H

#include "titlebar.h"

#include <QWidget>
#include <QGridLayout>
#include <QApplication>
#include <QPainter>
#include <QPainterPath>

namespace oclero::qlementine {
class Theme;
} // namespace oclero::qlementine

class FramelessWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
public:
    explicit FramelessWidget(QWidget *parent = nullptr);
    ~FramelessWidget();

    // Set window background color
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const { return m_backgroundColor; }

    // Set border corner radius
    void setRadius(const uint &r);
    // Set border shadow color
    void setShadowColor(const QColor &color);
    // Set border shadow blur radius
    void setBlurRadius(const uint &r);

    QWidget *centralWidget() const;
    void setCentralWidget(QWidget *centralWidget);

    TitleBar *titleBar() const;
    void restoreGeometry(const QByteArray &geometry);

protected:
    enum OpFlag { NONE = -1, Top, Bottom, Left, Right, TL, TR, BL, BR };
    void initialize();
    void calculateOpflag(QPoint pos);
    void updateRadius(const uint &r);
    void updateSize();

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *event) override;

private:
    void themeChanged(const oclero::qlementine::Theme *theme);
    QWidget *m_centralWidget = nullptr; // Center widget provided by user
    QWidget *m_border = nullptr; // Widget that simulates the border
    TitleBar *m_titleBar = nullptr; // Title bar
    QGridLayout *m_gridLayout = nullptr;

    int m_opFlag = OpFlag::NONE; // Window move/resize control flag
    bool m_isOp = false; // Whether move/resize is allowed
    QPointF m_lastPos; // Last mouse position
    QColor m_shadowColor = QColor(91, 91, 91, 90); // Border shadow color
    int m_blurRadius = 10; // Border shadow blur radius
    int m_radius = 10; // Border corner radius
    QColor m_backgroundColor = QColor(0, 0, 0, 0);
};

#endif // FRAMELESSWIDGET_H