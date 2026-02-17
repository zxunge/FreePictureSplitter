// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge

#include "skinoptionsdialog.h"
#include "clickablelabel.h"

#include "utils/stdpaths.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QFileInfo>

using namespace Qt::Literals::StringLiterals;

SkinOptionsDialog::SkinOptionsDialog(QWidget *parent)
    : QDialog(parent), m_info(std::make_tuple("", "", Util::ThemeManager::Theme::Light))
{
    QVBoxLayout *mainLayout{ new QVBoxLayout(this) };
    auto label1{ new QLabel(tr("Skin name"), this) };
    auto leName{ new QLineEdit(tr("Your skin name"), this) };
    auto label2{ new ClickableLabel(tr("Skin file name: "), this) };
    auto label3{ new QLabel(tr("<em>Your skin file\'s name</em>"), this) };
    label2->setStyleSheet("text-decoration: underline;");
    QHBoxLayout *layout{ new QHBoxLayout(this) };
    layout->addWidget(label2);
    layout->addWidget(label3);
    auto label4{ new QLabel(tr("Theme"), this) };
    auto cbxTheme{ new QComboBox(this) };
    cbxTheme->addItem(tr("Light"));
    cbxTheme->addItem(tr("Dark"));
    QDialogButtonBox *buttonBox{ new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this) };
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(leName, &QLineEdit::textChanged, this,
            [this](const QString &text) { std::get<0>(m_info) = text.toStdString(); });
    connect(label2, &ClickableLabel::clicked, this, [this, label3] {
        QFileInfo fileInfo(QFileDialog::getOpenFileName(
                this, tr("Open a .skin file..."), Util::skinsDir(), u"Skin files (*.skin)"_s));
        if (fileInfo.exists()) {
            std::get<1>(m_info) = fileInfo.fileName().toStdString();
            label3->setText(fileInfo.fileName());
        }
    });
    connect(cbxTheme, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        std::get<2>(m_info) = (text == u"Dark"_s) ? Util::ThemeManager::Theme::Dark
                                                  : Util::ThemeManager::Theme::Light;
    });

    mainLayout->addWidget(label1);
    mainLayout->addWidget(leName);
    mainLayout->addStretch();
    mainLayout->addLayout(layout);
    mainLayout->addStretch();
    mainLayout->addWidget(label4);
    mainLayout->addWidget(cbxTheme);
    mainLayout->addStretch();
    mainLayout->addWidget(buttonBox);
}
