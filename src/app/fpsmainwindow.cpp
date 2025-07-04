// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsmainwindow.h"
#include "ui_fpsmainwindow.h"
#include "fpsbatchdialog.h"
#include "fpssettingsdialog.h"
#include "fpsaboutdialog.h"
#include "fpsprogressdialog.h"
#include "jsonconfigitems.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QTextBrowser>
#include <QStringLiteral>
#include <QFileInfo>

extern Util::Config appConfig;

fpsMainWindow::fpsMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::fpsMainWindow)
{
    ui->setupUi(this);
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

fpsMainWindow::~fpsMainWindow()
{
    if (ui) {
        delete ui;
        ui = nullptr;
    }
}

void fpsMainWindow::on_actionOpen_triggered()
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
    fdlg.setFileMode(QFileDialog::ExistingFile);
    if (QDialog::Accepted == fdlg.exec()) {
        m_imgReader.setFileName(fdlg.selectedFiles()[0]);
        appConfig.dialog.lastOpenedDir = QFileInfo(fdlg.selectedFiles()[0]).path().toStdString();
    } else
        return;

    if (m_imgReader.fileName().isEmpty())
        return;

    m_imgReader.setAutoTransform(true);

    if (m_imgReader.canRead()) {
        QPixmap pixmap{ QPixmap::fromImageReader(&m_imgReader) };
        ui->graphicsView->showPixmap(pixmap);

        // Display image info on StatusBar; they are: file name, width * height, color depth,
        // vertical DPI, horizontal DPI
        ui->statusBar->showMessage(tr("%1, %2x%3, Depth: %4, Vertical: %5 dpi, Horizontal: %6 dpi")
                                           .arg(m_imgReader.fileName())
                                           .arg(m_imgReader.size().width())
                                           .arg(m_imgReader.size().height())
                                           .arg(pixmap.depth())
                                           .arg(pixmap.logicalDpiY())
                                           .arg(pixmap.logicalDpiX()));

        ui->btnReset->setEnabled(true);
        ui->actionZoomIn->setEnabled(true);
        ui->actionZoomOut->setEnabled(true);
        ui->sbxCols->setRange(1, m_imgReader.size().width());
        ui->sbxRows->setRange(1, m_imgReader.size().height());
        ui->sbxHeight->setRange(1, m_imgReader.size().height());
        ui->sbxWidth->setRange(1, m_imgReader.size().width());
        if (ui->rbtnManual->isChecked())
            ui->actionSave->setEnabled(true);
    } else
        QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                             tr("Error loading picture file: %1.").arg(m_imgReader.fileName()),
                             QMessageBox::Close);
}

void fpsMainWindow::on_actionSave_triggered()
{
    QVector<QImage> imageList;
    QImageWriter writer;
    QStringList outputList;
    QString out{ QFileDialog::getExistingDirectory(
            this, tr("Choose the output directory."),
            appConfig.dialog.lastSavedToDir.empty()
                    ? "/"
                    : QString::fromStdString(appConfig.dialog.lastSavedToDir)) };

    if (out.isEmpty())
        return;
    else
        appConfig.dialog.lastSavedToDir = out.toStdString();

    if (ui->rbtnManual->isChecked())
        m_rects = fpsImageHandler::linesToRects(ui->graphicsView);
    else if (m_rects.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                             tr("Please at least choose one splitting mode, offer "
                                "useful data then reset the splitting lines."),
                             QMessageBox::Close);
        return;
    }

    if (!m_rects.isEmpty()) {
        if (!fpsImageHandler::split(m_imgReader, imageList, m_rects)) {
            QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                                 tr("Error splitting picture."), QMessageBox::Close);
            return;
        }
        outputList = fpsImageHandler::getOutputList(
                appConfig.options.nameOpt.prefMode == Util::Prefix::same
                        ? QFileInfo(m_imgReader.fileName()).baseName()
                        : QString::fromStdString(appConfig.options.nameOpt.prefix),
                QString::fromStdString(appConfig.options.outputOpt.outFormat), m_rects.size(),
                m_rects[0].size(), appConfig.options.nameOpt.rcContained);
        fpsProgressDialog dlg(this, outputList.size());
        connect(this, &fpsMainWindow::splitProceed, &dlg, &fpsProgressDialog::proceed);
        dlg.show();
        for (int i{}; i != imageList.size(); ++i) {
            writer.setFileName(out + "/" + outputList[i]);
            writer.setFormat(
                    QString::fromStdString(appConfig.options.outputOpt.outFormat).toUtf8());
            writer.setQuality(appConfig.options.outputOpt.jpgQuality);
            if (!writer.write(imageList[i])) {
                QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                                     tr("Error writing to file \'%1\': %2.")
                                             .arg(writer.fileName())
                                             .arg(writer.errorString()),
                                     QMessageBox::Close);
                break;
            }
            Q_EMIT splitProceed(i + 1);
        }
        dlg.close();
    } else {
        QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                             tr("No rule to split this picture"), QMessageBox::Close);
        return;
    }
}

void fpsMainWindow::on_actionExit_triggered()
{
    close();
}

void fpsMainWindow::on_actionBatch_triggered()
{
    fpsBatchDialog *batchDlg{ new fpsBatchDialog(this) };
    batchDlg->setAttribute(Qt::WA_DeleteOnClose, true);
    batchDlg->exec();
}

