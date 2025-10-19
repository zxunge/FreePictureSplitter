// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge

#include "graphicsview.h"
#include "imagedocument.h"
#include "draggableline.h"

#include <QRect>

void Core::ImageDocument::drawLinesTo(GraphicsView *subject)
{
    if (0 == m_rects.size() || 0 == m_rects.constFirst().size())
        return;

    DraggableLine *line{};
    Q_FOREACH (auto m_rects, m_rects) {
        line = new DraggableLine(subject);
        line->setScenePos(m_rects.constFirst().bottom());
        line->show();
        subject->addDraggableLine(line);
        line = nullptr;
    }
    Q_FOREACH (auto rect, m_rects.constFirst()) {
        line = new DraggableLine(Qt::Vertical, subject);
        line->setScenePos(rect.right());
        line->show();
        subject->addDraggableLine(line);
        line = nullptr;
    }
}

void Core::ImageDocument::applyLinesFrom(GraphicsView *source)
{
    QList<int> vx, vy;

    Q_FOREACH (auto l, source->getDraggableLines())
        if (l->getOrientation() == Qt::Horizontal)
            vy.push_back(l->scenePos());
        else
            vx.push_back(l->scenePos());
    std::sort(vx.begin(), vx.end());
    std::sort(vy.begin(), vy.end());

    m_rects.resize(vy.size() + 1);
    for (auto &r : m_rects)
        r.resize(vx.size() + 1);

    // Axis X
    for (qsizetype i{}; i <= vy.size(); ++i)
        m_rects[i][0].setLeft(0);
    for (qsizetype i{}; i <= vy.size(); ++i)
        m_rects[i][vx.size()].setRight(source->scene()->width() - 1);
    for (qsizetype i{}; i <= vy.size(); ++i)
        for (qsizetype j{}; j != vx.size(); ++j) {
            m_rects[i][j].setRight(vx.at(j));
            m_rects[i][j + 1].setLeft(vx.at(j));
        }

    // Axis Y
    for (qsizetype j{}; j <= vx.size(); ++j)
        m_rects[0][j].setTop(0);
    for (qsizetype j{}; j <= vx.size(); ++j)
        m_rects[vy.size()][j].setBottom(source->scene()->height() - 1);
    for (qsizetype i{}; i != vy.size(); ++i)
        for (int j{}; j <= vx.size(); ++j) {
            m_rects[i][j].setBottom(vy.at(i));
            m_rects[i + 1][j].setTop(vy.at(i));
        }
}

void Core::ImageDocument::drawGridLines(QPixmap *pixmap, const QColor &color, int size)
{
    if (m_rects.empty() || pixmap->isNull())
        return;

    QPainter p(pixmap);
    QPen pen;
    pen.setColor(color);
    pen.setWidth(size);
    p.setPen(pen);

    for (qsizetype i{}; i != m_rects.size() - 1; ++i)
        p.drawLine(0, m_rects.at(i).constFirst().bottom(), pixmap->width() - 1,
                   m_rects.at(i).constFirst().bottom());
    for (qsizetype j{}; j != m_rects.constFirst().size() - 1; ++j)
        p.drawLine(m_rects.constFirst().at(j).right(), 0, m_rects.constFirst().at(j).right(),
                   pixmap->height() - 1);
}
