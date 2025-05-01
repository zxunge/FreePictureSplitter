//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsmainwindow.h"
#include "qimage.h"
#include "qlist.h"
#include "qobject.h"
#include "qvector.h"
#include "src/fpsbatchdialog.h"
#include "src/fpsimagehandler.h"
#include "ui_fpsmainwindow.h"
#include "fpsbatchdialog.h"
#include "fpsimagehandler.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QTextBrowser>
#include <QStringLiteral>
#include <QHashFunctions>

fpsMainWindow::fpsMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::fpsMainWindow)
{
    ui->setupUi(this);
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

fpsMainWindow::~fpsMainWindow()
{
    delete ui;
}

void fpsMainWindow::on_actionOpen_triggered()
{
    m_fileName = QFileDialog::getOpenFileName(this, tr("Open a picture..."), QStringLiteral("/"), tr("Image files (*.jpg *.png *.jpeg *.gif *.bmp)"));

    if(m_originalImage.load(m_fileName))
    {
        QPixmap pixmap { QPixmap::fromImage(m_originalImage.scaled(ui->graphicsView->width() - 10, ui->graphicsView->height() - 10, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)) };
        QGraphicsScene *scene { new QGraphicsScene };
        scene->addPixmap(pixmap);
        ui->graphicsView->setScene(scene);
        ui->graphicsView->show();
        ui->statusBar->showMessage(m_fileName);
        ui->btnReset->setEnabled(true);
    }
    else
        QMessageBox::warning(this, QStringLiteral("FreePictureSplitter"),
                             QString("Error loading picture file: %1.").arg(m_fileName),
                             QMessageBox::Close);
}


void fpsMainWindow::on_actionSave_triggered()
{
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
        for (int i {}; i != imageList.size(); ++i)
            imageList[i].save(out + "/" + outputList[i]);
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
    QDesktopServices::openUrl(QUrl("https://github.com/zxunge/FreePictureSplitter", QUrl::TolerantMode));
}


void fpsMainWindow::on_actionAbout_triggered()
{
    QTextBrowser *textBrowser { new QTextBrowser() };
    QFile file(":/other/about.html");

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        textBrowser->setHtml(file.readAll());
    textBrowser->setFixedHeight(450);
    textBrowser->setFixedWidth(600);
    textBrowser->show();
}


void fpsMainWindow::on_btnReset_clicked()
{
    if (ui->rbtnAver->isChecked())
        m_rects = fpsImageHandler::getSubRects(m_originalImage.width(),
                                               m_originalImage.height(),
                                               ui->sbxRows->value(),
                                               ui->sbxCols->value());
    else
        m_rects = fpsImageHandler::getSubRects(m_originalImage.width(),
                                               m_originalImage.height(),
                                               ui->sbxHeight->value(),
                                               ui->sbxWidth->value(),
                                               fpsImageHandler::Size);
}

