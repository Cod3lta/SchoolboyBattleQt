#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QAbstractSocket>
#include <QObject>
#include <QTcpSocket>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);

private:
    QTcpSocket *socket;
    bool loggedIn;
    void jsonReceived(const QJsonObject &doc);

public slots:
    void connectToServer(const QHostAddress &address, quint16 port);
    void disconnectFromHost();
    void sendMessage(const QString &text);

private slots:
    void onReadyRead();

signals:
    void connected();
    void UserLoggedIn();
    void loginError(const QString &reason);
    void disconnected();
    void userJoined();
    void userLeft();
    void messageReceived(const QString &sender, const QString &text);
    void error(QAbstractSocket::SocketError error);
    void userJoined(const QString &username);
    void userLeft(const QString &username);

};

#endif // TCPCLIENT_H
