/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "general.h"

class Registeration;
class Login;
class QStackedWidget;
class QLabel;
class QPushButton;
class QComboBox;
class MainWidget;
class QHBoxLayout;
class QVBoxLayout;

class LoginWidget : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    Registeration *reg;
    Login *log;

    QPushButton *logButton;
    QPushButton *regButton;
    QPushButton *aboutButton;
    QPushButton *exitButton;

    QVBoxLayout *buttonsLayout;
    QStackedWidget *sw;

    QVBoxLayout *rightLayout;
    QHBoxLayout *totalLayout;

    QLabel *serverLabel;
    QComboBox *serverBox;
    QPushButton *addButton;
    QPushButton *removeButton;
    QHBoxLayout *serverLayout;

public:
    explicit LoginWidget(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void updateServerSelection();
    
signals:
    void loginOK();

private slots:
    void selectLog();
    void selectReg();
    void selectAbout();
    void selectExit();

    void addServer();
    void removeServer();
};

#endif // LOGINWIDGET_H
