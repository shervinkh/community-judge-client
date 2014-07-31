/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QList>
#include <QUrl>
#include "general.h"

class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QStackedWidget;
class QPushButton;
class Connection;
class LoginWidget;
class UserWidget;
class AdminPanel;

class MainWidget : public QWidget
{
    Q_OBJECT
private:
    static const QString ver;
    static const QString verDate;

    Connection *conn;

    QLabel *title;
    QHBoxLayout *titleLayout;

    LoginWidget *loginP;
    UserWidget *userP;
    AdminPanel *adminP;
    QStackedWidget *sw;

    QLabel *statusLabel;
    QLabel *status;
    QPushButton *abortButton;
    QHBoxLayout *statusLayout;
    QVBoxLayout *totalLayout;

    DataTable auth;

    bool adminFlag;
    QList<QPair<QString, QUrl>> serverList;

    QString rememberedUser;
    QByteArray rememberedPass;
    QUrl rememberedHost;

    void saveState();
    void loadState();

public:
    explicit MainWidget(Connection *con, QWidget *parent = Q_NULLPTR);
    ~MainWidget();

    Connection *connection() {return conn;}

    bool handleGeneralStatCodes(StatCode retCode);
    void showAboutDialog();
    static QByteArray hashPassword(const QByteArray &input);
    static QString formatSize(qint64 bytes);
    static QString formatTime(qint64 time);
    static QString version() {return ver;}
    static QString versionDate() {return verDate;}

    QList<QPair<QString, QUrl>> &servers() {return serverList;}

    void setAuth(const QString &uname, const QByteArray &pass);
    void setRememberedUsername(const QString &username) {rememberedUser = username;}
    void setRememberedPassword(const QByteArray &password) {rememberedPass = password;}
    QString rememberedUsername() const {return rememberedUser;}
    QByteArray rememberedPassword() const {return rememberedPass;}
    QUrl rememberedHostAddress() const {return rememberedHost;}

    QString currentUsername() const {return auth["user"].toString();}
    QByteArray currentPassword() const {return auth["pass"].toByteArray();}
    bool isAdmin() const {return adminFlag;}
    void setAdmin(bool a) {adminFlag = a;}
    void setServer(int idx);
    void updateServerSelection();

public slots:
    void updateStatus();
    void switchToLoginPanel();
    void switchToUserPanel();
    void switchToAdminPanel();
    void loginDo();
};

#endif // MAINWIDGET_H
