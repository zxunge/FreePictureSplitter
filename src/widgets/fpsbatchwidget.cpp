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

#include "fpsbatchwidget.h"
#include "ui_fpsbatchwidget.h"
#include "jsonconfigitems.h"
#include "imagehandler.h"
#include "fpsprogressdialog.h"

#include <QButtonGroup>
#include <QActionGroup>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QFileInfo>
#include <QDesktopServices>
#include <QMessageBox>
#include <QProgressBar>
#include <QStatusBar>
#include <QVector>
#include <QPixmap>
#include <QSize>
#include <QImage>
#include <QCompleter>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QPersistentModelIndex>
#include <QModelIndex>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QFuture>
#include <QFutureWatcher>
#include <QPromise>
#include <QtConcurrent/QtConcurrentRun>
#include <QtAssert>

using namespace Qt::Literals::StringLiterals;
using namespace Util;

fpsBatchWidget::fpsBatchWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::fpsBatchWidget),
      m_model(new QStandardItemModel),
      m_contextMenu(new QMenu(this))
{
    ui->setupUi(this);

    QCompleter *completer{ new QCompleter(this) };
    completer->setModel(new QFileSystemModel(completer));
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lePath->setCompleter(completer);

    ui->viewTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
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

    m_contextMenu->addAction(ui->actionAddDirectory);
    m_contextMenu->addAction(ui->actionAddPicture);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(ui->actionRemoveFromList);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(ui->actionShowDetailInfo);
    m_contextMenu->addAction(ui->actionShowThumbnails);

    // Set up the model
    m_model->setColumnCount(3);
    m_model->setHeaderData(0, Qt::Horizontal, tr("File Name"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("File Path"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("File Size"));
    ui->viewList->setModel(m_model);
    ui->viewTable->setModel(m_model);

    // Load configurations
    ui->cbxLocation->setCurrentIndex(
            appConfig.options.batchOpt.savingTo == Util::SavingTo::specified ? 1 : 0);
    ui->btnOpen->setEnabled(appConfig.options.batchOpt.savingTo == Util::SavingTo::specified);
    ui->lePath->setEnabled(appConfig.options.batchOpt.savingTo == Util::SavingTo::specified);
    ui->lePath->setText(QString::fromStdString(appConfig.options.batchOpt.outPath));
    ui->btnChange->setEnabled(appConfig.options.batchOpt.savingTo == Util::SavingTo::specified);
    ui->chbSubdir->setChecked(appConfig.options.batchOpt.subDir);
}

fpsBatchWidget::~fpsBatchWidget()
{
    delete ui;
    delete m_model;
}

void fpsBatchWidget::on_actionShowThumbnails_toggled(bool checked)
{
    if (checked)
        ui->stView->setCurrentIndex(0); // pgThumbnail
}

void fpsBatchWidget::on_actionShowDetailInfo_toggled(bool checked)
{
    if (checked)
        ui->stView->setCurrentIndex(1); // pgTable
}

void fpsBatchWidget::on_actionAddPicture_triggered()
{
    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes{ QImageReader::supportedMimeTypes() };
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);

    mimeTypeFilters.sort();

    QFileDialog fdlg;
    fdlg.setWindowTitle(tr("Add pictures..."));
    fdlg.setDirectory(appConfig.dialog.lastOpenedDir.empty()
                              ? QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                              : QString::fromStdString(appConfig.dialog.lastOpenedDir));
    fdlg.setMimeTypeFilters(mimeTypeFilters);
    fdlg.setFileMode(QFileDialog::ExistingFiles);
    if (QDialog::Accepted == fdlg.exec() && !fdlg.selectedFiles().isEmpty()
        && QFileInfo(fdlg.selectedFiles().constFirst()).isFile()) {
        appConfig.dialog.lastOpenedDir =
                QFileInfo(fdlg.selectedFiles().constFirst()).path().toStdString();

        const QStringList list{ fdlg.selectedFiles() };
        // m_pbLoading->setRange(0, list.size());
        // m_pbLoading->setVisible(true);
        int count{};
        foreach (const auto file, list) {
            QCoreApplication::processEvents();
            addPicture(file);
            // m_pbLoading->setValue(++count);
        }
        // m_pbLoading->setVisible(false);
    } else
        return;
}

void fpsBatchWidget::closeEvent(QCloseEvent *event)
{
    // Save configurations
    if (ui->cbxLocation->currentIndex() == 0) // "The same"
        appConfig.options.batchOpt.savingTo = Util::SavingTo::same;
    else
        appConfig.options.batchOpt.savingTo = Util::SavingTo::specified;
    appConfig.options.batchOpt.outPath = ui->lePath->text().toStdString();
    appConfig.options.batchOpt.subDir = ui->chbSubdir->isChecked();

    QWidget::closeEvent(event);
}

void fpsBatchWidget::addPicture(const QString &fileName)
{
    QImageReader reader(fileName);
    QStandardItem *itemName{ new QStandardItem(QIcon(QPixmap::fromImageReader(&reader)),
                                               QFileInfo(fileName).fileName()) };
    QStandardItem *itemPath{ new QStandardItem(fileName) };
    QStandardItem *itemSize{ new QStandardItem(
            QString::number(static_cast<long>(QFileInfo(fileName).size() / 1024)) + u" KB"_s) };
    m_model->appendRow(QList<QStandardItem *>{ itemName, itemPath, itemSize });
}

void fpsBatchWidget::on_actionAddDirectory_triggered()
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
    foreach (const auto format, supportedFormats)
        nameFilters << u"*."_s + QString(format);

    const QStringList list{ dir.entryList(nameFilters, QDir::Files) };
    int count{};
    // m_pbLoading->setRange(0, list.size());
    // m_pbLoading->setVisible(true);
    foreach (const auto file, list) {
        QCoreApplication::processEvents();
        addPicture(in + u"/"_s + file);
        // m_pbLoading->setValue(++count);
    }
    // m_pbLoading->setVisible(false);
}

