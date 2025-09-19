/*
 * This file is part of FreePictureSplitter, a fast and easy-to-use picture splitter.
 * Copyright (c) 2024 2025 zxunge
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
// SPDX-License-Identifier: GPL-3.0-or-later
// clang-format Language: Cpp

#ifndef BATCHWIDGET_H
#define BATCHWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QItemSelection>
#include <QtCore/qstringlist.h>

namespace Ui {
class BatchWidget;
}
class QStandardItemModel;
class QItemSelectionModel;

class BatchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BatchWidget(QWidget *parent = nullptr);
    ~BatchWidget();

private:
    void addPicture(const QString &fileName);
    void removeSelectedItems();
    void openPictures();
    void openFolder();
    void changePath();
    void startSplit();

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void message(const QString &msg, int timeout = 0);

private:
    Ui::BatchWidget *ui;

    QMenu *m_contextMenu;
    QStandardItemModel *m_model;
    QItemSelectionModel *m_selModel;
    // QProgressBar *m_pbLoading;
};

#endif // BATCHWIDGET_H
