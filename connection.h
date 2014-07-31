/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QUrl>
#include <QSharedPointer>
#include <QMap>
#include <QVariant>
#include "general.h"
#include "mainwidget.h"

class QTcpSocket;
class QSslSocket;
class QProgressDialog;

class Connection : public QObject
{
    Q_OBJECT
public:
    enum class State {Disconnected, Connected, Connecting};

private:
    QString address;
    quint16 port;
    bool isSsl;
    int resCode;

    QTcpSocket *curSocket;
    QTcpSocket *tcpSocket;
    QSslSocket *sslSocket;

    QDataStream server;
    QByteArray sendingData;
    QByteArray receivedData;

    bool busy;
    bool aborted;
    int length;

    DataTable auth;

    QProgressDialog *progress;
    bool progressShown;

    void handleProgress();

signals:
    void data(StatCode, QByteArray);
    void stateChanged();
    void unsuccessfullyDone();

public:
    explicit Connection(QObject *parent = Q_NULLPTR);

    void send();
    State giveState() const;
    bool isBusy() const {return busy;}

    void setAuth(const QMap<QString, QVariant> &in) {auth = in; auth["version"] = MainWidget::version();}
    void setAddress(const QUrl &url) {address = url.host(); port = url.port(); isSsl = (url.scheme() == "ssl");}
    QUrl currentAddress() const {QUrl url; url.setHost(address); url.setPort(port); url.setScheme(isSsl ? "ssl" : "tcp"); return url;}

    QSharedPointer<QDataStream> prepare(QObject *receiver, const char *slot, QObject *receiverFailed = Q_NULLPTR,
                                        const char *slotFailed = Q_NULLPTR);

    void showProgressDialog();

private slots:
    void phase1();
    void readData();
    void failure();
    void success();
    void done();

public slots:
    void disconnectFromServer();
};

#endif // CONNECTION_H
