#include "tcpclient.h"

#include <QDataStream>
#include <QJsonParseError>
#include <QTcpSocket>
#include <QJsonObject>

TcpClient::TcpClient(QObject *parent) :
    QObject(parent),
    socket(new QTcpSocket(this)),
    loggedIn(false)
{
    connect(socket, &QTcpSocket::connected, this, &TcpClient::connected);
    connect(socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &TcpClient::error);
    connect(socket, &QTcpSocket::disconnected, this, &TcpClient::disconnected);
    connect(socket, &QTcpSocket::disconnected, this, [=]() {loggedIn = false; });
}

void TcpClient::jsonReceived(const QJsonObject &docObj) {
    // actions depend on the type of message
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return; // a message with no type was received so we just ignore it
    if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) == 0) { //It's a login message
        if (loggedIn)
            return; // if we are already logged in we ignore
        // the success field will contain the result of our attempt to login
        const QJsonValue resultVal = docObj.value(QLatin1String("success"));
        if (resultVal.isNull() || !resultVal.isBool())
            return; // the message had no success field so we ignore
        const bool loginSuccess = resultVal.toBool();
        if (loginSuccess) {
            // we logged in succesfully and we notify it via the loggedIn signal
            emit UserLoggedIn();
            return;
        }
        // the login attempt failed, we extract the reason of the failure from the JSON
        // and notify it via the loginError signal
        const QJsonValue reasonVal = docObj.value(QLatin1String("reason"));
        emit loginError(reasonVal.toString());
    } else if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0) { //It's a chat message
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
    } else if (typeVal.toString().compare(QLatin1String("newuser"), Qt::CaseInsensitive) == 0) { // A user joined the chat
        // we extract the username of the new user
        const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
        if (usernameVal.isNull() || !usernameVal.isString())
            return; // the username was invalid so we ignore
        // we notify of the new user via the userJoined signal
        emit userJoined(usernameVal.toString());
    } else if (typeVal.toString().compare(QLatin1String("userdisconnected"), Qt::CaseInsensitive) == 0) { // A user left the chat
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

void TcpClient::sendMessage(const QString &text) {
    if (text.isEmpty())
        return; // We don't send empty messages
    // create a QDataStream operating on the socket
    QDataStream clientStream(socket);
    // set the version so that programs compiled with different versions of Qt can agree on how to serialise
    clientStream.setVersion(QDataStream::Qt_5_7);
    // Create the JSON we want to send
    QJsonObject message;
    message[QStringLiteral("type")] = QStringLiteral("message");
    message[QStringLiteral("text")] = text;
    // send the JSON using QDataStream
    clientStream << QJsonDocument(message).toJson();
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
