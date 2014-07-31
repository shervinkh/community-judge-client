/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef REGISTERATION_H
#define REGISTERATION_H

#include <QWidget>
#include "general.h"

class QLabel;
class QLineEdit;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QPushButton;
class MainWidget;

class Registeration : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    QLabel *registerTitle;

    QLabel *usernameLabel;
    QLineEdit *usernameEdit;

    QLabel *passwordLabel;
    QLineEdit *passwordEdit;

    QLabel *passwordConfirmLabel;
    QLineEdit *passwordConfirmLE;

    QLabel *descriptionLabel;
    QLineEdit *descriptionEdit;

    QPushButton *registerButton;

    QHBoxLayout *titleLayout;
    QGridLayout *upLayout;
    QHBoxLayout *buttonLayout;

    QVBoxLayout *totalLayout;

    QHBoxLayout *inputLayout;

public:
    explicit Registeration(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();

private slots:
    void validateForm();
    void sendRequest();
    void registerDone(StatCode stat, const QByteArray &);
};

#endif // REGISTERATION_H
