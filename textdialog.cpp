/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "textdialog.h"
#include "scrollabletextarea.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

TextDialog::TextDialog(const QString &tit, const QString &text, QWidget *parent) :
    QDialog(parent)
{
    title = new QLabel(QString("<b>%1: </b>").arg(tit));

    QFont fnt;
    fnt.setPointSize(15);
    title->setFont(fnt);

    STA = new ScrollableTextArea(text);

    totalLayout = new QVBoxLayout;
    totalLayout->addWidget(title);
    totalLayout->addWidget(STA);

    setLayout(totalLayout);
}
