// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "framelesswidget.h"
#include "borderwidget.h"
#include "../../src/utils/thememanager.h"

#include <oclero/qlementine/style/Theme.hpp>

using namespace Qt::Literals::StringLiterals;

FramelessWidget::FramelessWidget(QWidget *parent) : QWidget(parent) { }

FramelessWidget::~FramelessWidget() { }

void FramelessWidget::setBackgroundColor(const QColor &color)
{
    m_border->setBackgroundColor(color);
    if (isMaximized() || isFullScreen())
        updateRadius(0);
    else
        updateRadius(m_radius);
}

QColor FramelessWidget::backgroundColor() const
{
    return m_border->backgroundColor();
}

void FramelessWidget::setRadius(const uint &r)
{
    m_radius = r;
    updateRadius(m_radius);
    updateSize();
}

void FramelessWidget::setShadowColor(const QColor &color)
{
    m_shadowColor = color;
    update();
}

void FramelessWidget::setBlurRadius(const uint &r)
{
    m_blurRadius = r;
    if (isMaximized() || isFullScreen()) {
        m_gridLayout->setContentsMargins(0, 0, 0, 0);
        return;
    } else {
        // Reserve area for shadow drawing
        m_gridLayout->setContentsMargins(m_blurRadius, m_blurRadius, m_blurRadius, m_blurRadius);
    }
}

void FramelessWidget::initialize()
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags()); // Hide default frame
    setAttribute(Qt::WA_TranslucentBackground, true); // Set transparent background
    setMouseTracking(true); // Enable mouse tracking

    m_border = new BorderWidget;
    m_border->setCursor(Qt::ArrowCursor);
    m_titleBar = new TitleBar(m_border);
    QVBoxLayout *centerLayout = new QVBoxLayout;
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(0);
    centerLayout->setAlignment(Qt::AlignTop);
    centerLayout->addWidget(m_titleBar);

    if (m_centralWidget) {
        m_titleBar->setTitleText(m_centralWidget->windowTitle());
        m_titleBar->setTitleIcon(m_centralWidget->windowIcon().pixmap(64, 64));
        centerLayout->addWidget(m_centralWidget);
        centerLayout->setStretch(1, 1);
        m_border->setCursor(m_centralWidget->cursor());
    }
    m_border->setLayout(centerLayout);
    m_border->setObjectName("border");
    m_gridLayout = new QGridLayout;
    m_gridLayout->setContentsMargins(m_blurRadius, m_blurRadius, m_blurRadius,
                                     m_blurRadius); // Reserve space for border shadow
    m_gridLayout->addWidget(m_border);
    setLayout(m_gridLayout);

    m_titleBar->setBackgroundColor(Qt::white);
    setRadius(m_radius);

    connect(m_titleBar, &TitleBar::closed, this, &FramelessWidget::close);
    connect(m_titleBar, &TitleBar::maximizedStateChanged, this, [this](bool isMax) {
        Q_UNUSED(isMax)
        if (isMaximized() || isFullScreen())
            updateRadius(0);
        else
            updateRadius(m_radius);
    });
    connect(&Util::ThemeManager::instance(), &Util::ThemeManager::themeChanged, this,
            &FramelessWidget::themeChanged);
}

