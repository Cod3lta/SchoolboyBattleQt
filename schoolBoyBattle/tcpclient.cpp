#include "tcpclient.h"

#include <QDataStream>
#include <QJsonParseError>
#include <QTcpSocket>
#include <QJsonObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QJsonArray>

TcpClient::TcpClient(QObject *parent) :
    QObject(parent),
    socket(new QTcpSocket(this)),
    loggedIn(false),
    candyMaster(false),
    descriptor(-1)
{
    connect(socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);         // Slot
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &TcpClient::error); // Slot
    connect(socket, &QTcpSocket::connected, this, &TcpClient::askUsername);           // Slot
    connect(socket, &QTcpSocket::connected, this, &TcpClient::connected);           // Signal
    connect(socket, &QTcpSocket::disconnected, this, &TcpClient::disconnected);     // Signal
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, [=] () {
        // Retourner au menu principal
        emit connectionError();
    });
    connect(socket, &QTcpSocket::disconnected, this, [=]() {loggedIn = false; });
}

QHash<int, QHash<QString, QString>> TcpClient::getUsersList() {
    return usersList;
}

void TcpClient::login(const QString &username)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QDataStream clientStream(socket);
        clientStream.setVersion(QDataStream::Qt_5_9);
        QJsonObject message;
        message[QStringLiteral("type")] = QStringLiteral("login");
        message[QStringLiteral("username")] = username;
        clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
    }
}

void TcpClient::sendMessage(const QString &text)
{
    if (text.isEmpty())
        return;
    QDataStream clientStream(socket);
    clientStream.setVersion(QDataStream::Qt_5_9);
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("message");
    message[QStringLiteral("text")] = text;
    clientStream << QJsonDocument(message).toJson();
}

/*
 * Quand le joueur clique sur "prêt" dans la salle d'attente
 */
void TcpClient::toggleReady() {
    QDataStream clientStream(socket);
    clientStream.setVersion(QDataStream::Qt_5_9);
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("toggleReady");
    clientStream << QJsonDocument(message).toJson();
}

/*
 * Quand le joueur appuie ou relache une touche de déplacement
 * du clavier
 */
void TcpClient::keyMove(int playerDescriptor, int direction, bool value) {
    QDataStream clientStream(socket);
    clientStream.setVersion(QDataStream::Qt_5_9);
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("playerMove");
    message[QStringLiteral("playerDescriptor")] = playerDescriptor;
    message[QStringLiteral("direction")] = direction;
    message[QStringLiteral("value")] = value;
    clientStream << QJsonDocument(message).toJson();
}

/*
 * Envoi du rollback au serveur
 * Infos à envoyer : la position du joueur et de ses candies
 */
void TcpClient::rollback(QPointF playerPos, QHash<int, QPointF> candiesTaken) {
    QDataStream clientStream(socket);
    clientStream.setVersion(QDataStream::Qt_5_9);

    QJsonObject candies;
    QHashIterator<int, QPointF> i(candiesTaken);
    while(i.hasNext()) {
        i.next();
        QJsonObject candyCoordinates;
        candyCoordinates.insert("x", i.value().x());
        candyCoordinates.insert("y", i.value().y());
        candies.insert(QString::number(i.key()), candyCoordinates);
    }

    QJsonObject rollback;
    //QJsonArray candiesDatas;
    rollback[QStringLiteral("type")] = QStringLiteral("playerRollback");
    rollback[QStringLiteral("playerX")] = playerPos.x();
    rollback[QStringLiteral("playerY")] = playerPos.y();
    rollback[QStringLiteral("candies")] = candies;
    clientStream << QJsonDocument(rollback).toJson();
}

/*
 * Envoi du nouveau candy créé au serveur
 */
void TcpClient::sendNewCandy(int candyType, int candySize, int tilePlacementId, int candyId) {
    QDataStream clientStream(socket);
    clientStream.setVersion(QDataStream::Qt_5_9);
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("newCandy");
    message[QStringLiteral("candyType")] = candyType;
    message[QStringLiteral("candySize")] = candySize;
    message[QStringLiteral("tilePlacementId")] = tilePlacementId;
    message[QStringLiteral("candyId")] = candyId;
    clientStream << QJsonDocument(message).toJson();
}

void TcpClient::isCandyFree(int candyId) {
    QDataStream clientStream(socket);
    clientStream.setVersion(QDataStream::Qt_5_9);
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("isCandyFree");
    message[QStringLiteral("candyId")] = candyId;
    clientStream << QJsonDocument(message).toJson();
}

