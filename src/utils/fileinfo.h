// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef FILEINFO_H
#define FILEINFO_H

#include <QString>
#include <QStringList>
#include <QMimeDatabase>
#include <QMimeType>
#include <QFileInfo>
#include <QFileDialog>

using namespace Qt::Literals::StringLiterals;

namespace Util {

inline namespace FileInfo {

inline QString fileSizeString(const QString &fn)
{
    constexpr qint64 kilobyte{ 1024 };
    constexpr qint64 megabyte{ 1048576 };
    constexpr qint64 gigabyte{ 1073741824 };
    const qint64 size{ QFileInfo(fn).size() };

    if (size >= gigabyte)
        return QString::number(static_cast<double>(size) / gigabyte, 'f', 2) % u" GB"_s;
    else if (size >= megabyte)
        return QString::number(static_cast<double>(size) / megabyte, 'f', 2) % u" MB"_s;
    else if (size >= kilobyte)
        return QString::number(static_cast<double>(size) / kilobyte, 'f', 2) % u" KB"_s;
    else
        return QString::number(size) % u" Bytes"_s;
}

inline QStringList mimeTypesToNameFilters(const QByteArrayList &types)
{
    QStringList filters;
    QMimeDatabase db;
    QString all{ QObject::tr("All supported file types (") };

    Q_FOREACH (const QByteArray &mimeType, types) {
        QMimeType mime(db.mimeTypeForName(mimeType));
        if (mime.isValid()) {
            if (mime.isDefault()) {
                filters.clear();
                filters.append(QFileDialog::tr("All files (*)"));
                return filters;
            } else {
                const QString patterns{ mime.globPatterns().join(u' ') };
                filters.append(mime.comment() + " ("_L1 + patterns + u')');
                all += patterns + u' ';
            }
        }
    }
    all += u')';
    filters.push_front(all);
    return filters;
}

} // namespace FileInfo

} // namespace Util

#endif // FILEINFO_H
