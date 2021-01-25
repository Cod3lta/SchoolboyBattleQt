#include "waitingroom.h"

#include <QLabel>
#include <QMessageBox>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#define MAX_USERS 8
#define MIN_USERS 2

WaitingRoom::WaitingRoom(TcpClient *tcpClient, QWidget *parent) :
    QWidget(parent),
    tcpClient(tcpClient)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::lightGray);
    setAutoFillBackground(true);
    setPalette(pal);

    // Mise en place du layout
    QVBoxLayout *vLayout = new QVBoxLayout;
    QHBoxLayout *btnsLayout = new QHBoxLayout;
    QHBoxLayout *labelInfoLayout = new QHBoxLayout;
    QHBoxLayout *labelMainLayout = new QHBoxLayout;
    QGridLayout *users = new QGridLayout;
    QHBoxLayout *imgPersonnageLayout = new QHBoxLayout;
    QHBoxLayout *mainLayout = new QHBoxLayout;

    //Création des widgets
    mainLabel = new QLabel;
    QLabel *labelInfos = new QLabel("Dès que toutes les personnes ayant rejoint le serveur sont prêtes, le jeu commencera");
    labelInfoLayout->addWidget(labelInfos);
    labelMainLayout->addWidget(mainLabel);
    btnReady = new QPushButton("Prêt");
    btnLeave = new QPushButton("Quitter le serveur");
    QPixmap pixmapPersonnage(":/Resources/brand/Personnage.png");
    QLabel *imgPersonnage=new QLabel;
    imgPersonnage->setPixmap(pixmapPersonnage);
    imgPersonnageLayout->addStretch(1);
    imgPersonnageLayout->addWidget(imgPersonnage);
    imgPersonnageLayout->addStretch(1);

    labelInfos->setStyleSheet("");

   /* QMediaPlaylist *playlist = new QMediaPlaylist();
       playlist->addMedia(QUrl("qrc:/Resources/music/music.wav"));
       playlist->setPlaybackMode(QMediaPlaylist::Loop);
       QMediaPlayer *music = new QMediaPlayer();
       music->setPlaylist(playlist);
       music->play();

*/


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

    vLayout->addStretch(1);
    vLayout->addLayout(imgPersonnageLayout);
    vLayout->addLayout(labelMainLayout);
    vLayout->addLayout(labelInfoLayout);
    vLayout->addLayout(users);
    btnsLayout->addWidget(btnReady);
    btnsLayout->addStretch(1);
    btnsLayout->addWidget(btnLeave);
    vLayout->addLayout(btnsLayout);
    vLayout->addStretch(1);
    mainLayout->addStretch(1);
    mainLayout->addLayout(vLayout);
    mainLayout->addStretch(1);
    setLayout(mainLayout);

    // Connexions
    connect(tcpClient, &TcpClient::userListRefresh, this, &WaitingRoom::userListRefresh);
    connect(btnLeave, &QPushButton::clicked, tcpClient, &TcpClient::disconnectFromHost);
    connect(btnReady, &QPushButton::clicked, tcpClient, &TcpClient::toggleReady);
    connect(tcpClient, &TcpClient::connected, this, &WaitingRoom::connected);
    connect(tcpClient, &TcpClient::startGame, this, [=] () {
        emit setVisibleWidget(0);
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
        if(i.key() == tcpClient->getSocketDescriptor()) {
            mainLabel->setText("Connecté au serveur en tant que " + i.value().value("username") + " !");
            if(i.value().value("ready") == "true")
                btnReady->setText("Pas prêt");
        }
        count++;
    }
    for(int i = users.size(); i < MAX_USERS; i++) {
        usersName[count]->setText("...");
        usersReady[count]->setText("...");
    }
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

