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

#include "fpssettingsdialog.h"
#include "ui_fpssettingsdialog.h"
#include "jsonconfigitems.h"
#include "skins.h"

#include <QImageWriter>
#include <QColorDialog>
#include <QFileInfo>
#include <QFileDialog>

using namespace Qt::Literals::StringLiterals;

extern Util::Config appConfig;

fpsSettingsDialog::fpsSettingsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::fpsSettingsDialog)
{
    ui->setupUi(this);

    // Load configurations
    /****************** Appearance ******************/
    ui->cbxStyle->addItems(Util::getAvailableSkins());
    ui->cbxStyle->setCurrentText(QString::fromStdString(appConfig.app.style));
    // `Language' is now fixed
    ui->cbxLang->addItem(tr("System"));
    ui->cbxLang->setCurrentIndex(0);
    /************************************************/

    /******************** Output ********************/
    // ----- Output File Options -----
    switch (appConfig.options.outputOpt.savingTo) {
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
    ui->chbSubDir->setChecked(appConfig.options.outputOpt.subDir);
    ui->lePath->setText(QString::fromStdString(appConfig.options.outputOpt.outPath));
    const QByteArrayList supportedImageFormats{ QImageWriter::supportedImageFormats() };
    foreach (const QByteArray &imageFormat, supportedImageFormats) {
        ui->cbxFormats->addItem(QString(imageFormat));
    }
    // TODO@25/07/01 Add corrupted config item handling.
    ui->cbxFormats->setCurrentIndex(ui->cbxFormats->findText(
            QString::fromStdString(appConfig.options.outputOpt.outFormat)));
    ui->sbxQuality->setValue(appConfig.options.outputOpt.jpgQuality);
    ui->dsbxFactor->setValue(appConfig.options.outputOpt.scalingFactor * 100.0);

    // ----- Grid Figure -----
    ui->frColor->setAutoFillBackground(true);
    m_color = QColor::fromString(QString::fromStdString(appConfig.options.gridOpt.colorRgb));
    ui->frColor->setPalette(QPalette(m_color));
    ui->sbxLineSize->setValue(appConfig.options.gridOpt.lineSize);
    ui->chbGrid->setChecked(appConfig.options.gridOpt.enabled);
    ui->frColor->setVisible(appConfig.options.gridOpt.enabled);
    ui->btnSelectColor->setEnabled(appConfig.options.gridOpt.enabled);
    ui->sbxLineSize->setEnabled(appConfig.options.gridOpt.enabled);

    // ----- Naming Conventions -----
    ui->rbtnSpecified->setChecked(appConfig.options.nameOpt.prefMode == Util::Prefix::specified);
    ui->lePrefix->setEnabled(appConfig.options.nameOpt.prefMode == Util::Prefix::specified);
    ui->lePrefix->setText(QString::fromStdString(appConfig.options.nameOpt.prefix));
    ui->rbtnOriName->setChecked(appConfig.options.nameOpt.prefMode == Util::Prefix::same);
    ui->chbNumberContained->setChecked(appConfig.options.nameOpt.rcContained);
    /************************************************/
}

fpsSettingsDialog::~fpsSettingsDialog()
{
    delete ui;
}

void fpsSettingsDialog::on_buttonBox_accepted()
{
    // Save configurations
    appConfig.app.style = ui->cbxStyle->currentText().toStdString();
    // Change theme
    Util::setAppSkin(qApp, QString::fromStdString(appConfig.app.style));

    switch (ui->cbxLocation->currentIndex()) {
    case 0:
        appConfig.options.outputOpt.savingTo = Util::SavingTo::inPlace;
        break;

    case 1:
        appConfig.options.outputOpt.savingTo = Util::SavingTo::same;
        break;

    case 2:
        appConfig.options.outputOpt.savingTo = Util::SavingTo::specified;
        break;
    }
    appConfig.options.outputOpt.subDir = ui->chbSubDir->isChecked();
    appConfig.options.outputOpt.outPath = ui->lePath->text().toStdString();
    appConfig.options.outputOpt.outFormat = ui->cbxFormats->currentText().toStdString();
    appConfig.options.outputOpt.jpgQuality = ui->sbxQuality->value();
    appConfig.options.outputOpt.scalingFactor = ui->dsbxFactor->value() / 100.0;
    appConfig.options.gridOpt.enabled = ui->chbGrid->isChecked();
    appConfig.options.gridOpt.lineSize = ui->sbxLineSize->value();
    appConfig.options.gridOpt.colorRgb = m_color.name().toStdString();
    appConfig.options.nameOpt.prefMode =
            ui->rbtnSpecified->isChecked() ? Util::Prefix::specified : Util::Prefix::same;
    appConfig.options.nameOpt.prefix = ui->lePrefix->text().toStdString();
    appConfig.options.nameOpt.rcContained = ui->chbNumberContained->isChecked();

    close();
}

void fpsSettingsDialog::on_cbxLocation_currentIndexChanged(int index)
{
    // 2 : "The following path:"
    ui->lePath->setEnabled(index == 2);
    ui->tbtnBrowse->setEnabled(index == 2);
}

void fpsSettingsDialog::on_cbxFormats_currentTextChanged(const QString &text)
{
    ui->sbxQuality->setEnabled(text.compare(u"jpg"_s, Qt::CaseInsensitive) == 0
                               || text.compare(u"jpeg"_s, Qt::CaseInsensitive) == 0);
}

void fpsSettingsDialog::on_btnSelectColor_clicked()
{
    QColor color{ QColorDialog::getColor(m_color, this, tr("Select a color for grid lines")) };
    if (color.isValid()) {
        m_color = color;
        ui->frColor->setPalette(QPalette(color));
    }
}

void fpsSettingsDialog::on_buttonBox_rejected()
{
    close();
}

void fpsSettingsDialog::on_rbtnSpecified_toggled(bool checked)
{
    ui->lePrefix->setEnabled(checked);
}

void fpsSettingsDialog::on_chbGrid_toggled(bool checked)
{
    ui->chbGrid->setChecked(checked);
    ui->frColor->setVisible(checked);
    ui->btnSelectColor->setEnabled(checked);
    ui->sbxLineSize->setEnabled(checked);
}

void fpsSettingsDialog::on_tbtnAppearance_toggled(bool checked)
{
    if (checked)
        ui->wgtOptions->setCurrentIndex(0); // "Appearance"
}

void fpsSettingsDialog::on_tbtnOutput_toggled(bool checked)
{
    if (checked)
        ui->wgtOptions->setCurrentIndex(1); // "Output"
}

void fpsSettingsDialog::on_tbtnBrowse_clicked()
{
    QString in{ QFileDialog::getExistingDirectory(
            this, tr("Choose a directory to save pictures."),
            appConfig.dialog.lastSavedToDir.empty()
                    ? u"."_s
                    : QString::fromStdString(appConfig.dialog.lastSavedToDir)) };
    if (in.isEmpty())
        return;

    appConfig.dialog.lastSavedToDir = in.toStdString();
    appConfig.options.outputOpt.outPath = in.toStdString();
    ui->lePath->setText(in);
}
