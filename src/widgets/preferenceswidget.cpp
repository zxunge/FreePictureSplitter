// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "preferenceswidget.h"
#include "ui_preferenceswidget.h"
#include "skinoptionsdialog.h"

#include "utils/jsonconfigitems.h"
#include "utils/languagemanager.h"

#include <QImageWriter>
#include <QColorDialog>
#include <QFileInfo>
#include <QFileDialog>
#include <QCompleter>
#include <QFileSystemModel>
#include <QAbstractItemModel>
#include <QLocale>
#include <QVariant>
#include <QStandardPaths>

using namespace Qt::Literals::StringLiterals;

PreferencesWidget::PreferencesWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::PreferencesWidget)
{
    ui->setupUi(this);

    QCompleter *completer{ new QCompleter(this) };
    completer->setModel(new QFileSystemModel(completer));
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lePath->setCompleter(completer);

    // Load configurations
    /****************** Appearance ******************/
    ui->cbxStyle->addItems(Util::ThemeManager::availableSkins());
    ui->cbxStyle->setCurrentText(QString::fromStdString(g_appConfig.app.skin));
    Q_FOREACH (const QString &name, Util::LanguageManager::availableLanguages()) {
        QLocale locale(name);
        QString string{ QStringLiteral("%1 (%2)").arg(
                QLocale::languageToString(locale.language()),
                QLocale::territoryToString(locale.territory())) };
        ui->cbxLang->addItem(string, name);
    }
    ui->cbxLang->model()->sort(0);
    ui->cbxLang->insertItem(0, tr("System default"));
    int languageIndex{ ui->cbxLang->findData(QString::fromStdString(g_appConfig.app.lang)) };
    if (languageIndex == -1)
        languageIndex = 0;
    ui->cbxLang->setCurrentIndex(languageIndex);
    /************************************************/

    /******************** Output ********************/
    // ----- Output File Options -----
    ui->cbxLocation->addItem(tr("The path specified when clicking \"Save pictures\""),
                             QVariant::fromValue(Util::SavingTo::inPlace));
    ui->cbxLocation->addItem(tr("The same location as the source picture"),
                             QVariant::fromValue(Util::SavingTo::same));
    ui->cbxLocation->addItem(tr("The following path:"),
                             QVariant::fromValue(Util::SavingTo::specified));
    ui->cbxLocation->setCurrentIndex(
            ui->cbxLocation->findData(QVariant::fromValue(g_appConfig.options.outputOpt.savingTo)));
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
    ui->labColor->setAutoFillBackground(true);
    ui->labColor->setStyleSheet("background-color: "
                                % QString::fromStdString(g_appConfig.options.gridOpt.colorRgb)
                                % ";");
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
    connect(ui->cbxLang, &QComboBox::currentIndexChanged, this, [&, this](int index) {
        g_appConfig.app.lang = ui->cbxLang->itemData(index).toString().toStdString();
        Util::LanguageManager::instance().installTranslators();
    });
    connect(ui->cbxLocation, &QComboBox::currentIndexChanged, this, [&, this](int index) {
        g_appConfig.options.outputOpt.savingTo =
                ui->cbxLang->itemData(index).value<Util::SavingTo>();
        ui->lePath->setEnabled(g_appConfig.options.outputOpt.savingTo == Util::SavingTo::specified);
        ui->tbtnBrowse->setEnabled(ui->lePath->isEnabled());
    });
    connect(ui->cbxFormats, &QComboBox::currentTextChanged, this, [&, this](const QString &text) {
        ui->sbxQuality->setEnabled(text == u"jpg"_s || text == u"jpeg"_s);
        g_appConfig.options.outputOpt.outFormat = text.toStdString();
    });
    connect(ui->cbxStyle, &QComboBox::currentTextChanged, this, [&](const QString &text) {
        g_appConfig.app.skin = text.toStdString();
        Util::ThemeManager::instance().setAppSkin(text.toStdString());
    });
    connect(ui->lePath, &QLineEdit::textChanged, this, [&](const QString &text) {
        g_appConfig.options.outputOpt.outPath = text.toStdString();
    });
    connect(ui->chbSubDir, &QCheckBox::toggled, this,
            [&](bool checked) { g_appConfig.options.outputOpt.subDir = checked; });
    connect(ui->btnSelectColor, &QPushButton::clicked, this, [&, this]() {
        QColor color{ QColorDialog::getColor(
                QColor::fromString(QString::fromStdString(g_appConfig.options.gridOpt.colorRgb)),
                this, tr("Select a color for grid lines")) };
        if (color.isValid()) {
            g_appConfig.options.gridOpt.colorRgb = color.name().toStdString();
            ui->labColor->setStyleSheet("background-color: " % color.name() % ";");
        }
    });
    connect(ui->sbxQuality, &QSpinBox::valueChanged, this,
            [&](int value) { g_appConfig.options.outputOpt.jpgQuality = value; });
    connect(ui->dsbxFactor, &QDoubleSpinBox::valueChanged, this,
            [&](double value) { g_appConfig.options.outputOpt.scalingFactor = value; });
    connect(ui->rbtnSpecified, &QRadioButton::toggled, this, [&](bool checked) {
        g_appConfig.options.nameOpt.prefMode =
                checked ? Util::Prefix::specified : Util::Prefix::same;
    });
    connect(ui->rbtnSpecified, &QRadioButton::toggled, ui->lePrefix, &QLineEdit::setEnabled);
    connect(ui->chbGrid, &QCheckBox::toggled, ui->gbxGridFigure, &QGroupBox::setEnabled);
    connect(ui->chbGrid, &QCheckBox::toggled, this,
            [&](bool checked) { g_appConfig.options.gridOpt.enabled = checked; });
    connect(ui->sbxLineSize, &QSpinBox::valueChanged, this,
            [&](int value) { g_appConfig.options.gridOpt.lineSize = value; });
    connect(ui->lePrefix, &QLineEdit::textChanged, this,
            [&](const QString &text) { g_appConfig.options.nameOpt.prefix = text.toStdString(); });
    connect(ui->chbNumberContained, &QCheckBox::toggled, this,
            [&](bool checked) { g_appConfig.options.nameOpt.rcContained = checked; });
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
    connect(ui->tbtnAddSkin, &QToolButton::clicked, this, [this] {
        SkinOptionsDialog *dlg{ new SkinOptionsDialog(this) };
        if (dlg->exec() == QDialog::Accepted && !std::get<0>(dlg->skinInfo()).empty()
            && !std::get<1>(dlg->skinInfo()).empty()) {
            ui->cbxStyle->addItem(QString::fromStdString(std::get<0>(dlg->skinInfo())));
            g_appConfig.skinList.push_back(dlg->skinInfo());
        }
    });
}

PreferencesWidget::~PreferencesWidget()
{
    delete ui;
}

void PreferencesWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}
