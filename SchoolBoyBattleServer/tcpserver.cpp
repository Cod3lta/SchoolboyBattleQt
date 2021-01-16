#include "tcpserver.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent),
    idealThreadCount(qMax(QThread::idealThreadCount(), 1)),
    nbUsersConnected(0)
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

void TcpServer::incomingConnection(qintptr socketDescriptor) {
    ServerWorker *worker = new ServerWorker(this);
    if(!worker->setSocketDescriptor(socketDescriptor)) {
        worker->deleteLater();
        return;
    }
    int threadIdx = availableThreads.size();
    if(threadIdx < idealThreadCount) {
        availableThreads.append(new QThread);
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
    connect(this, &TcpServer::stopAllClients, worker, &ServerWorker::disconnectFromClient);
    clients.append(worker);
    emit logMessage("Nouveau client connecté");
}

void TcpServer::sendJson(ServerWorker *destination, const QJsonObject &message)
{
    Q_ASSERT(destination);
    // Faire un qtimer avec un temps de 0 exécutera le code au prochain
    // instant de processeur disponible
    QTimer::singleShot(0, destination, std::bind(&ServerWorker::sendJson, destination, message));
}

void TcpServer::broadcast(const QJsonObject &message, ServerWorker *exclude) {
    for (int i = 0; i < clients.length(); i++) {
        Q_ASSERT(clients.at(i));
        if (clients.at(i) == exclude)
            continue;
        sendJson(clients.at(i), message);
    }
}

void TcpServer::sendEveryone(const QJsonObject &message) {
    for(int i = 0; i < clients.length(); i++) {
        Q_ASSERT(clients.at(i));
        sendJson(clients.at(i), message);
    }
}

void TcpServer::jsonReceived(ServerWorker *sender, const QJsonObject &doc)
{
    Q_ASSERT(sender);
    emit logMessage(QLatin1String("JSON recu ") + QString::fromUtf8(QJsonDocument(doc).toJson()));
    if (sender->getUsername().isEmpty())
        // Si le message qu'on reçoit vient d'un utilisateur qui n'a pas de username
        return jsonFromLoggedOut(sender, doc);
    // Si le message vient d'un utilisateur connecté
    jsonFromLoggedIn(sender, doc);
}

void TcpServer::userDisconnected(ServerWorker *sender, int threadIdx) {
    threadsLoaded[threadIdx]--;
    clients.removeAll(sender);
    const QString userName = sender->getUsername();
    if (!userName.isEmpty()) {
        QJsonObject userListMessage;
        userListMessage.insert("type", QJsonValue("updateUsersList"));
        userListMessage.insert("users", QJsonValue(generateUserList()));
        sendEveryone(userListMessage);

        emit logMessage(userName + QLatin1String(" disconnected"));
    }
    sender->deleteLater();
}

void TcpServer::userError(ServerWorker *sender)
{
    Q_UNUSED(sender)
    emit logMessage(QLatin1String("Erreur de ") + sender->getUsername());
}

void TcpServer::stopServer()
{
    emit stopAllClients();
    close();
}

/*
 * Tous les messages qu'on reçoit de clients qui n'ont pas de username
 */
void TcpServer::jsonFromLoggedOut(ServerWorker *sender, const QJsonObject &docObj)
{
    Q_ASSERT(sender);
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;
    if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) != 0)
        // Si ce n'est pas un login, on ne fait rien
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
    // Envoyer au client qui veut se logger qu'il a réussi
    sender->setUsername(newUserName);
    sender->setReady(false);
    QJsonObject successMessage;
    successMessage[QStringLiteral("type")] = QStringLiteral("login");
    successMessage[QStringLiteral("success")] = true;
    successMessage[QStringLiteral("descriptor")] = sender->getSocketDescriptor();
    sendJson(sender, successMessage);

    // Envoyer à tout le monde la liste des clients connectés
    QJsonObject userListMessage;
    userListMessage.insert("type", QJsonValue("updateUsersList"));
    userListMessage.insert("users", QJsonValue(generateUserList()));
    sendEveryone(userListMessage);
}


QJsonArray TcpServer::generateUserList() {
    QJsonArray clientsList;
    for(int i = 0; i < clients.length(); i++) {
        QJsonObject user;
        user.insert("username", clients.at(i)->getUsername());
        user.insert("ready", clients.at(i)->getReady());
        user.insert("socketDescriptor", clients.at(i)->getSocketDescriptor());
        clientsList.push_back(QJsonValue(user));
    }
    return clientsList;
}


/*
 * Si le message qu'on reçoit vient d'un utilisateur connecté (on traite le  message)
 */
void TcpServer::jsonFromLoggedIn(ServerWorker *sender, const QJsonObject &docObj)
{
    Q_ASSERT(sender);
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;
    if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0) { // Message
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
    }else if(typeVal.toString().compare(QLatin1String("toggleReady"), Qt::CaseInsensitive) == 0) {  // Toggle ready
        sender->setReady(!sender->getReady());
        QJsonObject userListMessage;
        userListMessage.insert("type", QJsonValue("updateUsersList"));
        userListMessage.insert("users", QJsonValue(generateUserList()));
        sendEveryone(userListMessage);
    }
}

