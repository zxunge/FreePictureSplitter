// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "fancytabwidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QToolTip>
#include <QApplication>

#include <cmath>
#include <utility>

// ========== Helper functions ==========
static int commonPrefixLength(const QString &s1, const QString &s2)
{
    int n = qMin(s1.size(), s2.size());
    for (int i = 0; i < n; ++i)
        if (s1[i] != s2[i])
            return i;
    return n;
}

// ========== FancyTabBar implementation ==========

FancyTabBar::FancyTabBar(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_Hover, true);
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

FancyTabBar::~FancyTabBar()
{
    for (PrivateTab &tab : m_tabs) {
        delete tab.animation;
        tab.animation = nullptr;
    }
}

void FancyTabBar::insertTab(int index, const QIcon &icon, const QString &label)
{
    PrivateTab tab;
    tab.icon = icon;
    tab.text = label;

    // Create animation
    QVariantAnimation *anim = new QVariantAnimation(this);
    anim->setDuration(160);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutQuad);
    connect(anim, &QVariantAnimation::valueChanged, this, [this, index](const QVariant &value) {
        if (index >= 0 && index < m_tabs.size()) {
            m_tabs[index].fader = value.toReal();
            update(tabRect(index));
        }
    });
    tab.animation = anim;

    m_tabs.insert(index, tab);
    if (m_currentIndex >= index)
        ++m_currentIndex;
    updateGeometry();
}

void FancyTabBar::removeTab(int index)
{
    if (index < 0 || index >= m_tabs.size())
        return;
    delete m_tabs[index].animation;
    m_tabs.removeAt(index);
    if (m_currentIndex == index)
        setCurrentIndex(-1);
    else if (m_currentIndex > index)
        --m_currentIndex;
    updateGeometry();
}

void FancyTabBar::setTab(int index, const QIcon &icon, const QString &label)
{
    if (index < 0 || index >= m_tabs.size())
        return;
    PrivateTab &tab = m_tabs[index];
    tab.icon = icon;
    tab.text = label;
    update(tabRect(index));
    updateGeometry();
}

void FancyTabBar::setTabEnabled(int index, bool enable)
{
    if (index >= 0 && index < m_tabs.size()) {
        m_tabs[index].enabled = enable;
        update(tabRect(index));
    }
}

bool FancyTabBar::isTabEnabled(int index) const
{
    return (index >= 0 && index < m_tabs.size()) ? m_tabs[index].enabled : false;
}

void FancyTabBar::setTabToolTip(int index, const QString &toolTip)
{
    if (index >= 0 && index < m_tabs.size())
        m_tabs[index].toolTip = toolTip;
}

QString FancyTabBar::tabToolTip(int index) const
{
    return (index >= 0 && index < m_tabs.size()) ? m_tabs[index].toolTip : QString();
}

void FancyTabBar::setCurrentIndex(int index)
{
    if (index == m_currentIndex || (index >= 0 && !isTabEnabled(index)))
        return;
    emit currentAboutToChange(index);
    m_currentIndex = index;
    update();
    emit currentChanged(index);
}

void FancyTabBar::setIconsOnly(bool iconsOnly)
{
    m_iconsOnly = iconsOnly;
    updateGeometry();
}

void FancyTabBar::setBaseColor(const QColor &color)
{
    m_baseColor = color;
    update();
}

void FancyTabBar::setHoverColor(const QColor &color)
{
    m_hoverColor = color;
    update();
}

void FancyTabBar::setSelectedColor(const QColor &color)
{
    m_selectedColor = color;
    update();
}

void FancyTabBar::setShadowColor(const QColor &color)
{
    m_shadowColor = color;
    update();
}

void FancyTabBar::setTextColor(const QColor &normal, const QColor &selected)
{
    m_textColorNormal = normal;
    m_textColorSelected = selected;
    update();
}

QRect FancyTabBar::tabRect(int index) const
{
    QSize sh = tabSizeHint();
    int totalHeight = sh.height() * m_tabs.size();
    if (totalHeight > height())
        sh.setHeight(height() / m_tabs.size());
    return QRect(0, index * sh.height(), sh.width(), sh.height());
}

QSize FancyTabBar::tabSizeHint(bool minimum) const
{
    if (m_iconsOnly) {
        return QSize(32, 32);
    }
    QFont boldFont = font();
    boldFont.setPointSizeF(9.0);
    boldFont.setBold(true);
    QFontMetrics fm(boldFont);
    int maxTextWidth = 0;
    for (const auto &tab : std::as_const(m_tabs)) {
        int w = fm.horizontalAdvance(tab.text);
        if (w > maxTextWidth)
            maxTextWidth = w;
    }
    int iconHeight = minimum ? 0 : 32;
    int spacing = 8;
    int width = qMax(60 + spacing, maxTextWidth + 8);
    return QSize(width, iconHeight + spacing + fm.height());
}

