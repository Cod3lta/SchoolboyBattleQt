#include "waitingroom.h"

#include <QLabel>
#include <QVBoxLayout>

WaitingRoom::WaitingRoom(TcpClient *tcpClient, QWidget *parent) :
    QWidget(parent),
    tcpClient(tcpClient)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    vLayout->addWidget(new QLabel("Vous êtes dans la salle d'attente", this));
    setLayout(vLayout);
}

void WaitingRoom::startClient(QHostAddress address, qint16 port) {
    tcpClient->connectToServer(address, port);
}
