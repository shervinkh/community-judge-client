/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include <QWidget>
#include "general.h"

class MainWidget;
class QLabel;
class QComboBox;
class QLineEdit;
class QCheckBox;
class QPushButton;
class QTableWidget;
class QHBoxLayout;
class QVBoxLayout;
class QAction;
class QMenu;

class UserManagement : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    QLabel *unameLabel;
    QLineEdit *unameLE;
    QLabel *countLabel;
    QLineEdit *countLE;

    QLabel *filterLabel;
    QCheckBox *enabledCheckBox;
    QLabel *sortLabel;
    QComboBox *sortCombo;
    QComboBox *sortTypeCombo;

    QPushButton *requestButton;

    QTableWidget *tw;

    QHBoxLayout *layT1;
    QHBoxLayout *layT2;
    QVBoxLayout *layT12;
    QHBoxLayout *layUp;
    QVBoxLayout *layout;

    QList<User> data;

    QMenu *contextMenu;
    QAction *resetPasswdAction;
    QAction *changeAdminAction;
    QAction *deleteAction;
    QAction *changeActivationAction;
    QAction *editDescAction;
    QAction *renameAction;
    QAction *recordAction;

    DataTable newAuth;

    void updateData();

    int lastRow;
    User *newUsr;

public:
    explicit UserManagement(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void onSwitch();

private slots:
    void handleSortType();
    void handleMenu(const QPoint &pos);
    void handleDblClick(int i, int j);
    void request();

    void resetPassword(int idx);
    void changeAdmin(int idx);
    void deleteUser(int idx);
    void changeActivation(int idx);
    void editDescription(int idx);
    void rename(int idx);
    void userRecord(int idx);

public slots:
    void requestDone(StatCode stat, const QByteArray &BA);
    void operationDone(StatCode stat, const QByteArray &);
};

#endif // USERMANAGEMENT_H
