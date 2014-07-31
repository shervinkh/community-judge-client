/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include <QWidget>
#include <QMap>
#include "general.h"

class QListWidget;
class QPushButton;
class QLabel;
class QLineEdit;
class QCheckBox;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class MainWidget;

class ServerConfig : public QWidget
{
    Q_OBJECT
private:
    DataTable original, now, description, type;

    MainWidget *mw;

    QListWidget *configList;

    QPushButton *getConfig;
    QPushButton *setConfig;

    QLabel *caution;

    QLabel *nameLabel;
    QLabel *name;
    QLabel *descLabel;
    QLabel *desc;
    QLabel *valueLabel;

    QWidget *dataWidget;

    QHBoxLayout *buttonLayout;
    QGridLayout *configView;
    QVBoxLayout *rightLayout;
    QHBoxLayout *downLayout;
    QVBoxLayout *totalLayout;

    QLineEdit *valueEdit;
    QCheckBox *valueCheckBox;

    void updateData();
    void initData();

public:
    explicit ServerConfig(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void onSwitch();

signals:

private slots:
    void setData();
    void getData();
    void handleChange();

    void checkBoxChanged();
    void lineEditChanged();

public slots:
    void getDone(StatCode stat, const QByteArray &BA);
    void setDone(StatCode stat, const QByteArray &);
};

#endif // SERVERCONFIG_H
