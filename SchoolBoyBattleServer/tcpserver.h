#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "clientworker.h"

#include <QTcpServer>
#include <QObject>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer(QObject *parent = nullptr);

private:
    QVector<ClientWorker *> clients;
    void jsonFromLoggedOut(ClientWorker *sender, const QJsonObject &doc);
    void jsonFromLoggedIn(ClientWorker *sender, const QJsonObject &doc);
    void sendJson(ClientWorker *destination, const QJsonObject &message);

protected:
    void incomingConnection(qintptr sockerDescription) override;

signals:
    void logMessage(const QString &msg);

public slots:
    void stopServer();

private slots:
    // On exclut un client car c'est lui qui a envoyé le packet
    void broadcast(const QJsonObject &msg, ClientWorker *exclude);
    void jsonReceived(ClientWorker *sender, const QJsonObject &doc);
    void userDisconnected(ClientWorker *client);
    void userError(ClientWorker *sender);
};

#endif // TCPSERVER_H
