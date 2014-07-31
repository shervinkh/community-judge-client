/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "scoreboardconfig.h"
#include "mainwidget.h"
#include "connection.h"
#include "scoreboardspecsdialog.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

ScoreboardConfig::ScoreboardConfig(MainWidget *MW, QWidget *parent) :
    QWidget(parent), mw(MW)
{
    pageLabel = new QLabel("Page: ");
    pageSB = new QSpinBox;
    pageSB->setMinimum(1);
    reqButton = new QPushButton("Re&quest");
    reqButton->setMinimumSize(reqButton->sizeHint() * 1.3);
    editButton = new QPushButton("&Edit");
    editButton->setMinimumSize(editButton->sizeHint() * 1.3);

    buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(editButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(pageLabel);
    buttonLayout->addWidget(pageSB);
    buttonLayout->addStretch();
    buttonLayout->addWidget(reqButton);
    buttonLayout->addStretch();

    tw = new QTableWidget;
    tw->setColumnCount(5);
    tw->setHorizontalHeaderItem(0, new QTableWidgetItem("Scoreboard"));
    tw->setHorizontalHeaderItem(1, new QTableWidgetItem("Type"));
    tw->setHorizontalHeaderItem(2, new QTableWidgetItem("Max. Users"));
    tw->setHorizontalHeaderItem(3, new QTableWidgetItem("Each Page"));
    tw->setHorizontalHeaderItem(4, new QTableWidgetItem("Color Data"));
    tw->resizeColumnsToContents();

#if QT_VERSION >= 0x050000
    tw->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
#endif

    tw->setSelectionBehavior(QAbstractItemView::SelectRows);

    totalLayout = new QVBoxLayout;
    totalLayout->addLayout(buttonLayout);
    totalLayout->addWidget(tw);

    setLayout(totalLayout);

    connect(reqButton, SIGNAL(clicked()), this, SLOT(request()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editScoreboard()));
    connect(tw, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(editScoreboard()));
}

void ScoreboardConfig::init()
{
    data.clear();
    updateTable();
}

void ScoreboardConfig::onSwitch()
{

}

void ScoreboardConfig::request()
{
    DataPointer DS;

    if (DS = mw->connection()->prepare(this, SLOT(requestDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::ScoreboardConfig);
        (*DS) << (pageSB->value() - 1);
        mw->connection()->send();
    }
}

void ScoreboardConfig::requestDone(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QDataStream DS(BA);
        DS.setVersion(QDataStream::Qt_4_8);
        data.clear();
        DS >> data;
        updateTable();

        if (data.isEmpty())
            QMessageBox::warning(this, "Error", "No Such Page");
    }
}

void ScoreboardConfig::updateTable()
{
    tw->setRowCount(0);
    tw->setRowCount(data.length());

    for (int i = 0; i < data.length(); i++)
    {
        QTableWidgetItem *col[tw->columnCount()];
        for (int j = 0; j < tw->columnCount(); j++)
        {
            col[j] = new QTableWidgetItem;
            col[j]->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        }

        col[0]->setText(data[i]["name"].toString());
        col[1]->setText(scoreboardTypes[data[i]["type"].toInt()]);
        col[2]->setText(data[i]["max"].toString());
        col[3]->setText(data[i]["page"].toString());
        col[4]->setText(data[i]["color"].toBool() ? "Yes" : "No");

        for (int j = 0; j < tw->columnCount(); j++)
            tw->setItem(i, j, col[j]);
    }

    tw->resizeColumnsToContents();
}

void ScoreboardConfig::editScoreboard()
{
    lastRow = tw->currentRow();

    if (lastRow != -1)
    {
        ScoreboardSpecsDialog dlg(data[lastRow]);

        if (dlg.exec())
        {
            DataTable DT = dlg.resultScoreboard();
            newData = DT;
            DT.remove("name");

            DataPointer DS;
            if (DS = mw->connection()->prepare(this, SLOT(operationDone(StatCode,QByteArray))))
            {
                (*DS) << static_cast<int>(StatCode::ScoreboardEdit);
                (*DS) << DT;
                mw->connection()->send();
            }
        }
    }
}

void ScoreboardConfig::operationDone(StatCode stat, const QByteArray &)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QMessageBox::information(this, "Successful", "Operation was successful");
        data[lastRow] = newData;
        updateTable();
    }
}
