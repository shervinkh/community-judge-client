/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "submissionpurge.h"
#include "mainwidget.h"
#include "connection.h"
#include <limits>

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

SubmissionPurge::SubmissionPurge(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    caution = new QLabel("<b><font color=\"red\">Notice the word \"Keep\"!</font> This means that only the submissions"
                         " you choose below (except username and problem filter) will be kept and all other submissions will "
                         "be deleted</b>");
    caution->setWordWrap(true);

    usernameLabel = new QLabel("Username: ");
    usernameLE = new QLineEdit;
    username = new QHBoxLayout;
    username->addWidget(usernameLabel);
    username->addWidget(usernameLE);

    problemLabel = new QLabel("Problem: ");
    problemLE = new QLineEdit;
    problem = new QHBoxLayout;
    problem->addWidget(problemLabel);
    problem->addWidget(problemLE);

    lastxLabel = new QCheckBox("Keep Last X Submission For Each (User, Problem) Pair");
    x1 = new QLabel("X=");
    lastxLE = new QLineEdit;
    lastxLE->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    lastxLE->setValidator(new QIntValidator(0, std::numeric_limits<int>::max()));
    lastx = new QHBoxLayout;
    lastx->addWidget(x1);
    lastx->addWidget(lastxLE);
    lastx->addStretch();

    lastCorxLabel = new QCheckBox("Keep Last X Correct Submission For Each (User, Problem) Pair");
    x2 = new QLabel("X=");
    lastCorxLE = new QLineEdit;
    lastCorxLE->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    lastCorxLE->setValidator(new QIntValidator(0, std::numeric_limits<int>::max()));
    lastCorx = new QHBoxLayout;
    lastCorx->addWidget(x2);
    lastCorx->addWidget(lastCorxLE);
    lastCorx->addStretch();

    newerThanLabel = new QCheckBox("Keep Newer Than: ");
    newerThan = new QDateTimeEdit;
    newerThan->setCalendarPopup(true);
    newerThan->setDisplayFormat(DATE_TIME_FORMAT);

    olderThanLabel = new QCheckBox("Keep Older Than: ");
    olderThan = new QDateTimeEdit;
    olderThan->setCalendarPopup(true);
    olderThan->setDisplayFormat("yyyy/MM/dd hh:mm:ss AP");

    nonCompletes = new QCheckBox("Keep Non-Complete-Judge Results");
    completes = new QCheckBox("Keep Complete-Judge Results");

    nonPublics = new QCheckBox("Keep Non-Public Submissions");
    publics = new QCheckBox("Keep Public Submissions");

    nonZero = new QCheckBox("Keep Submissions w/ Non-Zero Score");

    theGrid = new QGridLayout;
    theGrid->addLayout(username, 0, 0);
    theGrid->addLayout(problem, 0, 1);
    theGrid->addWidget(lastxLabel, 1, 0);
    theGrid->addLayout(lastx, 1, 1);
    theGrid->addWidget(lastCorxLabel, 2, 0);
    theGrid->addLayout(lastCorx, 2, 1);
    theGrid->addWidget(newerThanLabel, 3, 0);
    theGrid->addWidget(newerThan, 3, 1);
    theGrid->addWidget(olderThanLabel, 4, 0);
    theGrid->addWidget(olderThan, 4, 1);
    theGrid->addWidget(nonCompletes, 5, 0);
    theGrid->addWidget(completes, 5, 1);
    theGrid->addWidget(nonPublics, 6, 0);
    theGrid->addWidget(publics, 6, 1);
    theGrid->addWidget(nonZero, 7, 0);

    purgeButton = new QPushButton("&Purge");
    purgeButton->setMinimumSize(purgeButton->sizeHint() * 1.3);

    filesButton = new QPushButton("Purge &Files Related To Deleted Submissions");
    filesButton->setMinimumSize(filesButton->sizeHint() * 1.3);

    buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(purgeButton);
    buttonLayout->addStretch();

    button2Layout = new QHBoxLayout;
    button2Layout->addStretch();
    button2Layout->addWidget(filesButton);
    button2Layout->addStretch();

    total = new QVBoxLayout;
    total->addWidget(caution);
    total->addStretch();
    total->addLayout(theGrid);
    total->addStretch();
    total->addLayout(buttonLayout);
    total->addStretch();
    total->addLayout(button2Layout);
    total->addStretch();

    connect(lastxLabel, SIGNAL(stateChanged(int)), this, SLOT(handleLastX()));
    connect(lastCorxLabel, SIGNAL(stateChanged(int)), this, SLOT(handleLastCorX()));
    connect(newerThanLabel, SIGNAL(stateChanged(int)), this, SLOT(handleNewerThan()));
    connect(olderThanLabel, SIGNAL(stateChanged(int)), this, SLOT(handleOlderThan()));
    connect(purgeButton, SIGNAL(clicked()), this, SLOT(request()));
    connect(filesButton, SIGNAL(clicked()), this, SLOT(requestFiles()));

    setLayout(total);
}

