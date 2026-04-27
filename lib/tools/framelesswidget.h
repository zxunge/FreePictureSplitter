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

class FramelessWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FramelessWidget(QWidget *parent = nullptr);
    ~FramelessWidget();

    // Set title bar background color
    void setTitleBarColor(const QColor &color);
    // Set title text color
    void setTitleTextColor(const QColor &color);
    // Set title text font
    void setTitleTextFont(const QFont &font);
    // Set title
    void setTitleText(const QString &text);
    // Set title bar icon
    void setTitleIcon(const QString &path);
    // Set title bar icon
    void setTitleIcon(const QPixmap &icon);

    // Set window background color
    void setBackgroundColor(const QColor &color);

    // Set border corner radius
    void setRadius(const uint &r);
    // Set border shadow color
    void setShadowColor(const QColor &color);
    // Set border shadow blur radius
    void setBlurRadius(const uint &r);

    // Hide title bar minimize button
    void setHiddenMin(const bool &is);
    // Hide title bar maximize button
    void setHiddenMax(const bool &is);
    // Hide title bar
    void setHiddenTitleBar(const bool &is);
    // Set title bar height
    void setTitleBarHeight(const uint &h);
    // Set title bar minimize icon
    void setMinIcon(const QIcon &icon);
    // Set title bar maximize icon
    void setMaxIcon(const QIcon &icon);
    // Set title bar close icon
    void setCloseIcon(const QIcon &icon);
    // Set title bar minimize button hover background color
    void setHoverColorMin(const QColor &color);
    // Set title bar maximize button hover background color
    void setHoverColorMax(const QColor &color);
    // Set title bar close button hover background color
    void setHoverColorClose(const QColor &color);
    // Set title bar minimize button pressed background color
    void setPressedColorMin(const QColor &color);
    // Set title bar maximize button pressed background color
    void setPressedColorMax(const QColor &color);
    // Set title bar close button pressed background color
    void setPressedColorClose(const QColor &color);

    // Add widget after the title
    void addWidgetToTitleBar(QWidget *w);
    // Add layout after the title
    void addLayoutToTitleBar(QLayout *layout);

    QWidget *centralWidget() const;
    void setCentralWidget(QWidget *newCentralWidget);

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
    QColor m_bgColor = QColor(0, 0, 0, 0);
};

#endif // FRAMELESSWIDGET_H