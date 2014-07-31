/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef PROBLEMSPECSDIALOG_H
#define PROBLEMSPECSDIALOG_H

#include <QDialog>
#include <QList>
#include "general.h"

class QLabel;
class QLineEdit;
class QGridLayout;
class QDialogButtonBox;
class QCheckBox;
class QVBoxLayout;
class QComboBox;

class ProblemSpecsDialog : public QDialog
{
    Q_OBJECT
private:
    QLabel *nameLabel;
    QLineEdit *nameEdit;

    QLabel *folderLabel;
    QLineEdit *folderEdit;

    QLabel *descLabel;
    QLineEdit *descEdit;

    QLabel *descFileLabel;
    QLineEdit *descFileEdit;

    QLabel *ntLabel;
    QLineEdit *ntEdit;

    QLabel *tlLabel;
    QLineEdit *tlEdit;

    QLabel *mlLabel;
    QLineEdit *mlEdit;

    QLabel *scpLabel;
    QComboBox *scpComboBox;

    QLabel *contestLabel;
    QComboBox *contestComboBox;

    QLabel *pubsubLabel;
    QComboBox *pubsub;

    QLabel *othsubLabel;
    QComboBox *othsub;

    QCheckBox *completed;
    QCheckBox *enabled;

    QGridLayout *glayout;

    QVBoxLayout *totalLayout;

    QDialogButtonBox *buttons;

    Problem cur;

public:
    explicit ProblemSpecsDialog(const Problem &prob, const QList<QString> &scps,
                                const QList<QString> &cons, QWidget *parent = Q_NULLPTR);
    Problem resultProblem() const;

private slots:
    void validateForm();
};

#endif // PROBLEMSPECSDIALOG_H
