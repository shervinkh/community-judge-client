/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef VIEWPROFILE_H
#define VIEWPROFILE_H

#include <QWidget>
#include "general.h"

class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class MainWidget;

class ViewProfile : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    QPushButton *requestButton;
    QHBoxLayout *buttonLayout;

    QLabel *unameLabel;
    QLabel *uname;
    QPushButton *resetPassword;

    QLabel *regOnLabel;
    QLabel *regOn;

    QLabel *descLabel;
    QLabel *desc;

    QLabel *numSubmitLabel;
    QLabel *numSubmit;
    QLabel *numCorrectLabel;
    QLabel *numCorrect;
    QLabel *scoreLabel;
    QLabel *score;

    QPushButton *recordButton;
    QHBoxLayout *recordLayout;

    QGridLayout *grid;
    QHBoxLayout *fixedDownLayout;
    QVBoxLayout *total;

    QByteArray lastPass;

public:
    explicit ViewProfile(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void onSwitch();

signals:

private slots:
    void request();
    void showRecord();

public slots:
    void changePassword();
    void changePasswordDone(StatCode stat, const QByteArray &);
    void requestDone(StatCode stat, const QByteArray &BA);
};

#endif // VIEWPROFILE_H
