/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "userwidget.h"
#include "submission.h"
#include "viewresults.h"
#include "mainwidget.h"
#include "connection.h"
#include "viewprofile.h"
#include "scoreboard.h"
#include "problemview.h"
#include "news.h"
#include "contests.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

UserWidget::UserWidget(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    expl = new QTabWidget;
    pv = new ProblemView(mw);
    nws = new News(mw);
    cts = new Contests(mw);
    expl->addTab(nws, "News");
    expl->addTab(pv, "Problem View");
    expl->addTab(cts, "Contests");

    sub = new Submission(mw);
    viewRes = new ViewResults(mw);
    viewProf = new ViewProfile(mw);
    sb = new Scoreboard(mw);

    explButton = new QPushButton("E&xplore");
    subButton = new QPushButton("S&ubmit");
    viewButton = new QPushButton("View &Results");
    scoreboardButton = new QPushButton("S&coreboard");
    logoutButton = new QPushButton("&Logout");

    explButton->setMinimumHeight(explButton->sizeHint().height() * 1.5);
    subButton->setMinimumHeight(subButton->sizeHint().height() * 1.5);
    viewButton->setMinimumHeight(viewButton->sizeHint().height() * 1.5);
    scoreboardButton->setMinimumHeight(scoreboardButton->sizeHint().height() * 1.5);
    logoutButton->setMinimumHeight(logoutButton->sizeHint().height() * 1.5);

    adminButton = new QPushButton("Ad&min Panel");

    buttonsLayout = new QVBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(explButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(subButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(viewButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(scoreboardButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(logoutButton);
    buttonsLayout->addStretch();

    buttonsLayout->setSizeConstraint(QLayout::SetMaximumSize);

    connectedToLabel = new QLabel("<b>User: </b>");
    QFont fntC;
    fntC.setPointSize(12);
    connectedToLabel->setFont(fntC);

    viewProfileButton = new QPushButton("&View Profile");
    connect(viewProfileButton, SIGNAL(clicked()), this, SLOT(viewProfile()));

    userLayout = new QHBoxLayout;
    userLayout->addWidget(connectedToLabel);
    userLayout->addStretch();
    userLayout->addWidget(adminButton);
    userLayout->addWidget(viewProfileButton);

    sw = new QStackedWidget;
    sw->insertWidget(0, expl);
    sw->insertWidget(1, sub);
    sw->insertWidget(2, viewRes);
    sw->insertWidget(3, viewProf);
    sw->insertWidget(4, sb);

    rightLayout = new QVBoxLayout;
    rightLayout->addLayout(userLayout);
    rightLayout->addWidget(sw);

    totalLayout = new QHBoxLayout;
    totalLayout->addLayout(buttonsLayout);
    totalLayout->addLayout(rightLayout);

    setLayout(totalLayout);

    connect(explButton, SIGNAL(clicked()), this, SLOT(selectExplore()));
    connect(subButton, SIGNAL(clicked()), this, SLOT(selectSub()));
    connect(viewButton, SIGNAL(clicked()), this, SLOT(selectView()));
    connect(adminButton, SIGNAL(clicked()), this, SLOT(selectAdmin()));
    connect(scoreboardButton, SIGNAL(clicked()), this, SLOT(selectScoreboard()));
    connect(logoutButton, SIGNAL(clicked()), this, SIGNAL(logout()));
    connect(expl, SIGNAL(currentChanged(int)), this, SLOT(exploreTabOnSwitch()));
    connect(sw, SIGNAL(currentChanged(int)), this, SLOT(onSwitch()));
}

void UserWidget::init()
{
    disconnect(expl, SIGNAL(currentChanged(int)), this, SLOT(exploreTabOnSwitch()));
    disconnect(sw, SIGNAL(currentChanged(int)), this, SLOT(onSwitch()));

    connectedToLabel->setText(QString("<b>User: </b>%1@%4 %2:%3").arg(mw->currentUsername())
                              .arg(mw->connection()->currentAddress().host())
                              .arg(mw->connection()->currentAddress().port())
                              .arg(mw->connection()->currentAddress().scheme() == "ssl" ? "SSL:" : ""));

    adminButton->setEnabled(mw->isAdmin());
    adminButton->setVisible(mw->isAdmin());

    sub->init();
    viewRes->init();
    viewProf->init();
    sb->init();
    pv->init();
    nws->init();
    cts->init();

    expl->setCurrentIndex(0);
    sw->setCurrentIndex(0);

    connect(expl, SIGNAL(currentChanged(int)), this, SLOT(exploreTabOnSwitch()));
    connect(sw, SIGNAL(currentChanged(int)), this, SLOT(onSwitch()));
}

void UserWidget::onSwitch()
{
    switch (sw->currentIndex())
    {
        case 0:
            exploreTabOnSwitch();
            break;
        case 1:
            sub->onSwitch();
            break;
        case 2:
            viewRes->onSwitch();
            break;
        case 3:
            viewProf->onSwitch();
            break;
        case 4:
            sb->onSwitch();
            break;
        default:
            break;
    }
}

void UserWidget::exploreTabOnSwitch()
{
    switch (expl->currentIndex())
    {
        case 0:
            nws->onSwitch();
            break;
        case 1:
            pv->onSwitch();
            break;
        case 2:
            cts->onSwitch();
            break;
        default:
            break;
    }
}

void UserWidget::selectExplore()
{
    sw->setCurrentIndex(0);
}

void UserWidget::selectSub()
{
    sw->setCurrentIndex(1);
}

void UserWidget::selectView()
{
    sw->setCurrentIndex(2);
}

void UserWidget::selectAdmin()
{
    emit adminPanel();
}

void UserWidget::viewProfile()
{
    sw->setCurrentIndex(3);
}

void UserWidget::selectScoreboard()
{
    sw->setCurrentIndex(4);
}
