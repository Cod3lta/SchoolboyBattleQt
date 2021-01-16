#ifndef WAITINGROOM_H
#define WAITINGROOM_H

#include "tcpclient.h"

#include <QBoxLayout>
#include <QHostAddress>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class WaitingRoom : public QWidget
{
    Q_OBJECT
public:
    explicit WaitingRoom(TcpClient *tcpClient, QWidget *parent = nullptr);

private:
    TcpClient* tcpClient;
    QLabel *mainLabel;
    QList<QHBoxLayout *> usersLayout;
    QList<QLabel *> usersName;
    QList<QLabel *> usersReady;
    QPushButton *btnReady;
    QPushButton *btnLeave;


private slots:
    void userListRefresh(QList<QHash<QString, QString>>);
    void connected();

public slots:
    void startWaitingRoom(QHostAddress address, qint16 port);

signals:
    void setVisibleWidget(int i);
};

#endif // WAITINGROOM_H
