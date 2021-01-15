#ifndef WAITINGROOM_H
#define WAITINGROOM_H

#include "tcpclient.h"

#include <QHostAddress>
#include <QWidget>

class WaitingRoom : public QWidget
{
    Q_OBJECT
public:
    explicit WaitingRoom(TcpClient *tcpClient, QWidget *parent = nullptr);

private:
    TcpClient* tcpClient;

public slots:
    void startClient(QHostAddress address, qint16 port);

signals:
};

#endif // WAITINGROOM_H
