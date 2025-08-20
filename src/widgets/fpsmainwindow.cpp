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

#include "fpsmainwindow.h"
#include "ui_fpsmainwindow.h"
#include "fpsbatchdialog.h"
#include "fpssettingsdialog.h"
#include "fpsaboutdialog.h"
#include "fpsprogressdialog.h"
#include "jsonconfigitems.h"
#include "config.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QEvent>
#include <QAction>
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
#include <QPromise>
#include <QtConcurrent/QtConcurrentRun>

#include <QWKWidgets/widgetwindowagent.h>
#include <widgetframe/windowbar.h>
#include <widgetframe/windowbutton.h>

using namespace Qt::Literals::StringLiterals;
using namespace Util;

extern Config appConfig;

fpsMainWindow::fpsMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::fpsMainWindow)
{
    ui->setupUi(this);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);

    installWindowAgent();
}

fpsMainWindow::~fpsMainWindow()
{
    delete ui;
}

void fpsMainWindow::on_actionOpen_triggered()
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
        ui->graphicsView->showPixmap(pixmap);
        if (ui->rbtnManual->isChecked())
            ui->actionSave->setEnabled(true);
    } else
        QMessageBox::warning(this, fpsAppName,
                             tr("Error loading picture file: %1.").arg(m_imgReader.fileName()),
                             QMessageBox::Close);
}

void fpsMainWindow::on_actionSave_triggered()
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

        fpsProgressDialog dlg(this, outputList.size());
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

void fpsMainWindow::on_actionExit_triggered()
{
    close();
}

void fpsMainWindow::on_actionBatch_triggered()
{
    fpsBatchDialog batchDlg(this);
    batchDlg.exec();
}

void fpsMainWindow::on_actionSettings_triggered()
{
    fpsSettingsDialog settingsDlg(this);
    settingsDlg.exec();
}

void fpsMainWindow::on_actionHomepage_triggered()
{
    QDesktopServices::openUrl(QUrl(fpsHomepageUrl, QUrl::TolerantMode));
}

void fpsMainWindow::on_actionAbout_triggered()
{
    fpsAboutDialog aboutDlg(this);
    aboutDlg.exec();
}

void fpsMainWindow::on_btnReset_clicked()
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
    }
    m_rects = ImageHandler::getSubRects(
            m_imgReader.size().width(), m_imgReader.size().height(),
            mode == ImageHandler::SplitMode::Size ? ui->sbxHeight->value() : ui->sbxRows->value(),
            mode == ImageHandler::SplitMode::Size ? ui->sbxWidth->value() : ui->sbxCols->value(),
            mode, sequence);

    ui->actionSave->setEnabled(true);
    ui->graphicsView->removeAllDraggableLines();
    ImageHandler::rectsToLines(m_rects, ui->graphicsView);
}

void fpsMainWindow::on_rbtnSize_toggled(bool checked)
{
    ui->sbxHeight->setEnabled(checked);
    ui->sbxWidth->setEnabled(checked);

    if (checked) {
        ui->gbxSplitSeq->setEnabled(true);
        if (ui->graphicsView->scene())
            ui->btnReset->setEnabled(true);
    }
}

void fpsMainWindow::on_rbtnAver_toggled(bool checked)
{
    ui->sbxCols->setEnabled(checked);
    ui->sbxRows->setEnabled(checked);

    if (checked) {
        ui->gbxSplitSeq->setEnabled(true);
        if (ui->graphicsView->scene())
            ui->btnReset->setEnabled(true);
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
        ui->gbxSplitSeq->setEnabled(false);
        if (ui->graphicsView->scene())
            ui->actionSave->setEnabled(true);
    }
}

void fpsMainWindow::installWindowAgent()
{
    // Setup window agent
    m_windowAgent = new QWK::WidgetWindowAgent(this);
    m_windowAgent->setup(this);

    // Construct window bar
    auto windowBar{ new QWK::WindowBar() };
    auto titleLabel{ new QLabel() };
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName(u"win-title-label"_s);

    auto iconButton{ new QWK::WindowButton() };
    iconButton->setObjectName(u"icon-button"_s);
    iconButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto minButton{ new QWK::WindowButton() };
    minButton->setObjectName(u"min-button"_s);
    minButton->setProperty("system-button", true);
    minButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto maxButton{ new QWK::WindowButton() };
    maxButton->setCheckable(true);
    maxButton->setObjectName(u"max-button"_s);
    maxButton->setProperty("system-button", true);
    maxButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    auto closeButton{ new QWK::WindowButton() };
    closeButton->setObjectName(u"close-button"_s);
    closeButton->setProperty("system-button", true);
    closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    windowBar->setHostWidget(this);
    windowBar->setTitleLabel(titleLabel);
    windowBar->setIconButton(iconButton);
    windowBar->setMinButton(minButton);
    windowBar->setMaxButton(maxButton);
    windowBar->setCloseButton(closeButton);
    m_windowAgent->setTitleBar(windowBar);

    // Set properties
    m_windowAgent->setSystemButton(QWK::WindowAgentBase::WindowIcon, iconButton);
    m_windowAgent->setSystemButton(QWK::WindowAgentBase::Minimize, minButton);
    m_windowAgent->setSystemButton(QWK::WindowAgentBase::Maximize, maxButton);
    m_windowAgent->setSystemButton(QWK::WindowAgentBase::Close, closeButton);
    connect(windowBar, &QWK::WindowBar::minimizeRequested, this, &QWidget::showMinimized);
    connect(windowBar, &QWK::WindowBar::maximizeRequested, this, [this, maxButton](bool max) {
        if (max) {
            showMaximized();
        } else {
            showNormal();
        }
        // It's a Qt issue that if a QAbstractButton::clicked triggers a window's maximization,
        // the button remains to be hovered until the mouse move. As a result, we need to
        // manually send leave events to the button.
        // emulateLeaveEvent(maxButton);
    });
    connect(windowBar, &QWK::WindowBar::closeRequested, this, &QWidget::close);

    setMenuWidget(windowBar);
}

bool fpsMainWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::WindowActivate: {
        auto menu{ menuWidget() };
        menu->setProperty("bar-active", true);
        style()->polish(menu);
        break;
    }

    case QEvent::WindowDeactivate: {
        auto menu{ menuWidget() };
        menu->setProperty("bar-active", false);
        style()->polish(menu);
        break;
    }

    default:
        break;
    }
    return QMainWindow::event(event);
}
