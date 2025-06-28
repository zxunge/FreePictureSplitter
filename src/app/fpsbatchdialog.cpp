// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsbatchdialog.h"
#include "ui_fpsbatchdialog.h"

#include <QButtonGroup>
#include <QActionGroup>
#include <QFileDialog>
#include <QImageReader>
#include <QtCore/qfileinfo.h>

fpsBatchDialog::fpsBatchDialog(QWidget *parent) : QDialog(parent), ui(new Ui::fpsBatchDialog)
{
    ui->setupUi(this);
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->wgtList->setViewMode(QListView::IconMode);
    ui->wgtList->setSpacing(10);
    ui->wgtList->setResizeMode(QListView::Adjust);
    ui->wgtList->setMovement(QListWidget::Static);
    ui->wgtTable->setColumnCount(3);
    ui->wgtTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->wgtTable->setHorizontalHeaderLabels(QStringList{ "File Name", "File Path", "File Size" });
    ui->wgtTable->verticalHeader()->setVisible(false);

    QActionGroup *ag{ new QActionGroup(this) };
    ag->addAction(ui->actionShowDetailInfo);
    ag->addAction(ui->actionShowThumbnails);

    QButtonGroup *bg{ new QButtonGroup(this) };
    bg->addButton(ui->rbtnAverage);
    bg->addButton(ui->rbtnSize);
    bg->addButton(ui->rbtnTemplate);
}

fpsBatchDialog::~fpsBatchDialog()
{
    delete ui;
}

void fpsBatchDialog::on_actionShowThumbnails_toggled(bool checked)
{
    if (checked)
        ui->stView->setCurrentIndex(0); // pgThumbnail
}

void fpsBatchDialog::on_actionShowDetailInfo_toggled(bool checked)
{
    if (checked)
        ui->stView->setCurrentIndex(1); // pgTable
}

void fpsBatchDialog::on_actionAddPicture_triggered()
{
    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes{ QImageReader::supportedMimeTypes() };
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes) {
        mimeTypeFilters.append(mimeTypeName);
    }
    mimeTypeFilters.sort();

    QFileDialog fdlg;
    fdlg.setWindowTitle(tr("Open a picture..."));
    fdlg.setDirectory("/");
    fdlg.setMimeTypeFilters(mimeTypeFilters);
    fdlg.setFileMode(QFileDialog::ExistingFile);
    if (QDialog::Accepted == fdlg.exec() && !fdlg.selectedFiles()[0].isEmpty()) {
        QListWidgetItem *listItem{ new QListWidgetItem(ui->wgtList) };
        listItem->setIcon(QIcon(fdlg.selectedFiles()[0]));
        listItem->setText(fdlg.selectedFiles()[0]);
        listItem->setSizeHint(QSize(80, 100));

        int rowCount{ ui->wgtTable->rowCount() };
        ui->wgtTable->insertRow(rowCount);
        QTableWidgetItem *tableItemName{ new QTableWidgetItem(
                QIcon(fdlg.selectedFiles()[0]), QFileInfo(fdlg.selectedFiles()[0]).fileName()) };
        ui->wgtTable->setItem(rowCount, 0, tableItemName);
        QTableWidgetItem *tableItemPath{ new QTableWidgetItem(fdlg.selectedFiles()[0]) };
        ui->wgtTable->setItem(rowCount, 1, tableItemPath);
        QTableWidgetItem *tableItemSize{ new QTableWidgetItem(
                QString::number(static_cast<long>(QFileInfo(fdlg.selectedFiles()[0]).size() / 1024))
                + " KB") };
        ui->wgtTable->setItem(rowCount, 2, tableItemSize);
    } else
        return;
}
