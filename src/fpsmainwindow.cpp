// # This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
// # Copyright (c) 2024 2025 zxunge
// # See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "config.h"
#include "fpsmainwindow.h"
#include "ui_fpsmainwindow.h"
#include "fpsbatchdialog.h"
#include "fpsaboutdialog.h"
#include "fpsprogressdialog.h"
#include "debugutil.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QTextBrowser>
#include <QStringLiteral>

fpsMainWindow::fpsMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::fpsMainWindow), m_aboutDlg(nullptr),
      m_batchDlg(nullptr)
{
    ui->setupUi(this);
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

fpsMainWindow::~fpsMainWindow()
{
    if (ui)
    {
        delete ui;
        ui = nullptr;
    }
    if (m_batchDlg)
    {
        delete m_batchDlg;
        m_batchDlg = nullptr;
    }
    if (m_aboutDlg)
    {
        delete m_aboutDlg;
        m_aboutDlg = nullptr;
    }
}

void fpsMainWindow::on_actionOpen_triggered()
{
    m_fileName = QFileDialog::
        getOpenFileName(this, tr("Open a picture..."), QStringLiteral("/"),
                        tr("Image files (*.jpg *.png *.jpeg *.gif *.bmp)"));

    if (m_fileName.isEmpty()) return;

    if (m_originalImage.load(m_fileName))
    {
        ui->graphicsView->setImage(m_originalImage);

        // Display image info on StatusBar; they are: file name, width * height, color depth, vertical DPI, horizontal DPI
        ui->statusBar->showMessage(
            m_fileName + ", " + QString::number(m_originalImage.width()) + "x" +
            QString::number(m_originalImage.height()) + tr(", Depth: ") +
            QString::number(m_originalImage.depth()) + tr(", Vertical: ") +
            QString::number(
                static_cast<int>(m_originalImage.dotsPerMeterY() * 0.0254)) +
            tr(" dpi, Horizontal: ") +
            QString::number(
                static_cast<int>(m_originalImage.dotsPerMeterX() * 0.0254)) +
            " dpi");

        ui->btnReset->setEnabled(true);
        ui->actionZoomIn->setEnabled(true);
        ui->actionZoomIn->setIcon(QIcon(":/toolBar/toolBar/zoom-in.png"));
        ui->actionZoomOut->setEnabled(true);
        ui->actionZoomOut->setIcon(QIcon(":/toolBar/toolBar/zoom-out.png"));
        ui->sbxCols->setRange(1, m_originalImage.width());
        ui->sbxRows->setRange(1, m_originalImage.height());
        ui->sbxHeight->setRange(1, m_originalImage.height());
        ui->sbxWidth->setRange(1, m_originalImage.width());
        if (ui->rbtnManual->isChecked())
        {
            ui->actionSave->setEnabled(true);
            ui->actionSave->setIcon(QIcon(":/toolBar/toolBar/save.png"));
        }
    }
    else
        QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                             QString("Error loading picture file: %1.")
                                 .arg(m_fileName),
                             QMessageBox::Close);
}