QSize FancyTabBar::sizeHint() const
{
    QSize sh = tabSizeHint();
    return QSize(sh.width(), sh.height() * m_tabs.size());
}

void FancyTabBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.fillRect(rect(), m_baseColor);

    for (int i = 0; i < m_tabs.size(); ++i) {
        if (i != m_currentIndex)
            paintTab(&painter, i);
    }
    if (m_currentIndex >= 0)
        paintTab(&painter, m_currentIndex);
}

void FancyTabBar::paintTab(QPainter *painter, int index) const
{
    if (index < 0 || index >= m_tabs.size())
        return;
    const PrivateTab &tab = m_tabs.at(index);
    QRect rect = tabRect(index);
    bool selected = (index == m_currentIndex);
    bool enabled = tab.enabled;

    // Selected background (gradient)
    if (selected) {
        drawSelectedTabBackground(painter, rect);
    }

    // Hover overlay (gradient effect)
    if (tab.fader > 0.0 && !selected && enabled) {
        drawHoverOverlay(painter, rect, tab.fader);
    }

    // Draw icon and text
    if (m_iconsOnly) {
        drawIconWithShadow(painter, tab.icon, rect, enabled, selected);
    } else {
        QFont boldFont = painter->font();
        boldFont.setPointSizeF(9.0);
        boldFont.setBold(true);
        painter->setFont(boldFont);

        // Icon area
        QRect iconRect = rect.adjusted(0, 4, 0, -painter->fontMetrics().height() - 4);
        drawIconWithShadow(painter, tab.icon, iconRect, enabled, selected);

        // Text color
        if (enabled) {
            painter->setPen(selected ? m_textColorSelected : m_textColorNormal);
        } else {
            painter->setPen(m_textColorDisabled);
        }

        // Draw text (word-wrapped)
        QRect textRect = rect.adjusted(4, iconRect.height() + 8, -4, -4);
        painter->drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, tab.text);
    }
}

void FancyTabBar::drawSelectedTabBackground(QPainter *p, const QRect &rect) const
{
    // Mimic original Qt Creator selected background: semi-transparent white gradient from top to
    // bottom
    QLinearGradient grad(rect.topLeft(), rect.bottomLeft());
    grad.setColorAt(
            0, QColor(m_selectedColor.red(), m_selectedColor.green(), m_selectedColor.blue(), 100));
    grad.setColorAt(
            1, QColor(m_selectedColor.red(), m_selectedColor.green(), m_selectedColor.blue(), 210));
    p->fillRect(rect, grad);

    // Add simple inner glow effect (left highlight line)
    p->setPen(QColor(m_selectedColor.red(), m_selectedColor.green(), m_selectedColor.blue(), 50));
    p->drawLine(rect.topLeft(), rect.bottomLeft());
    // Right dark edge
    p->setPen(QColor(0, 0, 0, 40));
    p->drawLine(rect.topRight(), rect.bottomRight());
}

void FancyTabBar::drawIconWithShadow(QPainter *p, const QIcon &icon, const QRect &rect,
                                     bool enabled, bool selected) const
{
    QIcon::Mode mode = QIcon::Normal;
    if (!enabled)
        mode = QIcon::Disabled;
    else if (selected)
        mode = QIcon::Active;

    QPixmap pix = icon.pixmap(rect.size(), mode);
    QRect target = pix.rect();
    target.moveCenter(rect.center());

    // Draw simple shadow (offset by 1px, semi-transparent black)
    if (enabled && !selected) { // Only add shadow for normal state, avoid overlapping on selected
        QRect shadowRect = target.translated(1, 1);
        QPixmap shadowPix = pix; // copy
        QPainter shadowPainter(&shadowPix);
        shadowPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
        shadowPainter.fillRect(shadowPix.rect(), m_shadowColor);
        shadowPainter.end();
        p->drawPixmap(shadowRect, shadowPix);
    }

    p->drawPixmap(target, pix);
}

void FancyTabBar::drawHoverOverlay(QPainter *p, const QRect &rect, qreal opacity) const
{
    // Gradient from left to right, mimicking original hover effect
    QLinearGradient grad(rect.topLeft(), rect.topRight());
    grad.setColorAt(0, Qt::transparent);
    grad.setColorAt(0.5, m_hoverColor);
    grad.setColorAt(1, Qt::transparent);

    p->save();
    p->setOpacity(opacity);
    p->fillRect(rect, grad);
    p->restore();
}

void FancyTabBar::mousePressEvent(QMouseEvent *event)
{
    for (int i = 0; i < m_tabs.size(); ++i) {
        if (tabRect(i).contains(event->pos())) {
            if (isTabEnabled(i)) {
                setCurrentIndex(i);
            }
            break;
        }
    }
}

