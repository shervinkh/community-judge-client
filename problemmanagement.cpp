/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "problemmanagement.h"
#include "mainwidget.h"
#include "problemspecsdialog.h"
#include "connection.h"
#include <limits>

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

ProblemManagement::ProblemManagement(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    lastOp = Operation::None;

    searchMode = new QComboBox;
    searchMode->addItem("Recent Problems");
    searchMode->addItem("Search Problems");
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
    rejudgeButton = new QPushButton("Re&judge");
    manageButtons = new QVBoxLayout;
    manageButtons->addStretch();
    manageButtons->addWidget(editButton);
    manageButtons->addWidget(removeButton);
    manageButtons->addWidget(rejudgeButton);
    manageButtons->addStretch();

    connect(editButton, SIGNAL(clicked()), this, SLOT(editProblem()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeProblem()));
    connect(rejudgeButton, SIGNAL(clicked()), this, SLOT(rejudgeProblem()));

    lst = new QListWidget;
    lstLayout = new QHBoxLayout;
    lstLayout->addStretch();
    lstLayout->addWidget(lst);
    lstLayout->addLayout(manageButtons);
    lstLayout->addStretch();

    connect(lst, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editProblem()));

    addButton = new QPushButton("Add &New Problem");
    addButton->setMinimumSize(addButton->sizeHint() * 1.3);
    addLayout = new QHBoxLayout;
    addLayout->addStretch();
    addLayout->addWidget(addButton);
    addLayout->addStretch();

    connect(addButton, SIGNAL(clicked()), this, SLOT(addNewProblem()));

    totalLayout = new QVBoxLayout;
    totalLayout->addStretch();
    totalLayout->addLayout(searchLayout);
    totalLayout->addStretch();
    totalLayout->addLayout(lstLayout);
    totalLayout->addStretch();
    totalLayout->addLayout(addLayout);
    totalLayout->addStretch();

    setLayout(totalLayout);
}

void ProblemManagement::init()
{
    data.clear();
    scoreListCallback.clear();
    contestListCallback.clear();
    searchMode->setCurrentIndex(0);
    handleSearchMode(0);
    updateList();
}

void ProblemManagement::onSwitch()
{
}

void ProblemManagement::handleSearchMode(int id)
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

void ProblemManagement::request()
{
    DataPointer DS;

    if (DS = mw->connection()->prepare(this, SLOT(requestDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::ProblemManagementQuery);
        (*DS) << false << searchMode->currentIndex() << searchTerm->text() << QString();

        mw->connection()->send();
    }
}

void ProblemManagement::requestDone(StatCode stat, const QByteArray &BA)
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

void ProblemManagement::updateList()
{
    lst->clear();

    foreach (QString x, data)
        lst->addItem(x);
}

void ProblemManagement::addNewProblem()
{
    if (lastOp != Operation::None)
        return;

    lastOp = Operation::Add;

    if (!updateScoresList("addNewProblem"))
        return;

    if (!updateContestList("addNewProblem"))
        return;

    lastOp = Operation::None;

    ProblemSpecsDialog dlg(Problem(-1, "", "", 0, 0, 0, false, false, "", "", "", "", 2, 0), scores, contests);
    if (dlg.exec())
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray)), this, SLOT(networkOperationFailed())))
        {
            (*DS) << static_cast<int>(StatCode::ProblemManagementEdit);
            (*DS) << dlg.resultProblem();

            lastOp = Operation::Add;
            newName = dlg.resultProblem().name();

            mw->connection()->send();
        }
    }   
}

void ProblemManagement::editProblem()
{
    if (lastOp != Operation::None)
        return;

    lastRow = lst->currentRow();
    if (lastRow == -1)
        return;

    lastOp = Operation::Edit;

    if (!updateScoresList("editProblem"))
        return;

    if (!updateContestList("editProblem"))
        return;

    lastOp = Operation::None;

    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(editProblemStage2(StatCode,QByteArray)), this, SLOT(networkOperationFailed())))
    {
        lastOp = Operation::Edit;

        (*DS) << static_cast<int>(StatCode::ProblemManagementDetails);
        (*DS) << data[lastRow] << false;

        mw->connection()->send();
    }
}

void ProblemManagement::editProblemStage2(StatCode stat, const QByteArray &BA)
{
    lastOp = Operation::None;

    QDataStream DS(BA);
    DS.setVersion(QDataStream::Qt_4_8);

    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        Problem specs;
        DS >> specs;

        ProblemSpecsDialog dlg(specs, scores, contests);
        if (dlg.exec())
        {
            DataPointer DS;
            if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray)), this, SLOT(networkOperationFailed())))
            {
                (*DS) << static_cast<int>(StatCode::ProblemManagementEdit);
                (*DS) << dlg.resultProblem();

                lastOp = Operation::Edit;
                newName = dlg.resultProblem().name();

                mw->connection()->send();
            }
        }
    }
}

