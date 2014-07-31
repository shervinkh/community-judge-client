/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef CONTESTS_H
#define CONTESTS_H

#include <QWidget>
#include "general.h"

class QLabel;
class MainWidget;
class QLabel;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QComboBox;
class QTreeWidget;
class QTreeWidgetItem;

class Contests : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    QComboBox *searchMode;
    QLabel *searchConstr;
    QLineEdit *searchTerm;
    QPushButton *searchReq;
    QPushButton *regButton;

    QHBoxLayout *topLayout;

    QTreeWidget *tw;

    QVBoxLayout *total;

    QList<Contest> data;

    void updateList();

public:
    explicit Contests(MainWidget *MW, QWidget *parent = 0);
    void init();
    void onSwitch();

signals:

private slots:
    void handleSearchMode(int id);
    void request();
    void registerReq();

public slots:
    void requestDone(StatCode stat, const QByteArray &BA);
    void registerReqDone(StatCode stat, const QByteArray &);

};

#endif // CONTESTS_H