void FancyTabBar::mouseMoveEvent(QMouseEvent *event)
{
    int newHover = -1;
    for (int i = 0; i < m_tabs.size(); ++i) {
        if (tabRect(i).contains(event->pos())) {
            newHover = i;
            break;
        }
    }
    if (newHover != m_hoverIndex) {
        if (m_hoverIndex >= 0 && m_tabs[m_hoverIndex].animation) {
            auto anim = m_tabs[m_hoverIndex].animation;
            anim->stop();
            anim->setDirection(QAbstractAnimation::Backward);
            anim->start();
        }
        m_hoverIndex = newHover;

        if (m_hoverIndex >= 0 && m_tabs[m_hoverIndex].animation) {
            auto anim = m_tabs[m_hoverIndex].animation;
            anim->stop();
            anim->setDirection(QAbstractAnimation::Forward);
            anim->start();
        }
    }
}

void FancyTabBar::leaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    if (m_hoverIndex >= 0 && m_tabs[m_hoverIndex].animation) {
        auto anim = m_tabs[m_hoverIndex].animation;
        anim->stop();
        anim->setDirection(QAbstractAnimation::Backward);
        anim->start();
    }
    m_hoverIndex = -1;
}

bool FancyTabBar::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip && m_hoverIndex >= 0) {
        const QString tt = tabToolTip(m_hoverIndex);
        if (!tt.isEmpty()) {
            QToolTip::showText(static_cast<QHelpEvent *>(event)->globalPos(), tt, this);
            return true;
        }
    }
    return QWidget::event(event);
}

void FancyTabBar::updateHoverAnimation()
{
    // Animation automatically triggers update when fader changes
    update();
}

// ========== FancyTabWidget implementation ==========

FancyTabWidget::FancyTabWidget(QWidget *parent) : QWidget(parent)
{
    m_tabBar = new FancyTabBar(this);
    m_cornerWidgetContainer = new QWidget(this);
    m_cornerWidgetContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    QVBoxLayout *cornerLayout = new QVBoxLayout(m_cornerWidgetContainer);
    cornerLayout->setContentsMargins(0, 0, 0, 0);
    cornerLayout->addStretch();

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);
    leftLayout->addWidget(m_tabBar);
    leftLayout->addWidget(m_cornerWidgetContainer);

    m_stackedLayout = new QStackedLayout;

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(1);
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(m_stackedLayout);

    setBaseColor(m_tabBar->baseColor());

    connect(m_tabBar, &FancyTabBar::currentChanged, this, &FancyTabWidget::showWidget);
    connect(m_tabBar, &FancyTabBar::currentAboutToChange, this,
            &FancyTabWidget::currentAboutToShow);
}

void FancyTabWidget::insertTab(int index, QWidget *page, const QIcon &icon, const QString &label)
{
    m_stackedLayout->insertWidget(index, page);
    m_tabBar->insertTab(index, icon, label);
}

void FancyTabWidget::removeTab(int index)
{
    m_stackedLayout->removeWidget(m_stackedLayout->widget(index));
    m_tabBar->removeTab(index);
}

void FancyTabWidget::setTab(int index, const QIcon &icon, const QString &label)
{
    m_tabBar->setTab(index, icon, label);
}

void FancyTabWidget::setTabEnabled(int index, bool enable)
{
    m_tabBar->setTabEnabled(index, enable);
}

bool FancyTabWidget::isTabEnabled(int index) const
{
    return m_tabBar->isTabEnabled(index);
}

void FancyTabWidget::setTabToolTip(int index, const QString &toolTip)
{
    m_tabBar->setTabToolTip(index, toolTip);
}

void FancyTabWidget::setCurrentIndex(int index)
{
    m_tabBar->setCurrentIndex(index);
}

void FancyTabWidget::setBaseColor(const QColor &color)
{
    m_tabBar->setBaseColor(color);
    m_cornerWidgetContainer->setAutoFillBackground(true);
    QPalette pal = m_cornerWidgetContainer->palette();
    pal.setColor(QPalette::Window, color);
    m_cornerWidgetContainer->setPalette(pal);
}

void FancyTabWidget::showWidget(int index)
{
    m_stackedLayout->setCurrentIndex(index);
    emit currentChanged(index);
}

void FancyTabWidget::addCornerWidget(QWidget *widget)
{
    auto layout = qobject_cast<QVBoxLayout *>(m_cornerWidgetContainer->layout());
    layout->insertWidget(layout->count() - 1, widget); // Insert before the stretch
}

void FancyTabWidget::insertCornerWidget(int index, QWidget *widget)
{
    auto layout = qobject_cast<QVBoxLayout *>(m_cornerWidgetContainer->layout());
    layout->insertWidget(index, widget);
}
