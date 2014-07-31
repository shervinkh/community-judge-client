/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "adminpanel.h"
#include "mainwidget.h"
#include "problemmanagement.h"
#include "scoreplanmanager.h"
#include "usermanagement.h"
#include "connection.h"
#include "serverconfig.h"
#include "submissionpurge.h"
#include "contestmanagement.h"
#include "scoreboardconfig.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

AdminPanel::AdminPanel(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    PM = new ProblemManagement(mw);
    CM = new ContestManagement(mw);
    UM = new UserManagement(mw);
    SPM = new ScorePlanManager(mw);
    SC = new ServerConfig(mw);
    SP = new SubmissionPurge(mw);
    SBC = new ScoreboardConfig(mw);

    buttons = new QListWidget;
    buttons->addItem("Manage Problems");
    buttons->addItem("Manage Contests");
    buttons->addItem("Manage Users");
    buttons->addItem("Manage Score Plans");
    buttons->addItem("Scoreboard Configuration");
    buttons->addItem("Server Configurations");
    buttons->addItem("Submission Purge");
    buttons->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    connect(buttons, SIGNAL(currentRowChanged(int)), this, SLOT(selectChanged(int)));

    backButton = new QPushButton("&Back To User Panel");
    connect(backButton, SIGNAL(clicked()), this, SIGNAL(userPanel()));

    connectedToLabel = new QLabel("<b>User: </b>");
    QFont fntC;
    fntC.setPointSize(12);
    connectedToLabel->setFont(fntC);

    userLayout = new QHBoxLayout;
    userLayout->addWidget(connectedToLabel);
    userLayout->addStretch();
    userLayout->addWidget(backButton);

    sw = new QStackedWidget;
    sw->addWidget(PM);
    sw->addWidget(CM);
    sw->addWidget(UM);
    sw->addWidget(SPM);
    sw->addWidget(SBC);
    sw->addWidget(SC);
    sw->addWidget(SP);

    rightLayout = new QVBoxLayout;
    rightLayout->addLayout(userLayout);
    rightLayout->addWidget(sw);

    totalLayout = new QHBoxLayout;
    totalLayout->addWidget(buttons);
    totalLayout->addLayout(rightLayout);

    setLayout(totalLayout);

    connect(sw, SIGNAL(currentChanged(int)), this, SLOT(onSwitch()));
}

void AdminPanel::init()
{
    disconnect(sw, SIGNAL(currentChanged(int)), this, SLOT(onSwitch()));

    connectedToLabel->setText(QString("<b>User: </b>%1@%4 %2:%3").arg(mw->currentUsername())
                              .arg(mw->connection()->currentAddress().host())
                              .arg(mw->connection()->currentAddress().port())
                              .arg(mw->connection()->currentAddress().scheme() == "ssl" ? "SSL:" : ""));
    PM->init();
    UM->init();
    SPM->init();
    SC->init();
    SP->init();
    CM->init();
    SBC->init();
    sw->setCurrentIndex(0);
    buttons->setCurrentRow(0);

    connect(sw, SIGNAL(currentChanged(int)), this, SLOT(onSwitch()));
}

void AdminPanel::onSwitch()
{
    switch (sw->currentIndex())
    {
        case 0:
            PM->onSwitch();
            break;
        case 1:
            CM->onSwitch();
            break;
        case 2:
            UM->onSwitch();
            break;
        case 3:
            SPM->onSwitch();
            break;
        case 4:
            SBC->onSwitch();
            break;
        case 5:
            SC->onSwitch();
            break;
        case 6:
            SP->onSwitch();
            break;
        default:
            break;
    }
}

void AdminPanel::selectChanged(int sel)
{
    sw->setCurrentIndex(sel);
}