void fpsMainWindow::on_actionSave_triggered()
{
    if (m_originalImage.isNull()) return;

    QVector<QImage> imageList;
    QStringList     outputList;
    QString         out {
        QFileDialog::getExistingDirectory(this,
                                                  tr("Choose the output directory."))
    };

    if (ui->rbtnManual->isChecked())
        m_rects = fpsImageHandler::linesToRects(ui->graphicsView);
    else if (m_rects.isEmpty())
    {
        QMessageBox::
            warning(this, QStringLiteral("FreePictureSplitter"),
                    QString("Please at least choose one splitting mode, offer "
                            "useful data then reset the splitting lines."),
                    QMessageBox::Close);
        return;
    }

    if (!m_rects.isEmpty())
    {
        if (!fpsImageHandler::split(m_originalImage, imageList, m_rects))
        {
            QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                                 QString("Error splitting picture."),
                                 QMessageBox::Close);
            return;
        }
        outputList = fpsImageHandler::getOutputList(m_fileName, m_rects.size(),
                                                    m_rects[0].size());
        fpsProgressDialog dlg(this, outputList.size());
        connect(this, SIGNAL(proceed(int)), &dlg, SLOT(on_proceed(int)));
        dlg.show();
        for (int i {}; i != imageList.size(); ++i)
        {
            imageList[i].save(out + "/" + outputList[i]);
            Q_EMIT proceed(i + 1);
        }
        dlg.close();
    }
    else
    {
        QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                             QString("No rule to split this picture"),
                             QMessageBox::Close);
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
            m_rects = fpsImageHandler::getSubRects(m_originalImage.width(),
                                                   m_originalImage.height(),
                                                   ui->sbxRows->value(),
                                                   ui->sbxCols->value(),
                                                   fpsImageHandler::Average,
                                                   fpsImageHandler::Left);
        else if (ui->rbtnHoriRight->isChecked())
            m_rects = fpsImageHandler::getSubRects(m_originalImage.width(),
                                                   m_originalImage.height(),
                                                   ui->sbxRows->value(),
                                                   ui->sbxCols->value(),
                                                   fpsImageHandler::Average,
                                                   fpsImageHandler::Right);
        else if (ui->rbtnVertLeft->isChecked())
            m_rects = fpsImageHandler::getSubRects(m_originalImage.width(),
                                                   m_originalImage.height(),
                                                   ui->sbxRows->value(),
                                                   ui->sbxCols->value(),
                                                   fpsImageHandler::Average,
                                                   fpsImageHandler::Left);
        else
            m_rects = fpsImageHandler::getSubRects(m_originalImage.width(),
                                                   m_originalImage.height(),
                                                   ui->sbxRows->value(),
                                                   ui->sbxCols->value(),
                                                   fpsImageHandler::Average,
                                                   fpsImageHandler::Right);
    else if (ui->rbtnHoriLeft->isChecked())
        m_rects = fpsImageHandler::getSubRects(m_originalImage.width(),
                                               m_originalImage.height(),
                                               ui->sbxHeight->value(),
                                               ui->sbxWidth->value(),
                                               fpsImageHandler::Size,
                                               fpsImageHandler::Left);
    else if (ui->rbtnHoriRight->isChecked())
        m_rects = fpsImageHandler::getSubRects(m_originalImage.width(),
                                               m_originalImage.height(),
                                               ui->sbxHeight->value(),
                                               ui->sbxWidth->value(),
                                               fpsImageHandler::Size,
                                               fpsImageHandler::Right);
    else if (ui->rbtnVertLeft->isChecked())
        m_rects = fpsImageHandler::getSubRects(m_originalImage.width(),
                                               m_originalImage.height(),
                                               ui->sbxHeight->value(),
                                               ui->sbxWidth->value(),
                                               fpsImageHandler::Size,
                                               fpsImageHandler::Left);
    else
        m_rects = fpsImageHandler::getSubRects(m_originalImage.width(),
                                               m_originalImage.height(),
                                               ui->sbxHeight->value(),
                                               ui->sbxWidth->value(),
                                               fpsImageHandler::Size,
                                               fpsImageHandler::Right);
    ui->actionSave->setEnabled(true);
    ui->actionSave->setIcon(QIcon(":/toolBar/toolBar/save.png"));
    ui->graphicsView->removeAllFloatingLines();
    fpsImageHandler::rectsToLines(m_rects, ui->graphicsView);
}

void fpsMainWindow::on_rbtnSize_toggled(bool checked)
{
    if (checked)
    {
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
    if (checked)
    {
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
    if (checked)
    {
        ui->sbxCols->setEnabled(false);
        ui->sbxRows->setEnabled(false);
        ui->sbxHeight->setEnabled(false);
        ui->sbxWidth->setEnabled(false);
        ui->btnReset->setEnabled(false);
        ui->rbtnHoriLeft->setEnabled(false);
        ui->rbtnHoriRight->setEnabled(false);
        ui->rbtnVertLeft->setEnabled(false);
        ui->rbtnVertRight->setEnabled(false);
        if (!m_originalImage.isNull())
        {
            ui->actionSave->setEnabled(true);
            ui->actionSave->setIcon(QIcon(":/toolBar/toolBar/save.png"));
        }
    }
}
