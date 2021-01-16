#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>
#include <QReadWriteLock>
#include <QTcpSocket>

class ServerWorker : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ServerWorker)

public:
    explicit ServerWorker(QObject *parent = nullptr);
    virtual bool setSocketDescriptor(qintptr socketDescriptor);
    qintptr getSocketDescriptor();
    QString getUsername() const;
    bool getReady() const;
    void setReady(const bool ready);
    void setUsername(const QString &username);
    void sendJson(const QJsonObject &jsonData);

private:
    QTcpSocket *socket;
    QString username;
    bool ready;
    mutable QReadWriteLock usernameLock;
    mutable QReadWriteLock readyLock;

signals:
    void jsonRecieved(const QJsonObject &jsonDoc);
    void disconnectedFromClient();
    void error();
    void logMessage(const QString &msg);

public slots:
    void disconnectFromClient();

private slots:
    void receiveJson();

};

#endif // SERVERWORKER_H
