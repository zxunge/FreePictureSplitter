// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef POPUPMESSAGE_H
#define POPUPMESSAGE_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QTimer>
#include <QColor>

class PopupMessage : public QWidget
{
    Q_OBJECT
public:
    enum class MsgType { Error, Info, Success };
    explicit PopupMessage(QWidget *parent = nullptr);
    /*!
     * \brief PopupMessage For convenience
     * \param text
     * \param parent
     */
    explicit PopupMessage(const QString &text, int timeout = 3000, int offsetFromTop = 30,
                          MsgType type = MsgType::Info, QWidget *parent = nullptr);
    ~PopupMessage();

    /*!
     * \brief setContent Set the content widget to be displayed (can only be called once, or each
     * time to replace)
     * \param content
     */
    void setContent(QWidget *content);

    /*!
     * \brief showAnimated Show animation: timeout is auto-hide milliseconds, offsetFromTop is the
     * distance from the parent's top
     * \param timeout
     * \param offsetFromTop
     */
    void showAnimated(int timeout = 3000, int offsetFromTop = 30);

    static void error(const QString &text, int timeout = 3000, int offsetFromTop = 30);
    static void info(const QString &text, int timeout = 3000, int offsetFromTop = 30);
    static void success(const QString &text, int timeout = 3000, int offsetFromTop = 30);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void animationFinished();
    void timeout();

private:
    void startAnimation(bool hide = false);
    void hideAnimated();

    QPropertyAnimation *m_animation;
    QTimer *m_timer;
    int m_offsetFromTop;
    QPoint m_targetPos; // target position (global coordinates)
    QPoint m_startPos; // start position (global coordinates)
    bool m_isHiding;

private:
    static const QColor ERROR_COLOR;
    static const QColor INFO_COLOR;
    static const QColor SUCCESS_COLOR;
    static const QColor ERROR_MSG_COLOR;
    static const QColor INFO_MSG_COLOR;
    static const QColor SUCCESS_MSG_COLOR;
};

#endif // POPUPMESSAGE_H
