/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "scoreboardspecsdialog.h"
#include <limits>

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

ScoreboardSpecsDialog::ScoreboardSpecsDialog(const DataTable &scb, QWidget *parent) :
    QDialog(parent), scbConf(scb)
{
    scoreboardLabel = new QLabel("Scoreboard: ");
    scoreboard = new QLabel(scbConf["name"].toString());

    typeLabel = new QLabel("Type: ");
    type = new QComboBox;
    typeLabel->setBuddy(type);
    for (int i = 0; i < 4; i++)
        type->addItem(scoreboardTypes[i]);
    type->setCurrentIndex(scbConf["type"].toInt());

    maxLabel = new QLabel("Max. User (0 = unlimited):");
    max = new QLineEdit;
    maxLabel->setBuddy(max);
    max->setValidator(new QIntValidator(0, std::numeric_limits<int>::max()));
    max->setText(scbConf["max"].toString());

    pageLabel = new QLabel("Each Page (0 = unlimited):");
    page = new QLineEdit;
    pageLabel->setBuddy(page);
    page->setValidator(new QIntValidator(0, std::numeric_limits<int>::max()));
    page->setText(scbConf["page"].toString());

    colorData = new QCheckBox("Extra Data (Min, Avg, Max) And Coloring");
    colorData->setChecked(scbConf["color"].toBool());

    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    grid = new QGridLayout;
    grid->addWidget(scoreboardLabel, 0, 0);
    grid->addWidget(scoreboard, 0, 1);
    grid->addWidget(typeLabel, 1, 0);
    grid->addWidget(type, 1, 1);
    grid->addWidget(maxLabel, 2, 0);
    grid->addWidget(max, 2, 1);
    grid->addWidget(pageLabel, 3, 0);
    grid->addWidget(page, 3, 1);

    total = new QVBoxLayout;
    total->addLayout(grid);
    total->addWidget(colorData);
    total->addWidget(buttons);

    setLayout(total);

    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

DataTable ScoreboardSpecsDialog::resultScoreboard()
{
    scbConf["type"] = type->currentIndex();
    scbConf["max"] = max->text().toLongLong();
    scbConf["page"] = page->text().toInt();
    scbConf["color"] = colorData->isChecked();
    return scbConf;
}
