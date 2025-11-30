// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge

#include "graphicsview.h"
#include "imagedocument.h"
#include "draggableline.h"

#include <QRect>
#include <QtConcurrentMap>

using namespace Qt::Literals::StringLiterals;

namespace Core {

Result<QFuture<QList<Result<>>>> ImageDocument::saveImages()
{
    return split().and_then([this](auto imageList) -> Result<QFuture<QList<Result<>>>> {
        QFuture<QList<Result<>>> future{ QtConcurrent::mappedReduced(
                imageList,
                [this](const QPair<QString, QImage> &imageInfo) -> Result<> {
                    const auto &[filename, image]{ imageInfo };
                    QImageWriter writer(m_saveDir.filePath(filename));
                    m_wopt.applyToWriter(writer);
                    qInfo() << u"Saving Image: "_s << writer.fileName();
                    if (!writer.canWrite())
                        return std::unexpected(tr("Cannot write to ").arg(filename));
                    if (!writer.write(image.scaled(image.width() * option().scalingFactor(),
                                                   image.height() * option().scalingFactor(),
                                                   Qt::IgnoreAspectRatio,
                                                   Qt::SmoothTransformation)))
                        return std::unexpected(tr("Image saving failed: %1: %2.")
                                                       .arg(filename, writer.errorString()));
                    return {};
                },
                [](QList<Result<>> &results, const Result<> &result) { results.push_back(result); },
                QtConcurrent::OrderedReduce) };
        return future;
    });
}

void ImageDocument::setupSplitLines()
{
    m_rects.clear();
    if (!canRead())
        return;
    // TODO@25/05/10 Improve algorithm.
    // There're 2 steps we should follow basically:
    // First, write rows info to `rects'
    // Then, cols info.
    int width = size().width(), height = size().height();
    if (width * height == 0
        || (option().average() != std::nullopt
            && (option().average()->rows > height || option().average()->cols > width))
        || (option().size() != std::nullopt
            && (option().size()->height() > height || option().size()->width() > width)))
            [[unlikely]]
        return;

    int basicRowHeight, basicColWidth, legacyRowHeight, legacyColWidth;
    int rows, cols;
    // We need info about how many rows and columns when splitting by sizes ourself.
    if (option().size() != std::nullopt) {
        basicColWidth = option().size()->width();
        basicRowHeight = option().size()->height();
        rows = (height % option().size()->height() == 0) ? height / option().size()->height()
                                                         : height / option().size()->height() + 1;
        cols = (width % option().size()->width()) ? width / option().size()->width()
                                                  : width / option().size()->width() + 1;
    } else if (option().average() != std::nullopt) {
        basicRowHeight = std::round(static_cast<double>(height) / option().average()->rows);
        basicColWidth = std::round(static_cast<double>(width) / option().average()->cols);
        rows = option().average()->rows;
        cols = option().average()->cols;
    }
    legacyColWidth = width - basicColWidth * (cols - 1);
    legacyRowHeight = height - basicRowHeight * (rows - 1);

    // Initialize sizes
    m_rects.resize(rows);
    for (auto &i : m_rects)
        i.resize(cols);

    if (option().sequence().testAnyFlag(ImageOption::DownToUp)) {
        // ----- Rows: Down -> Up -----
        for (int i{ rows - 1 }; i != 0; --i)
            for (int j{}; j != cols; ++j) {
                m_rects[i][j].setTop(i * basicRowHeight);
                m_rects[i][j].setBottom((i + 1) * basicRowHeight);
            }
        // Legacy;
        for (int j{}; j != cols; ++j) {
            m_rects[0][j].setTop(0);
            m_rects[0][j].setHeight(legacyRowHeight);
        }
    } else if (option().sequence().testAnyFlag(ImageOption::UpToDown)) {
        // ----- Rows: Up -> Down -----
        for (int i{}; i != rows - 1; ++i)
            for (int j{}; j != cols; ++j) {
                m_rects[i][j].setTop(i * basicRowHeight);
                m_rects[i][j].setHeight(basicRowHeight);
            }
        // Legacy;
        for (int j{}; j != cols; ++j) {
            m_rects[rows - 1][j].setTop((rows - 1) * basicRowHeight);
            m_rects[rows - 1][j].setHeight(legacyRowHeight);
        }
    }
    if (option().sequence().testAnyFlag(ImageOption::LeftToRight)) {
        // ----- Columns: L -> R -----
        for (int i{}; i != rows; ++i)
            for (int j{}; j != cols - 1; ++j) {
                m_rects[i][j].setLeft(j * basicColWidth);
                m_rects[i][j].setWidth(basicColWidth);
            }
        // Legacy;
        for (int i{}; i != rows; ++i) {
            m_rects[i][cols - 1].setLeft((cols - 1) * basicColWidth);
            m_rects[i][cols - 1].setWidth(legacyColWidth);
        }
    } else if (option().sequence().testAnyFlag(ImageOption::RightToLeft)) {
        // ----- Columns: R -> L -----
        for (int i{}; i != rows; ++i)
            for (int j{ cols - 1 }; j != 0; --j) {
                m_rects[i][j].setLeft(width - (cols - j) * basicColWidth);
                m_rects[i][j].setWidth(basicColWidth);
            }
        // Legacy;
        for (int i{}; i != rows; ++i) {
            m_rects[i][0].setLeft(0);
            m_rects[i][0].setWidth(legacyColWidth);
        }
    }
}

void ImageDocument::drawLinesTo(GraphicsView *subject)
{
    if (0 == m_rects.size() || 0 == m_rects.constFirst().size())
        return;

    DraggableLine *line{};
    Q_FOREACH (auto rects, m_rects) {
        line = new DraggableLine(subject);
        line->setScenePos(rects.constFirst().bottom());
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

void ImageDocument::applyLinesFrom(GraphicsView *source)
{
    QList<int> vx, vy;
    m_rects.clear();

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

void ImageDocument::drawGridLines(QPixmap *pixmap, const ImageOption::GridInfo &gridInfo)
{
    if (m_rects.empty() || pixmap->isNull())
        return;

    QPainter p(pixmap);
    QPen pen;
    pen.setColor(std::get<QColor>(gridInfo));
    pen.setWidth(std::get<int>(gridInfo));
    p.setPen(pen);

    for (qsizetype i{}; i != m_rects.size() - 1; ++i)
        p.drawLine(0, m_rects.at(i).constFirst().bottom(), pixmap->width() - 1,
                   m_rects.at(i).constFirst().bottom());
    for (qsizetype j{}; j != m_rects.constFirst().size() - 1; ++j)
        p.drawLine(m_rects.constFirst().at(j).right(), 0, m_rects.constFirst().at(j).right(),
                   pixmap->height() - 1);
}

Result<QList<QPair<QString, QImage>>> ImageDocument::split()
{
    // Files include:
    // 1. Each splited picture
    // 2. (Optional) A grid figure

    // File naming conventions:
    // Prefix (the default is the original file's name) + '_'
    // + sequence number (%0*d) + '_'
    // (optional) + '_[row number]x[column number]'
    // + suffix (the default is none)
    // (optional) + '_grid'
    // + format
    QList<QPair<QString, QImage>> output;

    if (m_rects.isEmpty())
        return std::unexpected(tr("Null rectangle list"));

    auto cRows{ m_rects.size() }, cCols{ m_rects[0].size() };
    auto fieldWidth{ QString::asprintf("%lld", totalCount()).size() };
    // Avoid has been read
    m_imgReader.setFileName(m_imgReader.fileName());
    const QImage img{ m_imgReader.read() };
    QPixmap p{ QPixmap::fromImage(img) };

    if (img.isNull())
        return std::unexpected(tr("Null image"));

    for (qsizetype i{}; i != cRows; ++i)
        for (qsizetype j{}; j != cCols; ++j) {
            output.push_back(QPair<QString, QImage>(
                    // Output file's name
                    (option().savePrefix().isEmpty() ? baseName() : option().savePrefix())
                            % QString::asprintf("_%0*lld", static_cast<int>(fieldWidth),
                                                i * cCols + j + 1)
                            % (option().rowColContained() ? QString::asprintf("_%lldx%lld", i, j)
                                                          : u""_s)
                            % (option().saveSuffix().isEmpty() ? u""_s
                                                               : u"_"_s % option().saveSuffix())
                            % u"."_s % m_wopt.format(),
                    // Splited image
                    img.copy(m_rects[i][j])));
        }
    if (option().isGridEnabled())
        output.push_back(QPair<QString, QImage>(
                (option().savePrefix().isEmpty() ? baseName() : option().savePrefix())
                        % (option().saveSuffix().isEmpty() ? u""_s : u"_"_s % option().saveSuffix())
                        % u"_grid."_s % m_wopt.format(),
                (drawGridLines(&p, option().gridInfo()), p.toImage())));

    return output;
}

} // namespace Core
