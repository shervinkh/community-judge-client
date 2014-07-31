/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "viewresults.h"
#include "mainwidget.h"
#include "textdialog.h"
#include "submitdetails.h"
#include "connection.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include <limits>

ViewResults::ViewResults(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    fromLabel = new QLabel("Submissions From: ");
    fromCombo = new QComboBox;
    fromLabel->setBuddy(fromCombo);
    fromLE = new QLineEdit;
    problemLabel = new QLabel("On Problem: ");
    problemLE = new QLineEdit;
    problemLabel->setBuddy(problemLE);

    filtersLabel = new QLabel("Filters: ");
    publicCheckBox = new QCheckBox("Public");
    correctCheckBox = new QCheckBox("Correct");
    countCheckBox = new QCheckBox("Count: ");
    countLE = new QLineEdit;
    countLE->setValidator(new QIntValidator(0, std::numeric_limits<int>::max()));
    requestButton = new QPushButton("Re&quest");
    requestButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    byIDButton = new QPushButton("By &ID");
    byIDButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    layout1 = new QHBoxLayout;
    layout1->addWidget(fromLabel);
    layout1->addWidget(fromCombo);
    layout1->addWidget(fromLE);
    layout1->addStretch();
    layout1->addWidget(problemLabel);
    layout1->addWidget(problemLE);

    layout2 = new QHBoxLayout;
    layout2->addWidget(filtersLabel);
    layout2->addWidget(publicCheckBox);
    layout2->addWidget(correctCheckBox);
    layout2->addStretch();
    layout2->addWidget(countCheckBox);
    layout2->addWidget(countLE);

    upLayout1 = new QVBoxLayout;
    upLayout1->addLayout(layout1);
    upLayout1->addLayout(layout2);

    upLayout2 = new QHBoxLayout;
    upLayout2->addStretch();
    upLayout2->addLayout(upLayout1);
    upLayout2->addWidget(requestButton);
    upLayout2->addWidget(byIDButton);
    upLayout2->addStretch();

    tw = new QTableWidget;
    makeCols(true);

    tw->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tw, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(handleMenu(QPoint)));

    contextMenu = new QMenu(this);
    compilerLogAction = contextMenu->addAction("Compiler &Log");
    viewDetailsAction = contextMenu->addAction("View &Details");
    viewCodeAction = contextMenu->addAction("View &Code");
    changePublicState = contextMenu->addAction("");
    deleteSubmissionAction = contextMenu->addAction("&Delete Submission");

    totalLayout = new QVBoxLayout;
    totalLayout->addLayout(upLayout2);
    totalLayout->addWidget(tw);

    setLayout(totalLayout);

    connect(requestButton, SIGNAL(clicked()), this, SLOT(request()));
    connect(tw, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(handleDblClick(int,int)));
    connect(fromCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(handleFrom(int)));
    connect(countCheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleCount(int)));
    connect(byIDButton, SIGNAL(clicked()), this, SLOT(byIDRequest()));
}

void ViewResults::makeCols(bool score)
{
    tw->setColumnCount(0);
    tw->setColumnCount(8 + score);
    tw->setHorizontalHeaderItem(0, new QTableWidgetItem("Submission ID"));
    tw->setHorizontalHeaderItem(1, new QTableWidgetItem("Username"));
    tw->setHorizontalHeaderItem(2, new QTableWidgetItem("Date And Time"));
    tw->setHorizontalHeaderItem(3, new QTableWidgetItem("Problem Name"));
    tw->setHorizontalHeaderItem(4, new QTableWidgetItem("#Test"));
    tw->setHorizontalHeaderItem(5, new QTableWidgetItem("Judge Result"));

    if (score)
        tw->setHorizontalHeaderItem(6, new QTableWidgetItem("Score"));

    tw->setHorizontalHeaderItem(6 + score, new QTableWidgetItem("Max. Time"));
    tw->setHorizontalHeaderItem(7 + score, new QTableWidgetItem("Max. Memory"));
}

