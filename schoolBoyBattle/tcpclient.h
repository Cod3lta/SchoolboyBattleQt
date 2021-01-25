#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QAbstractSocket>
#include <QObject>
#include <QPointF>
#include <QTcpSocket>

class TcpClient : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(TcpClient)

public:
    TcpClient(QObject *parent = nullptr);
    int getSocketDescriptor();
    bool isCandyMaster();
    QHash<int, QHash<QString, QString>> getUsersList();

private:
    QHash<int, QHash<QString, QString>> usersList;
    QTcpSocket *socket;
    bool loggedIn;
    bool candyMaster;
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
    void rollback(QPointF playerPos, QHash<int, QPointF> candiesTaken);
    void sendNewCandy(int candyType, int candySize, int nbPoints, int tilePlacementId, int candyId);
    void isCandyFree(int candyId);
    void playerStealsCandies(int candyIdStartingFrom, int playerWinningId);
    void playerValidateCandies(int playerId);
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
    void userListRefresh(QHash<int, QHash<QString, QString>>);
    void userLeft(const QString &username);
    void userMove(int direction, int playerDescriptor, bool value);
    void userRollback(double playerX, double playerY, QHash<int, QPointF> candies, int playerDescriptor);
    void spawnNewCandy(int candyType, int candySize, int nbPoints, int tilePlacementId, int candyId);
    void playerPickUpCandy(int descriptor, int candyId);
    void playerStealCandy(int candyIdStartingFrom, int winnerDescriptor);
    void playerValidateCandy(int descriptor);

};

#endif // TCPCLIENT_H
