#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QAbstractSocket>
#include <QObject>
#include <QTcpSocket>

class TcpClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(TcpClient)

public:
    explicit TcpClient(QObject *parent = nullptr);
    int getDescriptor();
    QHash<int, QHash<QString, QString>> getUsersList();

private:
    QHash<int, QHash<QString, QString>> usersList;
    QTcpSocket *socket;
    bool loggedIn;
    int descriptor;
    void jsonReceived(const QJsonObject &doc);
public slots:
    void connectToServer(const QHostAddress &address, quint16 port);
    void login(const QString &username);
    void disconnectFromHost();
    void sendMessage(const QString &text);
    void toggleReady();
    // Signaux du jeu
    void keyMove(int playerId, int direction, bool value);
    void rollback(int playerX, int playerY);
private slots:
    void onReadyRead();
    void error(QAbstractSocket::SocketError error);
    void askUsername();
signals:
    void connected();
    void startGame(int nbPlayers, int nbViews);
    void UserLoggedIn();
    void loginError(const QString &reason);
    void disconnected();
    void userJoined();
    void userLeft();
    void messageReceived(const QString &sender, const QString &text);
    void connectionError();
    //void userListRefresh(QList<QVariant>);
    void userListRefresh(QHash<int, QHash<QString, QString>>);
    //void userJoined(const QString &username);
    void userLeft(const QString &username);
    void userMove(int direction, int playerDescriptor, bool value);
    void userRollback(int playerX, int playerY, int playerDescriptor);

};

#endif // TCPCLIENT_H
