/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "general.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class MainWidget;
class QCheckBox;

class Login : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    QLabel *loginTitle;

    QLabel *usernameLabel;
    QLineEdit *usernameLE;

    QLabel *passwordLabel;
    QLineEdit *passwordLE;

    QCheckBox *rememberCB;

    QPushButton *loginButton;

    QHBoxLayout *titleLayout;
    QGridLayout *fieldsLayout;
    QHBoxLayout *buttonLayout;
    QVBoxLayout *totalLayout;

    QHBoxLayout *inputLayout;

public:
    explicit Login(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    
signals:
    void loginOK();

private slots:
    void validateForm();
    void login();
    void loginDone(StatCode stat, const QByteArray &);
};

#endif // LOGIN_H
