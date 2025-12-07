// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge

#include "preferenceswidget.h"
#include "ui_preferenceswidget.h"

#include "utils/jsonconfigitems.h"
#include "utils/skins.h"

#include <QImageWriter>
#include <QColorDialog>
#include <QFileInfo>
#include <QFileDialog>
#include <QCompleter>
#include <QFileSystemModel>
#include <QStandardPaths>
#include <QtConcurrent/QtConcurrentRun>

using namespace Qt::Literals::StringLiterals;

PreferencesWidget::PreferencesWidget(QMainWindow *parent)
    : QWidget(parent), ui(new Ui::PreferencesWidget)
{
    ui->setupUi(this);

    QCompleter *completer{ new QCompleter(this) };
    completer->setModel(new QFileSystemModel(completer));
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lePath->setCompleter(completer);

    // Load configurations
    /****************** Appearance ******************/
    ui->cbxStyle->addItems(Util::availableSkins());
    ui->cbxStyle->setCurrentText(QString::fromStdString(g_appConfig.app.style));
    // `Language' is now fixed
    ui->cbxLang->addItem(tr("System"));
    ui->cbxLang->setCurrentIndex(0);
    /************************************************/

    /******************** Output ********************/
    // ----- Output File Options -----
    switch (g_appConfig.options.outputOpt.savingTo) {
    case Util::SavingTo::inPlace:
        ui->cbxLocation->setCurrentIndex(0);
        ui->lePath->setEnabled(false);
        ui->tbtnBrowse->setEnabled(false);
        break;

    case Util::SavingTo::same:
        ui->cbxLocation->setCurrentIndex(1);
        ui->lePath->setEnabled(false);
        ui->tbtnBrowse->setEnabled(false);
        break;

    case Util::SavingTo::specified:
        ui->cbxLocation->setCurrentIndex(2);
        ui->lePath->setEnabled(true);
        ui->tbtnBrowse->setEnabled(true);
        break;
    }
    ui->chbSubDir->setChecked(g_appConfig.options.outputOpt.subDir);
    ui->lePath->setText(QString::fromStdString(g_appConfig.options.outputOpt.outPath));
    const QByteArrayList supportedImageFormats{ QImageWriter::supportedImageFormats() };
    Q_FOREACH (const QByteArray &imageFormat, supportedImageFormats) {
        ui->cbxFormats->addItem(QString(imageFormat));
    }
    // TODO@25/07/01 Add corrupted config item handling.
    ui->cbxFormats->setCurrentIndex(ui->cbxFormats->findText(
            QString::fromStdString(g_appConfig.options.outputOpt.outFormat)));
    ui->sbxQuality->setEnabled(g_appConfig.options.outputOpt.outFormat == "jpg"
                               || g_appConfig.options.outputOpt.outFormat == "jpeg");
    ui->sbxQuality->setValue(g_appConfig.options.outputOpt.jpgQuality);
    ui->dsbxFactor->setValue(g_appConfig.options.outputOpt.scalingFactor * 100.0);

    // ----- Grid Figure -----
    ui->frColor->setAutoFillBackground(true);
    m_color = QColor::fromString(QString::fromStdString(g_appConfig.options.gridOpt.colorRgb));
    ui->frColor->setPalette(QPalette(m_color));
    ui->sbxLineSize->setValue(g_appConfig.options.gridOpt.lineSize);
    ui->chbGrid->setChecked(g_appConfig.options.gridOpt.enabled);
    ui->gbxGridFigure->setEnabled(g_appConfig.options.gridOpt.enabled);

    // ----- Naming Conventions -----
    ui->rbtnSpecified->setChecked(g_appConfig.options.nameOpt.prefMode == Util::Prefix::specified);
    ui->lePrefix->setEnabled(g_appConfig.options.nameOpt.prefMode == Util::Prefix::specified);
    ui->lePrefix->setText(QString::fromStdString(g_appConfig.options.nameOpt.prefix));
    ui->rbtnOriName->setChecked(g_appConfig.options.nameOpt.prefMode == Util::Prefix::same);
    ui->chbNumberContained->setChecked(g_appConfig.options.nameOpt.rcContained);
    /************************************************/

    // Signal connections
    connect(ui->cbxLocation, &QComboBox::currentIndexChanged, this, [this](int index) {
        // 2 : "The following path:"
        ui->lePath->setEnabled(index == 2);
        ui->tbtnBrowse->setEnabled(index == 2);
    });
    connect(ui->cbxFormats, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        ui->sbxQuality->setEnabled(text == u"jpg"_s || text == u"jpeg"_s);
    });
    connect(ui->btnSelectColor, &QPushButton::clicked, this, [this]() {
        QColor color{ QColorDialog::getColor(m_color, this, tr("Select a color for grid lines")) };
        if (color.isValid()) {
            m_color = color;
            ui->frColor->setPalette(QPalette(color));
        }
    });
    connect(ui->rbtnSpecified, &QRadioButton::toggled, this,
            [this](bool checked) { ui->lePrefix->setEnabled(checked); });
    connect(ui->chbGrid, &QCheckBox::toggled, this,
            [this](bool checked) { ui->gbxGridFigure->setEnabled(checked); });
    connect(ui->tbtnAppearance, &QToolButton::toggled, this, [this](bool checked) {
        if (checked)
            ui->wgtOptions->setCurrentIndex(0); // "Appearance"
    });
    connect(ui->tbtnOutput, &QToolButton::toggled, this, [this](bool checked) {
        if (checked)
            ui->wgtOptions->setCurrentIndex(1); // "Output"
    });
    connect(ui->tbtnBrowse, &QToolButton::clicked, this, [&, this]() {
        QString in{ QFileDialog::getExistingDirectory(
                this, tr("Choose a directory to save pictures."),
                g_appConfig.dialog.lastSavedToDir.empty()
                        ? QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                        : QString::fromStdString(g_appConfig.dialog.lastSavedToDir)) };
        if (in.isEmpty())
            return;

        g_appConfig.dialog.lastSavedToDir = in.toStdString();
        g_appConfig.options.outputOpt.outPath = in.toStdString();
        ui->lePath->setText(in);
    });
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, [&, this](QAbstractButton *button) {
        if (ui->buttonBox->buttonRole(button) == QDialogButtonBox::ApplyRole) {
            saveChanges();
            // Change theme
            Util::setAppSkin(qApp, QString::fromStdString(g_appConfig.app.style));
        }
    });
}