void SubmissionPurge::init()
{
    usernameLE->clear();
    problemLE->clear();
    lastxLabel->setChecked(false);
    lastCorxLabel->setChecked(false);
    nonCompletes->setChecked(false);
    completes->setChecked(false);
    nonPublics->setChecked(false);
    publics->setChecked(false);
    nonZero->setChecked(false);
    newerThanLabel->setChecked(false);
    olderThanLabel->setChecked(false);
    handleLastX();
    handleLastCorX();
    handleNewerThan();
    handleOlderThan();
}

void SubmissionPurge::onSwitch()
{

}

void SubmissionPurge::handleLastX()
{
    lastxLE->clear();
    lastxLE->setEnabled(lastxLabel->isChecked());
}

void SubmissionPurge::handleLastCorX()
{
    lastCorxLE->clear();
    lastCorxLE->setEnabled(lastCorxLabel->isChecked());
}

void SubmissionPurge::handleNewerThan()
{
    newerThan->setDateTime(QDateTime::currentDateTime());
    newerThan->setEnabled(newerThanLabel->isChecked());
}

void SubmissionPurge::handleOlderThan()
{
    olderThan->setDateTime(QDateTime::currentDateTime());
    olderThan->setEnabled(olderThanLabel->isChecked());
}

void SubmissionPurge::request()
{
    if (QMessageBox::warning(this, "Submission Purge", "Are you sure you want to purge submissions with the given properties?"
                             "<br />This process cannot be undone. Please recheck your input to be sure."
                             , QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(handleResult(StatCode,QByteArray))))
        {
            (*DS) << static_cast<int>(StatCode::SubmissionPurge);
            DataTable DT;
            if (!usernameLE->text().isEmpty())
                DT["username"] = usernameLE->text();
            if (!problemLE->text().isEmpty())
                DT["problem"] = problemLE->text();
            if (lastxLabel->isChecked())
                DT["lastx"] = lastxLE->text();
            if (lastCorxLabel->isChecked())
                DT["lastcorx"] = lastCorxLE->text();
            if (newerThanLabel->isChecked())
                DT["newerthan"] = newerThan->dateTime().toMSecsSinceEpoch();
            if (olderThanLabel->isChecked())
                DT["olderthan"] = olderThan->dateTime().toMSecsSinceEpoch();
            if (nonCompletes->isChecked())
                DT["noncompletes"] = true;
            if (completes->isChecked())
                DT["completes"] = true;
            if (publics->isChecked())
                DT["publics"] = true;
            if (nonPublics->isChecked())
                DT["nonpublics"] = true;
            if (nonZero->isChecked())
                DT["nonzero"] = true;
            (*DS) << DT;
            mw->connection()->send();
        }
    }
}

void SubmissionPurge::requestFiles()
{
    if (QMessageBox::warning(this, "Submission Purge", "Are you sure you want to purge files related to deleted submissions?"
                             "<br />This process cannot be undone."
                             , QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(handleResult(StatCode,QByteArray))))
        {
            (*DS) << static_cast<int>(StatCode::PurgeFiles);
            mw->connection()->send();
        }
    }
}

void SubmissionPurge::handleResult(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
        QMessageBox::information(this, "Submission Purge", "The operation was successful.");
}
