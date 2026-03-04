// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "popupmessage.h"

#include "utils/misc.h"

#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QDialog>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QFrame>
#include <QLabel>

using namespace Qt::Literals::StringLiterals;

const QColor PopupMessage::ERROR_COLOR = QColor(0xfef0f0);
const QColor PopupMessage::INFO_COLOR = QColor(0xEFF6FF);
const QColor PopupMessage::SUCCESS_COLOR = QColor(0xEDF7EF);
const QColor PopupMessage::ERROR_MSG_COLOR = QColor(0xf56c6c);
const QColor PopupMessage::INFO_MSG_COLOR = QColor(0x1A4C8C);
const QColor PopupMessage::SUCCESS_MSG_COLOR = QColor(0x1F7840);

PopupMessage::PopupMessage(QWidget *parent)
    : QWidget(parent, Qt::Tool | Qt::FramelessWindowHint),
      m_animation(new QPropertyAnimation(this, "geometry"_ba)),
      m_timer(new QTimer(this)),
      m_isHiding(false)
{
    // Allow transparent background (for rounded corners and translucency)
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose); // Automatically delete when closed

    // Default style: black semi-transparent, rounded corners, white text
    setStyleSheet(u"PopupMessage {"
                  "   background-color: rgba(0, 0, 0, 180);"
                  "   border-radius: 10px;"
                  "   color: white;"
                  "}"_s);

    // Create layout and set margins so that content does not stick to edges
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    setLayout(layout);

    // Animation configuration
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
    m_animation->setDuration(300);

    // Timer (single shot)
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &PopupMessage::timeout);
    connect(m_animation, &QPropertyAnimation::finished, this, &PopupMessage::animationFinished);
}

PopupMessage::PopupMessage(const QString &text, int timeout, int offsetFromTop, MsgType type,
                           QWidget *parent)
    : PopupMessage(parent)
{
    QFrame *content = new QFrame;
    QVBoxLayout *contentLayout = new QVBoxLayout(content);
    QLabel *textLabel = new QLabel(text);
    textLabel->setStyleSheet(u"font-size: 14px;"_s);
    contentLayout->addWidget(textLabel, Qt::AlignCenter);
    content->setAutoFillBackground(true);
    switch (type) {
    case PopupMessage::MsgType::Error:
        content->setStyleSheet(QString("background-color: %1; color: %2; border-radius: 10px;")
                                       .arg(ERROR_COLOR.name(), ERROR_MSG_COLOR.name()));
        break;

    case PopupMessage::MsgType::Info:
        content->setStyleSheet(QString("background-color: %1; color: %2; border-radius: 10px;")
                                       .arg(INFO_COLOR.name(), INFO_MSG_COLOR.name()));
        break;

    case PopupMessage::MsgType::Success:
        content->setStyleSheet(QString("background-color: %1; color: %2; border-radius: 10px;")
                                       .arg(SUCCESS_COLOR.name(), SUCCESS_MSG_COLOR.name()));
        break;
    }

    setContent(content);
    showAnimated(timeout, offsetFromTop);
}

PopupMessage::~PopupMessage() { }

void PopupMessage::setContent(QWidget *content)
{
    // Clear any existing content (do not delete the widget itself)
    QLayoutItem *child;
    while ((child = layout()->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->setParent(nullptr);
        }
        delete child;
    }
    // Add the new content
    layout()->addWidget(content);

    content->installEventFilter(this);
    for (QWidget *child : content->findChildren<QWidget *>()) {
        child->installEventFilter(this);
    }
}

