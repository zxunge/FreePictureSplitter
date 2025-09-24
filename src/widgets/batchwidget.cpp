// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge

#include "batchwidget.h"
#include "ui_batchwidget.h"
#include "jsonconfigitems.h"
#include "imagehandler.h"
#include "progressdialog.h"
#include "fileinfo.h"

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
#include <QItemSelectionModel>
#include <QPersistentModelIndex>
#include <QModelIndex>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QFuture>
#include <QFutureWatcher>
#include <QPromise>
#include <QtConcurrent/QtConcurrentRun>
#include <QtAssert>
#include <qobjectdefs.h>

using namespace Qt::Literals::StringLiterals;
using namespace Util;
using namespace Core;

BatchWidget::BatchWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::BatchWidget),
      m_contextMenu(new QMenu(this)),
      m_model(new QStandardItemModel(this)),
      m_selModel(new QItemSelectionModel(m_model, this))
{
    ui->setupUi(this);

    // UI preparations
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
    ui->viewList->setSelectionModel(m_selModel);
    ui->viewTable->setSelectionModel(m_selModel);

    // Load configurations
    ui->cbxLocation->setCurrentIndex(
            appConfig.options.batchOpt.savingTo == Util::SavingTo::specified ? 1 : 0);
    ui->btnOpen->setEnabled(appConfig.options.batchOpt.savingTo == Util::SavingTo::specified);
    ui->lePath->setEnabled(appConfig.options.batchOpt.savingTo == Util::SavingTo::specified);
    ui->lePath->setText(QString::fromStdString(appConfig.options.batchOpt.outPath));
    ui->btnChange->setEnabled(appConfig.options.batchOpt.savingTo == Util::SavingTo::specified);
    ui->chbSubdir->setChecked(appConfig.options.batchOpt.subDir);

    // Signal connections
    connect(m_selModel, &QItemSelectionModel::selectionChanged, this,
            [this](const QItemSelection &selected, const QItemSelection &deselected) {
                ui->actionRemoveFromList->setEnabled(m_selModel->hasSelection());
            });
    connect(ui->viewList, &QListView::clicked, this, [this](const QModelIndex &index) {
        Q_EMIT message(m_model->itemData(index).value(0).toString());
    });
    connect(ui->viewTable, &QTableView::clicked, this, [this](const QModelIndex &index) {
        Q_EMIT message(m_model->itemData(index).value(0).toString());
    });
    connect(ui->viewList, &QWidget::customContextMenuRequested, this,
            [this](const QPoint &pos) { m_contextMenu->exec(QCursor::pos()); });
    connect(ui->viewTable, &QWidget::customContextMenuRequested, this,
            [this](const QPoint &pos) { m_contextMenu->exec(QCursor::pos()); });
    connect(ui->actionRemoveFromList, &QAction::triggered, this, &BatchWidget::removeSelectedItems);
    connect(ui->actionShowThumbnails, &QAction::triggered, this, [this]() {
        ui->stView->setCurrentIndex(0); // pgThumbnail
    });
    connect(ui->actionShowDetailInfo, &QAction::triggered, this, [this]() {
        ui->stView->setCurrentIndex(1); // pgTable
    });
    connect(ui->actionAddPicture, &QAction::triggered, this, &BatchWidget::openPictures);
    connect(ui->actionAddDirectory, &QAction::triggered, this, &BatchWidget::openFolder);
    connect(ui->rbtnAverage, &QRadioButton::toggled, this, [this](bool checked) {
        ui->sbxCols->setEnabled(checked);
        ui->sbxRows->setEnabled(checked);

        if (checked)
            ui->gbxSplitSeq->setEnabled(true);
    });
    connect(ui->rbtnSize, &QRadioButton::toggled, this, [this](bool checked) {
        ui->sbxHeight->setEnabled(checked);
        ui->sbxWidth->setEnabled(checked);

        if (checked)
            ui->gbxSplitSeq->setEnabled(true);
    });
    connect(ui->rbtnTemplate, &QRadioButton::toggled, this, [this](bool checked) {
        ui->cbxTemplate->setEnabled(checked);

        if (checked) {
            ui->sbxCols->setEnabled(false);
            ui->sbxRows->setEnabled(false);
            ui->sbxHeight->setEnabled(false);
            ui->sbxWidth->setEnabled(false);
            ui->gbxSplitSeq->setEnabled(false);
        }
    });
    connect(ui->btnChange, &QPushButton::clicked, this, &BatchWidget::changePath);
    connect(ui->btnOpen, &QPushButton::clicked, this, [this] {
        QDesktopServices::openUrl(QUrl(u"file:"_s + ui->lePath->text(), QUrl::TolerantMode));
    });
    connect(ui->btnSplit, &QPushButton::clicked, this, &BatchWidget::startSplit);
    connect(ui->cbxLocation, &QComboBox::currentIndexChanged, this, [this](int index) {
        if (index == 1) { // "The following path:"
            ui->btnOpen->setEnabled(true);
            ui->lePath->setEnabled(true);
            ui->btnChange->setEnabled(true);
        } else {
            ui->btnOpen->setEnabled(false);
            ui->lePath->setEnabled(false);
            ui->btnChange->setEnabled(false);
        }
    });
}

BatchWidget::~BatchWidget()
{
    delete ui;
}

void BatchWidget::removeSelectedItems()
{
    QList<QPersistentModelIndex> indexes;
    foreach (const QModelIndex &i, m_selModel->selectedIndexes())
        indexes << i;
    foreach (const QPersistentModelIndex &i, indexes)
        m_model->removeRow(i.row());
}

void BatchWidget::openPictures()
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

void BatchWidget::openFolder()
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

void BatchWidget::closeEvent(QCloseEvent *event)
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

void BatchWidget::addPicture(const QString &fileName)
{
    QImageReader reader(fileName);
    QStandardItem *itemName{ new QStandardItem(QIcon(QPixmap::fromImageReader(&reader)),
                                               QFileInfo(fileName).fileName()) };
    QStandardItem *itemPath{ new QStandardItem(fileName) };
    QStandardItem *itemSize{ new QStandardItem(getFileSizeString(fileName)) };
    m_model->appendRow(QList<QStandardItem *>{ itemName, itemPath, itemSize });
}

void BatchWidget::changePath()
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

void BatchWidget::startSplit()
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

    ProgressDialog dlg(m_model->rowCount(), this);
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
