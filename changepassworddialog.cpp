/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "changepassworddialog.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent) :
    QDialog(parent)
{
    passLabel = new QLabel("Password:");
    passLE = new QLineEdit;
    passLabel->setBuddy(passLE);
    passLE->setMaxLength(64);
    passLE->setEchoMode(QLineEdit::Password);

    passConfirmLabel = new QLabel("Confirm Password:");
    passConfirmLE = new QLineEdit;
    passConfirmLabel->setBuddy(passConfirmLE);
    passConfirmLE->setMaxLength(64);
    passConfirmLE->setEchoMode(QLineEdit::Password);

    grid = new QGridLayout;
    grid->addWidget(passLabel, 0, 0);
    grid->addWidget(passLE, 0, 1);
    grid->addWidget(passConfirmLabel, 1, 0);
    grid->addWidget(passConfirmLE, 1, 1);

    buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    totalLayout = new QVBoxLayout;
    totalLayout->addLayout(grid);
    totalLayout->addWidget(buttons);

    setLayout(totalLayout);

    connect(passLE, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));
    connect(passConfirmLE, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));

    validateForm();
}

void ChangePasswordDialog::validateForm()
{
    buttons->button(QDialogButtonBox::Ok)->setEnabled(!passLE->text().isEmpty() && !passConfirmLE->text().isEmpty()
                                                      && passLE->text() == passConfirmLE->text());
}

QString ChangePasswordDialog::password() const
{
    return passLE->text();
}
