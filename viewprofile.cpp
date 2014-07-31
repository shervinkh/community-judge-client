/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "viewprofile.h"
#include "changepassworddialog.h"
#include "mainwidget.h"
#include "connection.h"
#include "userrecorddialog.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

ViewProfile::ViewProfile(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    QFont fnt;
    fnt.setPointSize(14);

    requestButton = new QPushButton("&Request");
    requestButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    requestButton->setMinimumSize(requestButton->sizeHint() * 1.3);
    buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(requestButton);
    buttonLayout->addStretch();

    unameLabel = new QLabel("<b>Username: </b>");
    unameLabel->setFont(fnt);
    uname = new QLabel;
    uname->setAlignment(Qt::AlignCenter);
    resetPassword = new QPushButton("Reset &Password");
    resetPassword->setMinimumSize(resetPassword->sizeHint() * 1.3);

    descLabel = new QLabel("<b>Description: </b>");
    descLabel->setFont(fnt);
    desc = new QLabel;
    desc->setWordWrap(true);
    desc->setAlignment(Qt::AlignCenter);

    regOnLabel = new QLabel("<b>Registered On: </b>");
    regOnLabel->setFont(fnt);
    regOn = new QLabel;
    regOn->setAlignment(Qt::AlignCenter);

    numSubmitLabel = new QLabel("<b>#Submissions: </b>");
    numSubmitLabel->setFont(fnt);
    numSubmit = new QLabel;
    numSubmit->setAlignment(Qt::AlignCenter);
    numCorrectLabel = new QLabel("<b>#Corrects: </b>");
    numCorrectLabel->setFont(fnt);
    numCorrect = new QLabel;
    numCorrect->setAlignment(Qt::AlignCenter);
    scoreLabel = new QLabel("<b>Score: </b>");
    scoreLabel->setFont(fnt);
    score = new QLabel;
    score->setAlignment(Qt::AlignCenter);

    grid = new QGridLayout;
    grid->addWidget(unameLabel, 0, 0);
    grid->addWidget(uname, 0, 1, 1, 5);
    grid->addWidget(resetPassword, 0, 6, 1, -1);
    grid->addWidget(descLabel, 1, 0);
    grid->addWidget(desc, 1, 1, 1, -1);
    grid->addWidget(regOnLabel, 2, 0);
    grid->addWidget(regOn, 2, 1, 1, -1);
    grid->addWidget(numSubmitLabel, 3, 0);
    grid->addWidget(numSubmit, 3, 1);
    grid->addItem(new QSpacerItem(50, 0), 3, 2);
    grid->addWidget(numCorrectLabel, 3, 3);
    grid->addWidget(numCorrect, 3, 4);
    grid->addItem(new QSpacerItem(50, 0), 3, 5);
    grid->addWidget(scoreLabel, 3, 6);
    grid->addWidget(score, 3, 7);
    grid->setSpacing(25);

    recordButton = new QPushButton("View Detailed Record");
    recordButton->setMinimumSize(recordButton->sizeHint() * 1.3);

    recordLayout = new QHBoxLayout;
    recordLayout->addStretch();
    recordLayout->addWidget(recordButton);
    recordLayout->addStretch();

    fixedDownLayout = new QHBoxLayout;
    fixedDownLayout->addStretch();
    fixedDownLayout->addLayout(grid);
    fixedDownLayout->addStretch();

    total = new QVBoxLayout;
    total->setSpacing(25);
    total->addLayout(buttonLayout);
    total->addStretch();
    total->addLayout(fixedDownLayout);
    total->addLayout(recordLayout);
    total->addStretch();

    connect(requestButton, SIGNAL(clicked()), this, SLOT(request()));
    connect(resetPassword, SIGNAL(clicked()), this, SLOT(changePassword()));
    connect(recordButton, SIGNAL(clicked()), this, SLOT(showRecord()));

    setLayout(total);
}

void ViewProfile::init()
{
    if (mw->isAdmin())
        uname->setText(QString("<font color=\"blue\">%1 (Admin)</font>").arg(mw->currentUsername()));
    else
        uname->setText(mw->currentUsername());

    desc->clear();
    regOn->clear();
    numSubmit->clear();
    numCorrect->clear();
    score->clear();
}

void ViewProfile::onSwitch()
{
    if (!mw->connection()->isBusy())
        request();
}

void ViewProfile::changePassword()
{
    ChangePasswordDialog CPD(this);

    if (CPD.exec())
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(changePasswordDone(StatCode,QByteArray))))
        {
            (*DS) << static_cast<int>(StatCode::ChangePasswordRequest) << mw->currentUsername()
                  << MainWidget::hashPassword(CPD.password().toUtf8());
            mw->connection()->send();
        }
    }
}

void ViewProfile::changePasswordDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QMessageBox::information(this, "Change Password", "Password successfully changed");
        mw->switchToLoginPanel();
    }
}

void ViewProfile::request()
{
    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(requestDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::ViewProfile);
        mw->connection()->send();
    }
}

void ViewProfile::requestDone(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QDataStream DS(BA);
        DS.setVersion(QDataStream::Qt_4_8);
        User user;
        DS >> user;

        desc->setText(user.description());
        regOn->setText(user.joinDate().toString(DATE_TIME_FORMAT));
        numSubmit->setText(QString::number(user.numSubmits()));
        numCorrect->setText(QString::number(user.numCorrects()));

        if (user.score() != -1)
        {
            scoreLabel->show();
            score->show();
            score->setText(QString::number(user.score(), 'f', 3));
        }
        else
        {
            scoreLabel->hide();
            score->hide();
        }
    }
}

void ViewProfile::showRecord()
{
    UserRecordDialog *urd = new UserRecordDialog(mw, mw->currentUsername(), this);
    urd->setAttribute(Qt::WA_DeleteOnClose);
    urd->show();
    urd->raise();
    urd->activateWindow();
}
