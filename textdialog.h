/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>
#include "general.h"

class QLabel;
class ScrollableTextArea;
class QVBoxLayout;

class TextDialog : public QDialog
{
    Q_OBJECT
private:
    QLabel *title;
    ScrollableTextArea *STA;
    QVBoxLayout *totalLayout;

public:
    explicit TextDialog(const QString &tit, const QString &text, QWidget *parent = Q_NULLPTR);
};

#endif // TEXTDIALOG_H
