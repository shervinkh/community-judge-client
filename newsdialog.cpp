/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "newsdialog.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

NewsDialog::NewsDialog(DataTable _in, QWidget *parent) :
    QDialog(parent), in(_in)
{
    titleLabel = new QLabel("Title: ");
    titleLE = new QLineEdit;
    titleLabel->setBuddy(titleLE);
    titleLE->setText(in["title"].toString());

    contentLabel = new QLabel("Content: ");
    contentLE = new QTextEdit;
    contentLE->setAcceptRichText(true);
    contentLabel->setBuddy(contentLE);
    contentLE->setText(in["content"].toString());

    grid = new QGridLayout;
    grid->addWidget(titleLabel, 0, 0);
    grid->addWidget(titleLE, 0, 1);
    grid->addWidget(contentLabel, 1, 0);
    grid->addWidget(contentLE, 1, 1);

    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    total = new QVBoxLayout;
    total->addLayout(grid);
    total->addWidget(buttons);

    setLayout(total);
}

DataTable NewsDialog::giveData()
{
    in["title"] = titleLE->text();
    in["content"] = contentLE->toHtml();
    return in;
}
