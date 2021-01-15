#include "tcpserver.h"

#include <QJsonDocument>
#include <QJsonObject>

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent),
    idealThreadCount(qMax(QThread::idealThreadCount(), 1))
{
    availableThreads.reserve(idealThreadCount);
    threadsLoaded.reserve(idealThreadCount);
}

TcpServer::~TcpServer() {
    for (int i = 0; i < availableThreads.size(); i++) {
        availableThreads.at(i)->quit();
        availableThreads.at(i)->wait();
    }
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    ServerWorker *worker = new ServerWorker(this);
    if(!worker->setSocketDescriptor(socketDescriptor)) {
        worker->deleteLater();
        return;
    }
    int threadIdx = availableThreads.size();
    if(threadIdx < idealThreadCount) {
        availableThreads.append(new QThread(this));
        threadsLoaded.append(1);
        availableThreads.last()->start();
    }else{
        // Trouver le thread avec le moins de clients et l'utiliser
        threadIdx = std::distance(threadsLoaded.cbegin(), std::min_element(threadsLoaded.cbegin(), threadsLoaded.cend()));
        threadsLoaded[threadIdx]++;
    }
    worker->moveToThread(availableThreads.at(threadIdx));
    connect(availableThreads.at(threadIdx), &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, &ServerWorker::disconnectedFromClient, this, std::bind(&TcpServer::userDisconnected, this, worker, threadIdx));
    connect(worker, &ServerWorker::error, this, std::bind(&TcpServer::userError, this, worker));
    connect(worker, &ServerWorker::jsonRecieved, this, std::bind(&TcpServer::jsonReceived, this, worker, std::placeholders::_1));
    connect(worker, &ServerWorker::logMessage, this, &TcpServer::logMessage);
    clients.append(worker);
    emit logMessage("Nouveau client connecté");
}

void TcpServer::sendJson(ServerWorker *destination, const QJsonObject &message)
{
    Q_ASSERT(destination);
    destination->sendJson(message);
}

void TcpServer::broadcast(const QJsonObject &message, ServerWorker *exclude)
{
    for (ServerWorker *worker : clients) {
        Q_ASSERT(worker);
        if (worker == exclude)
            continue;
        sendJson(worker, message);
    }
}

void TcpServer::jsonReceived(ServerWorker *sender, const QJsonObject &doc)
{
    Q_ASSERT(sender);
    emit logMessage(QLatin1String("JSON reçu ") + QString::fromUtf8(QJsonDocument(doc).toJson()));
    if (sender->getUsername().isEmpty())
        return jsonFromLoggedOut(sender, doc);
    jsonFromLoggedIn(sender, doc);
}

void TcpServer::userDisconnected(ServerWorker *sender, int threadIdx) {
    threadsLoaded[threadIdx]--;
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

void TcpServer::userError(ServerWorker *sender)
{
    Q_UNUSED(sender)
    emit logMessage(QLatin1String("Error from ") + sender->getUsername());
}

void TcpServer::stopServer()
{
    for (ServerWorker *worker : clients) {
        worker->disconnectFromClient();
    }
    close();
}

void TcpServer::jsonFromLoggedOut(ServerWorker *sender, const QJsonObject &docObj)
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
    for (ServerWorker *worker : qAsConst(clients)) {
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

void TcpServer::jsonFromLoggedIn(ServerWorker *sender, const QJsonObject &docObj)
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

