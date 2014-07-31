/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef CONTESTSPECSDIALOG_H
#define CONTESTSPECSDIALOG_H

#include <QDialog>
#include <QList>
#include "general.h"

class QLabel;
class QLineEdit;
class QDateTimeEdit;
class QGridLayout;
class QDialogButtonBox;
class QCheckBox;
class QVBoxLayout;

class ContestSpecsDialog : public QDialog
{
    Q_OBJECT
private:
    QLabel *nameLabel;
    QLineEdit *nameEdit;

    QLabel *descLabel;
    QLineEdit *descEdit;

    QLabel *rsTimeLabel;
    QDateTimeEdit *rsTime;

    QLabel *reTimeLabel;
    QDateTimeEdit *reTime;

    QLabel *csTimeLabel;
    QDateTimeEdit *csTime;

    QLabel *ceTimeLabel;
    QDateTimeEdit *ceTime;

    QCheckBox *enabled;
    QCheckBox *afterView;
    QCheckBox *afterSubmit;

    QGridLayout *glayout;

    QVBoxLayout *totalLayout;

    QDialogButtonBox *buttons;

    Contest cur;

public:
    explicit ContestSpecsDialog(const Contest &con, QWidget *parent = Q_NULLPTR);
    Contest resultContest() const;

private slots:
    void validateForm();
};

#endif // CONTESTSPECSDIALOG_H
