/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef USERRECORDDIALOG_H
#define USERRECORDDIALOG_H

#include <QDialog>
#include "general.h"

class QLabel;
class QSpinBox;
class QPushButton;
class QTableWidget;
class QHBoxLayout;
class QVBoxLayout;
class MainWidget;

class UserRecordDialog : public QDialog
{
    Q_OBJECT
private:
    MainWidget *mw;

    QString user;

    QLabel *title;
    QLabel *pageLabel;
    QSpinBox *pageSB;
    QPushButton *reqButton;
    QHBoxLayout *topLayout;

    QTableWidget *tw;

    QVBoxLayout *totalLayout;

    QList<DataTable> data;

    void updateTable();

public:
    explicit UserRecordDialog(MainWidget *MW, const QString &_user, QWidget *parent = Q_NULLPTR);

public slots:
    void requestDone(StatCode stat, const QByteArray &BA);

private slots:
    void request();
};

#endif // USERRECORDDIALOG_H
