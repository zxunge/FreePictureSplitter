// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "titlebar.h"
#include "../../src/utils/thememanager.h"

#include <oclero/qlementine/style/Theme.hpp>

using namespace Qt::Literals::StringLiterals;

TitleBar::TitleBar(QWidget *parent) : QWidget(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setFixedHeight(m_height);
    setObjectName("titleBar");

    m_btnClose = new QPushButton(this);
    m_btnMax = new QPushButton(this);
    m_btnMin = new QPushButton(this);
    m_labIcon = new QLabel(this);
    m_labTitle = new QLabel(this);

    m_btnClose->setFixedSize(m_height * 2, m_height);
    m_btnMin->setFixedSize(m_height * 2, m_height);
    m_btnMax->setFixedSize(m_height * 2, m_height);

    m_labTitle->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    m_labTitle->setContentsMargins(5, 0, 0, 0);
    m_labIcon->setScaledContents(true);
    m_labIcon->setHidden(true);
    m_labIcon->setFixedHeight(m_height - 10);

    setAttribute(Qt::WA_StyledBackground);

    m_mainLayout = new QHBoxLayout;
    m_customLayout = new QHBoxLayout;
    m_mainLayout->addWidget(m_labIcon);
    m_mainLayout->addWidget(m_labTitle);
    m_mainLayout->addLayout(m_customLayout);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_btnMin);
    m_mainLayout->addWidget(m_btnMax);
    m_mainLayout->addWidget(m_btnClose);
    m_mainLayout->setContentsMargins(10, 0, 0, 0);
    m_mainLayout->setSpacing(2);
    m_customLayout->setContentsMargins(0, 0, 0, 0);
    m_customLayout->setSpacing(0);

    setLayout(m_mainLayout);

    m_btnClose->setFocusPolicy(Qt::NoFocus);
    m_btnMin->setFocusPolicy(Qt::NoFocus);
    m_btnMax->setFocusPolicy(Qt::NoFocus);

    connect(m_btnMin, &QPushButton::clicked, this, &TitleBar::minimized);
    connect(m_btnMax, &QPushButton::clicked, this, &TitleBar::maximized);
    connect(m_btnClose, &QPushButton::clicked, this, &TitleBar::closed);
    connect(&Util::ThemeManager::instance(), &Util::ThemeManager::themeChanged, this,
            &TitleBar::themeChanged);
}

TitleBar::~TitleBar() { }

void TitleBar::setTitleText(const QString &text)
{
    m_labTitle->setText(text);
    window()->setWindowTitle(text);
}

void TitleBar::setTitleIcon(const QString &path)
{
    QPixmap pix(path);
    setTitleIcon(pix);
}

void TitleBar::setTitleIcon(const QPixmap &icon)
{
    if (icon.isNull())
        return;
    m_labIcon->setHidden(false);
    QPixmap pix = icon.scaledToHeight(m_height - 10, Qt::SmoothTransformation);
    m_labIcon->setPixmap(pix);
    window()->setWindowIcon(pix);
}

void TitleBar::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    updateStyle();
}

void TitleBar::setTextColor(const QColor &color)
{
    m_textColor = color;
    updateStyle();
}

void TitleBar::setHeight(const uint &h)
{
    m_height = h;
    updateStyle();
}

void TitleBar::setTitleTextFont(const QFont &font)
{
    m_labTitle->setFont(font);
}

void TitleBar::addWidget(QWidget *w)
{
    m_customLayout->addWidget(w);
}

void TitleBar::addLayout(QLayout *layout)
{
    m_customLayout->addLayout(layout);
}

void TitleBar::setHoverColorMin(const QColor &color)
{
    m_hoverColorMin = color;
    updateStyle();
}

void TitleBar::setHoverColorMax(const QColor &color)
{
    m_hoverColorMax = color;
    updateStyle();
}

void TitleBar::setHoverColorClose(const QColor &color)
{
    m_hoverColorClose = color;
    updateStyle();
}

void TitleBar::setPressedColorMin(const QColor &color)
{
    m_pressedColorMin = color;
    updateStyle();
}

void TitleBar::setPressedColorMax(const QColor &color)
{
    m_pressedColorMax = color;
    updateStyle();
}

void TitleBar::setPressedColorClose(const QColor &color)
{
    m_pressedColorClose = color;
    updateStyle();
}

void TitleBar::showFull(const bool &isFull)
{
    QWidget *p = window();
    if (isFull)
        p->showFullScreen();
    else
        p->showMaximized();
}

void TitleBar::setMoveEnable(const bool &moveEnable)
{
    m_moveEnabled = moveEnable;
}

void TitleBar::setRadius(const uint &radius)
{
    m_radius = radius;
    updateStyle();
}

