// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef FANCYTABWIDGET_H
#define FANCYTABWIDGET_H

#include <QWidget>
#include <QIcon>
#include <QSize>
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QStackedLayout>

// Custom tab bar (vertical layout, with visual effects)
class FancyTabBar : public QWidget
{
    Q_OBJECT
public:
    explicit FancyTabBar(QWidget *parent = nullptr);
    ~FancyTabBar();

    // Tab management
    void insertTab(int index, const QIcon &icon, const QString &label);
    void removeTab(int index);
    void setTab(int index, const QIcon &icon, const QString &label);
    void setTabEnabled(int index, bool enable);
    bool isTabEnabled(int index) const;
    void setTabToolTip(int index, const QString &toolTip);
    QString tabToolTip(int index) const;

    int count() const { return m_tabs.size(); }
    int currentIndex() const { return m_currentIndex; }
    void setCurrentIndex(int index);

    // Appearance settings
    void setIconsOnly(bool iconsOnly);
    QColor baseColor() const { return m_baseColor; }
    void setBaseColor(const QColor &color);
    void setHoverColor(const QColor &color);
    void setSelectedColor(const QColor &color);
    void setShadowColor(const QColor &color);
    void setTextColor(const QColor &normal, const QColor &selected);

    QSize sizeHint() const override;

    int topMargin() const { return m_topMargin; }
    void setTopMargin(int topMargin) { m_topMargin = topMargin; }

signals:
    void currentChanged(int index);
    void currentAboutToChange(int index);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    bool event(QEvent *event) override;

private:
    struct PrivateTab
    {
        QIcon icon;
        QString text;
        QString toolTip;
        bool enabled = true;
        qreal fader = 0.0;
        QVariantAnimation *animation = nullptr;
    };
    QList<PrivateTab> m_tabs;

    int m_currentIndex = -1;
    int m_hoverIndex = -1;
    bool m_iconsOnly = false;

    // Color configuration
    QColor m_baseColor = QColor(0xf9f9f9);
    QColor m_hoverColor = QColor(150, 150, 150, 80);
    QColor m_selectedColor = QColor(192, 192, 192);
    QColor m_shadowColor = QColor(0xfc, 0xfc, 0xfc, 80);
    QColor m_textColorNormal = Qt::black;
    QColor m_textColorSelected = Qt::black;
    QColor m_textColorDisabled = Qt::gray;

    int m_topMargin = 16;

private:
    QRect tabRect(int index) const;
    QSize tabSizeHint(bool minimum = false) const;
    void paintTab(QPainter *painter, int tabIndex) const;

    // Effect drawing functions
    void drawSelectedTabBackground(QPainter *p, const QRect &rect) const;
    void drawIconWithShadow(QPainter *p, const QIcon &icon, const QRect &rect, bool enabled,
                            bool selected) const;
    void drawHoverOverlay(QPainter *p, const QRect &rect, qreal opacity) const;

    void startHoverAnimation(int index, bool fadeIn);
    void setHoverIndex(int index) { m_hoverIndex = index; }
    int hoverIndex() const { return m_hoverIndex; }
    void updateHoverAnimation();
};

// Main tab container widget
class FancyTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FancyTabWidget(QWidget *parent = nullptr);

    // Tab management
    void insertTab(int index, QWidget *page, const QIcon &icon, const QString &label);
    void removeTab(int index);
    void setTab(int index, const QIcon &icon, const QString &label);
    void setTabEnabled(int index, bool enable);
    bool isTabEnabled(int index) const;
    void setTabToolTip(int index, const QString &toolTip);

    int currentIndex() const { return m_tabBar->currentIndex(); }
    void setCurrentIndex(int index);

    // Appearance proxy
    void setIconsOnly(bool iconsOnly) { m_tabBar->setIconsOnly(iconsOnly); }
    void setBaseColor(const QColor &color);
    void setHoverColor(const QColor &color) { m_tabBar->setHoverColor(color); }
    void setSelectedColor(const QColor &color) { m_tabBar->setSelectedColor(color); }
    void setShadowColor(const QColor &color) { m_tabBar->setShadowColor(color); }
    void setTabTextColor(const QColor &normal, const QColor &selected)
    {
        m_tabBar->setTextColor(normal, selected);
    }

    // Add corner widgets (e.g., extra buttons)
    void addCornerWidget(QWidget *widget);
    void insertCornerWidget(int index, QWidget *widget);

Q_SIGNALS:
    void currentChanged(int index);
    void currentAboutToShow(int index); // Emitted right before switching

private Q_SLOTS:
    void showWidget(int index);

private:
    FancyTabBar *m_tabBar;
    QWidget *m_cornerWidgetContainer;
    QStackedLayout *m_stackedLayout;
};

#endif // FANCYTABWIDGET_H