void fpsBatchWidget::on_cbxLocation_currentIndexChanged(int index)
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

void fpsBatchWidget::on_rbtnAverage_toggled(bool checked)
{
    ui->sbxCols->setEnabled(checked);
    ui->sbxRows->setEnabled(checked);

    if (checked)
        ui->gbxSplitSeq->setEnabled(true);
}

void fpsBatchWidget::on_rbtnSize_toggled(bool checked)
{
    ui->sbxHeight->setEnabled(checked);
    ui->sbxWidth->setEnabled(checked);

    if (checked)
        ui->gbxSplitSeq->setEnabled(true);
}

void fpsBatchWidget::on_rbtnTemplate_toggled(bool checked)
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

void fpsBatchWidget::on_btnChange_clicked()
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

void fpsBatchWidget::on_btnOpen_clicked()
{
    QDesktopServices::openUrl(QUrl(u"file:"_s + ui->lePath->text(), QUrl::TolerantMode));
}

void fpsBatchWidget::on_btnSplit_clicked()
{
    QImageReader reader;
    RectList rects;
    QImageWriter writer;
    QVector<QImage> imageList;
    QStringList outputList;
    int count{};

    // Detect splitting sequence and mode
    ImageHandler::SplitMode mode;
    ImageHandler::SplitSequence sequence;
    if (ui->rbtnAverage->isChecked()) {
        if (ui->rbtnHoriLeft->isChecked()) {
            mode = ImageHandler::SplitMode::Average;
            sequence = ImageHandler::SplitSequence::Left;
        } else if (ui->rbtnHoriRight->isChecked()) {
            mode = ImageHandler::SplitMode::Average;
            sequence = ImageHandler::SplitSequence::Right;
        } else if (ui->rbtnVertLeft->isChecked()) {
            mode = ImageHandler::SplitMode::Average;
            sequence = ImageHandler::SplitSequence::Left;
        } else {
            mode = ImageHandler::SplitMode::Average;
            sequence = ImageHandler::SplitSequence::Right;
        }
    } else if (ui->rbtnSize->isChecked()) {
        if (ui->rbtnHoriLeft->isChecked()) {
            mode = ImageHandler::SplitMode::Size;
            sequence = ImageHandler::SplitSequence::Left;
        } else if (ui->rbtnHoriRight->isChecked()) {
            mode = ImageHandler::SplitMode::Size;
            sequence = ImageHandler::SplitSequence::Right;
        } else if (ui->rbtnVertLeft->isChecked()) {
            mode = ImageHandler::SplitMode::Size;
            sequence = ImageHandler::SplitSequence::Left;
        } else {
            mode = ImageHandler::SplitMode::Size;
            sequence = ImageHandler::SplitSequence::Right;
        }
    } else
        return; // TODO@25/08/22 Add support for splitting using templates.

    fpsProgressDialog dlg(this, m_model->rowCount());
    QFutureWatcher<void> watcher;
    connect(&watcher, &QFutureWatcher<void>::progressValueChanged, [&](int progressValue) {
        if (progressValue == -1)
            QMessageBox::critical(this, fpsAppName, watcher.progressText(), QMessageBox::Close);
        else
            dlg.proceed(progressValue);
    });
    connect(&watcher, &QFutureWatcher<void>::finished, &dlg, &QDialog::close);
    connect(&dlg, &fpsProgressDialog::cancelled, &watcher, &QFutureWatcher<void>::cancel);
    watcher.setFuture(QtConcurrent::run([&](QPromise<void> &promise) {
        int count{};
        for (qsizetype i{}; i != m_model->rowCount(); ++i) {
            const QString path{ m_model->item(i, 1)->text() };
            promise.suspendIfRequested();
            if (promise.isCanceled())
                return;

            reader.setFileName(path);

            rects = ImageHandler::getSubRects(
                    reader.size().width(), reader.size().height(),
                    mode == ImageHandler::SplitMode::Size ? ui->sbxHeight->value()
                                                          : ui->sbxRows->value(),
                    mode == ImageHandler::SplitMode::Size ? ui->sbxWidth->value()
                                                          : ui->sbxCols->value(),
                    mode, sequence);

            if (!rects.isEmpty()) {
                if (!ImageHandler::split(reader, imageList, rects)) {
                    promise.setProgressValueAndText(-1, tr("Error splitting picture."));
                    return;
                }
                // TODO@25/07/04 Add batch splitting related options.
                outputList = ImageHandler::getOutputList(QFileInfo(path).baseName(),
                                                         QFileInfo(path).suffix(), rects.size(),
                                                         rects[0].size());

                // Get the output directory
                QDir baseDir;
                if (ui->cbxLocation->currentIndex() == 0) // "The same"
                    baseDir = QFileInfo(path).dir();
                else {
                    if (!ui->lePath->text().isEmpty())
                        baseDir = QDir(ui->lePath->text());
                    else {
                        promise.setProgressValueAndText(
                                -1,
                                tr("You have not specified the output directory yet, "
                                   "please try again."));
                        return;
                    }
                }

                const QString baseName{ QFileInfo(path).baseName() };
                QString outPath;
                if (ui->chbSubdir->isChecked()) {
                    if (!QFileInfo::exists(baseDir.absolutePath() + '/' + baseName + '/'))
                        if (!baseDir.mkdir(baseName)) {
                            promise.setProgressValueAndText(
                                    -1,
                                    tr("QDir::mkdir \'%1\' error!")
                                            .arg(baseDir.absolutePath() + '/' + baseName));
                            break;
                        }
                    outPath = baseDir.absolutePath() + '/' + baseName + '/';
                } else
                    outPath = baseDir.absolutePath() + '/';

                for (qsizetype i{}; i != imageList.size(); ++i) {
                    writer.setFileName(outPath + outputList[i]);
                    if (!writer.write(imageList[i])) {
                        promise.setProgressValueAndText(
                                -1,
                                tr("Error writing to file \'%1\': %2.")
                                        .arg(writer.fileName(), writer.errorString()));
                        return;
                    }
                }
            } else {
                promise.setProgressValueAndText(-1, tr("No rule to split picture: %1.").arg(path));
                return;
            }
            promise.setProgressValue(++count);
        }
    }));
    dlg.exec();
}

