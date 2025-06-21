// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "config.h"
#include "fpsmainwindow.h"
#include "ui_fpsmainwindow.h"
#include "fpsbatchdialog.h"
#include "fpssettingsdialog.h"
#include "fpsaboutdialog.h"
#include "fpsprogressdialog.h"

fpsMainWindow::fpsMainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::fpsMainWindow),
      m_aboutDlg(nullptr),
      m_batchDlg(nullptr),
      m_settingsDlg(nullptr)
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
    if (m_batchDlg) {
        delete m_batchDlg;
        m_batchDlg = nullptr;
    }
    if (m_aboutDlg) {
        delete m_aboutDlg;
        m_aboutDlg = nullptr;
    }
    if (m_settingsDlg) {
        delete m_settingsDlg;
        m_settingsDlg = nullptr;
    }
}

void fpsMainWindow::on_actionOpen_triggered()
{
    m_imgReader.setFileName(
            QFileDialog::getOpenFileName(this, tr("Open a picture..."), QStringLiteral("/"),
                                         tr("Image files (*.jpg *.png *.jpeg *.gif *.bmp)")));

    if (m_imgReader.fileName().isEmpty())
        return;

    m_imgReader.setAutoTransform(true);

    if (m_imgReader.canRead()) {
        ui->graphicsView->showPixmap(QPixmap::fromImageReader(&m_imgReader));

        // Because we have previously read the image, we need to re-setFileName.
        m_imgReader.setFileName(m_imgReader.fileName());
        QImage tempImg{ m_imgReader.read() };

        // Display image info on StatusBar; they are: file name, width * height, color depth,
        // vertical DPI, horizontal DPI
        ui->statusBar->showMessage(
                m_imgReader.fileName() + ", " + QString::number(m_imgReader.size().width()) + "x"
                + QString::number(m_imgReader.size().height()) + tr(", Depth: ")
                + QString::number(tempImg.depth()) + tr(", Vertical: ")
                + QString::number(static_cast<int>(tempImg.dotsPerMeterY() * 0.0254))
                + tr(" dpi, Horizontal: ")
                + QString::number(static_cast<int>(tempImg.dotsPerMeterX() * 0.0254)) + " dpi");

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
                             QString("Error loading picture file: %1.").arg(m_imgReader.fileName()),
                             QMessageBox::Close);
}

void fpsMainWindow::on_actionSave_triggered()
{
    QVector<QImage> imageList;
    QImageWriter writer;
    QStringList outputList;
    QString out{ QFileDialog::getExistingDirectory(this, tr("Choose the output directory.")) };

    if (ui->rbtnManual->isChecked())
        m_rects = fpsImageHandler::linesToRects(ui->graphicsView);
    else if (m_rects.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                             QString("Please at least choose one splitting mode, offer "
                                     "useful data then reset the splitting lines."),
                             QMessageBox::Close);
        return;
    }

    if (!m_rects.isEmpty()) {
        if (!fpsImageHandler::split(m_imgReader, imageList, m_rects)) {
            QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                                 QString("Error splitting picture."), QMessageBox::Close);
            return;
        }
        outputList = fpsImageHandler::getOutputList(m_imgReader.fileName(), m_rects.size(),
                                                    m_rects[0].size());
        fpsProgressDialog dlg(this, outputList.size());
        connect(this, &fpsMainWindow::proceed, &dlg, &fpsProgressDialog::on_proceed);
        dlg.show();
        for (int i{}; i != imageList.size(); ++i) {
            writer.setFileName(out + "/" + outputList[i]);
            if (!writer.write(imageList[i])) {
                QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                                     QStringLiteral("Error writing to file \'") + writer.fileName()
                                             + QStringLiteral("\', ") + writer.errorString(),
                                     QMessageBox::Close);
                break;
            }
            Q_EMIT proceed(i + 1);
        }
        dlg.close();
    } else {
        QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                             QString("No rule to split this picture"), QMessageBox::Close);
        return;
    }
}

void fpsMainWindow::on_actionExit_triggered()
{
    close();
}

void fpsMainWindow::on_actionBatch_triggered()
{
    m_batchDlg = new fpsBatchDialog(this);
    m_batchDlg->exec();
}

void fpsMainWindow::on_actionSettings_triggered()
{
    m_settingsDlg = new fpsSettingsDialog(this);
    m_settingsDlg->exec();
}

void fpsMainWindow::on_actionHomepage_triggered()
{
    QDesktopServices::openUrl(QUrl(fpsHomepageUrl, QUrl::TolerantMode));
}

void fpsMainWindow::on_actionAbout_triggered()
{
    m_aboutDlg = new fpsAboutDialog(this);
    m_aboutDlg->exec();
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
