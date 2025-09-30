// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge

#include "singlewidget.h"
#include "ui_singlewidget.h"
#include "progressdialog.h"

#include "core/imagehandler.h"
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
#include <QFuture>
#include <QFutureWatcher>
#include <QImageReader>
#include <QImageWriter>
#include <QGraphicsPixmapItem>
#include <QPromise>
#include <QtConcurrent/QtConcurrentRun>
#include <qaction.h>
#include <qpushbutton.h>
#include <qradiobutton.h>

using namespace Qt::Literals::StringLiterals;
using namespace Util;
using namespace Core;

SingleWidget::SingleWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SingleWidget)
{
    ui->setupUi(this);

    // Signal connections
    connect(ui->actionOpen, &QAction::triggered, this, &SingleWidget::openPicture);
    connect(ui->actionSave, &QAction::triggered, this, &SingleWidget::savePictures);
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
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);

    mimeTypeFilters.sort();

    QFileDialog fdlg;
    fdlg.setWindowTitle(tr("Open a picture..."));
    fdlg.setDirectory(appConfig.dialog.lastOpenedDir.empty()
                              ? QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                              : QString::fromStdString(appConfig.dialog.lastOpenedDir));
    fdlg.setMimeTypeFilters(mimeTypeFilters);
    fdlg.setFileMode(QFileDialog::ExistingFile);
    if (QDialog::Accepted == fdlg.exec() && !fdlg.selectedFiles().isEmpty()
        && QFileInfo(fdlg.selectedFiles().constFirst()).isFile()) {
        m_imgReader.setFileName(fdlg.selectedFiles().constFirst());
        appConfig.dialog.lastOpenedDir =
                QFileInfo(fdlg.selectedFiles().constFirst()).path().toStdString();
    } else
        return;

    m_imgReader.setAutoTransform(true);

    if (m_imgReader.canRead()) {
        QPixmap pixmap{ QPixmap::fromImageReader(&m_imgReader) };
        // We need to reset the file name before the calls to size(),
        // see https://bugreports.qt.io/browse/QTBUG-138530
        m_imgReader.setFileName(m_imgReader.fileName());
        // Display image info on StatusBar; they are: file name, width * height, color depth,
        // vertical DPI, horizontal DPI
        Q_EMIT message(tr("%1, %2x%3, Depth: %4, Vertical: %5 dpi, Horizontal: %6 dpi")
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
        ui->graphicsView->showPixmap(pixmap);
        if (ui->rbtnManual->isChecked())
            ui->actionSave->setEnabled(true);
    } else
        QMessageBox::warning(this, fpsAppName,
                             tr("Error loading picture file: %1.").arg(m_imgReader.fileName()),
                             QMessageBox::Close);
}

