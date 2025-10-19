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
#include <variant>

class GraphicsView;
class QPixmap;

namespace Core {

template <typename T = void>
using Result = std::expected<T, QString>;

struct ImageOption
{
public:
    typedef struct
    {
        int rows;
        int cols;
    } SplitAverage;

    enum SplitSequence {
        LeftToRight = 0x0001,
        RightToLeft = 0x0002,
        UpToDown = 0x0004,
        DownToUp = 0x0008
    };
    ImageOption() { }

    void setSequence(const int seq) { m_seq = static_cast<SplitSequence>(seq); }
    int sequence() const { return static_cast<int>(m_seq); }

    void setSize(const QSize size) { m_info = size; }
    void setAverage(const int rows, const int cols) { m_info = SplitAverage(rows, cols); }

    // If there's no value, then the mode is 'average'.
    Result<QSize> size() const
    {
        return (std::get_if<QSize>(&m_info)) ? Result<QSize>(std::get<QSize>(m_info))
                                             : std::unexpected(QString());
    }
    Result<SplitAverage> average() const
    {
        return (std::get_if<SplitAverage>(&m_info))
                ? Result<SplitAverage>(std::get<SplitAverage>(m_info))
                : std::unexpected(QString());
    }

private:
    std::variant<SplitAverage, QSize> m_info;
    SplitSequence m_seq;
};

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
        setImageFile(fn);
    }
    ~ImageDocument() { }

    inline QString filePath() const { return m_fileInfo.absoluteFilePath(); }
    inline QString fullName() const { return m_fileInfo.fileName(); }
    inline QString baseName() const { return m_fileInfo.baseName(); }
    inline QString format() const { return m_fileInfo.suffix(); }

    inline QImage toImage()
    {
        m_imgReader.setFileName(filePath());
        return m_imgReader.read();
    };

    bool setImageFile(const QString &fn)
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

    void setOption(const ImageOption &option) { m_opt = option; }
    void setSequence(int seq) { m_opt.setSequence(seq); }
    void setOutputDir(const QDir &dir) { m_saveDir = dir; }
    bool setOutputPath(const QString &path)
    {
        if (QFileInfo(path).isDir()) {
            m_saveDir.setPath(path);
            return true;
        }
        return false;
    }

    void drawLinesTo(GraphicsView *subject);
    void applyLinesFrom(GraphicsView *source);

Q_SIGNALS:
    void imageChanged(const QString &fn);

private:
    void drawGridLines(QPixmap *pixmap, const QColor &color, int size);

private:
    QImageReader m_imgReader;
    QImageWriter m_imgWriter;
    QFileInfo m_fileInfo;
    QString m_savePrefix;
    QString m_saveSuffix;
    QDir m_saveDir;
    RectList m_rects;
    ImageOption m_opt;
};

} // namespace Core

#endif // IMAGEDOCUMENT_H
