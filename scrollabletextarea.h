/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef SCROLLABLETEXTAREA_H
#define SCROLLABLETEXTAREA_H

#include <QScrollArea>
#include "general.h"

class QLabel;

class ScrollableTextArea : public QScrollArea
{
    Q_OBJECT
private:
    QLabel *label;

public:
    explicit ScrollableTextArea(const QString &text, QWidget *parent = Q_NULLPTR);
};

#endif // SCROLLABLETEXTAREA_H
