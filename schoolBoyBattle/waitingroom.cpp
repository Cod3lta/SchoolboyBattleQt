#include "waitingroom.h"

#include <QLabel>
#include <QVBoxLayout>

WaitingRoom::WaitingRoom(TcpClient *tcpClient, QWidget *parent) :
    QWidget(parent),
    tcpClient(tcpClient)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addWidget(new QLabel("Chargement...", this));
    setLayout(vLayout);

    connect(tcpClient, &TcpClient::connectionError, this, [=] () {
        emit setVisibleWidget(1);
    });
}

void WaitingRoom::startClient(QHostAddress address, qint16 port) {
    tcpClient->connectToServer(address, port);
}
