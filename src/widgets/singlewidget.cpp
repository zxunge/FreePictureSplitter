// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge

#include "singlewidget.h"
#include "ui_singlewidget.h"
#include "progressdialog.h"

#include "core/imagehandler.h"
#include "core/imagedocument.h"
#include "utils/fileinfo.h"
#include "utils/jsonconfigitems.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QUrl>
#include <QFileInfo>
#include <QColor>
#include <QStyle>
#include <QDir>
#include <QApplication>
#include <QGraphicsPixmapItem>

using namespace Qt::Literals::StringLiterals;
using namespace Util;
using namespace Core;

SingleWidget::SingleWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::SingleWidget), m_imgDoc(new ImageDocument)
{
    ui->setupUi(this);

    m_imgDoc->moveToThread(&m_imgThread);

    // Signal connections
    connect(ui->actionOpen, &QAction::triggered, this, &SingleWidget::openPicture);
    connect(ui->actionSave, &QAction::triggered, this, &SingleWidget::savePictures);
    connect(ui->actionClosePicture, &QAction::triggered, this, &SingleWidget::closePicture);
    connect(ui->btnReset, &QPushButton::clicked, this, &SingleWidget::resetSplitLines);
    connect(ui->actionZoomIn, &QAction::triggered, this, [this] { ui->graphicsView->zoomIn(); });
    connect(ui->actionZoomOut, &QAction::triggered, this, [this] { ui->graphicsView->zoomOut(); });
    connect(ui->rbtnAver, &QRadioButton::toggled, this, [this](bool checked) {
        ui->sbxCols->setEnabled(checked);
        ui->sbxRows->setEnabled(checked);

        if (checked) {
            ui->gbxSplitSeq->setEnabled(true);
            if (ui->graphicsView->scene())
                ui->btnReset->setEnabled(true);
        }
    });
    connect(ui->rbtnSize, &QRadioButton::toggled, this, [this](bool checked) {
        ui->sbxHeight->setEnabled(checked);
        ui->sbxWidth->setEnabled(checked);

        if (checked) {
            ui->gbxSplitSeq->setEnabled(true);
            if (ui->graphicsView->scene())
                ui->btnReset->setEnabled(true);
        }
    });
    connect(ui->rbtnManual, &QRadioButton::toggled, this, [this](bool checked) {
        if (checked) {
            ui->sbxCols->setEnabled(false);
            ui->sbxRows->setEnabled(false);
            ui->sbxHeight->setEnabled(false);
            ui->sbxWidth->setEnabled(false);
            ui->btnReset->setEnabled(false);
            ui->gbxSplitSeq->setEnabled(false);
            if (ui->graphicsView->scene())
                ui->actionSave->setEnabled(true);
        }
    });
}

SingleWidget::~SingleWidget()
{
    delete ui;
}

void SingleWidget::openPicture()
{
    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes{ QImageReader::supportedMimeTypes() };
    Q_FOREACH (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);

    mimeTypeFilters.sort();

    QFileDialog fdlg;
    fdlg.setWindowTitle(tr("Open a picture..."));
    fdlg.setDirectory(appConfig.dialog.lastOpenedDir.empty()
                              ? QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                              : QString::fromStdString(appConfig.dialog.lastOpenedDir));
    fdlg.setMimeTypeFilters(mimeTypesToFilters(QImageReader::supportedMimeTypes()));
    fdlg.setFileMode(QFileDialog::ExistingFile);
    if (QDialog::Accepted == fdlg.exec() && !fdlg.selectedFiles().isEmpty()
        && QFileInfo(fdlg.selectedFiles().constFirst()).isFile()) {
        m_imgDoc->openImageFile(fdlg.selectedFiles().constFirst());
        appConfig.dialog.lastOpenedDir =
                QFileInfo(fdlg.selectedFiles().constFirst()).path().toStdString();
    } else
        return;

    if (m_imgDoc->canRead()) {
        QPixmap pixmap{ QPixmap::fromImage(m_imgDoc->toImage()) };
        int width{ m_imgDoc->size().width() }, height{ m_imgDoc->size().height() };
        // Display image info on StatusBar; they are: file name, width * height, color depth,
        // vertical DPI, horizontal DPI
        Q_EMIT message(
                tr("%1, Width: %2, Height: %3, Depth: %4, Vertical: %5 dpi, Horizontal: %6 dpi")
                        .arg(m_imgDoc->fullName())
                        .arg(width)
                        .arg(height)
                        .arg(pixmap.depth())
                        .arg(pixmap.logicalDpiY())
                        .arg(pixmap.logicalDpiX()));
        ui->btnReset->setEnabled(true);
        ui->actionZoomIn->setEnabled(true);
        ui->actionZoomOut->setEnabled(true);
        ui->sbxCols->setRange(1, width);
        ui->sbxRows->setRange(1, height);
        ui->sbxHeight->setRange(1, height);
        ui->sbxWidth->setRange(1, width);
        ui->graphicsView->showPixmap(pixmap);
        if (ui->rbtnManual->isChecked())
            ui->actionSave->setEnabled(true);
    } else
        QMessageBox::warning(this, fpsAppName,
                             tr("Error loading picture file: %1.").arg(m_imgDoc->fullName()),
                             QMessageBox::Close);
}

