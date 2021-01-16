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

int TcpClient::getDescriptor() {
    return descriptor;
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

void TcpClient::toggleReady() {
    QDataStream clientStream(socket);
    clientStream.setVersion(QDataStream::Qt_5_9);
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("toggleReady");
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
        if(docObj.value("reason") == "duplicate username") {
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
    } else if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0) { // Message de chat
        // we extract the text field containing the chat text
        const QJsonValue textVal = docObj.value(QLatin1String("text"));
        // we extract the sender field containing the username of the sender
        const QJsonValue senderVal = docObj.value(QLatin1String("sender"));
        if (textVal.isNull() || !textVal.isString())
            return; // the text field was invalid so we ignore
        if (senderVal.isNull() || !senderVal.isString())
            return; // the sender field was invalid so we ignore
        // we notify a new message was received via the messageReceived signal
        emit messageReceived(senderVal.toString(), textVal.toString());
    } else if (typeVal.toString().compare(QLatin1String("updateUsersList"), Qt::CaseInsensitive) == 0) { // Refresh la liste des joueurs
        // Transformer les données json en QList<QHash<QString, QString>>
        QList<QVariant> users = docObj.value("users").toArray().toVariantList();
        QList<QHash<QString, QString>> usersList;
        for(int i = 0; i < users.size(); i++) {
            QHash<QString, QString> hashTemp;
            QHash<QString, QVariant> userHash = users.at(i).toHash();
            QHashIterator<QString, QVariant> j(userHash);
            while(j.hasNext()) {
                j.next();
                hashTemp.insert(j.key(), j.value().toString());
            }
            usersList.append(hashTemp);
        }
        emit userListRefresh(usersList);
    } else if (typeVal.toString().compare(QLatin1String("userdisconnected"), Qt::CaseInsensitive) == 0) { // Un utilisateur a quitté
         // we extract the username of the new user
        const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
        if (usernameVal.isNull() || !usernameVal.isString())
            return; // the username was invalid so we ignore
        // we notify of the user disconnection the userLeft signal
        emit userLeft(usernameVal.toString());
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
    /*
    // enable the button to connect to the server again
    ui->connectButton->setEnabled(true);
    // disable the ui to send and display messages
    ui->sendButton->setEnabled(false);
    ui->messageEdit->setEnabled(false);
    ui->chatView->setEnabled(false);*/
    // reset the last printed username
    //m_lastUserName.clear();
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
