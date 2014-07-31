/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "addserverdialog.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

AddServerDialog::AddServerDialog(QWidget *parent) :
    QDialog(parent)
{
    nameLabel = new QLabel("Name: ");
    nameLE = new QLineEdit;
    nameLabel->setBuddy(nameLE);

    addressLabel = new QLabel("Address: ");
    addressLE = new QLineEdit;
    addressLabel->setBuddy(addressLE);

    portLabel = new QLabel("Port: ");
    portLE = new QLineEdit;
    portLabel->setBuddy(portLE);
    portLE->setValidator(new QIntValidator(1, 65535));

    sslCheck = new QCheckBox("SSL");

    fields = new QGridLayout;
    fields->addWidget(nameLabel, 0, 0);
    fields->addWidget(nameLE, 0, 1);
    fields->addWidget(addressLabel, 1, 0);
    fields->addWidget(addressLE, 1, 1);
    fields->addWidget(portLabel, 2, 0);
    fields->addWidget(portLE, 2, 1);

    connect(nameLE, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));
    connect(addressLE, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));
    connect(portLE, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    totalLayout = new QVBoxLayout;
    totalLayout->addLayout(fields);
    totalLayout->addWidget(sslCheck);
    totalLayout->addWidget(buttonBox);

    setLayout(totalLayout);
    validateForm();
}

QString AddServerDialog::name() const
{
    return nameLE->text();
}

QUrl AddServerDialog::url() const
{
    QUrl URL;
    URL.setHost(addressLE->text());
    URL.setPort(portLE->text().toInt());

    if (sslCheck->isChecked())
        URL.setScheme("ssl");
    else
        URL.setScheme("tcp");

    return URL;
}

void AddServerDialog::validateForm()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!nameLE->text().isEmpty() && !addressLE->text().isEmpty()
                                                        && !portLE->text().isEmpty());
}
