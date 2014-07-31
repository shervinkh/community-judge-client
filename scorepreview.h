/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef SCOREPREVIEW_H
#define SCOREPREVIEW_H

#include <QDialog>
#include "general.h"

class QTableWidget;

class ScorePreview : public QDialog
{
    Q_OBJECT
private:
    static const int NUM_ATTEMPTS;
    static const int NUM_RANKS;

    QTableWidget *tw;

public:
    explicit ScorePreview(qreal aPenalty, qreal rPenalty, qreal cTo, qreal mul, QWidget *parent = Q_NULLPTR);
};

#endif // SCOREPREVIEW_H
