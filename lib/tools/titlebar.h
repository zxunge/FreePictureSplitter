// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QIcon>

#include <widgetframe/windowbar.h>

namespace oclero::qlementine {
class Theme;
} // namespace oclero::qlementine

class TitleBar : public QWK::WindowBar
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor hoverColorMin READ hoverColorMin WRITE setHoverColorMin)
    Q_PROPERTY(QColor hoverColorMax READ hoverColorMax WRITE setHoverColorMax)
    Q_PROPERTY(QColor hoverColorClose READ hoverColorClose WRITE setHoverColorClose)
    Q_PROPERTY(QColor pressedColorMin READ pressedColorMin WRITE setPressedColorMin)
    Q_PROPERTY(QColor pressedColorMax READ pressedColorMax WRITE setPressedColorMax)
    Q_PROPERTY(QColor pressedColorClose READ pressedColorClose WRITE setPressedColorClose)

public:
    explicit TitleBar(QWidget *parent = nullptr);
    ~TitleBar();

    void setBackgroundColor(const QColor &color);
    QColor backgroundColor() const { return m_backgroundColor; }
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

private:
    void themeChanged(const oclero::qlementine::Theme *theme);

    QColor m_backgroundColor = Qt::white;
    QColor m_hoverColorMin = QColor(240, 240, 240);
    QColor m_hoverColorMax = QColor(240, 240, 240);
    QColor m_hoverColorClose = QColor(232, 17, 35);
    QColor m_pressedColorMin = QColor(220, 220, 220);
    QColor m_pressedColorMax = QColor(220, 220, 220);
    QColor m_pressedColorClose = QColor(232, 17, 35);
};

#endif // TITLEBAR_H