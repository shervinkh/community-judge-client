/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include <QApplication>
#include <QInputDialog>
#include <QIcon>
#include "connection.h"
#include "mainwidget.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<StatCode>("StatCode");

    QApplication a(argc, argv);

    QApplication::setApplicationName("Community Judge Client");
    QApplication::setApplicationVersion(MainWidget::version());
    QApplication::setWindowIcon(QIcon(":/Images/icon.png"));

    Connection conn;
    MainWidget MW(&conn);
    MW.setWindowTitle("Community Judge Client");
    MW.show();

    return a.exec();
}
