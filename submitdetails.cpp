/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "submitdetails.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

SubmitDetails::SubmitDetails(const QString &tit, QList<qint8> res, QWidget *parent) :
    QDialog(parent)
{
    title = new QLabel(QString("<b>%1: </b>").arg(tit));
    QFont fnt;
    fnt.setPointSize(15);
    title->setFont(fnt);

    tw = new QTableWidget;

#if QT_VERSION >= 0x050000
    tw->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
#endif

    tw->setColumnCount(2);
    tw->setHorizontalHeaderItem(0, new QTableWidgetItem());
    tw->setHorizontalHeaderItem(1, new QTableWidgetItem());

    tw->setRowCount(res.size());
    for (int i = 0; i < res.size(); i++)
    {
        tw->setVerticalHeaderItem(i, new QTableWidgetItem());

        QTableWidgetItem *first, *second;
        first = new QTableWidgetItem(QString("Testcase #%1").arg(i + 1));
        first->setFlags(Qt::ItemIsEnabled);

        int status = static_cast<int>(res[i]);
        int baseStat = static_cast<int>(StatCode::InQueue);
        second = new QTableWidgetItem(statusString[status - baseStat]);
        second->setFlags(Qt::ItemIsEnabled);

        first->setBackgroundColor(backgroundColors[status - baseStat]);
        second->setBackgroundColor(backgroundColors[status - baseStat]);

        tw->setItem(i, 0, first);
        tw->setItem(i, 1, second);
    }

    tw->resizeColumnsToContents();

    totalLayout = new QVBoxLayout;
    totalLayout->addWidget(title);
    totalLayout->addWidget(tw);

    setLayout(totalLayout);
    setWindowTitle("Submission Details");
}
