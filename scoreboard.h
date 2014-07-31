/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QWidget>
#include "general.h"

class MainWidget;
class QLabel;
class QSpinBox;
class QPushButton;
class QTableWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QComboBox;

class Scoreboard : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    QPushButton *refreshButton;

    QLabel *scoreboardLabel;
    QComboBox *scoreboard;

    QLabel *factorLabel;
    QComboBox *factorCB;

    QLabel *pageLabel;
    QSpinBox *pageSB;

    QPushButton *requestButton;
    QHBoxLayout *upLayout;

    QLabel *minLabel;
    QLabel *minValue;
    QLabel *avgLabel;
    QLabel *avgValue;
    QLabel *maxLabel;
    QLabel *maxValue;
    QGridLayout *stat;

    QTableWidget *tw;
    QVBoxLayout *totalLayout;

    QList<DataTable> listData;

    QList<DataTable> data;
    int scoreboardType;
    qreal minVal;
    qreal avgVal;
    qreal maxVal;

    QHBoxLayout *scoreLayout;
    QVBoxLayout *statLayout;

    bool available;
    bool colorData;

    void updateTable();
    void updateScoreboardList();

public:
    explicit Scoreboard(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void onSwitch();

private slots:
    void request();
    void requestList();
    void handleScoreboardChange(int row);

public slots:
    void handleRequest(StatCode stat, const QByteArray &BA);
    void handleListRequest(StatCode stat, const QByteArray &BA);
};

#endif // SCOREBOARD_H