void SingleWidget::savePictures()
{
    QVector<QImage> imageList;
    QImageWriter writer;
    QStringList outputList;
    QString baseName{ QFileInfo(m_imgReader.fileName()).baseName() };

    // Check for user's selection: output folder
    QString outBase, out;
    switch (appConfig.options.outputOpt.savingTo) {
    case Util::SavingTo::inPlace:
        outBase = QFileDialog::getExistingDirectory(
                this, tr("Choose the output directory."),
                appConfig.dialog.lastSavedToDir.empty()
                        ? QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                        : QString::fromStdString(appConfig.dialog.lastSavedToDir));
        break;

    case Util::SavingTo::specified:
        outBase = QString::fromStdString(appConfig.options.outputOpt.outPath);
        break;

    case Util::SavingTo::same:
        outBase = QFileInfo(m_imgReader.fileName()).absoluteDir().path();
        break;
    }

    if (outBase.isEmpty())
        return;

    if (!appConfig.options.outputOpt.subDir)
        out = outBase;
    else {
        QDir dir(outBase);
        if (!dir.exists(baseName))
            if (!dir.mkdir(baseName)) {
                QMessageBox::warning(
                        this, fpsAppName,
                        tr("QDir::mkdir \'%1\' error!").arg(dir.absolutePath() + '/' + baseName),
                        QMessageBox::Close);
                return;
            }
        out = outBase + u"/"_s + baseName;
    }

    appConfig.dialog.lastSavedToDir = out.toStdString();

    if (ui->rbtnManual->isChecked())
        m_rects = ImageHandler::linesToRects(ui->graphicsView);
    else if (m_rects.isEmpty()) {
        QMessageBox::warning(this, fpsAppName,
                             tr("Please at least choose one splitting mode, offer "
                                "useful data then reset the splitting lines."),
                             QMessageBox::Close);
        return;
    }

    if (!m_rects.isEmpty()) {
        if (!ImageHandler::split(m_imgReader, imageList, m_rects)) {
            QMessageBox::warning(this, fpsAppName, tr("Error splitting picture."),
                                 QMessageBox::Close);
            return;
        }

        // Grid Figure
        if (appConfig.options.gridOpt.enabled) {
            m_imgReader.setFileName(m_imgReader.fileName());
            QPixmap p{ QPixmap::fromImageReader(&m_imgReader) };
            ImageHandler::drawGridLines(
                    &p, m_rects,
                    QColor::fromString(QString::fromStdString(appConfig.options.gridOpt.colorRgb)),
                    appConfig.options.gridOpt.lineSize);
            imageList.push_back(p.toImage());
        }

        outputList = ImageHandler::getOutputList(
                appConfig.options.nameOpt.prefMode == Util::Prefix::same
                        ? baseName
                        : QString::fromStdString(appConfig.options.nameOpt.prefix),
                QString::fromStdString(appConfig.options.outputOpt.outFormat), m_rects.size(),
                m_rects[0].size(), appConfig.options.nameOpt.rcContained,
                appConfig.options.gridOpt.enabled);

        ProgressDialog dlg(outputList.size(), this);
        QFutureWatcher<void> watcher;
        connect(&watcher, &QFutureWatcher<void>::progressValueChanged, [&](int progressValue) {
            if (progressValue == -1)
                QMessageBox::critical(this, fpsAppName, watcher.progressText(), QMessageBox::Close);
            else
                dlg.proceed(progressValue);
        });
        connect(&watcher, &QFutureWatcher<void>::finished, &dlg, &QDialog::close);
        connect(&dlg, &ProgressDialog::cancelled, &watcher, &QFutureWatcher<void>::cancel);
        watcher.setFuture(QtConcurrent::run([&](QPromise<void> &promise) {
            for (int i{}; i != imageList.size(); ++i) {
                promise.suspendIfRequested();
                if (promise.isCanceled())
                    return;

                writer.setFileName(out + '/' + outputList[i]);
                writer.setFormat(
                        QString::fromStdString(appConfig.options.outputOpt.outFormat).toUtf8());
                writer.setQuality(appConfig.options.outputOpt.jpgQuality);
                if (!writer.write(imageList[i].scaled(
                            imageList[i].width() * appConfig.options.outputOpt.scalingFactor,
                            imageList[i].height() * appConfig.options.outputOpt.scalingFactor,
                            Qt::IgnoreAspectRatio, Qt::SmoothTransformation))) {
                    promise.setProgressValueAndText(
                            -1,
                            tr("Error writing to file \'%1\': %2.")
                                    .arg(writer.fileName(), writer.errorString()));
                    return;
                }
                promise.setProgressValue(i + 1);
            }
        }));
        dlg.exec();
    } else {
        QMessageBox::warning(this, fpsAppName, tr("No rule to split this picture"),
                             QMessageBox::Close);
        return;
    }
}

void SingleWidget::resetSplitLines()
{
    m_imgReader.setFileName(m_imgReader.fileName());

    // Detect splitting sequence and mode
    ImageHandler::SplitMode mode;
    ImageHandler::SplitSequence sequence;
    if (ui->rbtnAver->isChecked()) {
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
        return;

    m_rects = ImageHandler::getSubRects(
            m_imgReader.size().width(), m_imgReader.size().height(),
            mode == ImageHandler::SplitMode::Size ? ui->sbxHeight->value() : ui->sbxRows->value(),
            mode == ImageHandler::SplitMode::Size ? ui->sbxWidth->value() : ui->sbxCols->value(),
            mode, sequence);

    ui->actionSave->setEnabled(true);
    ui->graphicsView->removeAllDraggableLines();
    ImageHandler::rectsToLines(m_rects, ui->graphicsView);
}