void TitleBar::maximized()
{
    QWidget *p = window();
    if (p->isMaximized()) {
        p->showNormal();
        emit maximizedStateChanged(false);
    } else {
        p->showMaximized();
        emit maximizedStateChanged(true);
    }
}

void TitleBar::minimized()
{
    window()->showMinimized();
}

void TitleBar::updateStyle()
{
    setStyleSheet(
            u"QWidget#titleBar{border-top-left-radius:"_s % QString::number(m_radius)
            % u"px;border-top-right-radius:"_s % QString::number(m_radius)
            % u"px;border-bottom-left-radius:0px;border-bottom-right-radius:0px;background-color:"_s
            % m_backgroundColor.name() % u";}"_s);
    m_labTitle->setStyleSheet(u"color:"_s % m_textColor.name() % u";"_s);
    m_btnClose->setStyleSheet(
            u"QPushButton{border:0px;}QPushButton:hover{background-color:"_s
            % m_hoverColorClose.name() % u";border-top-left-radius:0px;border-top-right-radius:"_s
            % QString::number(m_radius) % u"px;}QPushButton:pressed{background-color:"_s
            % m_pressedColorClose.name() % u";border-top-left-radius:0px;border-top-right-radius:"_s
            % QString::number(m_radius) % u"px;}"_s);
    m_btnMax->setStyleSheet(u"QPushButton{border:0px;}QPushButton:hover{background-color:"_s
                            % m_hoverColorMax.name()
                            % u";border-radius:0px;}QPushButton:pressed{background-color:"_s
                            % m_pressedColorMax.name() % u";border-radius:0px;}"_s);
    m_btnMin->setStyleSheet(u"QPushButton{border:0px;}QPushButton:hover{background-color:"_s
                            % m_hoverColorMin.name()
                            % u";border-radius:0px;}QPushButton:pressed{background-color:"_s
                            % m_pressedColorMin.name() % u";border-radius:0px;}"_s);
    m_labIcon->setStyleSheet(u"border-top-left-radius:"_s % QString::number(m_radius) % u"px;"_s);
    m_labIcon->setAttribute(Qt::WA_TranslucentBackground, true);
    m_labTitle->setAttribute(Qt::WA_TranslucentBackground, true);
    setFixedHeight(m_height);
    m_btnClose->setFixedSize(m_height * 2, m_height);
    m_btnMin->setFixedSize(m_height * 2, m_height);
    m_btnMax->setFixedSize(m_height * 2, m_height);
    m_labIcon->setFixedSize(m_height - 10, m_height - 10);
}

void TitleBar::moveTopParent(QWidget *wgt, QPoint movePoint)
{
    if (nullptr == wgt)
        return;

    if (wgt->parentWidget()) {
        moveTopParent(wgt->parentWidget(), movePoint);
        return;
    }

    QPoint widgetPos = wgt->pos();
    wgt->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
    if (m_ignoreNextPress) {
        m_ignoreNextPress = false;
        return QWidget::mousePressEvent(event);
    }
    m_isPressed = true;
    m_mousePos = event->globalPosition();
    QWidget::mousePressEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_moveEnabled)
        return;

    QPointF movePoint = event->globalPosition() - m_mousePos;
    m_mousePos = event->globalPosition();
    if (m_isPressed && !window()->isMaximized() && !window()->isFullScreen()) {
        window()->move(window()->pos() + movePoint.toPoint());
        emit movePos(movePoint);
    }
    if (m_isPressed && (window()->isMaximized() || window()->isFullScreen())) {
        QPoint pos = event->pos();
        int w = width();
        window()->showNormal();
        pos = QPoint(width() * 1.0 / w * pos.x(), this->pos().y() + pos.y());
        window()->move(m_mousePos.toPoint() - pos);
        emit movePos(movePoint);
        emit maximizedStateChanged(false);
    }

    QWidget::mouseMoveEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    QWidget::mouseReleaseEvent(event);
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    m_ignoreNextPress = true;
    m_isPressed = false;
    if (!m_btnMax->isVisible() || event->button() != Qt::MouseButton::LeftButton)
        return;

    maximized();
    QWidget::mouseDoubleClickEvent(event);
}

void TitleBar::themeChanged(const oclero::qlementine::Theme *theme)
{
    setBackgroundColor(theme->backgroundColorMain2);
    setTextColor(theme->secondaryColor);
    setHoverColorMax(theme->neutralColorHovered);
    setHoverColorMin(theme->neutralColorHovered);
    setPressedColorMin(theme->neutralColorPressed);
    setPressedColorMax(theme->neutralColorPressed);
}

QPushButton *TitleBar::minButton() const
{
    return m_btnMin;
}

QPushButton *TitleBar::maxButton() const
{
    return m_btnMax;
}

QPushButton *TitleBar::closeButton() const
{
    return m_btnClose;
}