void ViewResults::updateTable()
{
    tw->setRowCount(0);
    tw->setRowCount(data.size());

    bool score = true;
    if (data.size() > 0)
        score = (data[0].scoreDiff() != -1);

    makeCols(score);

    for (int i = 0; i < data.size(); i++)
    {
        QTableWidgetItem *col[tw->columnCount()];
        for (int j = 0; j < tw->columnCount(); j++)
        {
            col[j] = new QTableWidgetItem;
            col[j]->setFlags(Qt::ItemIsEnabled);
        }

        col[0]->setText(QString::number(data[i].submitID()) + (data[i].isPublic() ? " (P)" : ""));
        col[1]->setText(data[i].username());

        QString dateFormat(DATE_TIME_FORMAT);
        col[2]->setText(QDateTime::fromMSecsSinceEpoch(data[i].date()).toString(dateFormat));

        col[3]->setText(data[i].probName());
        col[4]->setText(QString ("%1 / %2").arg(data[i].numTests()).arg(data[i].totalTests()));

        int status = static_cast<int>(data[i].judgeStatus());
        int baseStat = static_cast<int>(StatCode::InQueue);
        col[5]->setText(statusString[status - baseStat]);

        if (score)
        {
            if (data[i].scoreDiff() == -2)
                col[6]->setText("Waiting");
            else
                col[6]->setText("+" + QString::number(data[i].scoreDiff(), 'f', 3));
        }

        col[6 + score]->setText(MainWidget::formatTime(data[i].maximumTime()));
        col[7 + score]->setText(MainWidget::formatSize(data[i].maximumMemory()));

        QColor background = backgroundColors[status - baseStat];
        for (int j = 0; j < tw->columnCount(); j++)
        {
            col[j]->setBackgroundColor(background);
            tw->setItem(i, j, col[j]);
        }
    }

    tw->resizeColumnsToContents();
}

void ViewResults::init()
{
    data.clear();
    updateTable();

    problemLE->setText(QString());
    fromLE->setText(QString());
    countCheckBox->setChecked(false);
    publicCheckBox->setChecked(false);
    correctCheckBox->setChecked(false);

    fromCombo->clear();
    fromCombo->addItem("You");
    fromCombo->addItem("All");
    if (mw->isAdmin())
        fromCombo->addItem("User:");
    fromCombo->setCurrentIndex(0);
    handleFrom(0);

    byIDButton->setVisible(mw->isAdmin());

    countLE->setVisible(mw->isAdmin());
    countCheckBox->setVisible(mw->isAdmin());
    countCheckBox->setChecked(false);
    handleCount(Qt::Unchecked);
}

void ViewResults::onSwitch()
{

}

void ViewResults::request()
{
    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(resultDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::ResultRequest);
        DataTable RT;
        if (fromCombo->currentIndex() == 0)
            RT["from"] = mw->currentUsername();
        else if (fromCombo->currentIndex() == 2 && !fromLE->text().isEmpty())
            RT["from"] = fromLE->text();
        if (!problemLE->text().isEmpty())
            RT["problem"] = problemLE->text();
        if (publicCheckBox->checkState() == Qt::Checked)
            RT["public"] = true;
        if (correctCheckBox->checkState() == Qt::Checked)
            RT["correct"] = true;
        if (countCheckBox->checkState() == Qt::Checked)
        {
            if (!countLE->text().isEmpty())
                RT["count"] = countLE->text().toInt();
        }
        else
            RT["count"] = "default";
        (*DS) << RT;
        mw->connection()->send();
    }
}

void ViewResults::resultDone(StatCode stat, const QByteArray &BA)
{
    QDataStream DS(BA);
    DS.setVersion(QDataStream::Qt_4_8);

    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        data.clear();
        DS >> data;
        updateTable();
    }
}

void ViewResults::handleDblClick(int i, int j)
{
    if (j == 5)
    {
        if (data[i].judgeStatus() == StatCode::CompleteResult)
                showSubmissionDetails(i);
        else if (data[i].judgeStatus() == StatCode::CompileError)
            requestCompilerLog(i);
    }
    else if (j == 4)
    {
        if (data[i].judgeStatus() == StatCode::CompleteResult)
            showSubmissionDetails(i);
    }
    else if (j == 0)
        requestViewCode(i);
}

void ViewResults::showSubmissionDetails(int row)
{
    QString title = QString("Submission #%1").arg(data[row].submitID());
    SubmitDetails *SD = new SubmitDetails(title, data[row].completeResult(), this);
    SD->setAttribute(Qt::WA_DeleteOnClose);
    SD->show();
    SD->raise();
    SD->activateWindow();
}

void ViewResults::requestCompilerLog(int row)
{
    DataPointer DS;

    if (DS = mw->connection()->prepare(this, SLOT(compilerLogDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::CompilerLogRequest);
        (*DS) << data[row].submitID();

        mw->connection()->send();
    }
}

void ViewResults::requestViewCode(int row)
{
    DataPointer DS;

    if (DS = mw->connection()->prepare(this, SLOT(viewCodeDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::ViewCodeRequest);
        (*DS) << data[row].submitID();
        mw->connection()->send();
    }
}

void ViewResults::changePubState(int row)
{
    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(changePubStateDone(StatCode,QByteArray))))
    {
        lastRow = row;
        (*DS) << static_cast<int>(StatCode::ChangePublicState);
        (*DS) << data[row].submitID();
        (*DS) << !data[row].isPublic();
        mw->connection()->send();
    }
}

