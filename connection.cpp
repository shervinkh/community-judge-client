/*
 * This file is part of community judge client project developed by Shervin Kh.
 * Copyright (C) 2014  Shervin Kh.
 * License: GPLv3 Or Later
 * Full license could be found in License file shipped with program or at http://www.gnu.org/licenses/
*/

#include "connection.h"
#include <QtGui>

#include <QtNetwork>

#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

Connection::Connection(QObject *parent) :
    QObject(parent), busy(false), length(-1)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(phase1()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(failure()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(done()));

    sslSocket = new QSslSocket(this);
    sslSocket->setProtocol(QSsl::SslV3);
    sslSocket->addCaCertificates(QDir::toNativeSeparators("certs/*"), QSsl::Pem, QRegExp::Wildcard);
    sslSocket->addCaCertificates(QSslConfiguration::defaultConfiguration().caCertificates());
    connect(sslSocket, SIGNAL(encrypted()), this, SLOT(phase1()));
    connect(sslSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(failure()));
    connect(sslSocket, SIGNAL(disconnected()), this, SLOT(done()));

    progress = new QProgressDialog;
    progress->setAutoReset(true);
    progress->setAutoClose(true);
    connect(progress, SIGNAL(canceled()), this, SLOT(disconnectFromServer()));

    curSocket = tcpSocket;
}

QSharedPointer<QDataStream> Connection::prepare(QObject *receiver, const char *slot, QObject *receiverFailed, const char *slotFailed)
{
    if (busy)
    {
        QMessageBox::critical(0, "Error", "Network Busy");
        return DataPointer();
    }
    else
    {
        busy = true;
        sendingData.clear();
        receivedData.clear();
        QDataStream *DS = new QDataStream(&sendingData, QIODevice::Append);
        DS->setVersion(QDataStream::Qt_4_8);
        (*DS) << auth;
        connect(this, SIGNAL(data(StatCode,QByteArray)), receiver, slot, Qt::QueuedConnection);
        if (receiverFailed)
            connect(this, SIGNAL(unsuccessfullyDone()), receiverFailed, slotFailed, Qt::QueuedConnection);
        return DataPointer(DS);
    }
}

void Connection::failure()
{
    if (curSocket->state() == QAbstractSocket::ConnectedState)
        curSocket->disconnectFromHost();
    else if (resCode == -1)
        done();
}

void Connection::success()
{
    QDataStream DS(receivedData);
    DS.setVersion(QDataStream::Qt_4_8);
    DS >> resCode;
    curSocket->disconnectFromHost();
}

void Connection::done()
{
    emit stateChanged();

    if (resCode == -1)
    {
        if (!aborted)
            QMessageBox::critical(0, "Error", QString("Problem in network operation<br/>%1").arg(curSocket->errorString()));

        emit unsuccessfullyDone();
    }
    else
        emit data(static_cast<StatCode>(resCode), receivedData.mid(4));

    progress->reset();
    disconnect(this, SIGNAL(data(StatCode,QByteArray)), 0, 0);
    disconnect(this, SIGNAL(unsuccessfullyDone()), 0, 0);
    busy = false;
}

void Connection::send()
{
    curSocket = isSsl ? sslSocket : tcpSocket;

    if (curSocket->state() == QTcpSocket::UnconnectedState)
    {
        aborted = false;
        resCode = -1;
        progressShown = false;

        server.setDevice(curSocket);

        if (isSsl)
            sslSocket->connectToHostEncrypted(address, port);
        else
            tcpSocket->connectToHost(address, port);

        emit stateChanged();
    }
}

void Connection::phase1()
{
    emit stateChanged();
    length = -1;
    server << sendingData;
}

void Connection::readData()
{
    if (length == -1)
    {
        if (curSocket->bytesAvailable() < sizeof(int))
            return;

        server >> length;

        if (length < 4)
            curSocket->disconnectFromHost();
    }

    qint64 bytesToRead = qMin(curSocket->bytesAvailable(), static_cast<qint64>(length - receivedData.length()));
    receivedData += curSocket->read(bytesToRead);

    if (progressShown && (((receivedData.length() / 1000) != progress->value())
                          || (receivedData.length() == length)))
        handleProgress();

    if (receivedData.length() == length)
    {
        success();
        curSocket->disconnectFromHost();
    }
}

void Connection::showProgressDialog()
{
    progress->show();
    progress->activateWindow();
    progress->raise();
    progress->setLabelText("Downloading ...");
    progress->setValue(0);
    progress->setMaximum(1);
    progressShown = true;
}

void Connection::handleProgress()
{
    progress->setLabelText(QString("Downloading (%1/%2)").arg(MainWidget::formatSize(receivedData.length() / 1000))
                           .arg(MainWidget::formatSize(length / 1000)));
    progress->setValue(receivedData.length());
    progress->setMaximum(length);
}

void Connection::disconnectFromServer()
{
    aborted = true;
    curSocket->disconnectFromHost();
}

Connection::State Connection::giveState() const
{
    if (curSocket->state() == QAbstractSocket::ConnectedState)
        return State::Connected;
    else if (curSocket->state() == QAbstractSocket::ConnectingState || curSocket->state() == QAbstractSocket::HostLookupState)
        return State::Connecting;
    else
        return State::Disconnected;
}
