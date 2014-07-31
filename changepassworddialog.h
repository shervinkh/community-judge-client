/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>
#include "general.h"

class QLabel;
class QLineEdit;
class QDialogButtonBox;
class QVBoxLayout;
class QGridLayout;

class ChangePasswordDialog : public QDialog
{
    Q_OBJECT
private:
    QLabel *passLabel;
    QLineEdit *passLE;

    QLabel *passConfirmLabel;
    QLineEdit *passConfirmLE;

    QDialogButtonBox *buttons;

    QGridLayout *grid;
    QVBoxLayout *totalLayout;

public:
    explicit ChangePasswordDialog(QWidget *parent = Q_NULLPTR);
    QString password() const;

private slots:
    void validateForm();
};

#endif // CHANGEPASSWORDDIALOG_H
