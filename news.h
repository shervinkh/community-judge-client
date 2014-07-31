/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef NEWS_H
#define NEWS_H

#include <QWidget>
#include "general.h"

class MainWidget;
class QListWidget;
class QVBoxLayout;
class QPushButton;
class QHBoxLayout;

class News : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    QPushButton *reqButton;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *removeButton;
    QHBoxLayout *buttonLayout;

    QListWidget *lw;
    QVBoxLayout *totalLayout;

    QList<DataTable> data;

    Operation lastOp;
    DataTable tmpChange;
    int lastRow;

    void updateList();

public:
    explicit News(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void onSwitch();

public slots:
    void operationDone(StatCode stat, const QByteArray &BA);

private slots:
    void request();
    void add();
    void edit();
    void remove();

};

#endif // NEWS_H
