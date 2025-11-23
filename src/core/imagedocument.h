// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H

#include <QObject>
#include <QImageReader>
#include <QImageWriter>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QPair>
#include <QList>
#include <QSize>

#include <expected>
#include <optional>
#include <variant>
#include <tuple>

class GraphicsView;
class QPixmap;

namespace Core {

template <typename T = void>
using Result = std::expected<T, QString>;

class ImageOption
{
    Q_GADGET
public:
    struct SplitAverage
    {
        int rows;
        int cols;
    };
    using GridInfo = std::tuple<bool, QColor, int>;

    enum SplitSequence { LeftToRight = 0x1, RightToLeft = 0x2, UpToDown = 0x4, DownToUp = 0x8 };
    Q_ENUM(SplitSequence)
    Q_DECLARE_FLAGS(SplitSequences, SplitSequence)
    Q_FLAG(SplitSequences)

    ImageOption() : m_scalingFactor(1.0), m_rcContained(false) { setGridEnabled(false); }

    void setSequence(const SplitSequences seq) { m_seq = seq; }
    SplitSequences sequence() const { return m_seq; }

    void setSize(const QSize size) { m_info = size; }
    void setAverage(const int rows, const int cols) { m_info = SplitAverage(rows, cols); }

    // If there's no value, then the mode is 'average'.
    std::optional<QSize> size() const
    {
        return std::holds_alternative<QSize>(m_info) ? std::optional<QSize>(std::get<QSize>(m_info))
                                                     : std::nullopt;
    }
    std::optional<SplitAverage> average() const
    {
        return std::holds_alternative<SplitAverage>(m_info)
                ? std::optional<SplitAverage>(std::get<SplitAverage>(m_info))
                : std::nullopt;
    }
    void setGridEnabled(bool enabled, const QColor &color = Qt::green, int lineSize = 3)
    {
        m_grid = std::make_tuple(enabled, color, lineSize);
    }
    bool isGridEnabled() const { return std::get<bool>(m_grid); }
    GridInfo gridInfo() const { return m_grid; }

    QString savePrefix() const { return m_savePrefix; }
    void setSavePrefix(const QString &savePrefix) { m_savePrefix = savePrefix; }
    QString saveSuffix() const { return m_saveSuffix; }
    void setSaveSuffix(const QString &saveSuffix) { m_saveSuffix = saveSuffix; }

    double scalingFactor() const { return m_scalingFactor; }
    void setScalingFactor(double ScalingFactor) { m_scalingFactor = ScalingFactor; }

    bool rowColContained() const { return m_rcContained; }
    void setRowColContained(bool rcContained) { m_rcContained = rcContained; }

private:
    std::variant<SplitAverage, QSize> m_info;
    SplitSequences m_seq;
    GridInfo m_grid;
    QString m_savePrefix;
    QString m_saveSuffix;
    bool m_rcContained;
    double m_scalingFactor;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(ImageOption::SplitSequences)

/*!
 * \brief The ImageDocument class
 * \inherits QObject
 * \details A simple image document abstraction.
 */
class ImageDocument : public QObject
{
    Q_OBJECT

public:
    typedef QList<QList<QRect>> RectList;

public:
    explicit ImageDocument(QObject *parent = nullptr) : QObject(parent) { }
    explicit ImageDocument(const QString &fn, QObject *parent = nullptr) : QObject(parent)
    {
        openImageFile(fn);
    }
    ~ImageDocument() { }

    inline QString filePath() const { return m_fileInfo.absoluteFilePath(); }
    inline QString fullName() const { return m_fileInfo.fileName(); }
    inline QString baseName() const { return m_fileInfo.baseName(); }
    inline QString parentPath() const { return m_fileInfo.absoluteDir().path(); }
    inline QString format() const { return m_fileInfo.suffix(); }

    QSize size()
    {
        // We need to reset the file name before the calls to size(),
        // see https://bugreports.qt.io/browse/QTBUG-138530
        m_imgReader.setFileName(m_imgReader.fileName());
        return m_imgReader.size();
    }

    inline QImage toImage()
    {
        m_imgReader.setFileName(filePath());
        return m_imgReader.read();
    };

    bool openImageFile(const QString &fn)
    {
        m_imgReader.setFileName(fn);
        if (m_imgReader.canRead()) {
            m_imgReader.setAutoTransform(true);
            m_fileInfo.setFile(QDir::cleanPath(fn));
            Q_EMIT imageChanged(m_fileInfo.absoluteFilePath());
            return true;
        } else
            return false;
    }
    bool canRead() const { return m_imgReader.canRead(); }

    ImageOption &option() { return m_opt; }
    ImageOption constOption() const { return m_opt; }
    void setOutputDir(const QDir &dir) { m_saveDir = dir; }
    bool setOutputPath(const QString &path)
    {
        if (QFileInfo(path).isDir()) {
            m_saveDir.setPath(path);
            return true;
        }
        return false;
    }

    QImageWriter &writer() { return m_imgWriter; }
    qsizetype totalCount() const
    {
        if (!m_rects.isEmpty())
            return m_rects.size() * m_rects[0].size() + constOption().isGridEnabled();
        else
            return 0;
    }
    Result<> saveImages();

    void setupSplitLines();
    void drawLinesTo(GraphicsView *subject);
    void applyLinesFrom(GraphicsView *source);
    bool isValid() const { return canRead() && !m_rects.isEmpty(); }

    void close() { m_imgReader.setFileName(QString()); }

Q_SIGNALS:
    void imageChanged(const QString &fn);
    void imageSaved(const QString &path);

private:
    void drawGridLines(QPixmap *pixmap, const ImageOption::GridInfo &gridInfo);
    Result<QList<QPair<QString, QImage>>> split();

private:
    QImageReader m_imgReader;
    QImageWriter m_imgWriter;
    QFileInfo m_fileInfo;
    QDir m_saveDir;
    RectList m_rects;
    ImageOption m_opt;
};

} // namespace Core

#endif // IMAGEDOCUMENT_H
