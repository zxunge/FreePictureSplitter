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

#include "fpsbatchdialog.h"
#include "ui_fpsbatchdialog.h"
#include "jsonconfigitems.h"
#include "fpsimagehandler.h"
#include "fpsprogressdialog.h"
#include "fpssplitworker.h"

#include <QButtonGroup>
#include <QActionGroup>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QFileInfo>
#include <QDesktopServices>
#include <QMessageBox>
#include <QVector>
#include <QImage>
#include <QTableWidgetItem>
#include <QListWidgetItem>
#include <QCompleter>
#include <QFileSystemModel>
#include <QStandardPaths>
#include <QThread>

#include <limits>

using namespace Qt::Literals::StringLiterals;

extern Util::Config appConfig;

fpsBatchDialog::fpsBatchDialog(QWidget *parent) : QDialog(parent), ui(new Ui::fpsBatchDialog)
{
    ui->setupUi(this);
    QCompleter *completer{ new QCompleter(this) };
    completer->setModel(new QFileSystemModel(completer));
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lePath->setCompleter(completer);

    ui->wgtTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->sbxCols->setMaximum(std::numeric_limits<int>::max());
    ui->sbxRows->setMaximum(std::numeric_limits<int>::max());
    ui->sbxWidth->setMaximum(std::numeric_limits<int>::max());
    ui->sbxHeight->setMaximum(std::numeric_limits<int>::max());

    QActionGroup *ag{ new QActionGroup(this) };
    ag->addAction(ui->actionShowDetailInfo);
    ag->addAction(ui->actionShowThumbnails);

    QButtonGroup *bg{ new QButtonGroup(this) };
    bg->addButton(ui->rbtnAverage);
    bg->addButton(ui->rbtnSize);
    bg->addButton(ui->rbtnTemplate);

    m_contextMenu = new QMenu(this);
    m_contextMenu->addAction(ui->actionAddDirectory);
    m_contextMenu->addAction(ui->actionAddPicture);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(ui->actionRemoveFromList);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(ui->actionShowDetailInfo);
    m_contextMenu->addAction(ui->actionShowThumbnails);

    // Load configurations
    ui->cbxLocation->setCurrentIndex(
            appConfig.options.batchOpt.savingTo == Util::SavingTo::specified ? 1 : 0);
    ui->btnOpen->setEnabled(appConfig.options.batchOpt.savingTo == Util::SavingTo::specified);
    ui->lePath->setEnabled(appConfig.options.batchOpt.savingTo == Util::SavingTo::specified);
    ui->lePath->setText(QString::fromStdString(appConfig.options.batchOpt.outPath));
    ui->btnChange->setEnabled(appConfig.options.batchOpt.savingTo == Util::SavingTo::specified);
    ui->chbSubdir->setChecked(appConfig.options.batchOpt.subDir);
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
    for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);

    mimeTypeFilters.sort();

    QFileDialog fdlg;
    fdlg.setWindowTitle(tr("Add pictures..."));
    fdlg.setDirectory(appConfig.dialog.lastOpenedDir.empty()
                              ? QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                              : QString::fromStdString(appConfig.dialog.lastOpenedDir));
    fdlg.setMimeTypeFilters(mimeTypeFilters);
    fdlg.setFileMode(QFileDialog::ExistingFiles);
    if (QDialog::Accepted == fdlg.exec() && !fdlg.selectedFiles()[0].isEmpty()) {
        appConfig.dialog.lastOpenedDir = QFileInfo(fdlg.selectedFiles()[0]).path().toStdString();

        for (const auto file : fdlg.selectedFiles())
            addPicture(file);
    } else
        return;
}

void fpsBatchDialog::closeEvent(QCloseEvent *event)
{
    // Save configurations
    if (ui->cbxLocation->currentIndex() == 0) // "The same"
        appConfig.options.batchOpt.savingTo = Util::SavingTo::same;
    else
        appConfig.options.batchOpt.savingTo = Util::SavingTo::specified;
    appConfig.options.batchOpt.outPath = ui->lePath->text().toStdString();
    appConfig.options.batchOpt.subDir = ui->chbSubdir->isChecked();

    QDialog::closeEvent(event);
}

