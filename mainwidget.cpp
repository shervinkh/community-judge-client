/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "mainwidget.h"
#include "connection.h"
#include "userwidget.h"
#include "loginwidget.h"
#include "adminpanel.h"

#include <QtGui>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include <QCryptographicHash>

const QString MainWidget::ver = "Beta-1";
const QString MainWidget::verDate = "2014/07/22";

MainWidget::MainWidget(Connection *con, QWidget *parent) :
    QWidget(parent), conn(con)
{
    loadState();

    title = new QLabel;
    QFont fnt;
    fnt.setPointSize(20);
    title->setFont(fnt);

    titleLayout = new QHBoxLayout;
    titleLayout->addWidget(title);

    loginP = new LoginWidget(this);
    userP = new UserWidget(this);
    adminP = new AdminPanel(this);

    sw = new QStackedWidget;
    sw->insertWidget(0, loginP);
    sw->insertWidget(1, userP);
    sw->insertWidget(2, adminP);

    connect(loginP, SIGNAL(loginOK()), this, SLOT(loginDo()));
    connect(userP, SIGNAL(adminPanel()), this, SLOT(switchToAdminPanel()));
    connect(adminP, SIGNAL(userPanel()), this, SLOT(switchToUserPanel()));
    connect(userP, SIGNAL(logout()), this, SLOT(switchToLoginPanel()));

    statusLabel = new QLabel("<b>Status: </b>");
    QFont fnt2;
    fnt2.setPointSize(15);
    statusLabel->setFont(fnt2);

    QFont fnt3;
    fnt3.setPointSize(12);
    status = new QLabel("<font color=\"red\">Idle</font>");
    status->setFont(fnt3);

    abortButton = new QPushButton("&Disconnect");
    abortButton->setMinimumSize(abortButton->sizeHint() * 1.2);
    abortButton->setEnabled(false);

    statusLayout = new QHBoxLayout;
    statusLayout->addWidget(statusLabel);
    statusLayout->addWidget(status);
    statusLayout->addStretch();
    statusLayout->addWidget(abortButton);

    connect(abortButton, SIGNAL(clicked()), conn, SLOT(disconnectFromServer()));
    connect(conn, SIGNAL(stateChanged()), this, SLOT(updateStatus()));

    totalLayout = new QVBoxLayout;
    totalLayout->addLayout(titleLayout);
    totalLayout->addWidget(sw);
    totalLayout->addLayout(statusLayout);

    setLayout(totalLayout);

    switchToLoginPanel();
}

MainWidget::~MainWidget()
{
    saveState();
}

void MainWidget::updateServerSelection()
{
    loginP->updateServerSelection();
}

void MainWidget::saveState()
{
    QSettings settings("Shervin Kh.", "Community Judge Client");
    settings.beginGroup("cjc_state");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("remem_user", rememberedUser);
    settings.setValue("remem_pass", rememberedPass);
    settings.setValue("remem_addr", rememberedHost.toEncoded());
    settings.endGroup();

    QFile serversFile("cjc_servers.dat");
    if (serversFile.open(QIODevice::WriteOnly))
    {
        QDataStream DS(&serversFile);
        DS.setVersion(QDataStream::Qt_4_8);
        DS << serverList;
        serversFile.close();
    }
    else
        qDebug() << "Failed to save servers";
}

void MainWidget::loadState()
{
    QSettings settings("Shervin Kh.", "Community Judge Client");
    settings.beginGroup("CJC_State");
    restoreGeometry(settings.value("geometry", "").toByteArray());
    rememberedUser = settings.value("remem_user", "").toString();
    rememberedPass = settings.value("remem_pass", "").toByteArray();
    rememberedHost = QUrl::fromEncoded(settings.value("remem_addr", "").toByteArray());
    settings.endGroup();

    QFile serversFile("cjc_servers.dat");
    if (serversFile.open(QIODevice::ReadOnly))
    {
        QDataStream DS(&serversFile);
        DS.setVersion(QDataStream::Qt_4_8);
        DS >> serverList;
        serversFile.close();
    }
    else
        qDebug() << "Failed to load servers";
}

void MainWidget::switchToLoginPanel()
{
    title->setText("<b>Community Judge Client - Login Panel</b>");
    loginP->init();
    sw->setCurrentIndex(0);
}

void MainWidget::switchToUserPanel()
{
    rememberedHost = conn->currentAddress();

    title->setText("<b>Community Judge Client - User Panel</b>");
    sw->setCurrentIndex(1);
}

