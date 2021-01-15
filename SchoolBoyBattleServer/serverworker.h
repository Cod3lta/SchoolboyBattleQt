#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>
#include <QTcpSocket>

class ServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit ServerWorker(QObject *parent = nullptr);
    virtual bool setSocketDescriptor(qintptr socketDescriptor);
    QString getUsername() const;
    void setUsername(const QString &username);
    void sendJson(const QJsonObject &jsonData);

private:
    QTcpSocket *socket;
    QString username;

signals:
    void jsonRecieved(const QJsonObject &jsonDoc);
    void disconnectedFromClient();
    void error();
    void logMessage(const QString &msg);

public slots:
    void disconnectFromClient();

private slots:
    void recieveJson();

};

#endif // SERVERWORKER_H
