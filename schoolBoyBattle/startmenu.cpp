#include "startmenu.h"

#include <QApplication>
#include <QBoxLayout>
#include <QPushButton>

StartMenu::StartMenu(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QPushButton *btnStartGame = new QPushButton("Démarrer le jeu en local à 2 joueurs");
    QPushButton *btnConnectToServer = new QPushButton("Se connecter à un serveur");
    QPushButton *btnQuit = new QPushButton("Quitter le jeu");

    vLayout->addWidget(btnStartGame);
    vLayout->addWidget(btnConnectToServer);
    vLayout->addWidget(btnQuit);

    setLayout(vLayout);

    connect(btnStartGame, &QPushButton::pressed, this, [=] () {
        emit setVisibleWidget(0);
        emit startLocalGame(2);
    });
    connect(btnConnectToServer, &QPushButton::pressed, this, [=] () {
        emit setVisibleWidget(2);
        emit startClient();
    });

    connect(btnQuit, &QPushButton::pressed, QApplication::instance(), &QApplication::quit);
}
