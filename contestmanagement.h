/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef CONTESTMANAGEMENT_H
#define CONTESTMANAGEMENT_H

#include <QWidget>
#include "general.h"

class MainWidget;
class QComboBox;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QListWidget;
class QVBoxLayout;
class QLabel;

class ContestManagement : public QWidget
{
    Q_OBJECT
private:
    Operation lastOp;
    int lastRow;
    QString newName;

    MainWidget *mw;

    QComboBox *searchMode;
    QLabel *searchConstr;
    QLineEdit *searchTerm;
    QPushButton *searchReq;
    QHBoxLayout *searchLayout;

    QPushButton *editButton;
    QPushButton *removeButton;
    QVBoxLayout *manageButtons;

    QListWidget *lst;
    QHBoxLayout *lstLayout;

    QPushButton *addButton;
    QHBoxLayout *addLayout;

    QVBoxLayout *totalLayout;

    QList<QString> data;

    void updateList();

public:
    explicit ContestManagement(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void onSwitch();

private slots:
    void handleSearchMode(int id);
    void request();
    void addNewContest();
    void editContest();
    void removeContest();

public slots:
    void editContestStage2(StatCode stat, const QByteArray &BA);
    void requestDone(StatCode stat, const QByteArray &BA);
    void operationDone(StatCode stat, const QByteArray &);
    void networkOperationFailed();
};

#endif // CONTESTMANAGEMENT_H
