// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#include <QObject>
#include <QImageReader>
#include <QImageWriter>
#include <QString>
#include <QFileInfo>
#include <QDir>

namespace Util {

/*!
 * \brief The ImageDocument class
 * \inherits QObject
 * \details A simple image document abstraction.
 */
class ImageDocument : public QObject
{
    Q_OBJECT

public:
    explicit ImageDocument(QObject *parent = nullptr) : QObject(parent) { }
    explicit ImageDocument(const QString &fn, QObject *parent = nullptr) : QObject(parent)
    {
        setImageFile(fn);
    }
    ~ImageDocument();

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
            emit imageChanged(m_filePath);
            return true;
        } else
            return false;
    }
    bool canRead() const { return m_imgReader.canRead(); }

signals:
    void imageChanged(const QString &fn);

private:
    QImageReader m_imgReader;
    QImageWriter m_imgWriter;
    QString m_filePath;
    QString m_savePrefix;
    QString m_saveSuffix;

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

} // namespace Util