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

#ifndef FPSBATCHWIDGET_H
#define FPSBATCHWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QItemSelection>
#include <QtCore/qstringlist.h>

namespace Ui {
class fpsBatchWidget;
}
class QStandardItemModel;
class QItemSelectionModel;

class fpsBatchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit fpsBatchWidget(QWidget *parent = nullptr);
    ~fpsBatchWidget();

private slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    void on_actionShowThumbnails_toggled(bool checked);

    void on_actionShowDetailInfo_toggled(bool checked);

    void on_actionAddPicture_triggered();

    void on_actionAddDirectory_triggered();

    void on_cbxLocation_currentIndexChanged(int index);

    void on_rbtnAverage_toggled(bool checked);

    void on_rbtnSize_toggled(bool checked);

    void on_rbtnTemplate_toggled(bool checked);

    void on_btnChange_clicked();

    void on_btnOpen_clicked();

    void on_btnSplit_clicked();

    void on_actionRemoveFromList_triggered();

    void on_viewList_customContextMenuRequested(const QPoint &pos);

    void on_viewList_clicked(const QModelIndex &index);

    void on_viewTable_clicked(const QModelIndex &index);

    void on_viewTable_customContextMenuRequested(const QPoint &pos);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void addPicture(const QString &fileName);

signals:
    void message(const QString &msg, int timeout = 0);

private:
    Ui::fpsBatchWidget *ui;

    QMenu *m_contextMenu;
    QStandardItemModel *m_model;
    QItemSelectionModel *m_selModel;
    // QProgressBar *m_pbLoading;
};

#endif // FPSBATCHWIDGET_H