void fpsBatchDialog::addPicture(const QString &fileName)
{
    QListWidgetItem *listItem{ new QListWidgetItem(ui->wgtList) };
    listItem->setIcon(QIcon(fileName));
    listItem->setText(fileName);
    listItem->setSizeHint(QSize(80, 100));

    int rowCount{ ui->wgtTable->rowCount() };
    ui->wgtTable->insertRow(rowCount);
    QTableWidgetItem *tableItemName{ new QTableWidgetItem(QIcon(fileName),
                                                          QFileInfo(fileName).fileName()) };
    ui->wgtTable->setItem(rowCount, 0, tableItemName);
    QTableWidgetItem *tableItemPath{ new QTableWidgetItem(fileName) };
    ui->wgtTable->setItem(rowCount, 1, tableItemPath);
    QTableWidgetItem *tableItemSize{ new QTableWidgetItem(
            QString::number(static_cast<long>(QFileInfo(fileName).size() / 1024)) + u" KB"_s) };
    ui->wgtTable->setItem(rowCount, 2, tableItemSize);
    m_filesList.push_back(fileName);
}

void fpsBatchDialog::on_actionAddDirectory_triggered()
{
    QString in{ QFileDialog::getExistingDirectory(
            this, tr("Choose a directory containing pictures."),
            appConfig.dialog.lastOpenedDir.empty()
                    ? QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                    : QString::fromStdString(appConfig.dialog.lastOpenedDir)) };
    if (in.isEmpty())
        return;
    else
        appConfig.dialog.lastOpenedDir = in.toStdString();

    QDir dir(in);
    QStringList nameFilters;
    const QByteArrayList supportedFormats{ QImageReader::supportedImageFormats() };
    for (const auto format : supportedFormats)
        nameFilters << u"*."_s + QString(format);

    for (const auto file : dir.entryList(nameFilters, QDir::Files))
        addPicture(in + u"/"_s + file);
}

void fpsBatchDialog::on_cbxLocation_currentIndexChanged(int index)
{
    if (index == 1) { // "The following path:"
        ui->btnOpen->setEnabled(true);
        ui->lePath->setEnabled(true);
        ui->btnChange->setEnabled(true);
    } else {
        ui->btnOpen->setEnabled(false);
        ui->lePath->setEnabled(false);
        ui->btnChange->setEnabled(false);
    }
}

void fpsBatchDialog::on_rbtnAverage_toggled(bool checked)
{
    ui->sbxCols->setEnabled(checked);
    ui->sbxRows->setEnabled(checked);

    if (checked)
        ui->gbxSplitSeq->setEnabled(true);
}

void fpsBatchDialog::on_rbtnSize_toggled(bool checked)
{
    ui->sbxHeight->setEnabled(checked);
    ui->sbxWidth->setEnabled(checked);

    if (checked)
        ui->gbxSplitSeq->setEnabled(true);
}

void fpsBatchDialog::on_rbtnTemplate_toggled(bool checked)
{
    ui->cbxTemplate->setEnabled(checked);

    if (checked) {
        ui->sbxCols->setEnabled(false);
        ui->sbxRows->setEnabled(false);
        ui->sbxHeight->setEnabled(false);
        ui->sbxWidth->setEnabled(false);
        ui->gbxSplitSeq->setEnabled(false);
    }
}

void fpsBatchDialog::on_btnChange_clicked()
{
    QString in{ QFileDialog::getExistingDirectory(
            this, tr("Choose a directory to save pictures."),
            appConfig.dialog.lastSavedToDir.empty()
                    ? QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                    : QString::fromStdString(appConfig.dialog.lastSavedToDir)) };
    if (in.isEmpty())
        return;

    appConfig.dialog.lastSavedToDir = in.toStdString();
    ui->lePath->setText(in);
}

void fpsBatchDialog::on_btnOpen_clicked()
{
    QDesktopServices::openUrl(QUrl(u"file:"_s + ui->lePath->text(), QUrl::TolerantMode));
}