void fpsBatchWidget::on_actionRemoveFromList_triggered()
{
    if (ui->stView->currentIndex() == 0) { // pgThumbnail
        if (ui->viewList->selectionModel()->hasSelection()) {
            QList<QPersistentModelIndex> indexes;
            foreach (const QModelIndex &i, ui->viewList->selectionModel()->selectedIndexes())
                indexes << i;
            foreach (const QPersistentModelIndex &i, indexes)
                m_model->removeRow(i.row());
        }
    } else { // pgTable
        if (ui->viewTable->selectionModel()->hasSelection()) {
            QList<QPersistentModelIndex> indexes;
            foreach (const QModelIndex &i, ui->viewTable->selectionModel()->selectedIndexes())
                indexes << i;
            foreach (const QPersistentModelIndex &i, indexes)
                m_model->removeRow(i.row());
        }
    }
}

void fpsBatchWidget::on_viewList_customContextMenuRequested(const QPoint &pos)
{
    ui->actionRemoveFromList->setEnabled(ui->viewList->selectionModel()->hasSelection());
    m_contextMenu->exec(QCursor::pos());
}

void fpsBatchWidget::on_viewList_clicked(const QModelIndex &index)
{
    ui->actionRemoveFromList->setEnabled(true);
    Q_EMIT message(m_model->itemData(index).value(0).toString());
}

void fpsBatchWidget::on_viewTable_clicked(const QModelIndex &index) 
{
    ui->actionRemoveFromList->setEnabled(true);
    Q_EMIT message(m_model->itemData(index).value(0).toString());
}

void fpsBatchWidget::on_viewTable_customContextMenuRequested(const QPoint &pos)
{
    ui->actionRemoveFromList->setEnabled(ui->viewTable->selectionModel()->hasSelection());
    m_contextMenu->exec(QCursor::pos());
}
