// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#include <QObject>
#include <QImageReader>
#include <QImageWriter>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QPair>
#include <QList>

class GraphicsView;
class QPixmap;

namespace Core {

/*!
 * \brief The ImageDocument class
 * \inherits QObject
 * \details A simple image document abstraction.
 */
class ImageDocument : public QObject
{
    Q_OBJECT

public:
    enum ErrorType {
        ERR_NO_ERROR = 0,
        ERR_NO_RULE,
        ERR_WRITING_FILE
    };
    enum SplitMode { Size, Average };

    enum SplitSequence {
        LeftToRight = 0x0001,
        RightToLeft = 0x0002,
        UpToDown = 0x0004,
        DownToUp = 0x0008
    };
    Q_ENUM(ErrorType)
    Q_ENUM(SplitMode)
    Q_ENUM(SplitSequence)
    typedef QPair<ErrorType, QString> ErrorInfo; // Error type + extra information.
    typedef QList<QList<QRect>> RectList;

public:
    explicit ImageDocument(QObject *parent = nullptr) : QObject(parent) { }
    explicit ImageDocument(const QString &fn, QObject *parent = nullptr) : QObject(parent)
    {
        setImageFile(fn);
    }
    ~ImageDocument() { }

    inline QString filePath() const { return m_filePath; }
    inline QString fullName() const { return QFileInfo(m_filePath).fileName(); }
    inline QString baseName() const { return QFileInfo(m_filePath).baseName(); }
    inline QString format() const { return QFileInfo(m_filePath).suffix(); }

    inline QImage toImage()
    {
        m_imgReader.setFileName(m_filePath);
        return m_imgReader.read();
    };
    operator QImage()
    {
        m_imgReader.setFileName(m_filePath);
        return m_imgReader.read();
    };

    bool setImageFile(const QString &fn)
    {
        m_imgReader.setFileName(fn);
        if (m_imgReader.canRead()) {
            m_filePath = QDir::cleanPath(fn);
            Q_EMIT imageChanged(m_filePath);
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
    QString m_filePath;
    QString m_savePrefix;
    QString m_saveSuffix;
    QDir m_saveDir;
    RectList m_rects;
    SplitMode m_mode;
    int m_seq;

    union {
        struct
        {
            int m_splitRows;
            int m_splitCols;
        };
        struct
        {
            int m_splitWidth;
            int m_splitHeight;
        };
    } m_splitInfo;
};

inline const QString ERROR_STRINGS[] {
    [ImageDocument::ERR_NO_RULE] = QObject::tr("No rule to split this picture"),
    [ImageDocument::ERR_WRITING_FILE] = QObject::tr("Error writing to file")
};

} // namespace Core
