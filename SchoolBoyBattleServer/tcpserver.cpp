#include "tcpserver.h"

#include <QJsonDocument>
#include <QJsonObject>

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent)
{}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    ClientWorker *client = new ClientWorker(this);
    if(!client->setSocketDescriptor(socketDescriptor)) {
        client->deleteLater();
        return;
    }
    connect(client, &ClientWorker::disconnectedFromClient, this, std::bind(&TcpServer::userDisconnected, this, client));
    connect(client, &ClientWorker::error, this, std::bind(&TcpServer::userError, this, client));
    connect(client, &ClientWorker::jsonRecieved, this, std::bind(&TcpServer::jsonReceived, this, client, std::placeholders::_1));
    connect(client, &ClientWorker::logMessage, this, &TcpServer::logMessage);
    clients.append(client);
    emit logMessage("Nouveau client connecté");
}

void TcpServer::sendJson(ClientWorker *destination, const QJsonObject &message)
{
    Q_ASSERT(destination);
    destination->sendJson(message);
}

void TcpServer::broadcast(const QJsonObject &message, ClientWorker *exclude)
{
    for (ClientWorker *worker : clients) {
        Q_ASSERT(worker);
        if (worker == exclude)
            continue;
        sendJson(worker, message);
    }
}

void TcpServer::jsonReceived(ClientWorker *sender, const QJsonObject &doc)
{
    Q_ASSERT(sender);
    emit logMessage(QLatin1String("JSON reçu ") + QString::fromUtf8(QJsonDocument(doc).toJson()));
    if (sender->getUsername().isEmpty())
        return jsonFromLoggedOut(sender, doc);
    jsonFromLoggedIn(sender, doc);
}

void TcpServer::userDisconnected(ClientWorker *sender)
{
    clients.removeAll(sender);
    const QString userName = sender->getUsername();
    if (!userName.isEmpty()) {
        QJsonObject disconnectedMessage;
        disconnectedMessage[QStringLiteral("type")] = QStringLiteral("userdisconnected");
        disconnectedMessage[QStringLiteral("username")] = userName;
        broadcast(disconnectedMessage, nullptr);
        emit logMessage(userName + QLatin1String(" disconnected"));
    }
    sender->deleteLater();
}

void TcpServer::userError(ClientWorker *sender)
{
    Q_UNUSED(sender)
    emit logMessage(QLatin1String("Error from ") + sender->getUsername());
}

void TcpServer::stopServer()
{
    for (ClientWorker *worker : clients) {
        worker->disconnectFromClient();
    }
    close();
}

void TcpServer::jsonFromLoggedOut(ClientWorker *sender, const QJsonObject &docObj)
{
    Q_ASSERT(sender);
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;
    if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
    if (usernameVal.isNull() || !usernameVal.isString())
        return;
    const QString newUserName = usernameVal.toString().simplified();
    if (newUserName.isEmpty())
        return;
    for (ClientWorker *worker : qAsConst(clients)) {
        if (worker == sender)
            continue;
        if (worker->getUsername().compare(newUserName, Qt::CaseInsensitive) == 0) {
            QJsonObject message;
            message[QStringLiteral("type")] = QStringLiteral("login");
            message[QStringLiteral("success")] = false;
            message[QStringLiteral("reason")] = QStringLiteral("duplicate username");
            sendJson(sender, message);
            return;
        }
    }
    sender->setUsername(newUserName);
    QJsonObject successMessage;
    successMessage[QStringLiteral("type")] = QStringLiteral("login");
    successMessage[QStringLiteral("success")] = true;
    sendJson(sender, successMessage);
    QJsonObject connectedMessage;
    connectedMessage[QStringLiteral("type")] = QStringLiteral("newuser");
    connectedMessage[QStringLiteral("username")] = newUserName;
    broadcast(connectedMessage, sender);
}

void TcpServer::jsonFromLoggedIn(ClientWorker *sender, const QJsonObject &docObj)
{
    Q_ASSERT(sender);
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;
    if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) != 0)
        return;
    const QJsonValue textVal = docObj.value(QLatin1String("text"));
    if (textVal.isNull() || !textVal.isString())
        return;
    const QString text = textVal.toString().trimmed();
    if (text.isEmpty())
        return;
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("message");
    message[QStringLiteral("text")] = text;
    message[QStringLiteral("sender")] = sender->getUsername();
    broadcast(message, sender);
}