void SingleWidget::savePictures()
{
    QString baseName{ m_imgDoc->baseName() };

    // Check for user's selection: output folder
    QString basePath, finalPath;
    switch (appConfig.options.outputOpt.savingTo) {
    case Util::SavingTo::inPlace:
        basePath = QFileDialog::getExistingDirectory(
                this, tr("Choose the output directory."),
                appConfig.dialog.lastSavedToDir.empty()
                        ? QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                        : QString::fromStdString(appConfig.dialog.lastSavedToDir));
        break;

    case Util::SavingTo::specified:
        basePath = QString::fromStdString(appConfig.options.outputOpt.outPath);
        break;

    case Util::SavingTo::same:
        basePath = m_imgDoc->parentPath();
        break;
    }

    if (basePath.isEmpty())
        return;

    if (!appConfig.options.outputOpt.subDir)
        finalPath = basePath;
    else {
        QDir dir(basePath);
        if (!dir.exists(baseName))
            if (!dir.mkdir(baseName)) {
                QMessageBox::warning(
                        this, fpsAppName,
                        tr("QDir::mkdir \'%1\' error!").arg(dir.absolutePath() + '/' + baseName),
                        QMessageBox::Close);
                return;
            }
        finalPath = basePath + u"/"_s + baseName;
    }

    appConfig.dialog.lastSavedToDir = finalPath.toStdString();

    if (ui->rbtnManual->isChecked())
        m_imgDoc->applyLinesFrom(ui->graphicsView);
    else if (!m_imgDoc->isValid()) {
        QMessageBox::warning(this, fpsAppName,
                             tr("Please at least choose one splitting mode, offer "
                                "useful data then reset the splitting lines."),
                             QMessageBox::Close);
        return;
    }

    if (m_imgDoc->isValid()) {
        m_imgDoc->option().setGridEnabled(appConfig.options.gridOpt.enabled);
        m_imgDoc->writer().setFormat(
                QString::fromStdString(appConfig.options.outputOpt.outFormat).toUtf8());
        m_imgDoc->writer().setQuality(appConfig.options.outputOpt.jpgQuality);
        ProgressDialog dlg(m_imgDoc->totalCount(), this);
        connect(m_imgDoc, &ImageDocument::imageSaved, &dlg,
                static_cast<void (ProgressDialog::*)()>(&ProgressDialog::proceed));
        connect(&m_imgThread, &QThread::finished, &dlg, &QDialog::close);
        m_imgThread.start();
        dlg.exec();
    } else {
        QMessageBox::warning(this, fpsAppName, tr("No rule to split this picture"),
                             QMessageBox::Close);
        return;
    }
}

void SingleWidget::resetSplitLines()
{
    // Detect splitting sequence and mode
    if (ui->rbtnHoriLeft->isChecked())
        m_imgDoc->option().setSequence(ImageOption::LeftToRight | ImageOption::UpToDown);
    else if (ui->rbtnHoriRight->isChecked())
        m_imgDoc->option().setSequence(ImageOption::RightToLeft | ImageOption::UpToDown);
    else if (ui->rbtnVertLeft->isChecked())
        m_imgDoc->option().setSequence(ImageOption::LeftToRight | ImageOption::DownToUp);
    else
        m_imgDoc->option().setSequence(ImageOption::RightToLeft | ImageOption::DownToUp);

    if (ui->rbtnAver->isChecked())
        m_imgDoc->option().setAverage(ui->sbxRows->value(), ui->sbxCols->value());
    else if (ui->rbtnSize->isChecked())
        m_imgDoc->option().setSize(QSize(ui->sbxWidth->value(), ui->sbxHeight->value()));
    else
        return;

    ui->actionSave->setEnabled(true);
    ui->graphicsView->removeAllDraggableLines();
    m_imgDoc->setupSplitLines();
    m_imgDoc->drawLinesTo(ui->graphicsView);
}

void SingleWidget::closePicture()
{
    ui->graphicsView->clearScene();
    m_imgDoc->close();
    ui->graphicsView->setRulersVisibility(false);
    ui->actionClosePicture->setEnabled(false);
}