PreferencesWidget::~PreferencesWidget()
{
    delete ui;
}

void PreferencesWidget::saveChanges()
{
    // Save configurations
    g_appConfig.app.style = ui->cbxStyle->currentText().toStdString();
    switch (ui->cbxLocation->currentIndex()) {
    case 0:
        g_appConfig.options.outputOpt.savingTo = Util::SavingTo::inPlace;
        break;

    case 1:
        g_appConfig.options.outputOpt.savingTo = Util::SavingTo::same;
        break;

    case 2:
        g_appConfig.options.outputOpt.savingTo = Util::SavingTo::specified;
        break;
    }
    g_appConfig.options.outputOpt.subDir = ui->chbSubDir->isChecked();
    g_appConfig.options.outputOpt.outPath = ui->lePath->text().toStdString();
    g_appConfig.options.outputOpt.outFormat = ui->cbxFormats->currentText().toStdString();
    g_appConfig.options.outputOpt.jpgQuality = ui->sbxQuality->value();
    g_appConfig.options.outputOpt.scalingFactor = ui->dsbxFactor->value() / 100.0;
    g_appConfig.options.gridOpt.enabled = ui->chbGrid->isChecked();
    g_appConfig.options.gridOpt.lineSize = ui->sbxLineSize->value();
    g_appConfig.options.gridOpt.colorRgb = m_color.name().toStdString();
    g_appConfig.options.nameOpt.prefMode =
            ui->rbtnSpecified->isChecked() ? Util::Prefix::specified : Util::Prefix::same;
    g_appConfig.options.nameOpt.prefix = ui->lePrefix->text().toStdString();
    g_appConfig.options.nameOpt.rcContained = ui->chbNumberContained->isChecked();
}