void ViewResults::deleteSubmission(int row)
{
    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(deleteDone(StatCode,QByteArray))))
    {
        lastRow = row;
        (*DS) << static_cast<int>(StatCode::DeleteSubmission);
        (*DS) << data[row].submitID();
        mw->connection()->send();
    }
}

void ViewResults::handleMenu(const QPoint &pos)
{
    QPoint globalPos = tw->mapToGlobal(pos);
    QTableWidgetItem *curItem = tw->itemAt(pos);

    if (curItem)
    {
        int row = tw->row(curItem);

        viewDetailsAction->setEnabled(data[row].judgeStatus() == StatCode::CompleteResult);
        viewDetailsAction->setVisible(data[row].judgeStatus() == StatCode::CompleteResult);

        deleteSubmissionAction->setEnabled(mw->isAdmin());
        deleteSubmissionAction->setVisible(mw->isAdmin());

        changePublicState->setVisible(mw->isAdmin());
        changePublicState->setEnabled(mw->isAdmin());
        changePublicState->setText(data[row].isPublic() ? "Make &Unpublic" : "Make &Public");

        QAction *action = contextMenu->exec(globalPos);
        if (action == compilerLogAction)
            requestCompilerLog(row);
        else if (action == viewDetailsAction)
            showSubmissionDetails(row);
        else if (action == viewCodeAction)
            requestViewCode(row);
        else if (action == changePublicState)
            changePubState(row);
        else if (action == deleteSubmissionAction)
        {
            if (QMessageBox::warning(this, "Delete Submission", QString("Are you sure you want to delete submission #%1?").arg(data[row].submitID())
                                     ,QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                deleteSubmission(row);
        }
    }
}

void ViewResults::compilerLogDone(StatCode stat, const QByteArray &BA)
{
    QDataStream DS(BA);
    DS.setVersion(QDataStream::Qt_4_8);

    if (!mw->handleGeneralStatCodes(stat))
    {
        if (stat == StatCode::CannotViewSubmission)
            QMessageBox::information(this, "Not Possible", "You cannot see details for this problem.");
        else if (stat == StatCode::OperationSuccessful)
        {
            QString cl;
            DS >> cl;
            TextDialog TD("Compiler Log", cl, this);
            TD.exec();
        }
    }
}

void ViewResults::viewCodeDone(StatCode stat, const QByteArray &BA)
{
    QDataStream DS(BA);
    DS.setVersion(QDataStream::Qt_4_8);

    if (!mw->handleGeneralStatCodes(stat))
    {
        if (stat == StatCode::CannotViewSubmission)
            QMessageBox::information(this, "Not Possible", "You cannot see details for this problem.");
        else if (stat == StatCode::OperationSuccessful)
        {
            QString sc;
            DS >> sc;
            TextDialog TD("View Code", sc, this);
            TD.exec();
        }
    }
}

void ViewResults::changePubStateDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        data[lastRow] = data[lastRow].invertedPublicState();
        updateTable();
    }
}

void ViewResults::deleteDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        data.removeAt(lastRow);
        updateTable();
    }
}

void ViewResults::rejudgeDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        data[lastRow] = data[lastRow].setInQueue();
        updateTable();
    }
}

void ViewResults::handleFrom(int cur)
{
    fromLE->setVisible(cur == 2);
}

void ViewResults::handleCount(int cur)
{
    if (cur == Qt::Checked)
    {
        countLE->setText("");
        countLE->setEnabled(true);
    }
    else
    {
        countLE->setText("Default");
        countLE->setEnabled(false);
    }
}

void ViewResults::byIDRequest()
{
    bool ok;
    int ID = QInputDialog::getInt(this, "By ID Request", "Enter submission ID:", 0,
                                  std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), 1, &ok);

    if (ok)
    {
        DataPointer DS;
        if (DS = mw->connection()->prepare(this, SLOT(resultDone(StatCode,QByteArray))))
        {
            (*DS) << static_cast<int>(StatCode::ResultRequest);
            DataTable RT;
            RT["id"] = ID;
            (*DS) << RT;
            mw->connection()->send();
        }
    }
}
