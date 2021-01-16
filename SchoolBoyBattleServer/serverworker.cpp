#include "serverworker.h"

#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>

ServerWorker::ServerWorker(QObject *parent) :
    QObject(parent),
    socket(new QTcpSocket(this))
{
    connect(socket, &QTcpSocket::readyRead, this, &ServerWorker::receiveJson);
    connect(socket, &QTcpSocket::disconnected, this, &ServerWorker::disconnectedFromClient);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &ServerWorker::error);
}

bool ServerWorker::setSocketDescriptor(qintptr socketDescriptor) {
    return socket->setSocketDescriptor(socketDescriptor);
}

void ServerWorker::sendJson(const QJsonObject &json) {
    const QByteArray jsonData = QJsonDocument(json).toJson(QJsonDocument::Compact);
    emit logMessage("Envoi à " + getUsername() + " - " + QString::fromUtf8(jsonData));
    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_9);
    socketStream << jsonData;
}

void ServerWorker::disconnectFromClient() {
    socket->disconnectFromHost();
}

QString ServerWorker::getUsername() const {
    usernameLock.lockForRead();
    const QString result = username;
    usernameLock.unlock();
    return result;
}

void ServerWorker::setUsername(const QString &username) {
    usernameLock.lockForWrite();
    this->username = username;
    usernameLock.unlock();
}

void ServerWorker::receiveJson() {
    QByteArray jsonData;
    QDataStream socketStream(socket);

    socketStream.setVersion(QDataStream::Qt_5_9);
    while(true) {
        socketStream.startTransaction();
        socketStream >> jsonData;

        if(socketStream.commitTransaction()) {
            QJsonParseError parseError;
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if(parseError.error == QJsonParseError::NoError) {
                if(jsonDoc.isObject())
                    emit jsonRecieved(jsonDoc.object());
                else
                    emit logMessage("Message invalide : " + QString::fromUtf8(jsonData));
            }else{
                emit logMessage("Message invalide : " + QString::fromUtf8(jsonData));
            }
        }else{
            break;
        }
    }
}