void PopupMessage::showAnimated(int timeout, int offsetFromTop)
{
    m_offsetFromTop = offsetFromTop;
    m_isHiding = false;

    // Adjust window size based on content
    adjustSize();

    // Calculate target position (relative to parent widget)
    QWidget *par = parentWidget();
    if (!par) {
        // When no parent, default to top center of the primary screen
        QRect screenRect = QApplication::primaryScreen()->availableGeometry();
        m_targetPos = QPoint(screenRect.center().x() - width() / 2, offsetFromTop);
    } else {
        // Relative to the parent's top, centered horizontally
        QPoint parTopLeft = par->mapToGlobal(QPoint(0, 0));
        int x = parTopLeft.x() + (par->width() - width()) / 2;
        int y = parTopLeft.y() + offsetFromTop;
        m_targetPos = QPoint(x, y);
    }

    // Start position: directly above the target position by one window height
    m_startPos = QPoint(m_targetPos.x(), m_targetPos.y() - height());

    // First move to the start position then show (avoid flickering)
    setGeometry(QRect(m_startPos, size()));
    show();

    // Start entry animation
    startAnimation();

    // Start auto-hide timer
    m_timer->start(timeout);
}

void PopupMessage::error(const QString &text, int timeout, int offsetFromTop)
{
    PopupMessage *msg = new PopupMessage(text, timeout, offsetFromTop, PopupMessage::MsgType::Error,
                                         Util::getMainWindow());
}

void PopupMessage::info(const QString &text, int timeout, int offsetFromTop)
{
    PopupMessage *msg = new PopupMessage(text, timeout, offsetFromTop, PopupMessage::MsgType::Info,
                                         Util::getMainWindow());
}

void PopupMessage::success(const QString &text, int timeout, int offsetFromTop)
{
    PopupMessage *msg = new PopupMessage(text, timeout, offsetFromTop,
                                         PopupMessage::MsgType::Success, Util::getMainWindow());
}

void PopupMessage::mousePressEvent(QMouseEvent *event)
{
    // Stop timer and animation
    m_timer->stop();
    m_animation->stop();

    // Retrieve the content widget (assuming there is only one item in the layout)
    QLayout *layout = this->layout();
    if (layout->count() > 0) {
        QLayoutItem *item = layout->takeAt(0);
        QWidget *contentWidget = item->widget();
        if (contentWidget) {
            // Create a dialog and transfer the content widget into it
            QDialog *dialog = new QDialog(parentWidget());
            dialog->setWindowTitle(tr("Details"));
            dialog->setModal(true);

            QVBoxLayout *dlgLayout = new QVBoxLayout(dialog);
            dlgLayout->addWidget(contentWidget);
            dialog->setLayout(dlgLayout);
            dialog->adjustSize();

            // Show the modal dialog (blocks, but deleteLater will delay destruction safely)
            dialog->exec();
        }
        delete item; // only delete the layout item, not the widget
    }

    // Close and destroy itself
    close(); // triggers hideEvent
    deleteLater();

    QWidget::mousePressEvent(event);
}

void PopupMessage::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    // Additional initialization can be placed here
}

void PopupMessage::hideEvent(QHideEvent *event)
{
    // Stop any running animations and timers
    m_timer->stop();
    m_animation->stop();
    QWidget::hideEvent(event);
}

bool PopupMessage::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        mousePressEvent(static_cast<QMouseEvent *>(event));
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void PopupMessage::animationFinished()
{
    if (m_isHiding) {
        // Hiding animation finished, actually hide and destroy
        hide();
        deleteLater();
    }
}

void PopupMessage::timeout()
{
    hideAnimated();
}

void PopupMessage::startAnimation(bool hide)
{
    m_animation->stop();
    if (hide) {
        // Reverse animation: slide from current position back to start position
        m_animation->setStartValue(geometry());
        m_animation->setEndValue(QRect(m_startPos, size()));
    } else {
        // Forward animation: slide from start position to target position
        m_animation->setStartValue(QRect(m_startPos, size()));
        m_animation->setEndValue(QRect(m_targetPos, size()));
    }
    m_animation->start();
}

void PopupMessage::hideAnimated()
{
    m_isHiding = true;
    m_timer->stop(); // ensure timer no longer fires
    startAnimation(true); // start hide animation
    // After animation finishes, onAnimationFinished will be called and destroy the window
}