void TcpClient::playerStealsCandies(int candyIdStartingFrom, int playerWinningId) {
    Q_UNUSED(playerWinningId)
    QDataStream clientStream(socket);
    clientStream.setVersion(QDataStream::Qt_5_9);
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("stealCandies");
    message[QStringLiteral("candyIdStartingFrom")] = candyIdStartingFrom;
    clientStream << QJsonDocument(message).toJson();
}

void TcpClient::jsonReceived(const QJsonObject &docObj) {
    // actions depend on the type of message
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return; // a message with no type was received so we just ignore it
    if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) == 0) { // Message de login
        if (loggedIn)
            return; // if we are already logged in we ignore
        // the success field will contain the result of our attempt to login
        const QJsonValue resultVal = docObj.value(QLatin1String("success"));
        if (resultVal.isNull() || !resultVal.isBool())
            return; // the message had no success field so we ignore
        if(docObj.value("reason") == "duplicateUsername") {
            QMessageBox::critical(nullptr, "Erreur", "Ce nom d'utilisateur est déjà pris");
            askUsername();
            return;
        }
        const bool loginSuccess = resultVal.toBool();
        if (loginSuccess) {
            // we logged in succesfully and we notify it via the loggedIn signal
            loggedIn = true;
            descriptor = docObj.value("descriptor").toInt();
            emit UserLoggedIn();
            return;
        }
        // the login attempt failed, we extract the reason of the failure from the JSON
        // and notify it via the loginError signal
        const QJsonValue reasonVal = docObj.value(QLatin1String("reason"));
        emit loginError(reasonVal.toString());
    } else if (typeVal.toString().compare(QLatin1String("updateUsersList"), Qt::CaseInsensitive) == 0) { // Refresh la liste des joueurs
        // Transformer les données json en QHash<int, QHash<QString, QString>>
        QHash<int, QHash<QString, QString>> usersList;
        QHash<QString, QVariant> users = docObj.value("users").toObject().toVariantHash();
        QHashIterator<QString, QVariant> i(users);
        while(i.hasNext()) {
            i.next();
            QHash<QString, QString> clientProps;
            QHash<QString, QVariant> clientPropsVariant = i.value().toHash();
            QHashIterator<QString, QVariant> j(clientPropsVariant);
            while(j.hasNext()) {
                j.next();
                clientProps.insert(j.key(), j.value().toString());
            }
            usersList.insert(i.key().toInt(), clientProps);
            // mettre la variable isCandyMaster à true si c'est nous le candy master
            if(docObj.value(QLatin1String("candyMasterDescriptor")).toInt() == getSocketDescriptor())
                candyMaster = true;
        }
        this->usersList = usersList;        // On sauvegarde la liste des infos de chaque utilisateur dans l'objet pour
                                            // reprendre les infos au démarrage du jeu
        emit userListRefresh(usersList);
    } else if (typeVal.toString().compare(QLatin1String("userdisconnected"), Qt::CaseInsensitive) == 0) { // Un utilisateur a quitté
         // we extract the username of the new user
        const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
        if (usernameVal.isNull() || !usernameVal.isString())
            return; // the username was invalid so we ignore
        // we notify of the user disconnection the userLeft signal
        emit userLeft(usernameVal.toString());
    } else if (typeVal.toString().compare(QLatin1String("startGame"), Qt::CaseInsensitive) == 0) {  // Démarrage du jeu
        // On peut avoir des parties à min 4 mais jamais en dessous de 2 en serveur
        if(docObj.value("nbUsers").toInt()  < 2)
            return;
        emit startGame(docObj.value("nbUsers").toInt(), 1);
    } else if(typeVal.toString().compare(QLatin1String("playerMove"), Qt::CaseInsensitive) == 0) {  // Déplacement d'un joueur
        emit userMove(
                docObj["playerDescriptor"].toInt(),
                docObj["direction"].toInt(),
                docObj["value"].toBool());
    } else if(typeVal.toString().compare(QLatin1String("playerRollback"), Qt::CaseInsensitive) == 0) {  // Rollback d'un joueur
        QHash<QString, QVariant> candiesVariant = docObj["candies"].toObject().toVariantHash();
        QHash<int, QPointF> candiesTaken;
        QHashIterator<QString, QVariant> i(candiesVariant);
        while(i.hasNext()) {
            i.next();
            QJsonObject test = i.value().toJsonObject();
            candiesTaken.insert(i.key().toInt(),
                                QPointF(
                                    test.value("x").toDouble(),
                                    test.value("y").toDouble()));
        }
        emit userRollback(
                docObj["playerX"].toDouble(),
                docObj["playerY"].toDouble(),
                candiesTaken,
                docObj["socketDescriptor"].toInt());
    } else if(typeVal.toString().compare(QLatin1String("newCandy"), Qt::CaseInsensitive) == 0) {  // Nouveau candy a spawné
        emit spawnNewCandy(
                docObj["candyType"].toInt(),
                docObj["candySize"].toInt(),
                docObj["tilePlacementId"].toInt(),
                docObj["candyId"].toInt());
    } else if(typeVal.toString().compare(QLatin1String("candyTaken"), Qt::CaseInsensitive) == 0) {  // Un joueur a pris un candy
        emit playerPickUpCandy(
                docObj["socketDescriptor"].toInt(),
                docObj["candyId"].toInt());
    } else if(typeVal.toString().compare(QLatin1String("stealCandies"), Qt::CaseInsensitive) == 0) {  // Un joueur a volé un candy
        emit playerStealCandy(
                docObj["candyIdStartingFrom"].toInt(),
                docObj["socketDescriptor"].toInt());
    }
}

