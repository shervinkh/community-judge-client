/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "contestspecsdialog.h"
#include <limits>

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

ContestSpecsDialog::ContestSpecsDialog(const Contest &con, QWidget *parent) :
    QDialog(parent), cur(con)
{
    nameLabel = new QLabel("Name: ");
    nameEdit = new QLineEdit;
    nameLabel->setBuddy(nameEdit);
    nameEdit->setText(con.name());
    connect(nameEdit, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));

    descLabel = new QLabel("Description: ");
    descEdit = new QLineEdit;
    descLabel->setBuddy(descEdit);
    descEdit->setText(con.description());

    rsTimeLabel = new QLabel("Registeration Start: ");
    rsTime = new QDateTimeEdit;
    rsTime->setDisplayFormat(DATE_TIME_FORMAT);
    rsTime->setCalendarPopup(true);
    rsTimeLabel->setBuddy(rsTime);
    rsTime->setDateTime(con.registerStart());

    reTimeLabel = new QLabel("Registeration End: ");
    reTime = new QDateTimeEdit;
    reTime->setDisplayFormat(DATE_TIME_FORMAT);
    reTime->setCalendarPopup(true);
    reTimeLabel->setBuddy(reTime);
    reTime->setDateTime(con.registerEnd());

    csTimeLabel = new QLabel("Contest Start: ");
    csTime = new QDateTimeEdit;
    csTime->setDisplayFormat(DATE_TIME_FORMAT);
    csTime->setCalendarPopup(true);
    csTimeLabel->setBuddy(csTime);
    csTime->setDateTime(con.contestStart());

    ceTimeLabel = new QLabel("Contest End: ");
    ceTime = new QDateTimeEdit;
    ceTime->setDisplayFormat(DATE_TIME_FORMAT);
    ceTime->setCalendarPopup(true);
    ceTimeLabel->setBuddy(ceTime);
    ceTime->setDateTime(con.contestEnd());

    glayout = new QGridLayout;
    glayout->addWidget(nameLabel, 0, 0);
    glayout->addWidget(nameEdit, 0, 1);
    glayout->addWidget(descLabel, 1, 0);
    glayout->addWidget(descEdit, 1, 1);
    glayout->addWidget(rsTimeLabel, 2, 0);
    glayout->addWidget(rsTime, 2, 1);
    glayout->addWidget(reTimeLabel, 3, 0);
    glayout->addWidget(reTime, 3, 1);
    glayout->addWidget(csTimeLabel, 4, 0);
    glayout->addWidget(csTime, 4, 1);
    glayout->addWidget(ceTimeLabel, 5, 0);
    glayout->addWidget(ceTime, 5, 1);

    enabled = new QCheckBox("Enabled");
    enabled->setChecked(con.enabled());

    afterView = new QCheckBox("View Problems After Contest");
    afterView->setChecked(con.afterContestView());

    afterSubmit = new QCheckBox("Allow Submission After Contest");
    afterSubmit->setChecked(con.afterContestSubmit());

    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    totalLayout = new QVBoxLayout;
    totalLayout->addLayout(glayout);
    totalLayout->addWidget(enabled);
    totalLayout->addWidget(afterView);
    totalLayout->addWidget(afterSubmit);
    totalLayout->addWidget(buttons);

    setLayout(totalLayout);

    validateForm();
}

Contest ContestSpecsDialog::resultContest() const
{
    return Contest(cur.ID(), nameEdit->text(), descEdit->text(), enabled->isChecked(),
                   rsTime->dateTime(), reTime->dateTime(), csTime->dateTime(), ceTime->dateTime(),
                   afterView->isChecked(), afterSubmit->isChecked());
}

void ContestSpecsDialog::validateForm()
{
    buttons->button(QDialogButtonBox::Ok)->setEnabled(!nameEdit->text().isEmpty()
                                                      && (reTime->dateTime() >= rsTime->dateTime())
                                                      && (ceTime->dateTime() >= ceTime->dateTime()));
}
