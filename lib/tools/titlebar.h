// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QStyle>

namespace oclero::qlementine {
class Theme;
} // namespace oclero::qlementine

class TitleBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(QColor hoverColorMin READ hoverColorMin WRITE setHoverColorMin)
    Q_PROPERTY(QColor hoverColorMax READ hoverColorMax WRITE setHoverColorMax)
    Q_PROPERTY(QColor hoverColorClose READ hoverColorClose WRITE setHoverColorClose)
    Q_PROPERTY(QColor pressedColorMin READ pressedColorMin WRITE setPressedColorMin)
    Q_PROPERTY(QColor pressedColorMax READ pressedColorMax WRITE setPressedColorMax)
    Q_PROPERTY(QColor pressedColorClose READ pressedColorClose WRITE setPressedColorClose)

public:
    explicit TitleBar(QWidget *parent = nullptr);
    ~TitleBar();

    void setTitleText(const QString &text);
    void setTitleIcon(const QString &path);
    void setTitleIcon(const QPixmap &icon);
    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const { return m_backgroundColor; }
    void setTextColor(const QColor &color);
    QColor textColor() const { return m_textColor; }
    void setHeight(const uint &h);

    void setTitleTextFont(const QFont &font);
    void addWidget(QWidget *w);
    void addLayout(QLayout *layout);
    void setHoverColorMin(const QColor &color);
    QColor hoverColorMin() const { return m_hoverColorMin; }
    void setHoverColorMax(const QColor &color);
    QColor hoverColorMax() const { return m_hoverColorMax; }
    void setHoverColorClose(const QColor &color);
    QColor hoverColorClose() const { return m_hoverColorClose; }
    void setPressedColorMin(const QColor &color);
    QColor pressedColorMin() const { return m_pressedColorMin; }
    void setPressedColorMax(const QColor &color);
    QColor pressedColorMax() const { return m_pressedColorMax; }
    void setPressedColorClose(const QColor &color);
    QColor pressedColorClose() const { return m_pressedColorClose; }

    void showFull(const bool &isFull);
    void setMoveEnable(const bool &moveEnable);
    void setRadius(const uint &radius);

    QPushButton *closeButton() const;
    QPushButton *maxButton() const;
    QPushButton *minButton() const;

    bool maximizedState() const;

public slots:
    void maximized();
    void minimized();

signals:
    void closed();
    void movePos(QPointF movePoint);
    void maximizedStateChanged(bool is);

protected:
    void moveTopParent(QWidget *pWgt, QPoint movePoint);

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    void themeChanged(const oclero::qlementine::Theme *theme);
    QHBoxLayout *m_mainLayout = nullptr;
    QHBoxLayout *m_customLayout = nullptr;
    QPushButton *m_btnClose = nullptr;
    QPushButton *m_btnMax = nullptr;
    QPushButton *m_btnMin = nullptr;
    QLabel *m_labIcon = nullptr;
    QLabel *m_labTitle = nullptr;

    bool m_isPressed = false;
    bool m_moveEnabled = true;
    bool m_ignoreNextPress = false;
    QPointF m_mousePos = QPoint(0, 0);
    uint m_height = 30;
    QColor m_backgroundColor = Qt::white;
    QColor m_textColor = Qt::black;
    uint m_radius = 10;
    QColor m_hoverColorMin = QColor(240, 240, 240);
    QColor m_hoverColorMax = QColor(240, 240, 240);
    QColor m_hoverColorClose = QColor(232, 17, 35);
    QColor m_pressedColorMin = QColor(220, 220, 220);
    QColor m_pressedColorMax = QColor(220, 220, 220);
    QColor m_pressedColorClose = QColor(232, 17, 35);
};

#endif // TITLEBAR_H