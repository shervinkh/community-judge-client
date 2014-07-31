/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "contests.h"
#include "mainwidget.h"
#include "connection.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include <limits>

Contests::Contests(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    searchMode = new QComboBox;
    searchMode->addItem("Recent Contests");
    searchMode->addItem("Search Contests");
    searchConstr = new QLabel;
    searchTerm = new QLineEdit;
    searchConstr->setBuddy(searchTerm);
    searchReq = new QPushButton("Re&quest");
    searchReq->setMinimumSize(searchReq->sizeHint() * 1.3);

    regButton = new QPushButton("&Register");
    regButton->setMinimumSize(regButton->sizeHint() * 1.3);

    topLayout = new QHBoxLayout;
    topLayout->addWidget(searchMode);
    topLayout->addWidget(searchConstr);
    topLayout->addWidget(searchTerm);
    topLayout->addWidget(searchReq);
    topLayout->addStretch();
    topLayout->addWidget(regButton);

    connect(searchMode, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSearchMode(int)));
    connect(searchReq, SIGNAL(clicked()), this, SLOT(request()));

    tw = new QTreeWidget;
    tw->setHeaderLabel("Contests");

    connect(regButton, SIGNAL(clicked()), this, SLOT(registerReq()));

    total = new QVBoxLayout;
    total->addLayout(topLayout);
    total->addWidget(tw);

    setLayout(total);
}

void Contests::init()
{
    data.clear();
    searchMode->setCurrentIndex(0);
    handleSearchMode(0);
    updateList();
}

void Contests::onSwitch()
{
    if (!mw->connection()->isBusy())
        request();
}

void Contests::updateList()
{
    tw->clear();

    foreach (const Contest &cts, data)
    {
        QTreeWidgetItem *head = new QTreeWidgetItem(QStringList(cts.name()));
        QTreeWidgetItem *body = new QTreeWidgetItem;

        QTextEdit *txt = new QTextEdit;
        txt->setText(QString("<p>%1</p><b>Registeration Time: </b>%2 -- %3<br/>"
                             "<b>Contest Time: </b>%4 -- %5<br/>"
                             "<b>Can View Problems After Contest: </b>%6<br/>"
                             "<b>Can Submit Problems After Contest (Without Affecting Scoreboard): </b> %7")
                     .arg(cts.description())
                     .arg(cts.registerStart().toString(DATE_TIME_FORMAT))
                     .arg(cts.registerEnd().toString(DATE_TIME_FORMAT))
                     .arg(cts.contestStart().toString(DATE_TIME_FORMAT))
                     .arg(cts.contestEnd().toString(DATE_TIME_FORMAT))
                     .arg(cts.afterContestView() ? "Yes" : "No")
                     .arg(cts.afterContestSubmit() ? "Yes" : "No"));
        txt->setReadOnly(true);

        head->addChild(body);
        body->setSizeHint(0, txt->sizeHint());
        body->setFlags(Qt::ItemIsEnabled);
        tw->addTopLevelItem(head);
        tw->setItemWidget(body, 0, txt);
    }
}

void Contests::handleSearchMode(int id)
{
    if (id == 0)
    {
        searchConstr->setText("Count: ");
        searchTerm->setValidator(new QIntValidator(0, std::numeric_limits<int>::max()));
        searchTerm->setText("10");
    }
    else if (id == 1)
    {
        searchConstr->setText("Search Term: ");
        searchTerm->setValidator(Q_NULLPTR);
        searchTerm->setText("");
    }
}

void Contests::request()
{
    DataPointer DS;

    if (DS = mw->connection()->prepare(this, SLOT(requestDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::ContestUserQuery);
        (*DS) << searchMode->currentIndex() << searchTerm->text();

        mw->connection()->send();
    }
}

void Contests::requestDone(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QDataStream DS(BA);
        DS.setVersion(QDataStream::Qt_4_8);
        data.clear();
        DS >> data;
        updateList();
    }
}

void Contests::registerReq()
{
    int idx = tw->indexOfTopLevelItem(tw->currentItem());

    if (idx != -1 && QMessageBox::information(this, "Confirm", QString("Are you sure you want to register in %1?")
                                             .arg(data[idx].name()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        DataPointer DS;

        if (DS = mw->connection()->prepare(this, SLOT(registerReqDone(StatCode,QByteArray))))
        {
            (*DS) << static_cast<int>(StatCode::RegisterContest);
            (*DS) << data[idx].ID();
            mw->connection()->send();
        }
    }
}

void Contests::registerReqDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat))
    {
        if (stat == StatCode::AlreadyRegistered)
            QMessageBox::warning(this, "Error", "You've already registered in this contest");
        else if (stat == StatCode::OperationSuccessful)
            QMessageBox::information(this, "Register Done", "Successfully registered in the contest");
    }
}
