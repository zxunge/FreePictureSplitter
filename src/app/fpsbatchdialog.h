/*
 * This file is a part of FreePictureSplitter, a fast and easy-to-use picture splitter.
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

#ifndef FPSBATCHDIALOG_H
#define FPSBATCHDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QtCore/qstringlist.h>

namespace Ui {
class fpsBatchDialog;
}
class QTableWidgetItem;
class QListWidgetItem;
class QProgressBar;

class fpsBatchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit fpsBatchDialog(QWidget *parent = nullptr);
    ~fpsBatchDialog();

private slots:
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

    void on_wgtTable_customContextMenuRequested(const QPoint &pos);

    void on_wgtTable_itemClicked(QTableWidgetItem *item);

    void on_wgtTable_itemSelectionChanged();

    void on_wgtList_customContextMenuRequested(const QPoint &pos);

    void on_wgtList_itemClicked(QListWidgetItem *item);

    void on_wgtList_itemSelectionChanged();

    void on_actionRemoveFromList_triggered();

signals:
    void splitProceed(int elapsed);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void addPicture(const QString &fileName);

private:
    Ui::fpsBatchDialog *ui;

    QStringList m_filesList;
    QMenu *m_contextMenu;
    QProgressBar *m_pbLoading;
};

#endif // FPSBATCHDIALOG_H
