// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "batchwidget.h"
#include "globaldefs.h"
#include "ui_batchwidget.h"
#include "progressdialog.h"
#include "errorlogdialog.h"

#include "utils/jsonconfigitems.h"
#include "utils/fileinfo.h"
#include "core/imagedocument.h"

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
            g_appConfig.options.batchOpt.savingTo == Util::SavingTo::specified ? 1 : 0);
    ui->btnOpen->setEnabled(g_appConfig.options.batchOpt.savingTo == Util::SavingTo::specified);
    ui->lePath->setEnabled(g_appConfig.options.batchOpt.savingTo == Util::SavingTo::specified);
    ui->lePath->setText(QString::fromStdString(g_appConfig.options.batchOpt.outPath));
    ui->btnChange->setEnabled(g_appConfig.options.batchOpt.savingTo == Util::SavingTo::specified);
    ui->chbSubdir->setChecked(g_appConfig.options.batchOpt.subDir);

    // Signal connections
    connect(m_selModel, &QItemSelectionModel::selectionChanged, this,
            [this](const QItemSelection &selected, const QItemSelection &deselected) {
                ui->actionRemoveFromList->setEnabled(m_selModel->hasSelection());
            });
    connect(ui->viewList, &QListView::clicked, this, [this](const QModelIndex &index) {
        g_mainWnd->statusBar()->showMessage(m_model->itemData(index).value(0).toString());
    });
    connect(ui->viewTable, &QTableView::clicked, this, [this](const QModelIndex &index) {
        g_mainWnd->statusBar()->showMessage(m_model->itemData(index).value(0).toString());
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
    Q_FOREACH (const QModelIndex &i, m_selModel->selectedIndexes())
        indexes << i;
    Q_FOREACH (const QPersistentModelIndex &i, indexes)
        m_model->removeRow(i.row());
}

void BatchWidget::openPictures()
{
    QFileDialog fdlg;
    fdlg.setWindowTitle(tr("Add pictures..."));
    fdlg.setDirectory(g_appConfig.dialog.lastOpenedDir.empty()
                              ? QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                              : QString::fromStdString(g_appConfig.dialog.lastOpenedDir));
    fdlg.setNameFilters(mimeTypesToNameFilters(QImageReader::supportedMimeTypes()));
    fdlg.setFileMode(QFileDialog::ExistingFiles);
    if (QDialog::Accepted == fdlg.exec() && !fdlg.selectedFiles().isEmpty()
        && QFileInfo(fdlg.selectedFiles().constFirst()).isFile()) {
        g_appConfig.dialog.lastOpenedDir =
                QFileInfo(fdlg.selectedFiles().constFirst()).path().toStdString();

        const QStringList list{ fdlg.selectedFiles() };
        // m_pbLoading->setRange(0, list.size());
        // m_pbLoading->setVisible(true);
        int count{};
        Q_FOREACH (const auto file, list) {
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
            g_appConfig.dialog.lastOpenedDir.empty()
                    ? QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                    : QString::fromStdString(g_appConfig.dialog.lastOpenedDir)) };
    if (in.isEmpty())
        return;
    else
        g_appConfig.dialog.lastOpenedDir = in.toStdString();

    QDir dir(in);
    QStringList nameFilters;
    const QByteArrayList supportedFormats{ QImageReader::supportedImageFormats() };
    Q_FOREACH (const auto format, supportedFormats)
        nameFilters << u"*."_s + QString(format);

    const QStringList list{ dir.entryList(nameFilters, QDir::Files) };
    int count{};
    // m_pbLoading->setRange(0, list.size());
    // m_pbLoading->setVisible(true);
    Q_FOREACH (const auto file, list) {
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
        g_appConfig.options.batchOpt.savingTo = Util::SavingTo::same;
    else
        g_appConfig.options.batchOpt.savingTo = Util::SavingTo::specified;
    g_appConfig.options.batchOpt.outPath = ui->lePath->text().toStdString();
    g_appConfig.options.batchOpt.subDir = ui->chbSubdir->isChecked();

    QWidget::closeEvent(event);
}

void BatchWidget::addPicture(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setScaledSize(QSize(80, 80));
    QStandardItem *itemName{ new QStandardItem(QIcon(QPixmap::fromImageReader(&reader)),
                                               QFileInfo(fileName).fileName()) };
    QStandardItem *itemPath{ new QStandardItem(fileName) };
    QStandardItem *itemSize{ new QStandardItem(fileSizeString(fileName)) };
    m_model->appendRow(QList<QStandardItem *>{ itemName, itemPath, itemSize });
}

void BatchWidget::changePath()
{
    QString in{ QFileDialog::getExistingDirectory(
            this, tr("Choose a directory to save pictures."),
            g_appConfig.dialog.lastSavedToDir.empty()
                    ? QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                    : QString::fromStdString(g_appConfig.dialog.lastSavedToDir)) };
    if (in.isEmpty())
        return;

    g_appConfig.dialog.lastSavedToDir = in.toStdString();
    ui->lePath->setText(in);
}

void BatchWidget::startSplit()
{
    ImageDocument imgDoc;
    // Detect splitting sequence and mode
    if (ui->rbtnLRTB->isChecked())
        imgDoc.option().setSequence(ImageOption::LeftToRight | ImageOption::UpToDown);
    else if (ui->rbtnRLTB->isChecked())
        imgDoc.option().setSequence(ImageOption::RightToLeft | ImageOption::UpToDown);
    else if (ui->rbtnLRBT->isChecked())
        imgDoc.option().setSequence(ImageOption::LeftToRight | ImageOption::DownToUp);
    else if (ui->rbtnRLBT->isChecked())
        imgDoc.option().setSequence(ImageOption::RightToLeft | ImageOption::DownToUp);

    if (ui->rbtnAverage->isChecked())
        imgDoc.option().setAverage(ui->sbxRows->value(), ui->sbxCols->value());
    else if (ui->rbtnSize->isChecked())
        imgDoc.option().setSize(QSize(ui->sbxWidth->value(), ui->sbxHeight->value()));
    else
        return;

    ProgressDialog dlg(m_model->rowCount(), this);
    dlg.show();
    dlg.raise();

    for (qsizetype i{}; i != m_model->rowCount(); ++i) {
        imgDoc.openImageFile(m_model->item(i, 1)->text());
        if (!imgDoc.canRead()) {
            QMessageBox::critical(this, tr("Batch Splitting"),
                                  tr("Cannot open file: %1.").arg(imgDoc.filePath()),
                                  QMessageBox::Close);
            return;
        }
        imgDoc.setupSplitLines();

        // Get the output directory
        QDir baseDir;
        if (ui->cbxLocation->currentIndex() == 0) // "The same"
            baseDir = imgDoc.parentPath();
        else {
            if (!ui->lePath->text().isEmpty())
                baseDir = ui->lePath->text();
            else {
                QMessageBox::warning(this, tr("Batch Splitting"),
                                     tr("You have not specified the output directory yet, "
                                        "please try again."),
                                     QMessageBox::Close);
                return;
            }
        }
        QString finalPath;
        if (ui->chbSubdir->isChecked()) {
            if (!QFileInfo::exists(baseDir.absolutePath() % '/' % imgDoc.baseName() % '/'))
                if (!baseDir.mkdir(imgDoc.baseName())) {
                    QMessageBox::warning(
                            this, tr("Batch Splitting"),
                            tr("QDir::mkdir \'%1\' error!")
                                    .arg(baseDir.absolutePath() % '/' % imgDoc.baseName()),
                            QMessageBox::Close);
                    break;
                }
            finalPath = baseDir.absolutePath() % '/' % imgDoc.baseName();
        } else
            finalPath = baseDir.absolutePath();

        if (!imgDoc.isValid()) {
            QMessageBox::warning(this, tr("Batch Splitting"), tr("No rule to split the picture"),
                                 QMessageBox::Close);
            return;
        }
        imgDoc.option().setGridEnabled(g_appConfig.options.gridOpt.enabled,
                                       QColor::fromString(g_appConfig.options.gridOpt.colorRgb),
                                       g_appConfig.options.gridOpt.lineSize);
        imgDoc.writerOption().setFormat(
                QString::fromStdString(g_appConfig.options.outputOpt.outFormat).toUtf8());
        imgDoc.writerOption().setQuality(g_appConfig.options.outputOpt.jpgQuality);
        imgDoc.option().setSavePrefix(g_appConfig.options.nameOpt.prefix);
        imgDoc.option().setRowColContained(g_appConfig.options.nameOpt.rcContained);
        imgDoc.setOutputPath(finalPath);

        if (auto result = imgDoc.saveImages(); result.has_value()) {
            result.value().waitForFinished();

            if (!result.value().result().empty()) {
                // Show errors
                ErrorLogDialog *dlgErr{ new ErrorLogDialog(this) };
                Q_FOREACH (auto &result, result.value().result()) {
                    if (!result.has_value())
                        dlgErr->addErrorInfo(
                                { std::make_tuple(imgDoc.filePath(), result.error()) });
                }
                dlgErr->exec();
            }

            QCoreApplication::processEvents();
            dlg.proceed();
        } else {
            QMessageBox::warning(this, tr("Batch Splitting"), result.error(), QMessageBox::Close);
            return;
        }
    }
    dlg.close();
}
