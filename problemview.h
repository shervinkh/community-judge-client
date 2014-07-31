/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef PROBLEMVIEW_H
#define PROBLEMVIEW_H

#include <QWidget>
#include "general.h"

class MainWidget;
class QLabel;
class QComboBox;
class QPushButton;
class QListWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QLineEdit;

class ProblemView : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    QComboBox *searchMode;
    QLabel *searchConstr;
    QLineEdit *searchTerm;
    QPushButton *searchReq;
    QHBoxLayout *searchDownLayout;
    QVBoxLayout *searchLeftLayout;
    QHBoxLayout *searchLayout;

    QLabel *contestLabel;
    QComboBox *contestList;
    QPushButton *refreshButton;
    QHBoxLayout *contestLayout;
    QWidget *contestWidget;

    QListWidget *lw;    
    QVBoxLayout *probsLayout;
    QWidget *probs;

    QVBoxLayout *leftLayout;

    QLabel *nameLabel;
    QLabel *name;
    QLabel *descLabel;
    QLabel *desc;
    QLabel *descFileLabel;
    QLabel *descFile;
    QPushButton *descFileDownload;
    QLabel *numTestsLabel;
    QLabel *numTests;
    QLabel *timeLimitLabel;
    QLabel *timeLimit;
    QLabel *memLimitLabel;
    QLabel *memLimit;
    QLabel *scorePlanLabel;
    QLabel *scorePlan;
    QLabel *completeJudgeLabel;
    QLabel *completeJudge;
    QLabel *pubsubLabel;
    QLabel *pubsub;
    QLabel *othsubLabel;
    QLabel *othsub;

    QGridLayout *probGrid;
    QVBoxLayout *infoLayout;
    QWidget *info;

    QPushButton *getInfo;
    QVBoxLayout *getLayout;

    QList<QString> data;

    QHBoxLayout *totalLayout;

    QString savePath;

    void updateList();
    void setInfo(const Problem &prob);

public:
    explicit ProblemView(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void onSwitch();

signals:

public slots:
    void requestDone(StatCode stat, const QByteArray &BA);
    void infoDone(StatCode stat, const QByteArray &BA);
    void descDone(StatCode stat, const QByteArray &BA);
    void refreshListDone(StatCode stat, const QByteArray &BA);

private slots:
    void handleSearchMode(int id);
    void handleGetInfo();
    void requestInfo();
    void request();
    void refreshList();
    void downloadDescription();
};

#endif // PROBLEMVIEW_H
