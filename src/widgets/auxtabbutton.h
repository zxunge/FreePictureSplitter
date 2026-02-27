// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024-2026 zxunge
// clang-format Language: Cpp

#ifndef AUXTABBUTTON_H
#define AUXTABBUTTON_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>

class AuxTabButton : public QWidget
{
    Q_OBJECT
public:
    explicit AuxTabButton(const QPixmap &pixmap, const QString &text, QWidget *parent = nullptr)
        : QWidget(parent), m_labPixmap(new QLabel(this)), m_labText(new QLabel(this))
    {
        QVBoxLayout *layout = new QVBoxLayout(this);
        m_labPixmap->setPixmap(pixmap.scaled(32, 32));
        m_labText->setText(text);
        m_labText->setWordWrap(true);
        m_labPixmap->setScaledContents(false);
        m_labPixmap->setAlignment(Qt::AlignHCenter);
        m_labText->setAlignment(Qt::AlignHCenter);
        layout->addWidget(m_labPixmap);
        layout->addWidget(m_labText);
    }

    void setText(const QString &text) { m_labText->setText(text); }
    void setPixmap(const QPixmap &pixmap) { m_labPixmap->setPixmap(pixmap.scaled(32, 32)); }

private:
    QLabel *m_labPixmap;
    QLabel *m_labText;
};

#endif // AUXTABBUTTON_H