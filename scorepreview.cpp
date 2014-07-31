/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include "scorepreview.h"

const int ScorePreview::NUM_ATTEMPTS = 25;
const int ScorePreview::NUM_RANKS = 25;

ScorePreview::ScorePreview(qreal aPenalty, qreal rPenalty, qreal cTo, qreal mul, QWidget *parent) :
    QDialog(parent)
{
    tw = new QTableWidget;
    tw->setRowCount(NUM_ATTEMPTS);
    tw->setColumnCount(NUM_RANKS);

    ScorePlan score;
    score.init(aPenalty, rPenalty, cTo, mul);

    for (int i = 0; i < NUM_ATTEMPTS; i++)
        tw->setVerticalHeaderItem(i, new QTableWidgetItem(QString("Attempt #%1").arg(i + 1)));

    for (int i = 0; i < NUM_RANKS; i++)
        tw->setHorizontalHeaderItem(i, new QTableWidgetItem(QString("Rank #%1").arg(i + 1)));

    for (int i = 0; i < NUM_ATTEMPTS; i++)
        for (int j = 0; j < NUM_RANKS; j++)
        {
            qreal scor = score.query(i + 1, j + 1);
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(scor, 'f', 3));
            item->setFlags(Qt::ItemIsEnabled);

            QColor backgroundColor;
            if (mul > 0)
            {
                qreal ratio = scor / mul;
                backgroundColor.setRgb((1 - ratio) * 255, ratio * 255, 0);
            }
            else
                backgroundColor = Qt::green;

            item->setBackgroundColor(backgroundColor);

            tw->setItem(i, j, item);
        }

    tw->resizeRowsToContents();
    tw->resizeColumnsToContents();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tw);

    setLayout(layout);
}
