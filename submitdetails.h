/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef SUBMITDETAILS_H
#define SUBMITDETAILS_H

#include <QDialog>
#include "general.h"

class QLabel;
class QTableWidget;
class QVBoxLayout;

class SubmitDetails : public QDialog
{
    Q_OBJECT
private:
    QLabel *title;
    QTableWidget *tw;

    QVBoxLayout *totalLayout;

public:
    explicit SubmitDetails(const QString &tit, QList<qint8> res, QWidget *parent = Q_NULLPTR);
};

#endif // SUBMITDETAILS_H
