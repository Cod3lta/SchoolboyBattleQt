#include "waitingroom.h"

#include <QLabel>
#include <QVBoxLayout>

WaitingRoom::WaitingRoom(TcpClient *tcpClient, QWidget *parent) :
    QWidget(parent),
    tcpClient(tcpClient)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    label = new QLabel("Chargement...", this);
    vLayout->addWidget(label);
    setLayout(vLayout);

    connect(tcpClient, &TcpClient::connectionError, this, [=] () {
        emit setVisibleWidget(1);
    });

    connect(tcpClient, &TcpClient::connected, this, [=] () {
        label->setText("Connecté au serveur !");
    });
}

void WaitingRoom::startClient(QHostAddress address, qint16 port) {
    tcpClient->connectToServer(address, port);
}
