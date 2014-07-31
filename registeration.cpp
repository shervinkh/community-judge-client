/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "registeration.h"
#include "mainwidget.h"
#include "connection.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

Registeration::Registeration(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    registerTitle = new QLabel("<b>Register</b>");

    QFont fnt;
    fnt.setPointSize(18);
    registerTitle->setFont(fnt);

    titleLayout = new QHBoxLayout;
    titleLayout->addStretch();
    titleLayout->addWidget(registerTitle);
    titleLayout->addStretch();

    usernameLabel = new QLabel("Username: ");
    usernameEdit = new QLineEdit;
    usernameEdit->setMaxLength(64);
    usernameLabel->setBuddy(usernameEdit);

    passwordLabel = new QLabel("Password: ");
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMaxLength(64);
    passwordLabel->setBuddy(passwordEdit);

    passwordConfirmLabel = new QLabel("Confirm Password:");
    passwordConfirmLE = new QLineEdit;
    passwordConfirmLE->setEchoMode(QLineEdit::Password);
    passwordConfirmLE->setMaxLength(64);
    passwordConfirmLabel->setBuddy(passwordConfirmLE);

    descriptionLabel = new QLabel("Description: ");
    descriptionEdit = new QLineEdit;
    descriptionEdit->setMaxLength(1024);
    descriptionLabel->setBuddy(descriptionEdit);

    upLayout = new QGridLayout;
    upLayout->addWidget(usernameLabel, 0, 0);
    upLayout->addWidget(usernameEdit, 0, 1);
    upLayout->addWidget(passwordLabel, 1, 0);
    upLayout->addWidget(passwordEdit, 1, 1);
    upLayout->addWidget(passwordConfirmLabel, 2, 0);
    upLayout->addWidget(passwordConfirmLE, 2, 1);
    upLayout->addWidget(descriptionLabel, 3, 0);
    upLayout->addWidget(descriptionEdit, 3, 1);

    inputLayout = new QHBoxLayout;
    inputLayout->addStretch();
    inputLayout->addLayout(upLayout);
    inputLayout->addStretch();

    registerButton = new QPushButton("&Register");
    registerButton->setMinimumSize(registerButton->sizeHint() * 1.3);

    buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(registerButton);
    buttonLayout->addStretch();

    totalLayout = new QVBoxLayout;
    totalLayout->addStretch();
    totalLayout->addLayout(titleLayout);
    totalLayout->addStretch();
    totalLayout->addLayout(inputLayout);
    totalLayout->addStretch();
    totalLayout->addLayout(buttonLayout);
    totalLayout->addStretch();

    setLayout(totalLayout);
    validateForm();

    connect(usernameEdit, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));
    connect(passwordEdit, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));
    connect(passwordConfirmLE, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));
    connect(registerButton, SIGNAL(clicked()), this, SLOT(sendRequest()));
}

void Registeration::validateForm()
{
    registerButton->setEnabled(!usernameEdit->text().isEmpty() && !passwordEdit->text().isEmpty()
                               && !passwordConfirmLE->text().isEmpty());
}

void Registeration::init()
{
    usernameEdit->setText("");
    passwordEdit->setText("");
    passwordConfirmLE->setText("");
    descriptionEdit->setText("");
}

void Registeration::sendRequest()
{
    if (passwordEdit->text() != passwordConfirmLE->text())
    {
        QMessageBox::warning(this, "Error", "Passwords do not match");
        return;
    }

    DataPointer DS;
    mw->connection()->setAuth(QMap<QString, QVariant>());
    if (DS = mw->connection()->prepare(this, SLOT(registerDone(StatCode,QByteArray))))
    {
       (*DS) << static_cast<int>(StatCode::RegisterationRequest);
       (*DS) << usernameEdit->text().toLower() << MainWidget::hashPassword(passwordEdit->text().toUtf8()) << descriptionEdit->text();
        mw->updateServerSelection();
        mw->connection()->send();
    }
}

void Registeration::registerDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat))
    {
        if (stat == StatCode::OperationSuccessful)
            QMessageBox::information(this, "Registeration", "Registeration request submitted successfully");
        else if (stat == StatCode::AlreadyExists)
            QMessageBox::warning(this, "Error", "Chosen username is inuse");
    }
}
