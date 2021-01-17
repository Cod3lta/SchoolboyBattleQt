#include "waitingroom.h"

#include <QLabel>
#include <QMessageBox>

#define MAX_USERS 8
#define MIN_USERS 2

WaitingRoom::WaitingRoom(TcpClient *tcpClient, QWidget *parent) :
    QWidget(parent),
    tcpClient(tcpClient)
{
    // Mise en place du layout
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QHBoxLayout *btnsLayout = new QHBoxLayout;
    QGridLayout *users = new QGridLayout;
    mainLabel = new QLabel;
    QLabel *labelInfos = new QLabel("Dès que toutes les personnes ayant rejoint le serveur sont prêtes, le jeu commencera");
    labelInfos->setStyleSheet("QLabel { color: gray; }");
    btnReady = new QPushButton("Prêt");
    btnLeave = new QPushButton("Quitter le serveur");

    for(int i = 0; i < MAX_USERS; i++) {
        usersLayout.append(new QHBoxLayout);
        usersName.append(new QLabel);
        usersReady.append(new QLabel);
        usersLayout[i]->addWidget(usersName[i]);
        usersLayout[i]->addStretch(1);
        usersLayout[i]->addWidget(usersReady[i]);
    }
    users->addLayout(usersLayout.at(0), 0, 0);
    users->addLayout(usersLayout.at(1), 1, 0);
    users->addLayout(usersLayout.at(2), 2, 0);
    users->addLayout(usersLayout.at(3), 3, 0);
    users->addLayout(usersLayout.at(4), 0, 1);
    users->addLayout(usersLayout.at(5), 1, 1);
    users->addLayout(usersLayout.at(6), 2, 1);
    users->addLayout(usersLayout.at(7), 3, 1);
    vLayout->addWidget(mainLabel);
    vLayout->addWidget(labelInfos);
    vLayout->addLayout(users);
    btnsLayout->addWidget(btnReady);
    btnsLayout->addStretch(1);
    btnsLayout->addWidget(btnLeave);
    vLayout->addLayout(btnsLayout);
    setLayout(vLayout);

    // Connexions
    connect(tcpClient, &TcpClient::userListRefresh, this, &WaitingRoom::userListRefresh);
    connect(btnLeave, &QPushButton::clicked, tcpClient, &TcpClient::disconnectFromHost);
    connect(btnReady, &QPushButton::clicked, tcpClient, &TcpClient::toggleReady);
    connect(tcpClient, &TcpClient::connected, this, &WaitingRoom::connected);
    connect(tcpClient, &TcpClient::startGame, this, [=] () {
        emit setVisibleWidget(0);
    });


    connect(tcpClient, &TcpClient::connectionError, this, [=] () {
        emit setVisibleWidget(1);
    });

    connect(tcpClient, &TcpClient::UserLoggedIn, this, [=] () {
        //label->setText()
    });

    connect(tcpClient, &TcpClient::disconnected, this, [=] () {
        //QMessageBox::critical(this, "Erreur", "Déconnecté du serveur");
        emit setVisibleWidget(1);
    });
}

void WaitingRoom::userListRefresh(QHash<int, QHash<QString, QString>> users) {
    // Activer / désactiver le bouton "prêt" s'il n'y a pas assez de monde
    if(users.size() >= MIN_USERS) {
        btnReady->setEnabled(true);
        btnReady->setText("Prêt");
    }else{
        btnReady->setEnabled(false);
        btnReady->setText("Encore " + QString::number(MIN_USERS - users.size()) + " joueur nécessaire");
    }
    // Mettre à jour les labels des utilisateurs
    QHashIterator<int, QHash<QString, QString>> i(users);
    int count = 0;
    while(i.hasNext()) {
        i.next();
        usersName[count]->setText(i.value().value("username"));
        usersReady[count]->setText(i.value().value("ready") == "true" ? "Prêt" : "Attente");
        if(i.key() == tcpClient->getDescriptor() && i.value().value("ready") == "true")
            btnReady->setText("Pas prêt");
        count++;
    }
    for(int i = users.size(); i < MAX_USERS; i++) {
        usersName[count]->setText("...");
        usersReady[count]->setText("...");
    }
    /*for(int i = 0; i < MAX_USERS; i++) {
        if(i < users.length()) {
            usersName[i]->setText(users.at(i).value("username"));
            usersReady[i]->setText(users.at(i).value("ready") == "true" ? "Prêt" : "Attente");
            if(users.at(i).value("socketDescriptor").toInt() == tcpClient->getDescriptor() && users.at(i).value("ready") == "true") {
                btnReady->setText("Pas prêt");
            }
        }else{
            usersName[i]->setText("...");
            usersReady[i]->setText("...");
        }
    }*/
}

void WaitingRoom::connected() {
    mainLabel->setText("Connecté au serveur !");
    btnReady->setEnabled(true);
    btnLeave->setEnabled(true);
}

void WaitingRoom::startWaitingRoom(QHostAddress address, qint16 port) {
    mainLabel->setText("Chargement...");
    tcpClient->connectToServer(address, port);
    btnReady->setEnabled(false);
    btnLeave->setEnabled(false);
    for(int i = 0; i < MAX_USERS; i++) {
        usersName[i]->setText("...");
        usersReady[i]->setText("...");
    }
}
