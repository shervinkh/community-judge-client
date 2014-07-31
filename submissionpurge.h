/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef SUBMISSIONPURGE_H
#define SUBMISSIONPURGE_H

#include <QWidget>
#include "general.h"

class MainWidget;
class QLabel;
class QLineEdit;
class QCheckBox;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QDateTimeEdit;

class SubmissionPurge : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    QLabel *caution;

    QLabel *usernameLabel;
    QLineEdit *usernameLE;
    QHBoxLayout *username;

    QLabel *problemLabel;
    QLineEdit *problemLE;
    QHBoxLayout *problem;

    QCheckBox *lastxLabel;
    QLineEdit *lastxLE;
    QLabel *x1;
    QHBoxLayout *lastx;

    QCheckBox *lastCorxLabel;
    QLineEdit *lastCorxLE;
    QLabel *x2;
    QHBoxLayout *lastCorx;

    QCheckBox *newerThanLabel;
    QDateTimeEdit *newerThan;

    QCheckBox *olderThanLabel;
    QDateTimeEdit *olderThan;

    QCheckBox *nonCompletes;
    QCheckBox *completes;

    QCheckBox *nonPublics;
    QCheckBox *publics;

    QCheckBox *nonZero;

    QGridLayout *theGrid;

    QPushButton *purgeButton;
    QPushButton *filesButton;

    QHBoxLayout *buttonLayout;
    QHBoxLayout *button2Layout;

    QVBoxLayout *total;

public:
    explicit SubmissionPurge(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void onSwitch();

signals:

private slots:
    void handleLastX();
    void handleLastCorX();
    void handleNewerThan();
    void handleOlderThan();
    void request();
    void requestFiles();

public slots:
    void handleResult(StatCode stat, const QByteArray &);
};

#endif // SUBMISSIONPURGE_H
