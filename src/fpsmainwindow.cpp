//# This file is a part of FreePictureSplitter and is subject to the the terms of the GPL-3.0 license.
//# Copyright (c) zxunge <3396563372@qq.com>
//# See https://github.com/zxunge/FreePictureSplitter/blob/main/LICENSE for the full license text.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fpsmainwindow.h"
#include "SARibbonPannelItem.h"
#include "qradiobutton.h"
#include "ui_fpsmainwindow.h"

#include <SARibbonCategory.h>
#include <SARibbonBar.h>

#include <QRadioButton>

fpsMainWindow::fpsMainWindow(QWidget *parent) :
    SARibbonMainWindow(parent),
    ui(new Ui::fpsMainWindow)
{
    ui->setupUi(this);
    SARibbonBar *ribbonbar { ribbonBar() };
    // ribbonbar->applicationButton()->setText("FreePictureSplitter");
    SARibbonCategory *categoryFile { ribbonbar->addCategoryPage(tr("File")) };
    SARibbonPannel* pannelFile { categoryFile->addPannel(tr("Picture")) };

    QAction* actOpen { createAction(tr("Open"), ":/ribbonBar/ribbonBar/file-add.svg", "actOpen") };
    actOpen->setShortcut(QKeySequence(QLatin1String("Ctrl+O")));

    QAction* actSave { createAction(tr("Save"), ":/ribbonBar/ribbonBar/file-export.svg", "actSave") };
    actSave->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));

    pannelFile->addLargeAction(actOpen);
    pannelFile->addLargeAction(actSave);

    SARibbonPannel* pannelOther { categoryFile->addPannel(tr("Output")) };
    QAction* actOutput { createAction(tr("Output Settings"), ":/ribbonBar/ribbonBar/settings.svg", "actOutput") };

    pannelOther->addLargeAction(actOutput);

    SARibbonCategory *categoryConfig { ribbonbar->addCategoryPage(tr("Split Args")) };
    SARibbonPannel* pannelPixel { categoryConfig->addPannel(tr("By Pixel")) };
    QRadioButton *rbtnPixel { new QRadioButton(tr("Split by pixel.")) };
    pannelPixel->addWidget(rbtnPixel, SARibbonPannelItem::Small);

    initRightButtonGroup();
}

fpsMainWindow::~fpsMainWindow()
{
    delete ui;
}

QAction *fpsMainWindow::createAction(const QString& text, const QString &iconurl, const QString& objName)
{
    QAction* act { new QAction(this) };
    act->setText(text);
    act->setIcon(QIcon(iconurl));
    act->setObjectName(objName);
    return act;
}

void fpsMainWindow::initRightButtonGroup()
{
    SARibbonBar* ribbon { ribbonBar() };
    SARibbonButtonGroupWidget* rightBar { ribbon->rightButtonGroup() };

    // rightBar->addAction(actionHelp);
}
