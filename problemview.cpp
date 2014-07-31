/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "problemview.h"
#include "mainwidget.h"
#include "connection.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

ProblemView::ProblemView(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    searchMode = new QComboBox;
    searchMode->addItem("Recent Problems");
    searchMode->addItem("Search Problems");
    searchConstr = new QLabel;
    searchTerm = new QLineEdit;
    searchConstr->setBuddy(searchTerm);
    searchReq = new QPushButton("Re&quest");
    searchReq->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    contestLabel = new QLabel("Contest: ");
    contestList = new QComboBox;
    contestList->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    contestLabel->setBuddy(contestList);
    refreshButton = new QPushButton("Re&fresh");
    contestLayout = new QHBoxLayout;
    contestLayout->addWidget(contestLabel);
    contestLayout->addWidget(contestList);
    contestLayout->addStretch();
    contestLayout->addWidget(refreshButton);
    contestWidget = new QWidget;
    contestWidget->setLayout(contestLayout);

    lw = new QListWidget;

    searchDownLayout = new QHBoxLayout;
    searchDownLayout->addWidget(searchConstr);
    searchDownLayout->addWidget(searchTerm);

    searchLeftLayout = new QVBoxLayout;
    searchLeftLayout->addWidget(searchMode);
    searchLeftLayout->addLayout(searchDownLayout);

    searchLayout = new QHBoxLayout;
    searchLayout->addLayout(searchLeftLayout);
    searchLayout->addWidget(searchReq);

    probsLayout = new QVBoxLayout;
    probsLayout->addLayout(searchLayout);
    probsLayout->addWidget(lw);

    probs = new QWidget;
    probs->setLayout(probsLayout);
    probs->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);

    contestWidget->setMaximumWidth(probs->sizeHint().width());
    probs->setMinimumWidth(probs->sizeHint().width());
    probsLayout->setMargin(0);
    contestLayout->setMargin(0);

    leftLayout = new QVBoxLayout;
    leftLayout->addWidget(contestWidget);
    leftLayout->addWidget(probs);

    nameLabel = new QLabel("<b>Name: </b>");
    name = new QLabel;
    name->setWordWrap(true);
    descLabel = new QLabel("<b>Description: </b>");
    desc = new QLabel;
    desc->setWordWrap(true);
    descFileLabel = new QLabel("<b>Description File: </b>");
    descFile = new QLabel;
    descFile->setWordWrap(true);
    descFileDownload = new QPushButton("Dow&nload");
    numTestsLabel = new QLabel("<b># Tests: </b>");
    numTests = new QLabel;
    numTests->setWordWrap(true);
    timeLimitLabel = new QLabel("<b>Time Limit: </b>");
    timeLimit = new QLabel;
    timeLimit->setWordWrap(true);
    memLimitLabel = new QLabel("<b>Memory Limit: </b>");
    memLimit = new QLabel;
    memLimit->setWordWrap(true);
    scorePlanLabel = new QLabel("<b>Score Plan: </b>");
    scorePlan = new QLabel;
    scorePlan->setWordWrap(true);
    completeJudgeLabel = new QLabel("<b>Complete Judge: </b>");
    completeJudge = new QLabel;
    completeJudge->setWordWrap(true);
    pubsubLabel = new QLabel("<b>View Public Submissions' Code:  </b>");
    pubsub = new QLabel;
    pubsub->setWordWrap(true);
    othsubLabel = new QLabel("<b>View Other's Submissions' Code: </b>");
    othsub = new QLabel;
    othsub->setWordWrap(true);

    probGrid = new QGridLayout;
    probGrid->addWidget(nameLabel, 0, 0);
    probGrid->addWidget(name, 0, 1);
    probGrid->addWidget(descLabel, 1, 0);
    probGrid->addWidget(desc, 1, 1);
    probGrid->addWidget(descFileLabel, 2, 0);
    probGrid->addWidget(descFile, 2, 1);
    probGrid->addWidget(descFileDownload, 2, 2);
    probGrid->addWidget(numTestsLabel, 3, 0);
    probGrid->addWidget(numTests, 3, 1);
    probGrid->addWidget(timeLimitLabel, 4, 0);
    probGrid->addWidget(timeLimit, 4, 1);
    probGrid->addWidget(memLimitLabel, 5, 0);
    probGrid->addWidget(memLimit, 5, 1);
    probGrid->addWidget(scorePlanLabel, 6, 0);
    probGrid->addWidget(scorePlan, 6, 1);
    probGrid->addWidget(completeJudgeLabel, 7, 0);
    probGrid->addWidget(completeJudge, 7, 1);
    probGrid->addWidget(pubsubLabel, 8, 0);
    probGrid->addWidget(pubsub, 8, 1);
    probGrid->addWidget(othsubLabel, 9, 0);
    probGrid->addWidget(othsub, 9, 1);

    infoLayout = new QVBoxLayout;
    infoLayout->addStretch();
    infoLayout->addLayout(probGrid);
    infoLayout->addStretch();

    info = new QWidget;
    info->setLayout(infoLayout);

    getInfo = new QPushButton("Get Info >>");
    getInfo->setMinimumHeight(getInfo->sizeHint().height() * 1.5);
    getLayout = new QVBoxLayout;
    getLayout->addStretch();
    getLayout->addWidget(getInfo);
    getLayout->addStretch();

    totalLayout = new QHBoxLayout;
    totalLayout->addLayout(leftLayout);
    totalLayout->addLayout(getLayout);
    totalLayout->addWidget(info);
    totalLayout->addStretch();

    connect(searchMode, SIGNAL(currentIndexChanged(int)), this, SLOT(handleSearchMode(int)));
    connect(lw, SIGNAL(currentRowChanged(int)), this, SLOT(handleGetInfo()));
    connect(searchReq, SIGNAL(clicked()), this, SLOT(request()));
    connect(getInfo, SIGNAL(clicked()), this, SLOT(requestInfo()));
    connect(descFileDownload, SIGNAL(clicked()), this, SLOT(downloadDescription()));
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(refreshList()));

    setLayout(totalLayout);
}

