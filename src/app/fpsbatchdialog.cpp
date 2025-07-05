// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsbatchdialog.h"
#include "ui_fpsbatchdialog.h"
#include "jsonconfigitems.h"
#include "fpsimagehandler.h"
#include "fpsprogressdialog.h"

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

#include <limits>

extern Util::Config appConfig;

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
    ui->wgtTable->setHorizontalHeaderLabels(QStringList{
            QT_TR_NOOP("File Name"), QT_TR_NOOP("File Path"), QT_TR_NOOP("File Size") });
    ui->wgtTable->verticalHeader()->setVisible(false);
    ui->wgtTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->wgtTable->horizontalHeader()->setStretchLastSection(true);
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
    fdlg.setWindowTitle(tr("Open a picture..."));
    fdlg.setDirectory(appConfig.dialog.lastOpenedDir.empty()
                              ? "."
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
            QString::number(static_cast<long>(QFileInfo(fileName).size() / 1024)) + " KB") };
    ui->wgtTable->setItem(rowCount, 2, tableItemSize);
    m_filesList.push_back(fileName);
}

void fpsBatchDialog::on_actionAddDirectory_triggered()
{
    QString in{ QFileDialog::getExistingDirectory(
            this, tr("Choose a directory containing pictures."),
            appConfig.dialog.lastOpenedDir.empty()
                    ? "."
                    : QString::fromStdString(appConfig.dialog.lastOpenedDir)) };
    if (in.isEmpty())
        return;
    else
        appConfig.dialog.lastOpenedDir = in.toStdString();

    QDir dir(in);
    QStringList nameFilters;
    const QByteArrayList supportedFormats{ QImageReader::supportedImageFormats() };
    for (const auto format : supportedFormats)
        nameFilters << "*." + QString(format);

    for (const auto file : dir.entryList(nameFilters, QDir::Files))
        addPicture(in + '/' + file);
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
                    ? "."
                    : QString::fromStdString(appConfig.dialog.lastSavedToDir)) };
    if (in.isEmpty())
        return;
    else
        appConfig.dialog.lastSavedToDir = in.toStdString();
    ui->lePath->setText(in);
}

void fpsBatchDialog::on_btnOpen_clicked()
{
    QDesktopServices::openUrl(QUrl("file:" + ui->lePath->text(), QUrl::TolerantMode));
}

