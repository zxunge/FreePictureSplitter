// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include "draggableline.h"

#include <QGraphicsView>
#include <QImage>
#include <QtCore/qvector.h>
#include <QtCore/qpointer.h>

class RulerBar;
class CornerBox;
class QMouseEvent;
class QKeyEvent;

/*!
 * \brief ZOOM_RATIO Ratio used when zooming in/out.
 */
constexpr double ZOOM_RATIO = 1.2;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QWidget *parent = nullptr);
    ~GraphicsView();

    void zoomIn();
    void zoomOut();

    /*!
     * \brief showPixmap Show an image on the GraphicsView.
     * \param pixmap
     * \param adaptive
     */
    void showPixmap(const QPixmap &pixmap, bool adaptive = true);
    void clearScene();

    /*!
     * \brief addDraggableLine Create a line using direction and initial position. pos -> whole
     * GraphicsView
     * \param ori
     * \param pos
     */
    void addDraggableLine(Qt::Orientation ori, const QPoint &pos);
    /*!
     * \brief addDraggableLine Add an existing line to list.
     * \param fl
     */
    void addDraggableLine(DraggableLine *fl);
    void removeAllDraggableLines();
    const QVector<QPointer<DraggableLine>> &getDraggableLines() const { return m_plines; }

    void setRulersVisibility(bool visible)
    {
        m_hruler->setVisible(visible);
        m_vruler->setVisible(visible);
        m_box->setVisible(visible);
    }

public Q_SLOTS:
    void lineDestruction();

Q_SIGNALS:
    void positionChanged(int x, int y);
    void lineExists(bool existence);
    void lineAddedByDragging();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void scrollContentsBy(int dx, int dy) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void updateRuler();

private Q_SLOTS:
    void dragStarted(const QPoint &startPos);
    void dragMoved(const QPoint &currentPos);
    void dragFinished(const QPoint &endPos, bool isReal);

private:
    RulerBar *m_hruler;
    RulerBar *m_vruler;
    CornerBox *m_box;
    QVector<QPointer<DraggableLine>> m_plines;

    DraggableLine *m_tempLine = nullptr; // Temporary line widget
    QPoint m_dragStartPos; // Position where dragging starts
    bool m_isCtrlPressed; // True if Key Ctrl is pressed
};

#endif // GRAPHICSVIEW_H