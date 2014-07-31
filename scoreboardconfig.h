/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef SCOREBOARDCONFIG_H
#define SCOREBOARDCONFIG_H

#include <QWidget>
#include "general.h"

class MainWidget;
class QPushButton;
class QTableWidget;
class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QSpinBox;

class ScoreboardConfig : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    int lastRow;
    DataTable newData;

    QLabel *pageLabel;
    QSpinBox *pageSB;
    QPushButton *reqButton;
    QPushButton *editButton;
    QHBoxLayout *buttonLayout;

    QTableWidget *tw;

    QVBoxLayout *totalLayout;

    QList<DataTable> data;

    void updateTable();

public:
    explicit ScoreboardConfig(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void onSwitch();

public slots:
    void requestDone(StatCode stat, const QByteArray &BA);
    void operationDone(StatCode stat, const QByteArray &);

private slots:
    void request();
    void editScoreboard();
};

#endif // SCOREBOARDCONFIG_H
