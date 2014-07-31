/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>
#include "general.h"

class MainWidget;
class Submission;
class ViewResults;
class QPushButton;
class QVBoxLayout;
class QStackedWidget;
class QHBoxLayout;
class QLabel;
class ViewProfile;
class Scoreboard;
class QTabWidget;
class ProblemView;
class Contests;
class News;

class UserWidget : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    QTabWidget *expl;
    ProblemView *pv;
    Contests *cts;
    News *nws;

    Submission *sub;
    ViewResults *viewRes;
    ViewProfile *viewProf;
    Scoreboard * sb;

    QPushButton *explButton;
    QPushButton *subButton;
    QPushButton *viewButton;
    QPushButton *scoreboardButton;
    QPushButton *logoutButton;
    QPushButton *adminButton;

    QVBoxLayout *buttonsLayout;
    QStackedWidget *sw;

    QVBoxLayout *rightLayout;
    QHBoxLayout *totalLayout;

    QLabel *connectedToLabel;
    QPushButton *viewProfileButton;
    QHBoxLayout *userLayout;

signals:
    void adminPanel();
    void logout();

public:
    explicit UserWidget(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();

public slots:
    void onSwitch();

private slots:
    void selectSub();
    void selectView();
    void selectAdmin();
    void selectScoreboard();
    void selectExplore();
    void viewProfile();
    void exploreTabOnSwitch();
};

#endif // USERWIDGET_H
