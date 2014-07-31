/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "scoreboard.h"
#include "mainwidget.h"
#include "connection.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

Scoreboard::Scoreboard(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    refreshButton = new QPushButton("Re&fresh List");
    refreshButton->setMinimumSize(refreshButton->sizeHint() * 1.3);

    scoreboardLabel = new QLabel("Scoreboard: ");
    scoreboard = new QComboBox;
    scoreboard->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    scoreboardLabel->setBuddy(scoreboard);

    factorLabel = new QLabel("Factor: ");
    factorCB = new QComboBox;
    factorCB->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    factorLabel->setBuddy(factorCB);

    pageLabel = new QLabel("Page: ");
    pageSB = new QSpinBox;
    pageSB->setMinimum(1);
    pageLabel->setBuddy(pageSB);

    requestButton = new QPushButton("Re&quest");
    requestButton->setMinimumSize(requestButton->sizeHint() * 1.3);

    upLayout = new QHBoxLayout;
    upLayout->addStretch();
    upLayout->addWidget(refreshButton);
    upLayout->addStretch();
    upLayout->addWidget(scoreboardLabel);
    upLayout->addWidget(scoreboard);
    upLayout->addStretch();
    upLayout->addWidget(factorLabel);
    upLayout->addWidget(factorCB);
    upLayout->addStretch();
    upLayout->addWidget(pageLabel);
    upLayout->addWidget(pageSB);
    upLayout->addStretch();
    upLayout->addWidget(requestButton);
    upLayout->addStretch();

    tw = new QTableWidget;

#if QT_VERSION >= 0x050000
    tw->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
#endif

    minLabel = new QLabel("<b>Minimum: </b>");
    minValue = new QLabel;
    avgLabel = new QLabel("<b>Average: </b>");
    avgValue = new QLabel;
    maxLabel = new QLabel("<b>Maximum: </b>");
    maxValue = new QLabel;

    stat = new QGridLayout;
    stat->addWidget(minLabel, 0, 0);
    stat->addWidget(minValue, 0, 1);
    stat->addWidget(avgLabel, 1, 0);
    stat->addWidget(avgValue, 1, 1);
    stat->addWidget(maxLabel, 2, 0);
    stat->addWidget(maxValue, 2, 1);

    statLayout = new QVBoxLayout;
    statLayout->addStretch();
    statLayout->addLayout(stat);
    statLayout->addStretch();

    scoreLayout = new QHBoxLayout;
    scoreLayout->addStretch();
    scoreLayout->addWidget(tw);
    scoreLayout->addLayout(statLayout);
    scoreLayout->addStretch();

    totalLayout = new QVBoxLayout;
    totalLayout->addLayout(upLayout);
    totalLayout->addLayout(scoreLayout);
    scoreboardType = 0;

    connect(requestButton, SIGNAL(clicked()), this, SLOT(request()));
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(requestList()));
    connect(scoreboard, SIGNAL(currentIndexChanged(int)), this, SLOT(handleScoreboardChange(int)));

    setLayout(totalLayout);
}

void Scoreboard::init()
{
    scoreboardType = -1;
    data.clear();
    listData.clear();
    updateScoreboardList();

    minValue->setText("N/A");
    avgValue->setText("N/A");
    maxValue->setText("N/A");
    updateTable();
    pageSB->setValue(1);
}

void Scoreboard::onSwitch()
{
    if (!mw->connection()->isBusy())
        requestList();
}

void Scoreboard::request()
{
    DataPointer DS;
    int idx = scoreboard->currentIndex();
    if (idx != -1 && (DS = mw->connection()->prepare(this, SLOT(handleRequest(StatCode,QByteArray)))))
    {
        (*DS) << static_cast<int>(StatCode::Scoreboard);

        if (factorCB->currentText() == "Score")
            scoreboardType = 0;
        else if (factorCB->currentText() == "#Corrects")
            scoreboardType = 1;
        else
            scoreboardType = -1;

        (*DS) << listData[idx]["id"].toLongLong();
        (*DS) << scoreboardType;
        (*DS) << (pageSB->value() - 1);
        mw->connection()->send();
    }
}

void Scoreboard::requestList()
{
    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(handleListRequest(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::ScoreboardMetaData);
        mw->connection()->send();
    }
}

void Scoreboard::handleListRequest(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QDataStream DS(BA);
        DS.setVersion(QDataStream::Qt_4_8);
        listData.clear();
        DS >> listData;
        updateScoreboardList();
    }
}

