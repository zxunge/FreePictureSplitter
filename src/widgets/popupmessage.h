// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef POPUPMESSAGE_H
#define POPUPMESSAGE_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimer>

class PopupMessage : public QWidget
{
    Q_OBJECT
public:
    explicit PopupMessage(QWidget *parent = nullptr);
    /*!
     * \brief PopupMessage For convenience
     * \param text
     * \param parent
     */
    explicit PopupMessage(const QString &text, int timeout = 3000, int offsetFromTop = 30,
                          QWidget *parent = nullptr);
    ~PopupMessage();

    // Set the content widget to be displayed (can only be called once, or each time to replace)
    void setContent(QWidget *content);

    // Show animation: timeout is auto-hide milliseconds, offsetFromTop is the distance from the
    // parent's top
    void showAnimated(int timeout = 3000, int offsetFromTop = 30);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onAnimationFinished();
    void onTimeout();

private:
    void startAnimation(bool hide = false);
    void hideAnimated();

    QPropertyAnimation *m_animation;
    QTimer *m_timer;
    int m_offsetFromTop;
    QPoint m_targetPos; // target position (global coordinates)
    QPoint m_startPos; // start position (global coordinates)
    bool m_isHiding;
};

#endif // POPUPMESSAGE_H
