/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QWidget>
#include "general.h"

class MainWidget;
class ProblemManagement;
class ScorePlanManager;
class SubmissionPurge;
class UserManagement;
class ContestManagement;
class ServerConfig;
class QPushButton;
class QVBoxLayout;
class QStackedWidget;
class QHBoxLayout;
class QLabel;
class QListWidget;
class ScoreboardConfig;

class AdminPanel : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;
    ProblemManagement *PM;
    ContestManagement *CM;
    UserManagement *UM;
    ScorePlanManager *SPM;
    ServerConfig *SC;
    SubmissionPurge *SP;
    ScoreboardConfig *SBC;

    QListWidget *buttons;
    QPushButton *backButton;

    QStackedWidget *sw;

    QVBoxLayout *rightLayout;
    QHBoxLayout *totalLayout;

    QLabel *connectedToLabel;
    QHBoxLayout *userLayout;

public:
    explicit AdminPanel(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();

public slots:
    void onSwitch();

signals:
    void userPanel();

private slots:
    void selectChanged(int sel);
};

#endif // ADMINPANEL_H