void ProblemView::init()
{
    data.clear();
    contestList->clear();
    searchMode->setCurrentIndex(0);
    handleSearchMode(0);
    handleGetInfo();
    updateList();
}

void ProblemView::onSwitch()
{
    if (!mw->connection()->isBusy())
        refreshList();
}

void ProblemView::handleSearchMode(int id)
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

void ProblemView::request()
{
    DataPointer DS;

    if (DS = mw->connection()->prepare(this, SLOT(requestDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::ProblemManagementQuery);
        (*DS) << true << searchMode->currentIndex() << searchTerm->text() << contestList->currentText();

        mw->connection()->send();
    }
}

void ProblemView::requestDone(StatCode stat, const QByteArray &BA)
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

void ProblemView::updateList()
{
    lw->clear();

    foreach (QString x, data)
        lw->addItem(x);

    setInfo(Problem());
}

void ProblemView::handleGetInfo()
{
    getInfo->setEnabled(lw->currentRow() != -1);
}

void ProblemView::requestInfo()
{
    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(infoDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::ProblemManagementDetails);
        (*DS) << data[lw->currentRow()] << true;

        mw->connection()->send();
    }
}

void ProblemView::infoDone(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat))
    {
        QDataStream DS(BA);
        DS.setVersion(QDataStream::Qt_4_8);
        Problem specs;
        DS >> specs;
        setInfo(specs);
    }
}

void ProblemView::setInfo(const Problem &prob)
{
    if (prob.name().isEmpty())
        info->hide();
    else
    {
        name->setText(prob.name());
        desc->setText(prob.description());

        descFile->setText(prob.descriptionFile());
        descFileDownload->setVisible(!prob.descriptionFile().isEmpty());
        if (descFile->text().isEmpty())
            descFile->setText("N/A");

        numTests->setText(QString::number(prob.numTests()));

        timeLimit->setText(MainWidget::formatTime(prob.timeLimit()));
        memLimit->setText(MainWidget::formatSize(prob.memoryLimit()));

        scorePlan->setText(prob.scorePlan());
        completeJudge->setText(prob.isComplete() ? "Yes" : "No");
        pubsub->setText(viewCodePermissions[prob.publicSubmissions()]);
        othsub->setText(viewCodePermissions[prob.othersSubmissions()]);
        info->show();
    }
}

void ProblemView::downloadDescription()
{
    int idx = descFile->text().lastIndexOf('.');
    QString fileExt;
    if (idx != -1)
        fileExt = descFile->text().mid(idx + 1);

    if (fileExt.isEmpty())
        savePath = QFileDialog::getSaveFileName(this, "Save Description File", descFile->text(), "All Files");
    else
        savePath = QFileDialog::getSaveFileName(this, "Save Description File", descFile->text(), QString("%1 Files (*.%2)")
                                                .arg(fileExt.toUpper()).arg(fileExt.toLower()));

    if (!savePath.isNull())
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(descDone(StatCode,QByteArray))))
        {
            (*DS) << static_cast<int>(StatCode::GiveDescription);
            (*DS) << name->text();
            mw->connection()->send();
            mw->connection()->showProgressDialog();
        }
    }
}

void ProblemView::descDone(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QFile file(savePath);
        if (!file.open(QIODevice::WriteOnly))
            mw->handleGeneralStatCodes(StatCode::InternalFileOperationError);
        else
        {
            file.write(BA);
            file.flush();
            file.close();
            QMessageBox::information(this, "Description File", "Description file successfully saved");
        }
    }
}

void ProblemView::refreshList()
{
    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(refreshListDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::CurrentContests);
        mw->connection()->send();
    }
}

void ProblemView::refreshListDone(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QDataStream DS(BA);
        DS.setVersion(QDataStream::Qt_4_8);

        QList<QString> data;
        DS >> data;

        contestList->clear();

        foreach (QString str, data)
            contestList->addItem(str);
    }
}
