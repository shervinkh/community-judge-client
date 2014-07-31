/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef ADDSERVERDIALOG_H
#define ADDSERVERDIALOG_H

#include <QDialog>
#include <QUrl>
#include "general.h"

class QLabel;
class QLineEdit;
class QCheckBox;
class QDialogButtonBox;
class QGridLayout;
class QVBoxLayout;

class AddServerDialog : public QDialog
{
    Q_OBJECT
private:
    QLabel *nameLabel;
    QLineEdit *nameLE;

    QLabel *addressLabel;
    QLineEdit *addressLE;

    QLabel *portLabel;
    QLineEdit *portLE;

    QCheckBox *sslCheck;

    QDialogButtonBox *buttonBox;
    QGridLayout *fields;
    QVBoxLayout *totalLayout;

public:
    explicit AddServerDialog(QWidget *parent = Q_NULLPTR);

    QString name() const;
    QUrl url() const;

private slots:
    void validateForm();
};

#endif // ADDSERVERDIALOG_H
