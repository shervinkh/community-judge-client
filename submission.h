/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <QWidget>
#include "general.h"

class QLabel;
class QLineEdit;
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class Connection;
class MainWidget;

class QGridLayout;

class Submission : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    QLabel *problemLabel;
    QLineEdit *problemEdit;

    QLabel *fileLabel;
    QLineEdit *fileEdit;
    QPushButton *fileOpen;

    QPushButton *submit;

    QGridLayout *upLayout;

    QHBoxLayout *buttonLayout;

    QVBoxLayout *totalLayout;

    QLabel *title;
    QHBoxLayout *titleLayout;

    QHBoxLayout *inputLayout;

public:
    explicit Submission(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void onSwitch();

private slots:
    void validateForm();
    void submitAction();
    void submitDone(StatCode stat, const QByteArray &);
    void handleOpen();
};

#endif // SUBMISSION_H
