/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "userrecorddialog.h"
#include "mainwidget.h"
#include "connection.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

UserRecordDialog::UserRecordDialog(MainWidget *MW, const QString &_user, QWidget *parent) :
    QDialog(parent), mw(MW), user(_user)
{
    title = new QLabel(QString("<b>User Record: </b>%1").arg(user));

    pageLabel = new QLabel("Page: ");
    pageSB = new QSpinBox;
    pageSB->setMinimum(1);
    pageLabel->setBuddy(pageSB);

    reqButton = new QPushButton("Re&quest");

    topLayout = new QHBoxLayout;
    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(pageLabel);
    topLayout->addWidget(pageSB);
    topLayout->addStretch();
    topLayout->addWidget(reqButton);

    tw = new QTableWidget;
    tw->setColumnCount(5);
    tw->setHorizontalHeaderItem(0, new QTableWidgetItem("Problem"));
    tw->setHorizontalHeaderItem(1, new QTableWidgetItem("Contest"));
    tw->setHorizontalHeaderItem(2, new QTableWidgetItem("#Submits"));
    tw->setHorizontalHeaderItem(3, new QTableWidgetItem("Correct"));
    tw->setHorizontalHeaderItem(4, new QTableWidgetItem("Score"));

    totalLayout = new QVBoxLayout;
    totalLayout->addLayout(topLayout);
    totalLayout->addWidget(tw);

    connect(reqButton, SIGNAL(clicked()), this, SLOT(request()));

    setLayout(totalLayout);
}

void UserRecordDialog::updateTable()
{
    tw->setRowCount(0);
    tw->setRowCount(data.length());

    for (int i = 0; i < data.length(); i++)
    {
        DataTable DT = data[i];

        QTableWidgetItem *col[tw->columnCount()];
        for (int j = 0; j < tw->columnCount(); j++)
        {
            col[j] = new QTableWidgetItem;
            col[j]->setFlags(Qt::ItemIsEnabled);
        }

        col[0]->setText(DT["problem"].toString());
        col[1]->setText(DT["contest"].toString());
        col[2]->setText(DT["submits"].toString());
        col[3]->setText(DT["correct"].toBool() ? "Yes" : "No");
        col[4]->setText(QString::number(DT["score"].toDouble(), 'f', 3));

        QColor background = DT["correct"].toBool() ? Qt::green : Qt::yellow;
        for (int j = 0; j < tw->columnCount(); j++)
        {
            col[j]->setBackgroundColor(background);
            tw->setItem(i, j, col[j]);
        }
    }

    tw->resizeColumnsToContents();
}

void UserRecordDialog::request()
{
    DataPointer DS;
    if (DS = mw->connection()->prepare(this, SLOT(requestDone(StatCode,QByteArray))))
    {
        (*DS) << static_cast<int>(StatCode::UserRecord);
        (*DS) << user << (pageSB->value() - 1);
        mw->connection()->send();
    }
}

void UserRecordDialog::requestDone(StatCode stat, const QByteArray &BA)
{
    if (!mw->handleGeneralStatCodes(stat) && stat == StatCode::OperationSuccessful)
    {
        QDataStream DS(BA);
        DS.setVersion(QDataStream::Qt_4_8);
        data.clear();
        DS >> data;
        updateTable();
    }
}
