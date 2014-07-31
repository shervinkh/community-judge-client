/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "login.h"
#include "mainwidget.h"
#include "connection.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

Login::Login(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    loginTitle = new QLabel("<b>Login</b>");

    QFont fnt;
    fnt.setPointSize(18);
    loginTitle->setFont(fnt);

    titleLayout = new QHBoxLayout;
    titleLayout->addStretch();
    titleLayout->addWidget(loginTitle);
    titleLayout->addStretch();

    usernameLabel = new QLabel("Username: ");
    usernameLE = new QLineEdit;
    usernameLabel->setBuddy(usernameLE);
    usernameLE->setMaxLength(64);

    passwordLabel = new QLabel("Password: ");
    passwordLE = new QLineEdit;
    passwordLE->setEchoMode(QLineEdit::Password);
    passwordLabel->setBuddy(passwordLE);
    passwordLE->setMaxLength(64);

    rememberCB = new QCheckBox("Remember?");

    fieldsLayout = new QGridLayout;
    fieldsLayout->addWidget(usernameLabel, 0, 0);
    fieldsLayout->addWidget(usernameLE, 0, 1);
    fieldsLayout->addWidget(passwordLabel, 1, 0);
    fieldsLayout->addWidget(passwordLE, 1, 1);

    inputLayout = new QHBoxLayout;
    inputLayout->addStretch();
    inputLayout->addLayout(fieldsLayout);
    inputLayout->addStretch();

    loginButton = new QPushButton("&Login");
    loginButton->setMinimumSize(loginButton->sizeHint() * 1.3);

    buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(rememberCB);
    buttonLayout->addWidget(loginButton);
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

    connect(usernameLE, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));
    connect(passwordLE, SIGNAL(textChanged(QString)), this, SLOT(validateForm()));
    connect(loginButton, SIGNAL(clicked()), this, SLOT(login()));
}

void Login::validateForm()
{
    loginButton->setEnabled(!usernameLE->text().isEmpty() && !passwordLE->text().isEmpty());
}

void Login::init()
{
    usernameLE->setText(mw->rememberedUsername());
    passwordLE->setText(QString::fromUtf8(QByteArray::fromBase64(mw->rememberedPassword())));
    if (!usernameLE->text().isEmpty())
        rememberCB->setCheckState(Qt::Checked);
    else
        rememberCB->setCheckState(Qt::Unchecked);
}

void Login::login()
{
    DataPointer DS;
    mw->setAuth(usernameLE->text().toLower(), MainWidget::hashPassword(passwordLE->text().toUtf8()));
    if (DS = mw->connection()->prepare(this, SLOT(loginDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::LoginRequest);

        if (rememberCB->checkState() == Qt::Checked)
        {
            mw->setRememberedUsername(usernameLE->text());
            mw->setRememberedPassword(passwordLE->text().toUtf8().toBase64());
        }

        mw->updateServerSelection();
        mw->connection()->send();
    }
}

void Login::loginDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat) && (stat == StatCode::UserOK || stat == StatCode::AdminOK))
    {
        mw->setAdmin(stat == StatCode::AdminOK);
        emit loginOK();
    }
}
