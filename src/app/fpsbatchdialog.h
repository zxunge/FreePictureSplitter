// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
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
};

#endif // FPSBATCHDIALOG_H
