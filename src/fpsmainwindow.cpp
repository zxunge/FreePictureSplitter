//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) 2024 2025 zxunge
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "config.h"
#include "fpsmainwindow.h"
#include "ui_fpsmainwindow.h"
#include "fpsbatchdialog.h"
#include "fpsimagehandler.h"
#include "fpsaboutdialog.h"
#include "fpsfloatingline.h"
#include "fpsrulerbar.h"
#include "fpsprogressdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QTextBrowser>
#include <QStringLiteral>

fpsMainWindow::fpsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fpsMainWindow)
{
    ui->setupUi(this);
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->labPicture->lower();
    m_hruler = new fpsRulerBar(ui->wgtHRuler, Qt::Horizontal);
    m_vruler = new fpsRulerBar(ui->wgtVRuler, Qt::Vertical);
    m_hruler->setRange(0, 1000, 1000);
    m_hruler->update();
}

fpsMainWindow::~fpsMainWindow()
{
    delete ui;
}

void fpsMainWindow::on_actionOpen_triggered()
{
    m_fileName = QFileDialog::getOpenFileName(this, tr("Open a picture..."), QStringLiteral("/"), tr("Image files (*.jpg *.png *.jpeg *.gif *.bmp)"));

    if (m_fileName.isEmpty())
        return;

    if(m_originalImage.load(m_fileName))
    {
        ui->labPicture->setPixmap(QPixmap::fromImage(m_originalImage));
        ui->labPicture->setScaledContents(true);
        ui->statusBar->showMessage(m_fileName);
        ui->btnReset->setEnabled(true);
        ui->sbxCols->setRange(1, m_originalImage.width());
        ui->sbxRows->setRange(1, m_originalImage.height());
        ui->sbxHeight->setRange(1, m_originalImage.height());
        ui->sbxWidth->setRange(1, m_originalImage.width());
    }
    else
        QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                             QString("Error loading picture file: %1.").arg(m_fileName),
                             QMessageBox::Close);
}


void fpsMainWindow::on_actionSave_triggered()
{
    if (m_originalImage.isNull() || m_rects.isEmpty())
        return;

    QVector<QImage> imageList;
    QStringList outputList;
    QString out = QFileDialog::getExistingDirectory(this, tr("Choose the output directory."));
    if (!fpsImageHandler::split(m_originalImage, imageList, m_rects))
    {
        QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                             QString("Error splitting picture."),
                             QMessageBox::Close);
        return;
    }
    else
    {
        outputList = fpsImageHandler::getOutputList(m_fileName, m_rects.size(), m_rects[0].size());
        fpsProgressDialog *dlg { new fpsProgressDialog(this, outputList.size()) };
        connect(this, SIGNAL(proceed(int)), dlg, SLOT(on_proceed(int)));
        dlg->show();
        for (int i {}; i != imageList.size(); ++i)
        {
            imageList[i].save(out + "/" + outputList[i]);
            Q_EMIT proceed(i + 1);
        }
        dlg->close();
    }

}


void fpsMainWindow::on_actionExit_triggered()
{
    close();
}


void fpsMainWindow::on_actionBatch_triggered()
{
    fpsBatchDialog *dlg { new fpsBatchDialog(this) };
    dlg->exec();
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
    fpsAboutDialog *dlg { new fpsAboutDialog(this) };
    dlg->exec();
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
    else
        if (ui->rbtnHoriLeft->isChecked())
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
    fpsFloatingLine *line { new fpsFloatingLine(ui->wgtShow, Qt::Horizontal) };
    line->show();
}


void fpsMainWindow::on_rbtnSize_toggled(bool checked)
{
    if (checked)
    {
        ui->sbxCols->setEnabled(false);
        ui->sbxRows->setEnabled(false);
        ui->sbxHeight->setEnabled(true);
        ui->sbxWidth->setEnabled(true);
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
    }
}