void fpsBatchDialog::on_btnSplit_clicked()
{
    QImageReader reader;
    RectList rects;
    QVector<QVector<QImage>> imageLists(m_filesList.size());
    QVector<QStringList> outputLists(m_filesList.size());
    QStringList outPaths;
    int count{};

    for (qsizetype i{}; i != m_filesList.size(); ++i) {
        reader.setFileName(m_filesList.at(i));

        if (ui->rbtnAverage->isChecked()) {
            if (ui->rbtnHoriLeft->isChecked())
                rects = fpsImageHandler::getSubRects(reader.size().width(), reader.size().height(),
                                                     ui->sbxRows->value(), ui->sbxCols->value(),
                                                     fpsImageHandler::SplitMode::Average,
                                                     fpsImageHandler::SplitSequence::Left);
            else if (ui->rbtnHoriRight->isChecked())
                rects = fpsImageHandler::getSubRects(reader.size().width(), reader.size().height(),
                                                     ui->sbxRows->value(), ui->sbxCols->value(),
                                                     fpsImageHandler::SplitMode::Average,
                                                     fpsImageHandler::SplitSequence::Right);
            else if (ui->rbtnVertLeft->isChecked())
                rects = fpsImageHandler::getSubRects(reader.size().width(), reader.size().height(),
                                                     ui->sbxRows->value(), ui->sbxCols->value(),
                                                     fpsImageHandler::SplitMode::Average,
                                                     fpsImageHandler::SplitSequence::Left);
            else
                rects = fpsImageHandler::getSubRects(reader.size().width(), reader.size().height(),
                                                     ui->sbxRows->value(), ui->sbxCols->value(),
                                                     fpsImageHandler::SplitMode::Average,
                                                     fpsImageHandler::SplitSequence::Right);
        } else if (ui->rbtnSize->isChecked()) {
            if (ui->rbtnHoriLeft->isChecked())
                rects = fpsImageHandler::getSubRects(reader.size().width(), reader.size().height(),
                                                     ui->sbxHeight->value(), ui->sbxWidth->value(),
                                                     fpsImageHandler::SplitMode::Size,
                                                     fpsImageHandler::SplitSequence::Left);
            else if (ui->rbtnHoriRight->isChecked())
                rects = fpsImageHandler::getSubRects(reader.size().width(), reader.size().height(),
                                                     ui->sbxHeight->value(), ui->sbxWidth->value(),
                                                     fpsImageHandler::SplitMode::Size,
                                                     fpsImageHandler::SplitSequence::Right);
            else if (ui->rbtnVertLeft->isChecked())
                rects = fpsImageHandler::getSubRects(reader.size().width(), reader.size().height(),
                                                     ui->sbxHeight->value(), ui->sbxWidth->value(),
                                                     fpsImageHandler::SplitMode::Size,
                                                     fpsImageHandler::SplitSequence::Left);
            else
                rects = fpsImageHandler::getSubRects(reader.size().width(), reader.size().height(),
                                                     ui->sbxHeight->value(), ui->sbxWidth->value(),
                                                     fpsImageHandler::SplitMode::Size,
                                                     fpsImageHandler::SplitSequence::Right);
        }

        if (!rects.isEmpty()) {
            if (!fpsImageHandler::split(reader, imageLists[i], rects)) {
                QMessageBox::warning(this, tr("Batch Splitting"), tr("Error splitting picture."),
                                     QMessageBox::Close);
                return;
            }
            // TODO@25/07/04 Add batch splitting related options.
            outputLists[i] = fpsImageHandler::getOutputList(QFileInfo(m_filesList.at(i)).baseName(),
                                                            QFileInfo(m_filesList.at(i)).suffix(),
                                                            rects.size(), rects[0].size());
            count += static_cast<int>(outputLists[i].size());

            // Get the output directory
            QDir baseDir;
            if (ui->cbxLocation->currentIndex() == 0) // "The same"
                baseDir = QFileInfo(m_filesList.at(i)).dir();
            else {
                if (!ui->lePath->text().isEmpty())
                    baseDir = QDir(ui->lePath->text());
                else {
                    QMessageBox::warning(this, tr("Batch Splitting"),
                                         tr("You have not specified the output directory yet, "
                                            "please try again."),
                                         QMessageBox::Close);
                    break;
                }
            }

            const QString baseName{ QFileInfo(m_filesList.at(i)).baseName() };
            QString outPath;
            if (ui->chbSubdir->isChecked()) {
                if (!QFileInfo::exists(baseDir.absolutePath() + '/' + baseName + '/'))
                    if (!baseDir.mkdir(baseName)) {
                        QMessageBox::warning(this, tr("Batch Splitting"),
                                             tr("QDir::mkdir \'%1\' error!")
                                                     .arg(baseDir.absolutePath() + '/' + baseName),
                                             QMessageBox::Close);
                        break;
                    }
                outPath = baseDir.absolutePath() + '/' + baseName + '/';
            } else
                outPath = baseDir.absolutePath() + '/';
            outPaths.push_back(outPath);
        } else {
            QMessageBox::warning(this, tr("Batch Splitting"),
                                 tr("No rule to split picture: %1.").arg(m_filesList.at(i)),
                                 QMessageBox::Close);
            return;
        }
    }

    fpsProgressDialog dlg(this, count);
    QThread thread;
    fpsSplitWorker worker(outputLists, imageLists, outPaths,
                          QString::fromStdString(appConfig.options.outputOpt.outFormat));
    worker.moveToThread(&thread);
    connect(&worker, &fpsSplitWorker::ready, &thread, &QThread::quit);
    connect(&thread, &QThread::finished, &dlg, &QDialog::close);
    connect(&thread, &QThread::started, &worker, &fpsSplitWorker::doSplit);
    connect(&worker, &fpsSplitWorker::proceed, &dlg, &fpsProgressDialog::proceed);
    connect(&dlg, &fpsProgressDialog::cancelled, &thread, &QThread::requestInterruption);
    connect(&worker, &fpsSplitWorker::error, this, [this](const QString &message) {
        QMessageBox::critical(this, tr("Batch Splitting"), message, QMessageBox::Close);
    });
    thread.start();
    dlg.exec();
}

