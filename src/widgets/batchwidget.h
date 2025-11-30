// SPDX-License-Identifier: GPL-3.0-only
// SPDX-FileCopyrightText: 2024 2025 zxunge
// clang-format Language: Cpp

#ifndef BATCHWIDGET_H
#define BATCHWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QItemSelection>
#include <QtCore/qstringlist.h>

namespace Ui {
class BatchWidget;
}
class QStandardItemModel;
class QItemSelectionModel;

class BatchWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit BatchWidget(QWidget *parent = nullptr);
    ~BatchWidget();

private:
    void addPicture(const QString &fileName);
    void removeSelectedItems();
    void openPictures();
    void openFolder();
    void changePath();
    void startSplit();

protected:
    void closeEvent(QCloseEvent *event) override;

Q_SIGNALS:
    void message(const QString &msg, int timeout = 0);

private:
    Ui::BatchWidget *ui;

    QMenu *m_contextMenu;
    QStandardItemModel *m_model;
    QItemSelectionModel *m_selModel;
    // QProgressBar *m_pbLoading;
};

#endif // BATCHWIDGET_H