void ProblemManagement::removeProblem()
{
    if (lastOp != Operation::None)
        return;

    lastRow = lst->currentRow();
    if (lastRow != -1)
    {
        QString name = data[lastRow];
        QString str = QString("Are you sure you want to remove %1?<br />This process cannot be undone.").arg(name);
        if (QMessageBox::warning(this, "Confirm", str, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            DataPointer DS;
            if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray)), this, SLOT(networkOperationFailed())))
            {
                (*DS) << static_cast<int>(StatCode::ProblemManagementRemove);
                (*DS) << name;

                lastOp = Operation::Remove;

                mw->connection()->send();
            }
        }
    }
}

void ProblemManagement::rejudgeProblem()
{
    if (lastOp != Operation::None)
        return;

    lastRow = lst->currentRow();
    if (lastRow != -1)
    {
        QString name = data[lastRow];
        QString str = QString("Are you sure you want to rejudge %1?<br />This may take a very long time.").arg(name);
        if (QMessageBox::warning(this, "Confirm", str, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            DataPointer DS;
            if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray)), this, SLOT(networkOperationFailed())))
            {
                lastOp = Operation::Other;

                (*DS) << static_cast<int>(StatCode::ProblemManagementRejudge);
                (*DS) << name;

                mw->connection()->send();
            }
        }
    }
}

void ProblemManagement::operationDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat))
    {
        if (stat == StatCode::AlreadyExists)
            QMessageBox::warning(this, "Error", "Problem with the same name exists");
        else if (stat == StatCode::ProblemNotEnoughData)
            QMessageBox::warning(this, "Error", "There's not enough data on server for the problem you've specified.");
        else if (stat == StatCode::OperationSuccessful)
        {
            QMessageBox::information(this, "Success", "Operation was successful");

            if (lastOp == Operation::Remove)
            {
                data.removeAt(lastRow);
                updateList();
            }
            else if (lastOp == Operation::Add)
            {
                data.append(newName);
                updateList();
            }
            else if (lastOp == Operation::Edit)
            {
                data[lastRow] = newName;
                updateList();
            }
        }
    }

    lastOp = Operation::None;
}

bool ProblemManagement::updateScoresList(const QString &name)
{
    if (scoreListCallback.isNull())
    {
        if (contestListCallback == name)
            return true;

        scoreListCallback = name;

        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(scoresDone(StatCode,QByteArray)), this, SLOT(networkOperationFailed())))
        {
            (*DS) << static_cast<int>(StatCode::ScorePlanQuery);
            mw->connection()->send();
        }
        return false;
    }
    else if (scoreListCallback == name)
        return true;
    else if (scoreListCallback != name)
    {
        QMessageBox::warning(this, "Error", "Sorry, Another conflicting operation is in process.");
        return false;
    }
}

void ProblemManagement::scoresDone(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QDataStream DS(BA);
        DS.setVersion(QDataStream::Qt_4_8);
        DS >> scores;
        lastOp = Operation::None;
        QMetaObject::invokeMethod(this, scoreListCallback.toStdString().c_str(), Qt::DirectConnection);
    }
    scoreListCallback.clear();
}

bool ProblemManagement::updateContestList(const QString &name)
{
    if (contestListCallback.isNull())
    {
        contestListCallback = name;

        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(contestDone(StatCode,QByteArray)), this, SLOT(networkOperationFailed())))
        {
            (*DS) << static_cast<int>(StatCode::ContestListQuery);
            (*DS) << 0 << QString("-1") << false;
            mw->connection()->send();
        }

        return false;
    }
    else if (contestListCallback == name)
        return true;
    else if (contestListCallback != name)
    {
        QMessageBox::warning(this, "Error", "Sorry, Another conflicting operation is in process.");
        return false;
    }
}

void ProblemManagement::contestDone(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QDataStream DS(BA);
        DS.setVersion(QDataStream::Qt_4_8);
        DS >> contests;
        lastOp = Operation::None;
        QMetaObject::invokeMethod(this, contestListCallback.toStdString().c_str(), Qt::DirectConnection);
    }
    contestListCallback.clear();
}

void ProblemManagement::networkOperationFailed()
{
    lastOp = Operation::None;
    scoreListCallback.clear();
    contestListCallback.clear();
}