void fpsBatchDialog::on_wgtTable_customContextMenuRequested(const QPoint &pos)
{
    ui->actionRemoveFromList->setEnabled(!ui->wgtTable->selectedItems().isEmpty());
    m_contextMenu->exec(QCursor::pos());
}

void fpsBatchDialog::on_wgtTable_itemClicked(QTableWidgetItem *item)
{
    ui->actionRemoveFromList->setEnabled(true);
    ui->statusBar->showMessage(ui->wgtTable->currentItem()->text());
}

void fpsBatchDialog::on_wgtTable_itemSelectionChanged()
{
    if (!ui->wgtTable->selectedItems().isEmpty()) {
        ui->actionRemoveFromList->setEnabled(true);
        ui->statusBar->showMessage(ui->wgtTable->currentItem()->text());
    }
}

void fpsBatchDialog::on_wgtList_customContextMenuRequested(const QPoint &pos)
{
    ui->actionRemoveFromList->setEnabled(!ui->wgtList->selectedItems().isEmpty());
    m_contextMenu->exec(QCursor::pos());
}

void fpsBatchDialog::on_wgtList_itemClicked(QListWidgetItem *item)
{
    ui->actionRemoveFromList->setEnabled(true);
    ui->statusBar->showMessage(ui->wgtList->currentItem()->text());
}

void fpsBatchDialog::on_wgtList_itemSelectionChanged()
{
    if (!ui->wgtList->selectedItems().isEmpty()) {
        ui->actionRemoveFromList->setEnabled(true);
        ui->statusBar->showMessage(ui->wgtList->currentItem()->text());
    }
}

void fpsBatchDialog::on_actionRemoveFromList_triggered()
{
    if (!ui->wgtTable->selectedItems().isEmpty()) { // Selected in table
        m_filesList.removeAt(ui->wgtTable->selectedItems()[0]->row());
        delete ui->wgtList->takeItem(ui->wgtTable->selectedItems()[0]->row());
        ui->wgtTable->removeRow(ui->wgtTable->selectedItems()[0]->row());
    } else {
        qsizetype index{ m_filesList.indexOf(ui->wgtList->selectedItems()[0]->text()) };
        m_filesList.removeAt(index);
        delete ui->wgtList->takeItem(index);
        ui->wgtTable->removeRow(index);
    }
}
