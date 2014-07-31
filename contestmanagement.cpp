/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "contestmanagement.h"
#include "contestspecsdialog.h"
#include "mainwidget.h"
#include "connection.h"
#include <limits>

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

ContestManagement::ContestManagement(MainWidget *MW, QWidget *parent) :
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

    connect(searchMode, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSearchMode(int)));
    connect(searchReq, SIGNAL(clicked()), this, SLOT(request()));

    searchLayout = new QHBoxLayout;
    searchLayout->addStretch();
    searchLayout->addWidget(searchMode);
    searchLayout->addStretch();
    searchLayout->addWidget(searchConstr);
    searchLayout->addWidget(searchTerm);
    searchLayout->addStretch();
    searchLayout->addWidget(searchReq);
    searchLayout->addStretch();

    editButton = new QPushButton("&Edit");
    removeButton = new QPushButton("&Remove");
    manageButtons = new QVBoxLayout;
    manageButtons->addStretch();
    manageButtons->addWidget(editButton);
    manageButtons->addWidget(removeButton);
    manageButtons->addStretch();

    connect(editButton, SIGNAL(clicked()), this, SLOT(editContest()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeContest()));

    lst = new QListWidget;
    lstLayout = new QHBoxLayout;
    lstLayout->addStretch();
    lstLayout->addWidget(lst);
    lstLayout->addLayout(manageButtons);
    lstLayout->addStretch();

    connect(lst, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editContest()));

    addButton = new QPushButton("Add &New Contest");
    addButton->setMinimumSize(addButton->sizeHint() * 1.3);
    addLayout = new QHBoxLayout;
    addLayout->addStretch();
    addLayout->addWidget(addButton);
    addLayout->addStretch();

    connect(addButton, SIGNAL(clicked()), this, SLOT(addNewContest()));

    totalLayout = new QVBoxLayout;
    totalLayout->addStretch();
    totalLayout->addLayout(searchLayout);
    totalLayout->addStretch();
    totalLayout->addLayout(lstLayout);
    totalLayout->addStretch();
    totalLayout->addLayout(addLayout);
    totalLayout->addStretch();

    setLayout(totalLayout);

    lastOp = Operation::None;
}

void ContestManagement::init()
{
    data.clear();
    searchMode->setCurrentIndex(0);
    handleSearchMode(0);
    updateList();
}

void ContestManagement::onSwitch()
{

}

void ContestManagement::handleSearchMode(int id)
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

void ContestManagement::request()
{
    DataPointer DS;

    if (DS = mw->connection()->prepare(this, SLOT(requestDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::ContestListQuery);
        (*DS) << searchMode->currentIndex() << searchTerm->text() << true;

        mw->connection()->send();
    }
}

void ContestManagement::requestDone(StatCode stat, const QByteArray &BA)
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

void ContestManagement::updateList()
{
    lst->clear();

    foreach (QString x, data)
        lst->addItem(x);
}

void ContestManagement::addNewContest()
{
    if (lastOp != Operation::None)
        return;

    ContestSpecsDialog dlg(Contest(-1, "", "", false, QDateTime::currentDateTime(), QDateTime::currentDateTime(),
                                   QDateTime::currentDateTime(), QDateTime::currentDateTime(), false, false));
    if (dlg.exec())
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray)), this, SLOT(networkOperationFailed())))
        {
            (*DS) << static_cast<int>(StatCode::ContestEdit);
            (*DS) << dlg.resultContest();

            lastOp = Operation::Add;
            newName = dlg.resultContest().name();

            mw->connection()->send();
        }
    }
}

void ContestManagement::editContest()
{
    if (lastOp != Operation::None)
        return;

    lastRow = lst->currentRow();
    if (lastRow != -1)
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(editContestStage2(StatCode,QByteArray)), this, SLOT(networkOperationFailed())))
        {
            lastOp = Operation::Edit;
            (*DS) << static_cast<int>(StatCode::ContestDetails);
            (*DS) << data[lastRow];

            mw->connection()->send();
        }
    }
}

void ContestManagement::editContestStage2(StatCode stat, const QByteArray &BA)
{
    lastOp = Operation::None;

    QDataStream DS(BA);
    DS.setVersion(QDataStream::Qt_4_8);

    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        Contest specs;
        DS >> specs;

        ContestSpecsDialog dlg(specs);
        if (dlg.exec())
        {
            DataPointer DS;
            if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray)), this, SLOT(networkOperationFailed())))
            {
                (*DS) << static_cast<int>(StatCode::ContestEdit);
                (*DS) << dlg.resultContest();

                lastOp = Operation::Edit;
                newName = dlg.resultContest().name();

                mw->connection()->send();
            }
        }
    }
}

void ContestManagement::removeContest()
{
    if (lastOp != Operation::None)
        return;

    lastRow = lst->currentRow();
    if (lastRow != -1)
    {
        QString str = QString("Are you sure you want to remove %1?<br />This process cannot be undone.")
                             .arg(data[lastRow]);

        if (QMessageBox::warning(this, "Confirm", str, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            DataPointer DS;
            if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray)), this, SLOT(networkOperationFailed())))
            {
                (*DS) << static_cast<int>(StatCode::ContestRemove);
                (*DS) << data[lastRow];

                lastOp = Operation::Remove;

                mw->connection()->send();
            }
        }
    }
}

void ContestManagement::operationDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat))
    {
        if (stat == StatCode::AlreadyExists)
            QMessageBox::warning(this, "Error", "Sorry, Another contest with the same name exists");
        else if (stat == StatCode::OperationSuccessful)
        {
            QMessageBox::information(this, "Success", "Operation was successful");

            if (lastOp == Operation::Remove)
                data.removeAt(lastRow);
            else if (lastOp == Operation::Add)
                data.append(newName);
            else if (lastOp == Operation::Edit)
                data[lastRow] = newName;

            updateList();
        }
    }

    lastOp = Operation::None;
}

void ContestManagement::networkOperationFailed()
{
    lastOp = Operation::None;
}