void MainWidget::switchToAdminPanel()
{
    title->setText("<b>Community Judge Client - Admin Panel</b>");
    adminP->onSwitch();
    sw->setCurrentIndex(2);
}

void MainWidget::updateStatus()
{
    Connection::State in = conn->giveState();

    if (in == Connection::State::Disconnected)
        status->setText("<font color=\"red\">Idle</font>");
    else if (in == Connection::State::Connecting)
        status->setText("<font color=\"green\">Connecting...</font>");
    else if (in == Connection::State::Connected)
        status->setText("<font color=\"blue\">Connected</font>");

    abortButton->setEnabled(in != Connection::State::Disconnected);
}

bool MainWidget::handleGeneralStatCodes(StatCode retCode)
{
    if (retCode == StatCode::UserNotApproved)
        QMessageBox::information(this, "Error", "Your registeration has not approved yet");
    else if (retCode == StatCode::NoSuchUser)
        QMessageBox::warning(this, "Error", "No such user");
    else if (retCode == StatCode::InvalidPassword)
        QMessageBox::warning(this, "Error", "Invalid password");
    else if (retCode == StatCode::InternalDatabaseError)
        QMessageBox::critical(this, "Error", "Internal database error in server");
    else if (retCode == StatCode::InternalFileOperationError)
        QMessageBox::critical(this, "Error", "Internal file operation error in server");
    else if (retCode == StatCode::NotAuthorized)
        QMessageBox::warning(this, "Error", "Not Authorized");
    else if (retCode == StatCode::IncompatibleVersion)
        QMessageBox::warning(this, "Error", "Your client version is not compatible with the server.");
    else if (retCode == StatCode::NotAvailable)
        QMessageBox::warning(this, "Error", "The service you requested is not available at the moment.");
    else if (retCode == StatCode::CannotProcessAtThisTime)
        QMessageBox::warning(this, "Error", "Cannot process your request at this time.");
    else
        return false;

    return true;
}

void MainWidget::showAboutDialog()
{
    QMessageBox::about(this, "About Community Judge",
                       QString("<b>Community Judge Client</b>"
                               "<br/><b>Version:</b> %1 (%2)"
                               "<br/><b>Developer:</b> Shervin Khastoo (www.shervin.org)"
                               "<br/><b>Comments and Bug report:</b> communityjudge@shervin.org"
                               "<br/><b>License:</b> GPLv3 Or Any Later Version"
                               "<p>This is the client software for Community Judge. A software for grading programs written"
                               " by a group of students or contestants. Currently only working with C/C++ codes. It supports"
                               " time limit and memory limit and custom tester program for problems, and runs programs in a"
                               " controlled enviroment.</p>"
                               "<p>Copyright (C) 2014  Shervin Kh.</p>"
                               "<p>This program is free software: you can redistribute it and/or modify "
                               "it under the terms of the GNU General Public License as published by "
                               "the Free Software Foundation, either version 3 of the License, or "
                               "any later version.<p>"
                               "<p>This program is distributed in the hope that it will be useful, "
                               "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                               "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
                               "GNU General Public License for more details.</p>"
                               "<p>Full license could be found in License file shipped with program "
                               "or at http://www.gnu.org/licenses/</p>"
                               "<b>145c4d596165560ea05f0dc235a4e70e6a33895c</b>")
                       .arg(version()).arg(versionDate()));
}

QByteArray MainWidget::hashPassword(const QByteArray &input)
{
    return QCryptographicHash::hash(input, QCryptographicHash::Md5);
}

void MainWidget::setServer(int idx)
{
    if (idx == -1)
        conn->setAddress(QUrl());
    else
        conn->setAddress(serverList[idx].second);
}

void MainWidget::setAuth(const QString &uname, const QByteArray &pass)
{
    auth["user"] = uname;
    auth["pass"] = pass;

    connection()->setAuth(auth);

    userP->init();
    adminP->init();
}

void MainWidget::loginDo()
{
    userP->init();
    adminP->init();
    userP->onSwitch();
    switchToUserPanel();
}

QString MainWidget::formatSize(qint64 bytes)
{
    long double size = bytes;
    int munit = 0;
    while (size >= 1000)
    {
        size /= 1000;
        munit++;
    }

    return QString::number(size, 'f', 2) + " " + memUnits[munit];
}

QString MainWidget::formatTime(qint64 time)
{
    long double val = time;
    int tunit = 0;
    while (val >= 1000)
    {
        val /= 1000;
        tunit++;
    }

    return QString::number(val, 'f', 2) + " " + timeUnits[tunit];
}
