#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "serverworker.h"

#include <QTcpServer>
#include <QObject>
#include <QThread>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer(QObject *parent = nullptr);
    ~TcpServer();

private:
    const int idealThreadCount;
    QVector<QThread *> availableThreads;
    QVector<int> threadsLoaded;
    int nbUsersConnected;
    QVector<ServerWorker *> clients;
    void jsonFromLoggedOut(ServerWorker *sender, const QJsonObject &doc);
    void jsonFromLoggedIn(ServerWorker *sender, const QJsonObject &doc);
    void sendJson(ServerWorker *destination, const QJsonObject &message);
    QJsonArray generateUserList();

protected:
    void incomingConnection(qintptr socketDescription) override;

signals:
    void logMessage(const QString &msg);
    void stopAllClients();

public slots:
    void stopServer();

private slots:
    // On exclut un client car c'est lui qui a envoyé le packet
    void broadcast(const QJsonObject &msg, ServerWorker *exclude);
    void jsonReceived(ServerWorker *sender, const QJsonObject &doc);
    void userDisconnected(ServerWorker *client, int threadIdx);
    void userError(ServerWorker *sender);
    void sendEveryone(const QJsonObject &message);
};

#endif // TCPSERVER_H
