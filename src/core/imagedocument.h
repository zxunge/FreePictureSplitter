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

#include <expected>

class GraphicsView;
class QPixmap;

namespace Core {

template<typename T = void>
using Result = std::expected<T, QString>;

/*!
 * \brief The ImageDocument class
 * \inherits QObject
 * \details A simple image document abstraction.
 */
class ImageDocument : public QObject
{
    Q_OBJECT

public:
    enum SplitMode { Size, Average };
    enum SplitSequence {
        LeftToRight = 0x0001,
        RightToLeft = 0x0002,
        UpToDown = 0x0004,
        DownToUp = 0x0008
    };
    Q_ENUM(SplitMode)
    Q_ENUM(SplitSequence)
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

    void setMode(const SplitMode &mode) { m_mode = mode; }
    void setSequence(int seq) { m_seq = seq; }
    void setOutputDir(const QDir &dir) { m_saveDir = dir; }
    bool setOutputPath(const QString &path) {
        if (QFileInfo(path).isDir()) {
            m_saveDir.setPath(path);
            return true;
        }
        return false;
    }

    void drawLines(GraphicsView *subject);
    void applyLines(GraphicsView *source);

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
    SplitMode m_mode;
    int m_seq;

    union {
        struct
        {
            int rows;
            int cols;
        };
        struct
        {
            int width;
            int height;
        };
    } m_splitInfo;
};

} // namespace Core

#endif // IMAGEDOCUMENT_H
