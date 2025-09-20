// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef PREFERENCESWIDGET_H
#define PREFERENCESWIDGET_H

#include <QWidget>
#include <QColor>

namespace Ui {
class PreferencesWidget;
}
class QAbstractButton;

class PreferencesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PreferencesWidget(QWidget *parent = nullptr);
    ~PreferencesWidget();

    void saveChanges(); // This does not apply skin changes.

private slots:
    void on_cbxLocation_currentIndexChanged(int index);

    void on_cbxFormats_currentTextChanged(const QString &text);

    void on_btnSelectColor_clicked();

    void on_rbtnSpecified_toggled(bool checked);

    void on_chbGrid_toggled(bool checked);

    void on_tbtnAppearance_toggled(bool checked);

    void on_tbtnOutput_toggled(bool checked);

    void on_tbtnBrowse_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::PreferencesWidget *ui;
    QColor m_color;
};

#endif // PREFERENCESWIDGET_H
