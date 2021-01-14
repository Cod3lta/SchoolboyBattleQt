#include "startmenu.h"
#include "mainwindow.h"

#include <QBoxLayout>
#include <QPushButton>

StartMenu::StartMenu(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout(this);
    QPushButton *btnStartGame = new QPushButton("Démarrer le jeu en local à 2 joueurs");
    QPushButton *btnCreateServer = new QPushButton("Créer un serveur");
    QPushButton *btnConnectToServer = new QPushButton("Se connecter à un serveur");

    vLayout->addWidget(btnStartGame);
    vLayout->addWidget(btnCreateServer);
    vLayout->addWidget(btnConnectToServer);

    setLayout(vLayout);

    connect(btnStartGame, &QPushButton::pressed, [=] () {
        emit changeWidget(0);
    });
}

void StartMenu::startLocalGame()
{
    emit changeWidget(1);
}
