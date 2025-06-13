// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef FPSGRAPHICSVIEW_H
#define FPSGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QImage>

class fpsRulerBar;
class fpsCornerBox;
class QMouseEvent;

class fpsGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    fpsGraphicsView(QWidget *parent);

    void zoomIn();
    void zoomOut();

    void setImage(QImage img);
    void addFloatingLine();

signals:
    void positionChanged(int x, int y);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void scrollContentsBy(int dx, int dy) override;
    void updateRuler();

protected:
    fpsRulerBar  *m_hruler;
    fpsRulerBar  *m_vruler;
    fpsCornerBox *m_box;
};

#endif      // FPSGRAPHICSVIEW_H
