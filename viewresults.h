/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef VIEWRESULTS_H
#define VIEWRESULTS_H

#include <QWidget>
#include "general.h"

class QLabel;
class QLineEdit;
class QComboBox;
class QCheckBox;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QTableWidget;
class MainWidget;
class QAction;
class QMenu;

class ViewResults : public QWidget
{
    Q_OBJECT
private:
    MainWidget *mw;

    QLabel *fromLabel;
    QComboBox *fromCombo;
    QLineEdit *fromLE;
    QLabel *problemLabel;
    QLineEdit *problemLE;

    QLabel *filtersLabel;
    QCheckBox *publicCheckBox;
    QCheckBox *correctCheckBox;
    QLabel *countLabel;
    QLineEdit *countLE;
    QCheckBox *countCheckBox;
    QPushButton *requestButton;
    QPushButton *byIDButton;

    QHBoxLayout *layout1;
    QHBoxLayout *layout2;
    QVBoxLayout *upLayout1;
    QHBoxLayout *upLayout2;

    QTableWidget *tw;

    QVBoxLayout *totalLayout;

    QList<SubmissionResult> data;

    QAction *compilerLogAction;
    QAction *viewDetailsAction;
    QAction *viewCodeAction;
    QAction *changePublicState;
    QAction *deleteSubmissionAction;
    QMenu *contextMenu;

    int lastRow;

    void updateTable();

    void requestCompilerLog(int row);
    void showSubmissionDetails(int row);
    void requestViewCode(int row);
    void changePubState(int row);
    void deleteSubmission(int row);
    void makeCols(bool score);

public:
    explicit ViewResults(MainWidget *MW, QWidget *parent = Q_NULLPTR);
    void init();
    void onSwitch();

private slots:
    void request();
    void resultDone(StatCode stat, const QByteArray &BA);
    void handleDblClick(int i, int j);
    void compilerLogDone(StatCode stat, const QByteArray &BA);
    void viewCodeDone(StatCode stat, const QByteArray &BA);
    void handleMenu(const QPoint &pos);
    void handleFrom(int cur);
    void handleCount(int cur);
    void changePubStateDone(StatCode stat, const QByteArray &);
    void deleteDone(StatCode stat, const QByteArray &);
    void rejudgeDone(StatCode stat, const QByteArray &);
    void byIDRequest();
};

#endif // VIEWRESULTS_H