void TcpClient::connectToServer(const QHostAddress &address, quint16 port){
    socket->connectToHost(address, port);
}

void TcpClient::disconnectFromHost() {
    socket->disconnectFromHost();
}

void TcpClient::onReadyRead() {
    QByteArray jsonData;
    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_7);
    while(true) {
        socketStream.startTransaction();
        socketStream >> jsonData;
        if (socketStream.commitTransaction()) {
            QJsonParseError parseError;
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if (parseError.error == QJsonParseError::NoError) {
                if (jsonDoc.isObject()) // and is a JSON object
                    jsonReceived(jsonDoc.object()); // parse the JSON
            }
        } else {
            break;
        }
    }
}



void TcpClient::error(QAbstractSocket::SocketError error) {
    // show a message to the user that informs of what kind of error occurred
    switch (error) {
    case QAbstractSocket::RemoteHostClosedError:
    case QAbstractSocket::ProxyConnectionClosedError:
        return; // handled by disconnectedFromServer
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::critical(nullptr, tr("Error"), tr("The host refused the connection"));
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        QMessageBox::critical(nullptr, tr("Error"), tr("The proxy refused the connection"));
        break;
    case QAbstractSocket::ProxyNotFoundError:
        QMessageBox::critical(nullptr, tr("Error"), tr("Could not find the proxy"));
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::critical(nullptr, tr("Error"), tr("Could not find the server"));
        break;
    case QAbstractSocket::SocketAccessError:
        QMessageBox::critical(nullptr, tr("Error"), tr("You don't have permissions to execute this operation"));
        break;
    case QAbstractSocket::SocketResourceError:
        QMessageBox::critical(nullptr, tr("Error"), tr("Too many connections opened"));
        break;
    case QAbstractSocket::SocketTimeoutError:
        QMessageBox::warning(nullptr, tr("Error"), tr("Operation timed out"));
        return;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        QMessageBox::critical(nullptr, tr("Error"), tr("Proxy timed out"));
        break;
    case QAbstractSocket::NetworkError:
        QMessageBox::critical(nullptr, tr("Error"), tr("Unable to reach the network"));
        break;
    case QAbstractSocket::UnknownSocketError:
        QMessageBox::critical(nullptr, tr("Error"), tr("An unknown error occured"));
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        QMessageBox::critical(nullptr, tr("Error"), tr("Operation not supported"));
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        QMessageBox::critical(nullptr, tr("Error"), tr("Your proxy requires authentication"));
        break;
    case QAbstractSocket::ProxyProtocolError:
        QMessageBox::critical(nullptr, tr("Error"), tr("Proxy comunication failed"));
        break;
    case QAbstractSocket::TemporaryError:
    case QAbstractSocket::OperationError:
        QMessageBox::warning(nullptr, tr("Error"), tr("Operation failed, please try again"));
        return;
    default:
        Q_UNREACHABLE();
    }
}

void TcpClient::askUsername() {
    // once we connected to the server we ask the user for what username they would like to use
    const QString newUsername = QInputDialog::getText(nullptr, tr("Choisissez un nom d'utilisateur"), tr("Nom d'utilisateur"));
    if (newUsername.isEmpty()){
        // if the user clicked cancel or typed nothing, we just disconnect from the server
        return socket->disconnectFromHost();
    }
    // try to login with the given username
    login(newUsername);
}

int TcpClient::getSocketDescriptor() {
    return descriptor;
}

bool TcpClient::isCandyMaster() {
    return candyMaster;
}
