/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "scrollabletextarea.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

ScrollableTextArea::ScrollableTextArea(const QString &text, QWidget *parent) :
    QScrollArea(parent)
{
    label = new QLabel(text);
    label->setWordWrap(true);
    setWidget(label);
}