void fpsBatchDialog::on_btnSplit_clicked()
{
    QImageReader reader;
    QImageWriter writer;
    RectList rects;
    QVector<QImage> imageList;
    QStringList outputList;
    int count{};

    fpsProgressDialog dlg(this, m_filesList.size());
    bool cancelled{ false };

    connect(this, &fpsBatchDialog::splitProceed, &dlg, &fpsProgressDialog::proceed);
    connect(&dlg, &fpsProgressDialog::cancelled, this, [&cancelled]() { cancelled = true; });

    dlg.show();

    for (const auto file : m_filesList) {
        if (cancelled)
            break;

        reader.setFileName(file);

        if (ui->rbtnAverage->isChecked()) {
            if (ui->rbtnHoriLeft->isChecked())
                rects = fpsImageHandler::getSubRects(
                        reader.size().width(), reader.size().height(), ui->sbxRows->value(),
                        ui->sbxCols->value(), fpsImageHandler::Average, fpsImageHandler::Left);
            else if (ui->rbtnHoriRight->isChecked())
                rects = fpsImageHandler::getSubRects(
                        reader.size().width(), reader.size().height(), ui->sbxRows->value(),
                        ui->sbxCols->value(), fpsImageHandler::Average, fpsImageHandler::Right);
            else if (ui->rbtnVertLeft->isChecked())
                rects = fpsImageHandler::getSubRects(
                        reader.size().width(), reader.size().height(), ui->sbxRows->value(),
                        ui->sbxCols->value(), fpsImageHandler::Average, fpsImageHandler::Left);
            else
                rects = fpsImageHandler::getSubRects(
                        reader.size().width(), reader.size().height(), ui->sbxRows->value(),
                        ui->sbxCols->value(), fpsImageHandler::Average, fpsImageHandler::Right);
        } else if (ui->rbtnSize->isChecked()) {
            if (ui->rbtnHoriLeft->isChecked())
                rects = fpsImageHandler::getSubRects(reader.size().width(), reader.size().height(),
                                                     ui->sbxHeight->value(), ui->sbxWidth->value(),
                                                     fpsImageHandler::Size, fpsImageHandler::Left);
            else if (ui->rbtnHoriRight->isChecked())
                rects = fpsImageHandler::getSubRects(reader.size().width(), reader.size().height(),
                                                     ui->sbxHeight->value(), ui->sbxWidth->value(),
                                                     fpsImageHandler::Size, fpsImageHandler::Right);
            else if (ui->rbtnVertLeft->isChecked())
                rects = fpsImageHandler::getSubRects(reader.size().width(), reader.size().height(),
                                                     ui->sbxHeight->value(), ui->sbxWidth->value(),
                                                     fpsImageHandler::Size, fpsImageHandler::Left);
            else
                rects = fpsImageHandler::getSubRects(reader.size().width(), reader.size().height(),
                                                     ui->sbxHeight->value(), ui->sbxWidth->value(),
                                                     fpsImageHandler::Size, fpsImageHandler::Right);
        }

        if (!rects.isEmpty()) {
            if (!fpsImageHandler::split(reader, imageList, rects)) {
                QMessageBox::warning(this, tr("FreePictureSplitter Batch Splitting"),
                                     tr("Error splitting picture."), QMessageBox::Close);
                return;
            }
            // TODO@25/07/04 Add batch splitting related options.
            outputList = fpsImageHandler::getOutputList(QFileInfo(file).baseName(),
                                                        QFileInfo(file).suffix(), rects.size(),
                                                        rects[0].size());

            // Get the output directory
            QDir baseDir;
            if (ui->cbxLocation->currentIndex() == 0) // "The same"
                baseDir = QFileInfo(file).dir();
            else {
                if (!ui->lePath->text().isEmpty())
                    baseDir = QDir(ui->lePath->text());
                else {
                    QMessageBox::warning(this, tr("FreePictureSplitter Batch Splitting"),
                                         tr("You have not specified the output directory yet, "
                                            "please try again."),
                                         QMessageBox::Close);
                    break;
                }
            }

            const QString baseName{ QFileInfo(file).baseName() };
            QString outPath;
            if (ui->chbSubdir->isChecked()) {
                if (!QFileInfo::exists(baseDir.absolutePath() + '/' + baseName + '/'))
                    if (!baseDir.mkdir(baseName)) {
                        QMessageBox::warning(this, tr("FreePictureSplitter Batch Splitting"),
                                             tr("QDir::mkdir \'%1\' error!")
                                                     .arg(baseDir.absolutePath() + '/' + baseName),
                                             QMessageBox::Close);
                        break;
                    }
                outPath = baseDir.absolutePath() + '/' + baseName + '/';
            } else
                outPath = baseDir.absolutePath() + '/';

            for (int i{}; i != imageList.size(); ++i) {
                writer.setFileName(outPath + outputList[i]);
                if (!writer.write(imageList[i])) {
                    QMessageBox::warning(this, tr("FreePictureSplitter Batch Splitting"),
                                         tr("Error writing to file \'%1\': %2.")
                                                 .arg(writer.fileName())
                                                 .arg(writer.errorString()),
                                         QMessageBox::Close);
                    break;
                }
            }
        } else {
            QMessageBox::warning(this, tr("FreePictureSplitter Batch Splitting"),
                                 tr("No rule to split this picture"), QMessageBox::Close);
            return;
        }
        Q_EMIT splitProceed(++count);
    }
    dlg.close();
}