void fpsMainWindow::on_actionSettings_triggered()
{
    fpsSettingsDialog *settingsDlg{ new fpsSettingsDialog(this) };
    settingsDlg->setAttribute(Qt::WA_DeleteOnClose, true);
    settingsDlg->exec();
}

void fpsMainWindow::on_actionHomepage_triggered()
{
    QDesktopServices::openUrl(QUrl(fpsHomepageUrl, QUrl::TolerantMode));
}

void fpsMainWindow::on_actionAbout_triggered()
{
    fpsAboutDialog *aboutDlg{ new fpsAboutDialog(this) };
    aboutDlg->setAttribute(Qt::WA_DeleteOnClose, true);
    aboutDlg->exec();
}

void fpsMainWindow::on_btnReset_clicked()
{
    if (ui->rbtnAver->isChecked())
        if (ui->rbtnHoriLeft->isChecked())
            m_rects = fpsImageHandler::getSubRects(
                    m_imgReader.size().width(), m_imgReader.size().height(), ui->sbxRows->value(),
                    ui->sbxCols->value(), fpsImageHandler::Average, fpsImageHandler::Left);
        else if (ui->rbtnHoriRight->isChecked())
            m_rects = fpsImageHandler::getSubRects(
                    m_imgReader.size().width(), m_imgReader.size().height(), ui->sbxRows->value(),
                    ui->sbxCols->value(), fpsImageHandler::Average, fpsImageHandler::Right);
        else if (ui->rbtnVertLeft->isChecked())
            m_rects = fpsImageHandler::getSubRects(
                    m_imgReader.size().width(), m_imgReader.size().height(), ui->sbxRows->value(),
                    ui->sbxCols->value(), fpsImageHandler::Average, fpsImageHandler::Left);
        else
            m_rects = fpsImageHandler::getSubRects(
                    m_imgReader.size().width(), m_imgReader.size().height(), ui->sbxRows->value(),
                    ui->sbxCols->value(), fpsImageHandler::Average, fpsImageHandler::Right);
    else if (ui->rbtnHoriLeft->isChecked())
        m_rects = fpsImageHandler::getSubRects(
                m_imgReader.size().width(), m_imgReader.size().height(), ui->sbxHeight->value(),
                ui->sbxWidth->value(), fpsImageHandler::Size, fpsImageHandler::Left);
    else if (ui->rbtnHoriRight->isChecked())
        m_rects = fpsImageHandler::getSubRects(
                m_imgReader.size().width(), m_imgReader.size().height(), ui->sbxHeight->value(),
                ui->sbxWidth->value(), fpsImageHandler::Size, fpsImageHandler::Right);
    else if (ui->rbtnVertLeft->isChecked())
        m_rects = fpsImageHandler::getSubRects(
                m_imgReader.size().width(), m_imgReader.size().height(), ui->sbxHeight->value(),
                ui->sbxWidth->value(), fpsImageHandler::Size, fpsImageHandler::Left);
    else
        m_rects = fpsImageHandler::getSubRects(
                m_imgReader.size().width(), m_imgReader.size().height(), ui->sbxHeight->value(),
                ui->sbxWidth->value(), fpsImageHandler::Size, fpsImageHandler::Right);
    ui->actionSave->setEnabled(true);
    ui->graphicsView->removeAllFloatingLines();
    fpsImageHandler::rectsToLines(m_rects, ui->graphicsView);
}

void fpsMainWindow::on_rbtnSize_toggled(bool checked)
{
    if (checked) {
        ui->sbxCols->setEnabled(false);
        ui->sbxRows->setEnabled(false);
        ui->sbxHeight->setEnabled(true);
        ui->sbxWidth->setEnabled(true);
        ui->btnReset->setEnabled(true);
        ui->rbtnHoriLeft->setEnabled(true);
        ui->rbtnHoriRight->setEnabled(true);
        ui->rbtnVertLeft->setEnabled(true);
        ui->rbtnVertRight->setEnabled(true);
        if (!ui->graphicsView->scene())
            ui->btnReset->setEnabled(false);
    }
}

void fpsMainWindow::on_rbtnAver_toggled(bool checked)
{
    if (checked) {
        ui->sbxCols->setEnabled(true);
        ui->sbxRows->setEnabled(true);
        ui->sbxHeight->setEnabled(false);
        ui->sbxWidth->setEnabled(false);
        ui->btnReset->setEnabled(true);
        ui->rbtnHoriLeft->setEnabled(true);
        ui->rbtnHoriRight->setEnabled(true);
        ui->rbtnVertLeft->setEnabled(true);
        ui->rbtnVertRight->setEnabled(true);
        if (!ui->graphicsView->scene())
            ui->btnReset->setEnabled(false);
    }
}

void fpsMainWindow::on_actionZoomIn_triggered()
{
    ui->graphicsView->zoomIn();
}

void fpsMainWindow::on_actionZoomOut_triggered()
{
    ui->graphicsView->zoomOut();
}

void fpsMainWindow::on_rbtnManual_toggled(bool checked)
{
    if (checked) {
        ui->sbxCols->setEnabled(false);
        ui->sbxRows->setEnabled(false);
        ui->sbxHeight->setEnabled(false);
        ui->sbxWidth->setEnabled(false);
        ui->btnReset->setEnabled(false);
        ui->rbtnHoriLeft->setEnabled(false);
        ui->rbtnHoriRight->setEnabled(false);
        ui->rbtnVertLeft->setEnabled(false);
        ui->rbtnVertRight->setEnabled(false);
        if (ui->graphicsView->scene())
            ui->actionSave->setEnabled(true);
    }
}
