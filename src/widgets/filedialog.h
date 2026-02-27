// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QSizePolicy>

class FileDialog : public QDialog
{
    Q_OBJECT
public:
    FileDialog(QWidget *toAdd, QWidget *parent = nullptr) : QDialog(parent)
    {
        m_dlg = new QFileDialog(this, Qt::SubWindow);
        m_dlg->setOption(QFileDialog::DontUseNativeDialog, true);
        QVBoxLayout *layout{ new QVBoxLayout(this) };
        m_dlg->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        toAdd->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        layout->addWidget(m_dlg);
        layout->addWidget(toAdd);

        connect(m_dlg, &QFileDialog::accepted, this, &QDialog::accept);
        connect(m_dlg, &QFileDialog::rejected, this, &QDialog::reject);
    }

    QFileDialog *fileDialog() const { return m_dlg; }

private:
    QFileDialog *m_dlg;
};

#endif // FILEDIALOG_H