void FramelessWidget::calculateOpflag(QPoint pos)
{
    QCursor cursor = Qt::ArrowCursor;
    m_opFlag = NONE;
    if (pos.x() < m_blurRadius) {
        if (pos.y() < m_blurRadius + m_radius) {
            m_opFlag = OpFlag::TL;
            cursor = Qt::SizeFDiagCursor;
        } else if (pos.y() > height() - m_blurRadius - m_radius) {
            m_opFlag = OpFlag::BL;
            cursor = Qt::SizeBDiagCursor;
        } else {
            m_opFlag = OpFlag::Left;
            cursor = Qt::SizeHorCursor;
        }
    } else if (pos.x() > width() - m_blurRadius) {
        if (pos.y() < m_blurRadius + m_radius) {
            m_opFlag = OpFlag::TR;
            cursor = Qt::SizeBDiagCursor;
        } else if (pos.y() > height() - m_blurRadius - m_radius) {
            m_opFlag = OpFlag::BR;
            cursor = Qt::SizeFDiagCursor;
        } else {
            m_opFlag = OpFlag::Right;
            cursor = Qt::SizeHorCursor;
        }
    } else {
        if (pos.y() < m_blurRadius) {
            m_opFlag = OpFlag::Top;
            cursor = Qt::SizeVerCursor;
        } else if (pos.y() > height() - m_blurRadius) {
            m_opFlag = OpFlag::Bottom;
            cursor = Qt::SizeVerCursor;
        } else {
            m_opFlag = OpFlag::NONE;
            cursor = Qt::ArrowCursor;
        }
    }
    setCursor(cursor);
}

void FramelessWidget::updateRadius(const uint &r)
{
    m_border->setBorderRadius(r);
    m_titleBar->setRadius(r);
    update();
}

void FramelessWidget::updateSize()
{
    if (m_centralWidget == nullptr)
        return;

    int w = m_centralWidget->maximumWidth() + m_blurRadius;
    int h = m_centralWidget->maximumHeight() + m_blurRadius;

    if (!m_titleBar->isHidden())
        h += m_titleBar->height();

    setMaximumSize(qMin(w, QWIDGETSIZE_MAX), qMin(h, QWIDGETSIZE_MAX));

    w = m_centralWidget->width() + m_blurRadius;
    h = m_centralWidget->height() + m_blurRadius;

    if (!m_titleBar->isHidden())
        h += m_titleBar->height();
    resize(w, h);
}

void FramelessWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if (m_border->geometry().contains(pos)) {
        event->ignore();
        return;
    }
    if (m_opFlag != -1) {
        m_isOp = true;
        m_lastPos = event->globalPosition();
    }
}

void FramelessWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if (!m_isOp)
        calculateOpflag(pos);

    // Get the current mouse drag position
    QPointF currPos = event->globalPosition();
    // Calculate the displacement of the mouse drag
    QPointF ptemp = currPos - m_lastPos;
    // Save the current mouse drag position for the next displacement calculation
    m_lastPos = currPos;
    event->ignore();
    if (m_isOp) {
        if (isMaximized())
            return;
        int x = 0, y = 0, w = 0, h = 0;
        // Determine whether to stretch or shrink the window based on which border is dragged
        switch (m_opFlag) {
        // Left border stretched
        case Left:
            x = this->pos().x() + ptemp.x();
            y = this->pos().y();
            w = size().width() - ptemp.x();
            h = size().height();
            if (w < minimumWidth())
                m_lastPos.setX(this->pos().x());
            break;
            // Right border stretched
        case Right:
            x = this->pos().x();
            y = this->pos().y();
            w = size().width() + ptemp.x();
            h = size().height();
            if (w < minimumWidth())
                m_lastPos.setX(this->pos().x() + width());
            break;
            // Top border stretched
        case Top:
            x = this->pos().x();
            y = this->pos().y() + ptemp.y();
            w = size().width();
            h = size().height() - ptemp.y();
            if (h < minimumHeight())
                m_lastPos.setY(this->pos().y());
            break;
            // Bottom border stretched
        case Bottom:
            x = this->pos().x();
            y = this->pos().y();
            w = size().width();
            h = size().height() + ptemp.y();
            if (h < minimumHeight())
                m_lastPos.setY(this->pos().y() + height());
            break;
            // Top-right corner stretched
        case TR:
            x = this->pos().x();
            y = this->pos().y() + ptemp.y();
            w = size().width() + ptemp.x();
            h = size().height() - ptemp.y();
            if (w < minimumWidth())
                m_lastPos.setX(this->pos().x() + width());
            if (h < minimumHeight())
                m_lastPos.setY(this->pos().y());
            break;
            // Top-left corner stretched
        case TL:
            x = this->pos().x() + ptemp.x();
            y = this->pos().y() + ptemp.y();
            w = size().width() - ptemp.x();
            h = size().height() - ptemp.y();
            if (w < minimumWidth())
                m_lastPos.setX(this->pos().x());
            if (h < minimumHeight())
                m_lastPos.setY(this->pos().y());
            break;
            // Bottom-right corner stretched
        case BR:
            x = this->pos().x();
            y = this->pos().y();
            w = size().width() + ptemp.x();
            h = size().height() + ptemp.y();
            if (w < minimumWidth())
                m_lastPos.setX(this->pos().x() + width());
            if (h < minimumHeight())
                m_lastPos.setY(this->pos().y() + height());
            break;
            // Bottom-left corner stretched
        case BL:
            x = this->pos().x() + ptemp.x();
            y = this->pos().y();
            w = size().width() - ptemp.x();
            h = size().height() + ptemp.y();
            if (w < minimumWidth())
                m_lastPos.setX(this->pos().x());
            if (h < minimumHeight())
                m_lastPos.setY(this->pos().y() + height());
            break;
        default:
            return;
        }
        // Change the window size and position
        if (w < minimumWidth()) {
            x = this->pos().x();
            w = minimumWidth();
        }
        if (h < minimumHeight()) {
            y = this->pos().y();
            h = minimumHeight();
        }
        if (w > maximumWidth()) {
            x = this->pos().x();
            w = maximumWidth();
        }
        if (h > maximumHeight()) {
            y = this->pos().y();
            h = maximumHeight();
        }
        setGeometry(x, y, w, h);
    }
}

void FramelessWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    m_isOp = false;
}

void FramelessWidget::paintEvent(QPaintEvent *event)
{
    if (isMaximized() || isFullScreen()) {
        m_gridLayout->setContentsMargins(0, 0, 0, 0);
        return;
    } else {
        // Reserve area for shadow drawing
        m_gridLayout->setContentsMargins(m_blurRadius, m_blurRadius, m_blurRadius, m_blurRadius);
    }
    int radius = m_blurRadius + m_radius;
    int width = this->width();
    int height = this->height();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true); // Anti-aliasing
    painter.setPen(Qt::NoPen);

    QPainterPath rectPath;
    rectPath.addRect(rect());
    QRect borderRect(m_blurRadius, m_blurRadius, m_border->width(), m_border->height());
    // Create a rounded rectangle path
    QPainterPath clipPath;
    clipPath.addRoundedRect(borderRect, m_radius, m_radius);
    // Restrict the drawing area
    painter.setClipPath(rectPath - clipPath);

    // Linear gradient
    QLinearGradient linearGradient;
    linearGradient.setColorAt(0, m_shadowColor);
    linearGradient.setColorAt(m_radius * 1.0 / radius, m_shadowColor);
    QColor color = m_shadowColor;
    color.setAlpha(0);
    linearGradient.setColorAt(1, color);

    // Radial gradient
    QRadialGradient radialGradient;
    radialGradient.setColorAt(0, m_shadowColor);
    radialGradient.setColorAt(m_radius * 1.0 / radius, m_shadowColor);
    radialGradient.setColorAt(1, color);

    // Top-left corner
    radialGradient.setCenter(radius, radius); // Center point
    radialGradient.setRadius(radius); // Radius
    radialGradient.setFocalPoint(radius, radius); // Focal point
    painter.setBrush(radialGradient);
    QRectF rectf(0, 0, radius * 2, radius * 2);
    QPainterPath path;
    path.moveTo(radius, radius); // Move to circle center
    path.arcTo(rectf, 90, 90);
    painter.drawPath(path); // Draw path (sector)

    // Left side
    linearGradient.setStart(radius, height / 2);
    linearGradient.setFinalStop(0, height / 2);
    painter.setBrush(linearGradient);
    path.clear();
    path.addRect(0, radius, radius, height - radius * 2);
    painter.drawPath(path);

    // Bottom-left corner
    radialGradient.setCenter(radius, height - radius); // Center point
    radialGradient.setRadius(radius); // Radius
    radialGradient.setFocalPoint(radius, height - radius); // Focal point
    painter.setBrush(radialGradient);
    path.clear();
    path.moveTo(radius, height - radius); // Move to circle center
    rectf.setRect(0, height - radius * 2, radius * 2, radius * 2);
    path.arcTo(rectf, 180, 90);
    painter.drawPath(path); // Draw path (sector)

    // Bottom side
    linearGradient.setStart(width / 2, height - radius);
    linearGradient.setFinalStop(width / 2, height);
    painter.setBrush(linearGradient);
    path.clear();
    path.addRect(radius, height - radius, width - radius * 2, radius);
    painter.drawPath(path);

    // Bottom-right corner
    radialGradient.setCenter(width - radius, height - radius); // Center point
    radialGradient.setRadius(radius); // Radius
    radialGradient.setFocalPoint(width - radius, height - radius); // Focal point
    painter.setBrush(radialGradient);
    path.clear();
    path.moveTo(width - radius, height - radius); // Move to circle center
    rectf.setRect(width - radius * 2, height - radius * 2, radius * 2, radius * 2);
    path.arcTo(rectf, 270, 90);
    painter.drawPath(path); // Draw path (sector)

    // Right side
    linearGradient.setStart(width - radius, height / 2);
    linearGradient.setFinalStop(width, height / 2);
    painter.setBrush(linearGradient);
    path.clear();
    path.addRect(width - radius, radius, radius, height - radius * 2);
    painter.drawPath(path);

    // Top-right corner
    radialGradient.setCenter(width - radius, radius); // Center point
    radialGradient.setRadius(radius); // Radius
    radialGradient.setFocalPoint(width - radius, radius); // Focal point
    painter.setBrush(radialGradient);
    path.clear();
    path.moveTo(width - radius, radius); // Move to circle center
    rectf.setRect(width - radius * 2, 0, radius * 2, radius * 2);
    path.arcTo(rectf, 0, 90);
    painter.drawPath(path); // Draw path (sector)

    // Top side
    linearGradient.setStart(height / 2, radius);
    linearGradient.setFinalStop(height / 2, 0);
    painter.setBrush(linearGradient);
    path.clear();
    path.addRect(radius, 0, width - radius * 2, radius);
    painter.drawPath(path);
}

void FramelessWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
        QWindowStateChangeEvent *stateChangeEvent = static_cast<QWindowStateChangeEvent *>(event);
        Qt::WindowStates oldState = stateChangeEvent->oldState();
        Qt::WindowStates newState = windowState();
        if (newState == Qt::WindowMaximized || newState == Qt::WindowFullScreen) {
            // Window maximized
            updateRadius(0);
        } else if (newState == Qt::WindowMinimized) {
            // Window minimized
            updateRadius(m_radius);
        } else if (oldState == Qt::WindowMaximized || oldState == Qt::WindowMinimized
                   || oldState == Qt::WindowFullScreen) {
            // Window restored
            updateRadius(m_radius);
        }
    }
    QWidget::changeEvent(event);
}

void FramelessWidget::themeChanged(const oclero::qlementine::Theme *theme)
{
    setBackgroundColor(theme->backgroundColorMain2);
}

TitleBar *FramelessWidget::titleBar() const
{
    return m_titleBar;
}

void FramelessWidget::restoreGeometry(const QByteArray &geometry)
{
    QWidget::restoreGeometry(geometry);
    if (isMaximized())
        emit m_titleBar->maximizedStateChanged(true);
    else
        emit m_titleBar->maximizedStateChanged(false);
}

QWidget *FramelessWidget::centralWidget() const
{
    return m_centralWidget;
}

void FramelessWidget::setCentralWidget(QWidget *centralWidget)
{
    m_centralWidget = centralWidget;
    initialize();
}
