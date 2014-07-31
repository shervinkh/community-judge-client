/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef PROBLEMMANAGEMENT_H
#define PROBLEMMANAGEMENT_H

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

class ProblemManagement : public QWidget
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
    QPushButton *rejudgeButton;
    QVBoxLayout *manageButtons;

    QListWidget *lst;
    QHBoxLayout *lstLayout;

    QPushButton *addButton;
    QHBoxLayout *addLayout;

    QVBoxLayout *totalLayout;

    QList<QString> data;

    QString scoreListCallback;
    QString contestListCallback;

    QList<QString> scores;
    QList<QString> contests;

    void updateList();

public:
    explicit ProblemManagement(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void onSwitch();

private slots:
    void handleSearchMode(int id);
    void request();
    void requestDone(StatCode stat, const QByteArray &BA);
    void addNewProblem();
    void editProblem();
    void editProblemStage2(StatCode stat, const QByteArray &BA);
    void removeProblem();
    void rejudgeProblem();
    void operationDone(StatCode stat, const QByteArray &);

    bool updateScoresList(const QString &name);
    void scoresDone(StatCode stat, const QByteArray &BA);

    bool updateContestList(const QString &name);
    void contestDone(StatCode stat, const QByteArray &BA);

    void networkOperationFailed();
};

#endif // PROBLEMMANAGEMENT_H