void Scoreboard::handleRequest(StatCode stat, const QByteArray &BA)
{
    QDataStream DS(BA);
    DS.setVersion(QDataStream::Qt_4_8);

    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        data.clear();
        DS >> data;

        DataTable info = data.takeFirst();

        if (info.contains("min"))
        {
            minVal = info["min"].toDouble();
            avgVal = info["avg"].toDouble();
            maxVal = info["max"].toDouble();

            if (scoreboardType == 0)
            {
                minValue->setText(QString::number(minVal, 'f', 3));
                avgValue->setText(QString::number(avgVal, 'f', 3));
                maxValue->setText(QString::number(maxVal, 'f', 3));
            }
            else if (scoreboardType == 1)
            {
                minValue->setText(QString::number(minVal));
                avgValue->setText(QString::number(avgVal));
                maxValue->setText(QString::number(maxVal));
            }

            colorData = true;
        }
        else
        {
            minValue->setText("N/A");
            avgValue->setText("N/A");
            maxValue->setText("N/A");
            colorData = false;
        }

        updateTable();
    }
}

void Scoreboard::updateTable()
{
    if (scoreboardType == -1)
    {
        tw->setRowCount(0);
        tw->setColumnCount(0);
        return;
    }

    tw->setRowCount(0);
    tw->setRowCount(data.size());
    tw->setColumnCount(2);

    tw->setHorizontalHeaderItem(0, new QTableWidgetItem("Username"));
    if (scoreboardType == 0)
        tw->setHorizontalHeaderItem(1, new QTableWidgetItem("Score"));
    else if (scoreboardType == 1)
        tw->setHorizontalHeaderItem(1, new QTableWidgetItem("#Corrects"));

    for (int i = 0; i < tw->rowCount(); i++)
    {
        qreal value = data[i]["value"].toDouble();

        QColor backgroundColor = Qt::white;

        if (colorData)
        {
            if (value >= avgVal)
            {
                qreal redRatio;
                if (maxVal - avgVal > 0.01)
                    redRatio = static_cast<qreal>(maxVal - value) / (maxVal - avgVal);
                else
                    redRatio = 0;
                backgroundColor = QColor(redRatio * 255, 255, 0);
            }
            else
            {
                qreal greenRatio;
                if (avgVal - minVal > 0.01)
                    greenRatio = static_cast<qreal>(avgVal - value) / (avgVal - minVal);
                else
                    greenRatio = 0;
                backgroundColor = QColor(255, 255 * (1 - greenRatio), 0);
            }
        }

        QTableWidgetItem *usernameItem = new QTableWidgetItem(data[i]["username"].toString());
        usernameItem->setFlags(Qt::ItemIsEnabled);
        usernameItem->setBackgroundColor(backgroundColor);
        tw->setItem(i, 0, usernameItem);

        QTableWidgetItem *valueItem = new QTableWidgetItem;
        valueItem->setFlags(Qt::ItemIsEnabled);
        valueItem->setBackgroundColor(backgroundColor);
        if (scoreboardType == 0)
            valueItem->setText(QString::number(data[i]["value"].toDouble(), 'f', 3));
        else if (scoreboardType == 1)
            valueItem->setText(QString::number(data[i]["value"].toLongLong()));

        tw->setItem(i, 1, valueItem);
    }

    tw->resizeColumnsToContents();
}

void Scoreboard::updateScoreboardList()
{
    scoreboard->clear();
    factorCB->clear();

    available = !listData.isEmpty();

    if (!available)
    {
        scoreboard->addItem("Not Available");
        factorCB->addItem("Not Available");
    }
    else
    {
        foreach (DataTable DT, listData)
            scoreboard->addItem(DT["name"].toString());
        scoreboard->setCurrentIndex(0);
        handleScoreboardChange(0);
    }

    scoreboard->setEnabled(available);
    factorCB->setEnabled(available);
    pageSB->setEnabled(available);
    requestButton->setEnabled(available);
}

void Scoreboard::handleScoreboardChange(int row)
{
    if (available)
    {
        factorCB->clear();

        if (row != -1)
        {
            int state = listData[row]["type"].toInt();
            if (state & 1)
                factorCB->addItem("Score");
            if (state & 2)
                factorCB->addItem("#Corrects");
        }
    }
}
