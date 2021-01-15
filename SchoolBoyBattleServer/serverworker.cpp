#include "serverworker.h"

#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>

ServerWorker::ServerWorker(QObject *parent) : QObject(parent) {

}

bool ServerWorker::setSocketDescriptor(qintptr socketDescriptor) {
    bool result = socket->setSocketDescriptor(socketDescriptor);
    return result;

}

QString ServerWorker::getUsername() const {
    return username;
}

void ServerWorker::setUsername(const QString &username) {
    this->username = username;
}

void ServerWorker::sendJson(const QJsonObject &json) {
    const QByteArray jsonData = QJsonDocument(json).toJson(QJsonDocument::Compact);
    emit logMessage("Sending to " + getUsername() + " - " + QString::fromUtf8(jsonData));
    QDataStream socketStream(socket);
    socketStream.setVersion(QDataStream::Qt_5_9);
    socketStream << jsonData;
}

void ServerWorker::disconnectFromClient() {
    socket->disconnectFromHost();
}

void ServerWorker::recieveJson() {
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
