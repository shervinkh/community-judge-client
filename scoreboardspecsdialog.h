/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef SCOREBOARDSPECSDIALOG_H
#define SCOREBOARDSPECSDIALOG_H

#include <QDialog>
#include "general.h"

class QLabel;
class QLineEdit;
class QComboBox;
class QCheckBox;
class QDialogButtonBox;
class QGridLayout;
class QVBoxLayout;

class ScoreboardSpecsDialog : public QDialog
{
    Q_OBJECT
private:
    QLabel *scoreboardLabel;
    QLabel *scoreboard;

    QLabel *typeLabel;
    QComboBox *type;

    QLabel *maxLabel;
    QLineEdit *max;

    QLabel *pageLabel;
    QLineEdit *page;

    QCheckBox *colorData;

    QDialogButtonBox *buttons;

    QGridLayout *grid;

    QVBoxLayout *total;

    DataTable scbConf;

public:
    explicit ScoreboardSpecsDialog(const DataTable &scb, QWidget *parent = Q_NULLPTR);
    DataTable resultScoreboard();

};

#endif // SCOREBOARDSPECSDIALOG_H
