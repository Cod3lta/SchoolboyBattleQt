#ifndef WAITINGROOM_H
#define WAITINGROOM_H

#include "tcpclient.h"

#include <QHostAddress>
#include <QLabel>
#include <QWidget>

class WaitingRoom : public QWidget
{
    Q_OBJECT
public:
    explicit WaitingRoom(TcpClient *tcpClient, QWidget *parent = nullptr);

private:
    TcpClient* tcpClient;
    QLabel *label;

public slots:
    void startClient(QHostAddress address, qint16 port);

signals:
    void setVisibleWidget(int i);
};

#endif // WAITINGROOM_H
