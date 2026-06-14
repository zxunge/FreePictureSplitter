// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "titlebar.h"
#include "../../src/utils/thememanager.h"

#include <QPushButton>
#include <QPalette>
#include <QLabel>
#include <QHBoxLayout>

#include <oclero/qlementine/style/Theme.hpp>
#include <widgetframe/windowbutton.h>

using namespace Qt::Literals::StringLiterals;

TitleBar::TitleBar(QWidget *parent) : QWK::WindowBar(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setObjectName("titleBar");
    setAttribute(Qt::WA_StyledBackground);

    auto minButton = new QWK::WindowButton();
    minButton->setObjectName(u"min-button"_s);
    minButton->setProperty("system-button", true);
    minButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto maxButton = new QWK::WindowButton();
    maxButton->setCheckable(true);
    maxButton->setObjectName(u"max-button"_s);
    maxButton->setProperty("system-button", true);
    maxButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto closeButton = new QWK::WindowButton();
    closeButton->setObjectName(u"close-button"_s);
    closeButton->setProperty("system-button", true);
    closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    setMinButton(minButton);
    setMaxButton(maxButton);
    setCloseButton(closeButton);

    setBackgroundColor(m_backgroundColor);
    setHoverColorMin(m_hoverColorMin);
    setHoverColorMax(m_hoverColorMax);
    setHoverColorClose(m_hoverColorClose);

    connect(&Util::ThemeManager::instance(), &Util::ThemeManager::themeChanged, this,
            &TitleBar::themeChanged);
}

TitleBar::~TitleBar() { }

void TitleBar::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    setStyleSheet(u"QWidget#titleBar{background-color:"_s % m_backgroundColor.name()
                  % u";}QWK--WindowBar>QAbstractButton[system-button=true]"
                    "{qproperty-iconSize:12px 12px;min-width:50px;border:none;"
                    "padding:0;background-color:transparent;}"
                    "QWK--WindowBar>QLabel#win-title-label{"
                    "padding:0;border:none;background-color:transparent;"
                    "min-height:28px;}"
                    "QWK--WindowBar>QAbstractButton#icon-button{"
                    "qproperty-iconNormal:url(\":/icons/fps.ico\");qproperty-iconSize:18px 18px;"
                    "min-width:40px;border:none;padding:0;background-color:transparent;}"_s);
}

void TitleBar::setHoverColorMin(const QColor &color)
{
    m_hoverColorMin = color;
    minButton()->setStyleSheet(u"QWK--WindowBar>QAbstractButton:hover{background-color:"_s
                               % m_hoverColorMin.name()
                               % u";}QWK--WindowBar>QAbstractButton:pressed{background-color:"_s
                               % m_pressedColorMin.name() % u";}"_s);
}

void TitleBar::setHoverColorMax(const QColor &color)
{
    m_hoverColorMax = color;
    maxButton()->setStyleSheet(u"QWK--WindowBar>QAbstractButton:hover{background-color:"_s
                               % m_hoverColorMax.name()
                               % u";}QWK--WindowBar>QAbstractButton:pressed{background-color:"_s
                               % m_pressedColorMax.name() % u";}"_s);
}

void TitleBar::setHoverColorClose(const QColor &color)
{
    m_hoverColorClose = color;
    closeButton()->setStyleSheet(u"QWK--WindowBar>QAbstractButton:hover{background-color:"_s
                                 % m_hoverColorClose.name()
                                 % u";}QWK--WindowBar>QAbstractButton:pressed{background-color:"_s
                                 % m_pressedColorClose.name() % u";}"_s);
}

void TitleBar::setPressedColorMin(const QColor &color)
{
    m_pressedColorMin = color;
    minButton()->setStyleSheet(u"QWK--WindowBar>QAbstractButton:hover{background-color:"_s
                               % m_hoverColorMin.name()
                               % u";}QWK--WindowBar>QAbstractButton:pressed{background-color:"_s
                               % m_pressedColorMin.name() % u";}"_s);
}

void TitleBar::setPressedColorMax(const QColor &color)
{
    m_pressedColorMax = color;
    maxButton()->setStyleSheet(u"QWK--WindowBar>QAbstractButton:hover{background-color:"_s
                               % m_hoverColorMax.name()
                               % u";}QWK--WindowBar>QAbstractButton:pressed{background-color:"_s
                               % m_pressedColorMax.name() % u";}"_s);
}

void TitleBar::setPressedColorClose(const QColor &color)
{
    m_pressedColorClose = color;
    closeButton()->setStyleSheet(u"QWK--WindowBar>QAbstractButton:hover{background-color:"_s
                                 % m_hoverColorClose.name()
                                 % u";}QWK--WindowBar>QAbstractButton:pressed{background-color:"_s
                                 % m_pressedColorClose.name() % u";}"_s);
}

void TitleBar::themeChanged(const oclero::qlementine::Theme *theme)
{
    setBackgroundColor(theme->backgroundColorMain2);
    setHoverColorMax(theme->neutralColorHovered);
    setHoverColorMin(theme->neutralColorHovered);
    setPressedColorMin(theme->neutralColorPressed);
    setPressedColorMax(theme->neutralColorPressed);
    QPalette pe;
    pe.setColor(QPalette::WindowText, theme->secondaryColor);
    titleLabel()->setPalette(pe);
}